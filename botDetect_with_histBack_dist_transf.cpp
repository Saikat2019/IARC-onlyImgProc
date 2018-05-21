#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>


#define CLIONS_COLORHISTOGRAM_H
#define CLIONS_CONTENTFINDER_H

using namespace cv;
using namespace std;

class ColorHistogram {
  private:
    int histSize[3];        // size of each dimension
    float hranges[2];       // range of values (same for the 3 dimensions)
    const float* ranges[3]; // array of ranges for each dimension
    int channels[3];        // channel to be considered
  public:
    ColorHistogram() {
        // Prepare default arguments for a color histogram
        // each dimension has equal size and range
        histSize[0]= histSize[1]= histSize[2]= 256;
        hranges[0]= 0.0;    // BRG range from 0 to 256
        hranges[1]= 256.0;
        ranges[0]= hranges; // in this class,
        ranges[1]= hranges; // all channels have the same range
        ranges[2]= hranges;
        channels[0]= 0;     // the three channels: B
        channels[1]= 1;     // G
        channels[2]= 2;     // R
    }
    // set histogram size for each dimension
    void setSize(int size) {
        // each dimension has equal size
        histSize[0]= histSize[1]= histSize[2]= size;
    }
    // Computes the histogram.
    cv::Mat getHistogram(const cv::Mat &image) {
        cv::Mat hist;
        // BGR color histogram
        hranges[0]= 0.0;    // BRG range
        hranges[1]= 256.0;
        channels[0]= 0;     // the three channels
        channels[1]= 1;
        channels[2]= 2;
        // Compute histogram
        cv::calcHist(&image,
            1,          // histogram of 1 image only
            channels,   // the channel used
            cv::Mat(),  // no mask is used
            hist,       // the resulting histogram
            3,          // it is a 3D histogram
            histSize,   // number of bins
            ranges      // pixel value range
        );
        return hist;
    }
    // Computes the histogram.
    cv::SparseMat getSparseHistogram(const cv::Mat &image) {
        cv::SparseMat hist(3,        // number of dimensions
                           histSize, // size of each dimension
                           CV_32F);
        // BGR color histogram
        hranges[0]= 0.0;    // BRG range
        hranges[1]= 256.0;
        channels[0]= 0;     // the three channels
        channels[1]= 1;
        channels[2]= 2;
        // Compute histogram
        cv::calcHist(&image,
            1,          // histogram of 1 image only
            channels,   // the channel used
            cv::Mat(),  // no mask is used
            hist,       // the resulting histogram
            3,          // it is a 3D histogram
            histSize,   // number of bins
            ranges      // pixel value range
        );
        return hist;
    }
    // Computes the 1D Hue histogram.
    // BGR source image is converted to HSV
    // Pixels with low saturation are ignored
    cv::Mat getHueHistogram(const cv::Mat &image,
                             int minSaturation=0) {
        cv::Mat hist;
        // Convert to HSV colour space
        cv::Mat hsv;
        cv::cvtColor(image, hsv, CV_BGR2HSV);
        // Mask to be used (or not)
        cv::Mat mask;
        // creating the mask if required
        if (minSaturation>0) {
            // Spliting the 3 channels into 3 images
            std::vector<cv::Mat> v;
            cv::split(hsv,v);
            // Mask out the low saturated pixels
            cv::threshold(v[1],mask,minSaturation,255,
                                 cv::THRESH_BINARY);
        }
        // Prepare arguments for a 1D hue histogram
        hranges[0]= 0.0;    // range is from 0 to 180
        hranges[1]= 180.0;
        channels[0]= 0;    // the hue channel
        // Compute histogram
        cv::calcHist(&hsv,
            1,          // histogram of 1 image only
            channels,   // the channel used
            mask,       // binary mask
            hist,       // the resulting histogram
            1,          // it is a 1D histogram
            histSize,   // number of bins
            ranges      // pixel value range
        );
        return hist;
    }
    // Computes the 2D ab histogram.
    // BGR source image is converted to Lab
    cv::Mat getabHistogram(const cv::Mat &image) {
        cv::Mat hist;
        // Convert to Lab color space
        cv::Mat lab;
        cv::cvtColor(image, lab, CV_BGR2Lab);
        // Prepare arguments for a 2D color histogram
        hranges[0]= 0;
        hranges[1]= 256.0;
        channels[0]= 1; // the two channels used are ab
        channels[1]= 2;
        // Compute histogram
        cv::calcHist(&lab,
            1,          // histogram of 1 image only
            channels,   // the channel used
            cv::Mat(),  // no mask is used
            hist,       // the resulting histogram
            2,          // it is a 2D histogram
            histSize,   // number of bins
            ranges      // pixel value range
        );
        return hist;
    }
};

class ContentFinder{
private:
    // histogram parameters
    float hranges[2];
    const float* ranges[3];
    int channels[3];
    float threshold; //decision threshold
    cv::Mat histogram;  // input histogram
public:
    ContentFinder():threshold(0.1f){
        // in this class, all channels have the same range
        ranges[0] = hranges;
        ranges[1] = hranges;
        ranges[2] = hranges;
    }
    // Sets the refrence histogram
    void setHistogram(const cv::Mat& h){
        histogram = h;
        cv::normalize(histogram, histogram, 1.0);
    }
    // Sets the threshold
    void setThreshold(float f){
        threshold = f;
    }
    //Simplified version in which
    //all channels used, with range[0,256]
    cv::Mat find(const cv::Mat& image){
        cv::Mat result;
        hranges[0] = 0.0;
        hranges[1] = 256.0;
        channels[0] = 0;
        channels[1] = 1;
        channels[2] = 2;
        return find(image, hranges[0], hranges[1], channels);
    }
    // Finds the pixel belonging to the histogram
    cv::Mat find(const cv::Mat& image, float minValue, float maxValue, int * channels){
        cv::Mat result;
        hranges[0] = minValue;
        hranges[1] = maxValue;
        // histogram dim matches channel list
        for(int i=0; i<histogram.dims; i++)
            this->channels[i]=channels[i];
        cv::calcBackProject(&image, 1,
                            channels,
                            histogram,
                            result,
                            ranges, 255.0);
        if(threshold>0.0)
            cv::threshold(result, result, 255.0*threshold, 255.0,cv::THRESH_BINARY);
        return result;
    }
};


int main(){
    ColorHistogram hc;
    namedWindow("image with rect",WINDOW_NORMAL);
    namedWindow("Color Detection Result before dilation",WINDOW_NORMAL);
    namedWindow("Color Detection Result after dilation",WINDOW_NORMAL);
    namedWindow("after distance transform",WINDOW_NORMAL);
    namedWindow("bot's parts",WINDOW_NORMAL);
    cv::Mat color = cv::imread("histBack9m.png");
    cv::Mat imageROI = cv::imread("histBack10m.png");
    //imageROI = color(cv::Rect(0,0,100,45)); // sky region
      //image with rectangle
    cv::rectangle(color, cv::Rect(0,0,100,45), cv::Scalar(0,0,255));
    cv::imshow("image with rect", color);
    // Get 3D color histogram (8 bins per channel)
    hc.setSize(8);
    cv::Mat shist = hc.getHistogram(imageROI);
    // Create the content finder
    ContentFinder finder;
    // set histogram to be bask-projected
    finder.setHistogram(shist);
    finder.setThreshold(0.05f);
    //Get back-projection of color histogram
    cv::Mat result = finder.find(color);
    Mat element=getStructuringElement(MORPH_ELLIPSE,Size(9,9),Point(4,4));
    cv::imshow("Color Detection Result before dilation", result);
    Mat bot(result.rows,result.cols,CV_8UC3,Scalar(0,0,0));
    for(int i=0;i<result.rows;i++)
    {
        for(int j=0;j<result.cols;j++)
        {
            //if(result.at<Vec3b>(i,j)[0]<3)
            //{
              //  if(result.at<Vec3b>(i,j)[1]<3)
                //{
                    if(result.at<uchar>(i,j)<3)
                    {
                        bot.at<Vec3b>(i,j)[0]=color.at<Vec3b>(i,j)[0];
                        bot.at<Vec3b>(i,j)[1]=color.at<Vec3b>(i,j)[1];
                        bot.at<Vec3b>(i,j)[2]=color.at<Vec3b>(i,j)[2];
                    }
                //}   
            //}
        }
    }
                
    dilate(result,result,element);
    //erode(result,result,element);
    dilate(result,result,element);
   for(int m=0;m<result.rows;m++)
    {
        for(int n=0;n<result.cols;n++)
            {
                if(result.at<uchar>(m,n)<5)result.at<uchar>(m,n)=255;
                else result.at<uchar>(m,n)=0;
            }
    }

    cv::imshow("Color Detection Result after dilation", result);

    distanceTransform(result,result,CV_DIST_L2,3);
    normalize(result,result,0,1,NORM_MINMAX);
    threshold(result, result, .4, 1, CV_THRESH_BINARY);
    imshow("after distance transform",result);
    imshow("bot's parts",bot);
    namedWindow("input part",WINDOW_NORMAL);
    imshow("input part",imageROI);
    cv::waitKey(0);
    return 0;
}