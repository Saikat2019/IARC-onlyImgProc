#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"                                 
#include <cmath>
#include "opencv2/video/tracking.hpp"
#include <vector>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

int thMin=115;  //thresholds for canny
int thMax=255;
int kernal_size=2;
int intersects=100;
Mat frame;

void updateVideo(int thMin,void *saikat)
    {
    	VideoCapture vid("botDetect2.avi");
    	while(1)
    	{
    		vid >> frame;
    		Mat gaussBlur;//contains img after gaussian blur
    		Mat canny1;  //contains img after canny
    		GaussianBlur( frame,gaussBlur, Size(2*kernal_size+1 ,2*kernal_size+1 ), 0, 0 );
    		Canny(gaussBlur,canny1,thMin,thMax,3);
    		vector<Vec2f> lines;
            HoughLines(canny1, lines, 1, CV_PI/360, intersects, 0, 0 );
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
                   line(frame, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
                }
    		imshow("Original",frame);
    		imshow("gaussBlur",gaussBlur);
    		imshow("canny1",canny1);
    		if(waitKey(30)>=0)break;

    	}
    	waitKey(0);

    }

int main()
    {
    	namedWindow("Original",WINDOW_NORMAL);
    	namedWindow("gaussBlur",WINDOW_NORMAL);
    	namedWindow("canny1",WINDOW_NORMAL);
    	createTrackbar("thMin","Original",&thMin,255,updateVideo);
    	createTrackbar("thMax","Original",&thMax,255,updateVideo);
    	createTrackbar("kernal_size","Original",&kernal_size,21,updateVideo);
    	createTrackbar("intersects","Original",&intersects,1000,updateVideo);
    	updateVideo(thMin,NULL);
    	//updateVideo(thMax,NULL);
    	return 0;
    }