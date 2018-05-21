#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat img;

int low1=75;
int high1=179;
int low2=0;
int high2=162;
int low3=51;
int high3=255;


void borderLines()
  {
  	namedWindow("HSV",CV_WINDOW_NORMAL);
  	namedWindow("border",CV_WINDOW_NORMAL);
    namedWindow("border1",CV_WINDOW_NORMAL);
  	
    Mat imgHSV;
    
  	cvtColor(img,imgHSV,COLOR_BGR2HSV);
  	imshow("HSV",imgHSV);
  	Scalar min(low1,low2,low3);
  	Scalar max(high1,high2,high3);
  	inRange(imgHSV,min,max,imgHSV);
  	
    int erosion_type;
    int erosion_size=5;
    erosion_type = MORPH_RECT; 
    //erosion_type = MORPH_CROSS;
    //erosion_type = MORPH_ELLIPSE;

    Mat element = getStructuringElement( erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );

    /// Apply the erosion operation
    dilate( imgHSV, imgHSV , element );
  
    imshow("border",imgHSV);

    Canny(imgHSV,imgHSV,0,255,3);
    vector<Vec2f> lines;
    HoughLines(imgHSV, lines, 1, CV_PI/180, 100, 0, 0 );
    
    for( size_t i = 0; i < lines.size(); i++ )
    {
     float rho = lines[i][0], theta = lines[i][1];
     cout <<" r = "<<rho<<" theta(degree) = "<<((theta*180.0)/CV_PI)<<"\n";
     Point pt1, pt2;
     double a = cos(theta), b = sin(theta);
     double x0 = a*rho, y0 = b*rho;
     pt1.x = cvRound(x0 + 1000*(-b));
     pt1.y = cvRound(y0 + 1000*(a));
     pt2.x = cvRound(x0 - 1000*(-b));
     pt2.y = cvRound(y0 - 1000*(a));
     line( imgHSV, pt1, pt2, Scalar(255,0,0), 2,8);
    }
    imshow("border1",imgHSV);
  	
    
 	
  }
  
int main(int argc,char const **argv)
  {
    namedWindow("main Image",CV_WINDOW_NORMAL);
    //namedWindow("BGR",CV_WINDOW_NORMAL);
    img=imread(argv[1]);
    imshow("main Image",img);

    borderLines();


    
    
    
    
    
    waitKey(0);
    return 0;
  }