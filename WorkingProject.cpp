
//// Background subtractor is working. Now just need to get it to work with a stream of images instead of just one image.



#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/background_segm.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <sstream>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

// Global variables
Mat frame; // Frame for the normal image
Mat edgeFrame; // Frame for the edge detected version of the image
Mat fgMaskMOG2; // foreground mask generated by the MOG2 method
Mat fgMaskMOG2Edge; // foregroundmask for the edge detected version of the image
Mat gradientImage; // create a mat image for the edge detected image

bool firstFrameFlag = TRUE; // Set a flag so that we gather information about the image size the first time we read grab a frame
bool saveFrame = FALSE; // This flag will be sent to let main know whether or not to write an image to a folder
int keyboard;
int height = 0; // the height of the images we are working with
int width = 0; // the width of the images we are working with
int pixels = 0; // The total number of pixels in the images
int totalImgCount;
float rollingMean, rollingVarience, rollingStandardDev;

Ptr<BackgroundSubtractorMOG2> pMOG2; // MOG2 Background subtractor for normal image
Ptr<BackgroundSubtractorMOG2> pMOG2edge; // Background subtractor for the edge detected image

// declare the functions
void backgroundSubtraction(Mat image);
float backgroundSubtractionEdge(Mat image);
float edgeDetection(Mat image);
void blobDetection(Mat image); // Function to detect blobs


// Simple background subtraction that runs on a normal image, included for testing purposes for comparison between two functions
void backgroundSubtraction(Mat image)
{
    pMOG2->operator()(image, fgMaskMOG2);
    
    imshow("Frame", image);
    
    imshow("FG Mask MOG 2", fgMaskMOG2);
    
    return;
}

// Background subtraction that runs on our edge detected images
float backgroundSubtractionEdge(Mat image)
{
    int x, y; // X and Y values to determine which pixel we are at
    int intensity; // value of intensity at a given pixel
    int runningTotal; // summed value of the intensity of an image, needed for the mean
    float imageMean, imgDeviation;
    
    pMOG2edge->operator()(image, fgMaskMOG2Edge);
    
    imshow("FG Mask Edge", fgMaskMOG2Edge); // Show the image
    
    runningTotal = 0; //set the runningTotal value to 0
    // Calculate the mean value of the pixels in this image
    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            intensity = (int)image.at<uchar>(y,x); // get the intensity of the pixel we are currently looking at <<<<<<<<<< ********************** image should be MOG img
            runningTotal = runningTotal + intensity; // Add to the running total we are keeping
        }
    }
    
    // We don't want to start calculating through the first 10 frames so the algorithm has time to normalize from the first frame
    if (totalImgCount > 10)
    {
        imageMean = runningTotal/pixels; // We divide the total intensity value by the number of pixels to get a mean pixel value for this frame
    }
    else
    {
        imageMean = 0;
    }
    
    // This is where we will want to caluclate whether or not this mean falls within our standard deviation
    // If it falls outside of 3 standard deviations then we want to flag it as an anomaly
    if (totalImgCount > 20)
    {
        saveFrame = TRUE;
        imgDeviation = abs(imageMean - rollingMean);
        if (imgDeviation > (rollingStandardDev * 2.5)) // This image's pixel value falls outside the normal region, so it is anomalous
        {
            // Here we want to mark the time what time the image occured so we can compare to the data collected from our citizen scientists
            // also want to write the images to a file so we
            saveFrame = TRUE; // flip the flag so our main function knows to save the normal image for comparison
            printf("Found an anomaly! \n");
        }
    }
    
    return imageMean; // We return the imageMean back to the main function to continue our calculations
}

float edgeDetection(Mat image)
{
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    float calculatedMean;
    Mat gray_image; // create an Mat where we will save the gray version of this image
    // Declare the mat objects we will need for our Sobel functions
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    
    // (7, 7) is the ideal filter for species 2
    
    GaussianBlur(image, image, Size(9,9), 0, 0, BORDER_DEFAULT); // (7, 7) is defining the kernel size, this value was the most effective kernel size for all 3 different video types
    
    cvtColor(image, gray_image, CV_BGR2GRAY); // convert the original image to gray scale
    
    Sobel(gray_image, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(grad_x, abs_grad_x);
    
    Sobel(gray_image, grad_y, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(grad_y, abs_grad_y);
    
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, gradientImage);
    
    //imshow("Edge Frame", gradientImage); // Just to show the image, not necessarily needed
    
    calculatedMean = backgroundSubtractionEdge(gradientImage); // Send this image to the background subtraction algorithm
    
    return calculatedMean;
}

void blobDetection(Mat image)
{
    SimpleBlobDetector::Params parameters;
    
    // Set up a max and min threshold for the blobs
    parameters.minThreshold = 10;
    parameters.maxThreshold = 150;
    
    parameters.filterByColor = TRUE;
    
    // Filter by area
    parameters.filterByArea = TRUE;
    parameters.minArea = 12;
    
    // Filter by circularity
    parameters.filterByCircularity = TRUE;
    parameters.minCircularity = 0.5;
    parameters.maxCircularity = 0.6;
    
    // Filter by convexity
    parameters.filterByConvexity = TRUE;
    parameters.minConvexity = 0.75;
    parameters.maxConvexity = 0.85;
    
    // Filter by Inertia
    parameters.filterByInertia = TRUE;
    parameters.minInertiaRatio = 0.10;
    
//    
//    Mat gray_image; // create an Mat where we will save the gray version of this image
//    cvtColor(image, gray_image, CV_BGR2GRAY); // convert the original image to gray scale
    
    SimpleBlobDetector detector(parameters);
    // Detect blobs in the image
    std::vector<KeyPoint> keypoints;
    detector.detect(image, keypoints);
    
    // Draw detected blobs as red circles
    Mat im_with_keypoints;
    drawKeypoints(image, keypoints, im_with_keypoints, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    
    // Show the blobs that were detected in a frame
    imshow("keypoints", im_with_keypoints);
    
    return;
}

int main(int argc, const char** argv)
{
    stringstream ssstream;
    string imgType, fileName, concatName;
    std::vector<Mat> holder;
    Mat frame2;
    int calcImages, anomalyCount; // This will be used to calculate the actual number of images we ran our calculations on
    float totalAvgPixels, returnedMean, oldRollingMean;
    
    // Create windows to display the different images
    namedWindow("Frame");
    //namedWindow("Edge Frame");
    namedWindow("FG Mask Edge");
    //namedWindow("FG Mask MOG 2");
    //namedWindow("keypoints");
    
    // Create and initialze the background subtractor objects
    pMOG2 = new BackgroundSubtractorMOG2();
    pMOG2edge = new BackgroundSubtractorMOG2();
    
    VideoCapture capture("test2.mp4");
    if(!capture.isOpened())
    {
        cerr << "Unable to open the video file - test.mp4" << endl;
        exit(EXIT_FAILURE);
    }
    
    // initialze all our counting values
    totalImgCount = 1;
    calcImages = 1;
    rollingMean = 0;
    rollingVarience = 0;
    rollingStandardDev = 0;
    anomalyCount = 0;
    imgType = ".jpg";
    
    // Main loop that will run through our video
    while( (char)keyboard != 'q' && (char)keyboard != 27) // user can stop at any time by hitting q
    {
        // read in the current frame
        if(!capture.read(frame))
        {
            cerr << "Unable to read next frame." <<endl;
            cerr << "Exiting." << endl;
            exit(EXIT_FAILURE);
        }
        
        if (firstFrameFlag == TRUE)
        {
            // Get the information regarding the size of the image for our calculations
            height = frame.rows; // get the height of the image
            width = frame.cols; // Get the width of the image
            
            pixels = height * width; // Get the total number of pixels in the images that we will be working with
            firstFrameFlag = FALSE; // Set the flag to false so we don't do these operations again
        }
        
        // Next too lines are for testing purposes only
        frame2 = frame; // save a copy of the frame to send to the background subtraction and one for the edge detected model
        //backgroundSubtraction(frame2);
        
        imshow("Frame", frame);
        
        returnedMean = edgeDetection(frame); // Call the first algorithm and then when all calculations are done we receive the mean value of intensity from image
        
        // Check to see if the image was anomalous, and if it is then send a normal version of the image to our save file for comparison
        if (saveFrame == TRUE)
        {
            // Write the image to our save file
            
            //printf("Made it to the write stage. \n");
            concatName = format("../Desktop/ProjImages/normalImage_"); // Create the save location and the first part of the name of this image
            ssstream << concatName << (anomalyCount + 1) << imgType; // Concatanate the string together to form our file name
            
            fileName = ssstream.str();
            ssstream.str("");
            
            // Now write the image to this file
            imwrite(fileName, frame2);
            anomalyCount++;
            saveFrame = FALSE; // FLip this flag back to false so we don't keep writing images
        }
        
        
        if (totalImgCount > 10) // Be sure we have given the background subtraction algorithm 10 frames to normalize
        {
            totalAvgPixels = totalAvgPixels + returnedMean; // Add the just calculated mean into our running total
            
            oldRollingMean = rollingMean; // We need to store mean before changing it for calculating our varience
            
            rollingMean = rollingMean + (returnedMean - rollingMean)/calcImages; // Calculate the new average
            
            rollingVarience = ( (rollingVarience + (returnedMean - rollingMean) * (returnedMean - oldRollingMean) ) / (calcImages - 1)); // calculate the new rollingVarience
            
            rollingVarience = abs(rollingVarience);
            
            rollingStandardDev = sqrt(rollingVarience); // Find the square root of this value so that we have the square root
            
            calcImages++; // Increment the counter for the number of images that we are running our calculations on
        }
        
        keyboard = waitKey( 30 ); // check to see if the user has hit escape
        totalImgCount++;
    }
    
    printf("Size of the images is %dx%d and the total pixels is - %d \n", height, width, pixels);
    
    printf("The running mean we calculated was - %f \n", rollingMean);
    printf("The running varience we calculated was - %f \n", rollingVarience);
    printf("The running standard deviation we calculated was - %f \n", rollingStandardDev);

    destroyAllWindows();
    
    return(0);
    
}
