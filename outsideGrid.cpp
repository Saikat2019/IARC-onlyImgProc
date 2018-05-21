#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <cmath>
#include <string>


using namespace cv;
using namespace std;

int main(int argc, char const *argv[])   
{
  
  Mat image=imread(argv[1]);
  
  //while(1)
  //{
	//namedWindow("original Image",WINDOW_NORMAL);
	//imshow("original Image",image);
	long double x,y,z;//coordinates of drone
    cout <<"enter the x coordinate \t";
    cin >>x;
    cout <<"enter the y coordinate \t";
    cin >>y;
    cout <<"enter the z coordinate \t";
    cin >>z;
  
    long double theta,alpha;
  
    cout<<"enter angle of camera axis with z axis >> theta(in degrees) = ";
    cin >>theta;

    cout<<"enter angle of view >> alpha(in degrees) = ";
    cin >>alpha;
   
    theta=(CV_PI*theta)/180.0;
    //cout <<"thete "<<theta;
    alpha=(CV_PI*alpha)/360.0;
    //cout <<"alpha "<<alpha<<endl;
    long double ltx,lty,lbx,lby,rtx,rty,rbx,rby;//ltx-left top x coordinate

    lby = rby = y + (z*(tan(theta-alpha)));
    lty = rty = y + (z*(tan(theta+alpha)));

    lbx = x - ((z*tan(alpha))/cos(theta-alpha));
    rbx = x + ((z*tan(alpha))/cos(theta-alpha));
    
    ltx = x - ((z*tan(alpha))/cos(theta+alpha));
    rtx = x + ((z*tan(alpha))/cos(theta+alpha));
   
    cout<<"lbx = "<<lbx<<"  lby = "<<lby<<"  rbx = "<<rbx<<"  rby = "<<rby<<endl;
    cout <<">>>>>>>>>>>>1"<<endl;
    cout<<"ltx = "<<ltx<<"  lty = "<<lty<<"  rtx = "<<rtx<<"  rty = "<<rty<<endl;
    if(lty>10.0)
    {
      int row = (image.rows*(10.0-lby))/(lty-lby);
      row = image.rows-row;
      for(int i=0;i<row;i++)
      {
        for(int j=0;j<image.cols;j++)
        {
            image.at<Vec3b>(i,j)[0]=0;
            image.at<Vec3b>(i,j)[1]=0;
            image.at<Vec3b>(i,j)[2]=0;
        }
      }
    }

    if(ltx<-10.0)
    {
      int colt=(image.cols*(rtx+10.0))/(rtx-ltx); //colt - top column
      colt=image.cols-colt;
      int colb=(image.cols*(rbx+10.0))/(rbx-lbx); //colb - bottom column
      colb=image.cols-colb;
      double x1,x2,y1,y2;
      x1=colt;x2=colb;
      y1=0;y2=-image.cols;
      if(x1!=x2)
      {
        double m=(y2-y1)/(x2-x1);
        double c=((x1*y2)-(x2*y1))/(x2-x1);
        for(int i=0;i<image.rows;i++)
        {
          for(int j=0;j<image.cols;j++)
          {
            double val=(c+-i-(m*j));
            if((val*c)>0.0)
            {
              image.at<Vec3b>(i,j)[0]=0;
              image.at<Vec3b>(i,j)[1]=0;
              image.at<Vec3b>(i,j)[2]=0;
            }
          }
        }
      }
      else
      {
        for(int i=0;i<image.rows;i++)
        {
          for(int j=0;j<colt;j++)
          {
              image.at<Vec3b>(i,j)[0]=0;
              image.at<Vec3b>(i,j)[1]=0;
              image.at<Vec3b>(i,j)[2]=0;
          }
        }
      }
    }
    
    if(rtx>10.0)
    {
      int colt=(image.cols*(10.0-ltx))/(rtx-ltx);
      int colb=(image.cols*(10.0-lbx))/(rbx-lbx);
      double x1,x2,y1,y2;
      x1=colt;x2=colb;
      y1=0;y2=-image.cols;
      if(x1!=x2)
      {
        double m=(y2-y1)/(x2-x1);
        double c=((x1*y2)-(x2*y1))/(x2-x1);
        for(int i=0;i<image.rows;i++)
        {
          for(int j=0;j<image.cols;j++)
          {
            double val=(c+-i-(m*j));
            if((val*c)<0.0)
            {
              image.at<Vec3b>(i,j)[0]=0;
              image.at<Vec3b>(i,j)[1]=0;
              image.at<Vec3b>(i,j)[2]=0;
            }
          }
        }
      }
      else
      {
        for(int i=0;i<image.rows;i++)
        {
          for(int j=colt;j<image.cols;j++)
          {
              image.at<Vec3b>(i,j)[0]=0;
              image.at<Vec3b>(i,j)[1]=0;
              image.at<Vec3b>(i,j)[2]=0;
          }
        } 
      }
    }
    if(lby<-10.0)
    {
      int row=(image.rows*(lby+10.0))/(lty-lby);
      for(int i=row;i<image.rows;i++)
      {
        for(int j=0;j<image.cols;j++)
        {
          image.at<Vec3b>(i,j)[0]=0;
          image.at<Vec3b>(i,j)[1]=0;
          image.at<Vec3b>(i,j)[2]=0;
        }
      }
    }
    namedWindow("only Grid",WINDOW_NORMAL);
    imshow("only Grid",image);
    waitKey(0);
  // }
	return 0;
}