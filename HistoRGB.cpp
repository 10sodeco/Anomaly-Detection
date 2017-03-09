
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <sstream>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, const char** argv)
{
    // read the image in the file"MyPic.JPG" and store it in img
    Mat img = imread("MyPic.JPG", CV_LOAD_IMAGE_UNCHANGED);
    if (img.empty()) // check to see if the image was loaded
    {
        cout<<"Error: Image cannot be loaded!" << endl;
        return -1;
    }
    
    // Now we will use the built in split function to split the image into 3 planes (R,G,B)
    
    vector<Mat> bgr_planes;
    split(img, bgr_planes);
    
    // Establish number of bins (num. different values on horizontal axis of our histogram)
    
    int histSize = 256; // 256 because B,G,R values range from 0 to 256
    
    // Set ranges. Again range from 0, 256 because we are using RGB
    float range[] = {0, 256};
    const float* histRange = {range};
    
    // We want our bins to be equal so we set the uniform value to true
    bool uniform = true;
    
    // We want a new histogram everytime we run the operation so we do not accumulate previous results
    bool accumulate = false;
    
    // Declare the 3 image containers for our seperate color values
    Mat b_hist, g_hist, r_hist;
    
    // Create the histograms using OpenCV function calcHist
    
    // calcHist(const Mat* images, int nimages, const int* channels, InputArray mask, OutputArray hist, int dims, const int* histSize, const float** ranges, bool uniform, bool accumulate)
    
    calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[1], 1, 0, Mat (), g_hist, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[2], 1, 0, Mat (), r_hist, 1, &histSize, &histRange, uniform, accumulate);
    
    // Draw the histograms for the three colors
    
    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound((double) hist_w/histSize);
    
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0)); // CV_8UC3: U = Unsigned integer image with 3 channels
    
    // Normalize the histograms
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    
    // Draw for each channel
    for(int i = 1; i < histSize; i++)
    {
        line(histImage, Point(bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1))),
             Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
             Scalar ( 255, 0, 0), 2, 8, 0);
        
        line(histImage, Point(bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1))),
             Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
             Scalar ( 0, 255, 0), 2, 8, 0);
        
        line(histImage, Point(bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1))),
             Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
             Scalar ( 0, 0, 255), 2, 8, 0);
    }
    
    // Display the histogram that we created
    namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
    imshow("calcHist Demo", histImage);
    
    waitKey(0);
    
    destroyWindow("calcHist Demo"); // destroy the window we created for the histogram
    
    return 0;
}
