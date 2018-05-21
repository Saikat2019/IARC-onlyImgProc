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

int thMin=25;  //thresholds for canny
int thMax=95;
int kernal_size=2;
int intersects=100;
Mat frame;

void updateImage(int thMin,void *saikat)
    {
      frame=imread("twoCluster.png",1);
      Mat detectedLines(frame.rows,frame.cols,CV_8UC1,Scalar(0));
      //VideoCapture vid("botDetect2.avi");
      //while(1)
      //{
        //vid >> frame;
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
                   pt1.x = cvRound(x0 + (frame.cols)*(-b));
                   pt1.y = cvRound(y0 + (frame.rows)*(a));
                   pt2.x = cvRound(x0 - (frame.cols)*(-b));
                   pt2.y = cvRound(y0 - (frame.rows)*(a));
                   line(detectedLines, pt1, pt2, Scalar(255), 3,8);// CV_AA);
                }
        imshow("Original",frame);
        imshow("gaussBlur",gaussBlur);
        imshow("canny1",canny1);
        imshow("detectedLines",detectedLines);
       // if(waitKey(30)>=0)break;

     // }
      waitKey(0);

    }

int main()
    {

      namedWindow("Original",WINDOW_NORMAL);
      namedWindow("gaussBlur",WINDOW_NORMAL);
      namedWindow("canny1",WINDOW_NORMAL);
      namedWindow("detectedLines",WINDOW_NORMAL);
      createTrackbar("thMin","Original",&thMin,255,updateImage);
      createTrackbar("thMax","Original",&thMax,255,updateImage);
      createTrackbar("kernal_size","Original",&kernal_size,21,updateImage);
      //createTrackbar("intersects","Original",&intersects,1000,updateVideo);
      updateImage(thMin,NULL);
      //updateVideo(thMax,NULL);
      return 0;
    }