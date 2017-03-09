void makeHisto(Mat subImage)
{
    int histSize = 256; // Establish number of bins (num. different values on horizontal axis of our histogram) 256 because B,G,R values range from 0 to 256
    float range[] = {0, 256}; // Set ranges. Again range from 0, 256 because we are using RGB
    const float* histRange = {range};
    bool uniform = true; // We want our bins to be equal so we set the uniform value to true
    bool accumulate = false; // We want a new histogram everytime we run the operation so we do not accumulate previous results
    Mat blue_histo, green_histo, red_histo; // Declare the 3 image containers for our seperate color values
    
    // Now we will use the built in split function to split the image into 3 planes (R,G,B)
    
    vector<Mat> bgr_planes;
    split(subImage, bgr_planes);
    
    // Create the histograms using OpenCV function calcHist
    // calcHist(const Mat* images, int nimages, const int* channels, InputArray mask, OutputArray hist, int dims, const int* histSize, const float** ranges, bool uniform, bool accumulate)
    
    calcHist(&bgr_planes[0], 1, 0, Mat(), blue_histo, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[1], 1, 0, Mat (), green_histo, 1, &histSize, &histRange, uniform, accumulate);
    calcHist(&bgr_planes[2], 1, 0, Mat (), red_histo, 1, &histSize, &histRange, uniform, accumulate);
    
    // Draw the histograms for the three colors
    
    int hist_w = 512; // setting the height for the histogram
    int hist_h = 400; // setting the width
    int bin_w = cvRound((double) hist_w/histSize); // finds the size for each bin
    
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0)); // CV_8UC3: U = Unsigned integer image with 3 channels
    
    // Normalize the histograms
    normalize(blue_histo, blue_histo, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(green_histo, green_histo, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(red_histo, red_histo, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    
    // Draw for each channel
    for(int i = 1; i < histSize; i++)
    {
     line(histImage, Point(bin_w*(i-1), hist_h - cvRound(blue_histo.at<float>(i-1))),
                     Point( bin_w*(i), hist_h - cvRound(blue_histo.at<float>(i)) ),
                     Scalar ( 255, 0, 0), 2, 8, 0);
        
     line(histImage, Point(bin_w*(i-1), hist_h - cvRound(green_histo.at<float>(i-1))),
             Point( bin_w*(i), hist_h - cvRound(green_histo.at<float>(i)) ),
             Scalar ( 0, 255, 0), 2, 8, 0);
        
     line(histImage, Point(bin_w*(i-1), hist_h - cvRound(red_histo.at<float>(i-1))),
             Point( bin_w*(i), hist_h - cvRound(red_histo.at<float>(i)) ),
             Scalar ( 0, 0, 255), 2, 8, 0);
    }
    
    // Display the histogram that we created
    namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE); // Creates the window to display this
    imshow("calcHist Demo", histImage); // calls the window
    
    waitKey(0); // waits until the user hits a key
    
    destroyWindow("calcHist Demo"); // destroy the window we created for the histogram
    
    return 0;
}
