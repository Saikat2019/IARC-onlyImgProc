#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

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
  	
  #if 1
    Mat imgHSV;
  	cvtColor(img,imgHSV,COLOR_BGR2HSV);
  	imshow("HSV",imgHSV);
  	Scalar min(low1,low2,low3);
  	Scalar max(high1,high2,high3);
  	inRange(imgHSV,min,max,imgHSV);
  	imshow("border",imgHSV);
  	//waitKey(0);
  	/*Canny(imgHSV,imgHSV,0,255,3);
  	vector<Vec2f> lines;
    HoughLines(imgHSV, lines, 1, CV_PI/180, 200, 0, 0 );
    
    for( size_t i = 0; i < lines.size(); i++ )
    {
     float rho = lines[i][0], theta = lines[i][1];
     Point pt1, pt2;
     double a = cos(theta), b = sin(theta);
     double x0 = a*rho, y0 = b*rho;
     pt1.x = cvRound(x0 + 1000*(-b));
     pt1.y = cvRound(y0 + 1000*(a));
     pt2.x = cvRound(x0 - 1000*(-b));
     pt2.y = cvRound(y0 - 1000*(a));
     line( img, pt1, pt2, Scalar(0,0,255), 10,8);
    }
  	imshow("border",imgHSV);*/
    
  #else
  	Mat imgBGR=img;
    Scalar min(low1,low2,low3);
    Scalar max(high1,high2,high3);
    inRange(imgBGR,min,max,imgBGR);
    imshow("border",imgBGR);
    //waitKey(0);
  	Canny(imgBGR,imgBGR,0,255,3);
  	vector<Vec2f> lines;
    HoughLines(imgBGR, lines, 1, CV_PI/180, 200, 0, 0 );
    
    for( size_t i = 0; i < lines.size(); i++ )
    {
     float rho = lines[i][0], theta = lines[i][1];
     Point pt1, pt2;
     double a = cos(theta), b = sin(theta);
     double x0 = a*rho, y0 = b*rho;
     pt1.x = cvRound(x0 + 1000*(-b));
     pt1.y = cvRound(y0 + 1000*(a));
     pt2.x = cvRound(x0 - 1000*(-b));
     pt2.y = cvRound(y0 - 1000*(a));
     line( img, pt1, pt2, Scalar(0,0,255), 10,8);
    }
  
  #endif	
  	
  }
  
int main(int argc,char const **argv)
  {
    namedWindow("main Image",CV_WINDOW_NORMAL);
    //namedWindow("BGR",CV_WINDOW_NORMAL);
    img=imread(argv[1]);
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
    
    
    
    waitKey(0);
    return 0;
  }