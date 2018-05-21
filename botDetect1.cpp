#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include<iostream>

using namespace cv;
using namespace std;

RNG rng(12345);
Mat image;
Mat checkit(Mat ,Mat);

int main(int argc, char const *argv[])
   {

    namedWindow("detectedBot",WINDOW_NORMAL);
   	
   	Mat greenBot=imread("bot1.png",1);
   	Mat redBot=imread("bot5.png",1);
   	Mat ObstacleBot=imread("bot3.png",1);
   	VideoCapture vid("botDetect1.avi");
   	while(1)
   	{
   		vid >> image;
   		Mat findObstacleBot = checkit(ObstacleBot,image);
   		Mat findGreenBot = checkit(greenBot, findObstacleBot);
   		Mat findRedBot = checkit(redBot,findGreenBot);
        imshow("detectedBot",findRedBot);
        waitKey(0);//if(waitKey(30)>=0)break;
   	}
    
    waitKey(0);
	
	return 0;
   }

Mat checkit(Mat model ,Mat image1)
  {
   namedWindow("ratio histogram backprojection onto the image",WINDOW_NORMAL);
   namedWindow("convolution with circular HSV mask",WINDOW_NORMAL);
   namedWindow("convolution with circular BGR mask",WINDOW_NORMAL);
   namedWindow("location of the peak",WINDOW_NORMAL);
   namedWindow("probable bot position",WINDOW_NORMAL);
   namedWindow("contours",WINDOW_NORMAL);
   //Mat model;// = imread("bot3.png");
   //Mat image;// = imread("histBack12.png");

   int bins = 32;
   double d, max;
   Point maxPt;

   Mat histModel, histImage, histRatio;
   Mat model32fc3, image32fc3, backprj, kernel, conv;

   Mat temp,temp1, colorHSV,colorBGR,colorBinary;//bot;

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
   
   
   /*cv::*/min(histRatio, 1.0, histRatio);
   // backproject ratio histogram onto the image
   calcBackProject(&image32fc3, 1, channels, histRatio, backprj, ranges);
   // obtain a circular kernel having the same area as the model
   d =model.rows;// sqrt(4*model.rows*model.cols/CV_PI);
   cout<<"d" << d << endl;
   kernel = getStructuringElement(MORPH_ELLIPSE, Size((int)d, (int)d));
   // convolve the kernel with the backprojected image
   filter2D(backprj, conv, CV_32F, kernel);
   // find the peak
   minMaxLoc(conv, NULL, &max, NULL, &maxPt);

   // display with color map
   // ratio histogram backprojected image
   backprj.convertTo(temp, CV_8U, 255);
   applyColorMap(temp, colorHSV, COLORMAP_JET);
   imshow("ratio histogram backprojection onto the image", colorHSV);
   // convolution result
   conv.convertTo(temp, CV_8U, 255/max);
   applyColorMap(temp, colorHSV, COLORMAP_JET);
   cvtColor(colorHSV, colorBGR,COLOR_HSV2BGR);
   cvtColor(colorBGR, colorBGR,COLOR_BGR2GRAY);
   threshold(colorBGR, colorBinary, 1, 255,THRESH_BINARY);
   Mat element=getStructuringElement(MORPH_ELLIPSE,Size(5,5),Point(2,2));
   erode(colorBinary,colorBinary,element);

   //no's of contours
   Mat canny_output;
   vector< vector<Point> > contours;
   vector<Vec4i> hierarchy;

   Canny(colorBinary,canny_output,1,255,3);
   findContours(canny_output,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
   Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
   int no_of_contours=contours.size();
   cout<<no_of_contours<<endl;
       for(int i=0;i<no_of_contours;i++)
           {
              Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
              drawContours(drawing,contours,i,color,2,8,hierarchy,0,Point());
           }
   imshow("convolution with circular HSV mask", colorHSV);
   imshow("convolution with circular BGR mask", colorBGR);
   imshow("probable bot position", colorBinary);
   imshow("contours", drawing);
   // location of the peak
   image.copyTo(image1);
   if(no_of_contours<4)
   {
       
       circle(image1, Point(maxPt.x, maxPt.y), (int)(d/2), Scalar(0, 0, 255), 3);
       
   }
   imshow("location of the peak", image1);       
   return image1;

   }