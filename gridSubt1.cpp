#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include<iostream>

using namespace cv;
using namespace std;

RNG rng(12345);

int main(int argc, char* argv[])   
{ 
   Mat model = imread("bot2.png");
   Mat image = imread("histBack1.png");

   int bins = 32;
   double d, max;
   Point maxPt;

   MatND histModel, histImage, histRatio;
   Mat model32fc3, image32fc3, backprj, kernel, conv;

   Mat temp, color;

   const int channels[] = {0, 1, 2};
   const int histSize[] = {bins, bins, bins};
   const float rgbRange[] = {0, 256};
   const float* ranges[] = {rgbRange, rgbRange, rgbRange};

   // model histogram
   model.convertTo(model32fc3, CV_32FC3);
   calcHist(&model32fc3, 1, channels, Mat(), histModel, 3, histSize, ranges, true, false);
   // image histogram
   image.convertTo(image32fc3, CV_32FC3);
   calcHist(&image32fc3, 1, channels, Mat(), histImage, 3, histSize, ranges, true, false);
   // ratio histogram
   divide(histModel, histImage, histRatio, 1.0, CV_32F);
   cv::min(histRatio, 1.0, histRatio);
   // backproject ratio histogram onto the image
   calcBackProject(&image32fc3, 1, channels, histRatio, backprj, ranges);
   // obtain a circular kernel having the same area as the model
   d = sqrt(4*model.rows*model.cols/CV_PI);
   kernel = getStructuringElement(MORPH_ELLIPSE, Size((int)d, (int)d));
   // convolve the kernel with the backprojected image
   filter2D(backprj, conv, CV_32F, kernel);
   // find the peak
   minMaxLoc(conv, NULL, &max, NULL, &maxPt);
   
   // display with color map
   // ratio histogram backprojected image
   backprj.convertTo(temp, CV_8U, 255);
   applyColorMap(temp, color, COLORMAP_JET);
   imshow("ratio histogram backprojection onto the image", color);
   // convolution result
   conv.convertTo(temp, CV_8U, 255/max);
   applyColorMap(temp, color, COLORMAP_JET);
   imshow("convolution with circular mask", color);
   // location of the peak
   image.copyTo(color);
   circle(color, Point(maxPt.x, maxPt.y), (int)(d/2), Scalar(0, 0, 255), 3);
   imshow("location of the peak", color);
   waitKey();
                                                
   return 0;      
}  