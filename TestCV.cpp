//// Background subtractor is working. Now just need to get it to work with a stream of images instead of just one image.



#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/background_segm.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

// Global variables
//Mat frame;
Mat fgMaskMOG2; // foreground mask generated by the MOG2 method
Mat gradientImage; // create a mat image for the edge detected image

Ptr<BackgroundSubtractorMOG2> pMOG2; // MOG2 Background subtractor

void backgroundSubtraction(Mat image)
{
    stringstream ss;
    
    string type = ".jpg";
    
    pMOG2->operator()(image, fgMaskMOG2);
    
    imshow("Frame", image);
    
    waitKey(0);
    
    imshow("FG Mask MOG 2", fgMaskMOG2);
    waitKey(0);
    
    return;
}

void edgeDetection(Mat image)
{
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    
    GaussianBlur(image, image, Size(3,3), 0, 0, BORDER_DEFAULT); // (3, 3) is defining the kernel size, this can be played with
    
    Mat gray_image; // create an Mat where we will save the gray version of this image
    cvtColor(image, gray_image, CV_BGR2GRAY); // convert the original image to gray scale
    
    // Declare the mat objects we will need for our Sobel functions
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    
    Sobel(gray_image, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(grad_x, abs_grad_x);
    
    Sobel(gray_image, grad_y, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(grad_y, abs_grad_y);
    
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, gradientImage);
    
    imshow("Frame", gradientImage);
    
    //backgroundSubtraction(gradientImage);
    
    waitKey(0);
}

int main(int argc, const char** argv)
{
    stringstream ss;
    std::vector<Mat> holder;
    Mat frame;
    
    namedWindow("Frame");
    namedWindow("FG Mask MOG 2");
    
    // Create the background subtractor objects
    pMOG2 = new BackgroundSubtractorMOG2();
    
    frame = imread("image1.JPG", CV_LOAD_IMAGE_UNCHANGED); // read the image in the file"MyPic.JPG" and store it in img
    if (frame.empty()) // check to see if the image was loaded
    {
        cout<<"Error: Image cannot be loaded!" << endl;
        //system("pause"); // wait for a key press
        return -1;
    }
    
    
    backgroundSubtraction(frame); // pass the image to the manipulateImage function
    
    edgeDetection(frame);
    
    
    destroyWindow("Frame");
    destroyWindow("FG Mask MOG 2");
    
    return(0);
    
}
