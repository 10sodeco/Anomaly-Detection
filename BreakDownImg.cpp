// Test file for loading an image, then changing it to grayscale and striding over it to break it into smaller subimages

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <sstream>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>

using namespace cv;
using namespace std;
 /*
// create global variables for the canny edges portions of the porject
Mat src, src_gray;
Mat dst, detected_edges;


int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";

// Haven't got this part of the program working yet
void CannyThreshold(int, void*)
{
    
    blur( src_gray, detected_edges, Size(3,3) ); // reduce noise
    
    
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size ); // Canny detector algorithm
    
    dst = Scalar::all(0); // use algorithms output as a mask
    
    src.copyTo( dst, detected_edges);
    imshow( window_name, dst );
}
*/
int main(int argc, const char** argv)
{
    stringstream ss;
    
    string type = ".jpg";
    
    Mat img = imread("image1.JPG", CV_LOAD_IMAGE_UNCHANGED); // read the image in the file"MyPic.JPG" and store it in img
    if (img.empty()) // check to see if the image was loaded
    {
        cout<<"Error: Image cannot be loaded!" << endl;
        //system("pause"); // wait for a key press
        return -1;
    }
    
    Mat gray_image; // create an Mat where we will save the gray version of this image
    cvtColor(img, gray_image, CV_BGR2GRAY); // convert the original image to gray scale
    
    int height = gray_image.rows; // get the height of the image
    int width = gray_image.cols; // get the weight of the image
    
    printf("We are processing a %dx%d image. \n", height, width); // print out the size
    
    int smallHeight = height/5;
    int smallWidth = width/5;
    
    imwrite("../Desktop/OpenCVTest/Gray_Image.jpg", gray_image); // save the grayscale image to our folder
    
    namedWindow("MyWindow", CV_WINDOW_AUTOSIZE); // create a window named MyWindow to display original image
    namedWindow("GrayWindow", CV_WINDOW_AUTOSIZE);
    imshow("GrayWindow", gray_image); // display the grayscale image in the window
    imshow("MyWindow", img); // display the original image in the windowcmak
    
    waitKey(0); // wait infinite time for a key press
    
    destroyWindow("MyWindow"); // destroy the window that we created
    destroyWindow("GrayWindow"); // destroy window containing the grayscale image we created
    
    /*
    // Don't have canny threshold working correctly
    
    dst.create( gray_image.size(), gray_image.type() );
    
    
    namedWindow( window_name, CV_WINDOW_AUTOSIZE );
    
    
    createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold ); // create trackbar to enter manually
    
    
    CannyThreshold(0, 0); // display
    
    waitKey(0);
    */
    
    cv:: Size smallSize(smallWidth, smallHeight); //creating a image 1/10 the size of the larger image
    std::vector<Mat> smallImages;
    
    printf("We will be creating images of size %dx%d \n", smallSize.height, smallSize.width);
    
    // Create a process that will run through the image and break it down into 30 x 30 sub images
    int count = 0; // to keep count of images
    for (int y = 0; y < height; y += smallSize.height)
    {
        for (int x = 0; x < width; x += smallSize.width)
        {
            cv::Rect rect = cv::Rect(x, y, smallSize.width, smallSize.height);
            smallImages.push_back(cv::Mat(gray_image, rect));
            string name = format("../Desktop/OpenCVTest/SmallImages/smallImage_"); // create the text of the file name, will add counter at the end to identify which image we are looking at
            ss << name << (count+1) << type; // concat the string together to create our file name
            string filename = ss.str();
            ss.str("");
            imwrite(filename, cv::Mat(gray_image, rect)); // write the image to the small image folder
            namedWindow("SmallImages", CV_WINDOW_AUTOSIZE);
            imshow("SmallImages", cv::Mat(gray_image, rect));
            waitKey(0);
        }
    }
    
    destroyWindow("SmallImages"); // destroy the window we created for the small images
    
    return 0;
}
