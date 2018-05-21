#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;

Mat img;

int low1=75;
int high1=179;
int low2=0;
int high2=162;
int low3=51;
int high3=255;


void borderLines(int low1,void *s)
  {
  	namedWindow("HSV",CV_WINDOW_NORMAL);
  	namedWindow("border",CV_WINDOW_NORMAL);
    Mat imgHSV;
  	cvtColor(img,imgHSV,COLOR_BGR2HSV);
  	imshow("HSV",imgHSV);
  	Scalar min(low1,low2,low3);
  	Scalar max(high1,high2,high3);
  	inRange(imgHSV,min,max,imgHSV);
  	imshow("border",imgHSV);

  }
  
int main(int argc,char const **argv)
  {
    namedWindow("main Image",CV_WINDOW_NORMAL);
    //namedWindow("BGR",CV_WINDOW_NORMAL);
    VideoCapture vid(0);
    while(1)
    {
      
    imshow("main Image",img);
    
    createTrackbar("low1","main Image",&low1,255,borderLines);
    borderLines(low1,NULL);
    
    createTrackbar("high1","main Image",&high1,255,borderLines);
    borderLines(high1,NULL);
    
    createTrackbar("low2","main Image",&low2,255,borderLines);
    borderLines(low2,NULL);
    
    createTrackbar("high2","main Image",&high2,255,borderLines);
    borderLines(high2,NULL);
    
    createTrackbar("low3","main Image",&low3,255,borderLines);
    borderLines(low3,NULL);
    
    createTrackbar("high3","main Image",&high3,255,borderLines);
    borderLines(high3,NULL);
    
    waitKey(30);
    }
    
    waitKey(0);
    return 0;
  }