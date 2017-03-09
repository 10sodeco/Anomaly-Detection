std::vector<Mat> manipulateImage(Mat image1)
{
    stringstream ss;
    cv:: Size smallSize(smallWidth, smallHeight); //creating an image 1/25 the size of the larger image
    std::vector<Mat> smallImages; // create a vector to store the small images
    std::vector<Mat> histograms; // create a vector where we will store the histrogram information created
    Mat tempHisto; // create a temporary image holder for each histogram we pass back
    string type = ".jpg"; // just stores type of image we are dealing with
    Mat gray_image; // create an Mat where we will save the gray version of this image
    int height, width, smallHeight, smallWidth, count; // create the different int values we will need throughout
    string name, fileName;
    
    
    cvtColor(image1, gray_image, CV_BGR2GRAY); // convert the original image to gray scale
    
    height = gray_image.rows; // get the height of the image
    width = gray_image.cols; // get the weight of the image
    
    // We will create 25 sub-images, so we want to create a 5x5 grid of sub-images
    smallHeight = height/5;
    smallWidth = width/5;
    
    imwrite("../Desktop/OpenCVTest/Gray_Image.jpg", gray_image); // save the grayscale image to our folder
    
    
    // Will now run through the image row by row and break it down into smaller images, and then create the histogram for these smaller images
    count = 0; // to keep count of images
    for (int y = 0; y < height; y += smallSize.height) // outer loop which runs through our vertical placement in the image
    {
        for (int x = 0; x < width; x += smallSize.width) // inner loop which runs through the horizontal placement in the image
        {
            cv::Rect rect = cv::Rect(x, y, smallSize.width, smallSize.height); // pulls the rectangle out from the larger image
            smallImages.push_back(cv::Mat(gray_image, rect)); // add this sub image to our vector storing all smaller images 
            name = format("../Desktop/OpenCVTest/SmallImages/smallImage_"); // create the text of the file name, will add counter at the end to identify which image we are looking at
            ss << name << (count+1) << type; // concat the string together to create our file name, which keeps track of what image we are on
            string filename = ss.str();  
            //ss.str("");
            imwrite(filename, cv::Mat(gray_image, rect)); // write the image to the small image folder
            
            // Now we will call another function which will create the histogram of this sub image
            
            tempHisto = makeHisto(cv::Mat(gray_image, rect)); // create the histogram and then save it
            
            histograms.push_back(tempHisto); // save the histogram to our histogram array so we can use it to compare with other histograms
        }
    }
    return(histograms); // returns our vector of histograms to the calling function so we can create our PDF from this data
}

Mat makeHisto(Mat sampleImage)
{
    int histSize; // Establish number of bins (num. different values on horizontal axis of our histogram)
    float range[];
    bool uniform, accumulate;
    Mat gray_hist; // creates a container for the histogram we create
    int hist_width, hist_height, bin_width, i; // declaring the int values we will need in the code
    

    histSize = 256; // 256 because grayscale intensity values range from 0 to 255
    
    // Set ranges. Again range from 0, 255 because we are using RGB
    range[] = {0, 256};
    const float* histRange = {range};
    
    uniform = true;  // We want our bins to be equal so we set the uniform value to true
    accumulate = false; // We want a new histogram everytime we run the operation so we do not accumulate previous results
    
    // Create the histogram using OpenCV function calcHist
    // Following line is from OpenCV documentation describing calcHist function
    // calcHist(const Mat* images, int nimages, const int* channels, InputArray mask, OutputArray hist, int dims, const int* histSize, const float** ranges, bool uniform, bool accumulate)
    
    calcHist(&sampleImage, 1, 0, Mat(), gray_hist, 1, &histSize, &histRange, uniform, accumulate); 
    
    // Set the dimensions of the histogram that will be created
    hist_width = 512; 
    hist_height = 400;
    bin_width = cvRound((double) hist_width/histSize); // assigns the width of each bin on our histogram (x-axis value)
    
    // Create window and background for the histogram
    Mat histImage(hist_height, hist_width, CV_8UC3, Scalar(0, 0, 0)); // CV_8UC3: U = Unsigned integer image with 3 channels
    
    // Normalize the histogram so it falls within the values we created
    normalize(gray_hist, gray_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    
    // Draw the histogram 
    for(i = 1; i < histSize; i++)
    {
        line(histImage, Point(bin_width*(i-1), hist_height - cvRound(gray_hist.at<float>(i-1))),
             Point( bin_width*(i), hist_height - cvRound(gray_hist.at<float>(i)) ),
             Scalar ( 255, 255, 255), 2, 8, 0); // creates at white line
    }
    
    namedWindow("calcHisto", CV_WINDOW_AUTOSIZE); // create a window to display the histogram for testing purposes
    imshow("calcHisto", histImage); // show the window we created
    
    waitKey(0); // wait for the user to press a key to exit the window
    
    destroyWindow("calcHisto"); // destroy the window that we created
    
    return(gray_hist); // return the histogram data that we created so we can add it our array
}
