std::vector<Mat> manipulateImage(Mat image1)
{
    stringstream ss;
    
    string type = ".jpg";
    
    Mat gray_image; // create an Mat where we will save the gray version of this image
    cvtColor(image1, gray_image, CV_BGR2GRAY); // convert the original image to gray scale
    
    int height = gray_image.rows; // get the height of the image
    int width = gray_image.cols; // get the weight of the image
    
    printf("We are processing a %dx%d image. \n", height, width); // print out the size
    
    int smallHeight = height/5;
    int smallWidth = width/5;
    
    imwrite("../Desktop/OpenCVTest/Gray_Image.jpg", gray_image); // save the grayscale image to our folder
    
    //    makeHisto(gray_image);
    cv:: Size smallSize(smallWidth, smallHeight); //creating a image 1/10 the size of the larger image
    std::vector<Mat> smallImages; // create a vector to store the small images
    std::vector<Mat> histograms;
    Mat tempHisto;
    
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
            
            tempHisto = makeHisto(cv::Mat(gray_image, rect)); // create the histogram and then save it
            
            histograms.push_back(tempHisto); // save the histogram to our histogram array so we can use it to compare with other histograms
            
            // Code to print off small images to compare
            
            //            namedWindow("SmallImages", CV_WINDOW_AUTOSIZE);
            //            imshow("SmallImages", cv::Mat(gray_image, rect));
            //            waitKey(0);
            //            destroyWindow("SmallImages"); // destroy the window we created for the small images
        }
    }
    
    return(histograms);
}

Mat makeHisto(Mat sampleImage)
{
    // Establish number of bins (num. different values on horizontal axis of our histogram)
    
    int histSize = 256; // 256 because grayscale intensity values range from 0 to 255
    
    // Set ranges. Again range from 0, 255 because we are using RGB
    float range[] = {0, 256};
    const float* histRange = {range};
    
    // We want our bins to be equal so we set the uniform value to true
    bool uniform = true;
    
    // We want a new histogram everytime we run the operation so we do not accumulate previous results
    bool accumulate = false;
    
    Mat gray_hist;
    
    // Create the histogram using OpenCV function calcHist
    
    // calcHist(const Mat* images, int nimages, const int* channels, InputArray mask, OutputArray hist, int dims, const int* histSize, const float** ranges, bool uniform, bool accumulate)
    
    calcHist(&sampleImage, 1, 0, Mat(), gray_hist, 1, &histSize, &histRange, uniform, accumulate);
    
    // Draw the histogram for the subimage
    
    int hist_w = 512;
    int hist_h = 400;
    int bin_w = cvRound((double) hist_w/histSize);
    
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0)); // CV_8UC3: U = Unsigned integer image with 3 channels
    
    // Normalize the histogram
    normalize(gray_hist, gray_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    
    for(int i = 1; i < histSize; i++)
    {
        line(histImage, Point(bin_w*(i-1), hist_h - cvRound(gray_hist.at<float>(i-1))),
             Point( bin_w*(i), hist_h - cvRound(gray_hist.at<float>(i)) ),
             Scalar ( 255, 255, 255), 2, 8, 0);
    }
    
    namedWindow("calcHisto", CV_WINDOW_AUTOSIZE);
    imshow("calcHisto", histImage);
    
    waitKey(0);
    
    destroyWindow("calcHisto");
    
    return(gray_hist);
}
