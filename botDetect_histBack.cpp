#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include<iostream>

using namespace cv;
using namespace std;

int main()   
{ 
   Mat model = imread("bot5.png"); //model image
   Mat image = imread("histBack9.png");//where to search


   namedWindow("model",WINDOW_NORMAL);
   namedWindow("ratio histogram backprojection onto the image",WINDOW_NORMAL);
   namedWindow("convolution with circular mask",WINDOW_NORMAL);
   namedWindow("location of the peak",WINDOW_NORMAL);


   int bins = 32; //no of bins for histogram

   double d, max;

   Point maxPt;

   MatND histModel, histImage, histRatio;
   Mat model32fc3, image32fc3, backprj, kernel, conv;
   Mat temp, color;

   const int channels[] = {0, 1, 2};
   const int histSize[] = {bins, bins, bins};
   const float rgbRange[] = {0, 256};
   const float* ranges[] = {rgbRange, rgbRange, rgbRange};

   //converting 8UC3 to 32FC3 for precision in calculation
   model.convertTo(model32fc3, CV_32FC3);

   calcHist(&model32fc3, 1, channels, Mat(), histModel, 3, histSize, ranges, true, false);

   //converting 8UC3 to 32FC3 for precision in calculation
   image.convertTo(image32fc3, CV_32FC3);

   calcHist(&image32fc3, 1, channels, Mat(), histImage, 3, histSize, ranges, true, false);
   
   divide(histModel, histImage, histRatio, 1.0, CV_32F);
   min(histRatio, 1.0, histRatio);
   calcBackProject(&image32fc3, 1, channels, histRatio, backprj, ranges);
   d = sqrt(4*model.rows*model.cols/CV_PI);
   kernel = getStructuringElement(MORPH_ELLIPSE, Size((int)d, (int)d));
   filter2D(backprj, conv, CV_32F, kernel);
   minMaxLoc(conv, NULL, &max, NULL, &maxPt);
   backprj.convertTo(temp, CV_8U, 255);
   applyColorMap(temp, color, COLORMAP_JET);//BONE//WINTER//RAINBOW 've to think 
   imshow("ratio histogram backprojection onto the image", color);
   conv.convertTo(temp, CV_8U, 255/max);
   applyColorMap(temp, color, COLORMAP_JET);//OCEAN//SUMMER//COOL//PINK//HOT
   imshow("convolution with circular mask", color);
   image.copyTo(color);
   circle(color, Point(maxPt.x, maxPt.y), (int)(d/2), Scalar(0, 0, 255), 3);
   imshow("location of the peak", color);
   imshow("model",model);
   waitKey(0);
   return 0;      
}  