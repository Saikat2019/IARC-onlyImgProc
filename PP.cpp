#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include<stdio.h>
#include<iostream>

using namespace cv;
using namespace std;

int m,n;
Mat img;

Mat Final(1000,1000,CV_8UC3,Scalar(0,0,0));   //where the final image will be created

int i=0;                 //to calculate no of corners
Point2f Corners[4];       //to store the corners

void onMouse(int evt,int x,int y,int flags,void* param)     //onmose for the call back function
   {
      if(evt==CV_EVENT_LBUTTONDOWN)
        {
           Corners[i].x=x;
           Corners[i].y=y;
           circle(img,Corners[i],15,Scalar( 0, 0, 255 ),-1);
           ++i;
        }
   }

int main(int argc,char *argv[])
    {
       img=imread(argv[1]);
       int n;
       namedWindow("win",WINDOW_NORMAL);
       setMouseCallback("win",onMouse,NULL);
       cout<< "\n\n>>>>>>>>>>>>>> select the corners by left click and then Press any key to continue >>>> "<<endl;
        while(1)
       {
        imshow("win",img);
        waitKey(1);
        if(i>=4)break;                               //will continue untill we get four corners
       }
    
       vector<Point2f> src_img;
       
       vector<Point2f> dst_img;
       
       for(n=0;n<4;n++)
          {
            src_img.push_back(Corners[n]);
          }

       dst_img.push_back(Point2f(0,0));
       dst_img.push_back(Point2f(1000,0));
       dst_img.push_back(Point2f(1000,1000));
       dst_img.push_back(Point2f(0,1000));
       
       Mat persMat = getPerspectiveTransform(src_img, dst_img);
             
       for(int I=0;I<persMat.rows;I++)
           for(int J=0;J<persMat.cols;J++)
               cout<<"{{{{{{{{{{{{{{{{ "<<persMat.at<Vec3b>(I,J)<<endl;
       
       warpPerspective(img,Final,persMat, Final.size()); 
                  
       namedWindow("Final",WINDOW_NORMAL);       
       
       imshow("Final",Final);
       
       waitKey(0);                 
    }   