// Test file for loading an image, then changing it to grayscale and striding over it to break it into smaller subimages

#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, const char** argv)
{
    Mat img = imread("MyPic.JPG", CV_LOAD_IMAGE_UNCHANGED); // read the image in the file"MyPic.JPG" and store it in img
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
    
    cv:: Size smallSize(smallWidth, smallHeight); //creating a image 1/10 the size of the larger image
    std::vector<Mat> smallImages;
    
    printf("We will be creating images of size %dx%d \n", smallSize.height, smallSize.width);
    
    // Create a process that will run through the image and break it down into 30 x 30 sub images
    for (int y = 0; y < height; y += smallSize.height)
    {
        for (int x = 0; x < width; x += smallSize.width)
        {
            cv::Rect rect = cv::Rect(x, y, smallSize.width, smallSize.height);
            smallImages.push_back(cv::Mat(gray_image, rect));
            namedWindow("SmallImages", CV_WINDOW_AUTOSIZE);
            imshow("SmallImages", cv::Mat(gray_image, rect));
            waitKey(0);
        }
    }
    
    destroyWindow("SmallImages"); // destroy the window we created for the small images
    
    return 0;
}
