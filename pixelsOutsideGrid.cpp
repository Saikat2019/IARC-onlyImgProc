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
	//namedWindow("original Image",WINDOW_NORMAL);
	//imshow("original Image",image);
	long double x,y,z;//coordinates of drone
    cout <<"enter the x coordinate \t";
    cin >>x;
    cout <<"enter the y coordinate \t";
    cin >>y;
    cout <<"enter the z coordinate \t";
    cin >>z;
    
    long double length_in_meters,width_in_meters;
    long double length_in_pixels,width_in_pixels;

    long double theta,alpha;

    cout<<"enter angle of camera axis with z axis >> theta(in degrees) = ";
    cin >>theta;

    cout<<"enter angle of view >> alpha(in degrees) = ";
    cin >>alpha;
   
    theta=(CV_PI*theta)/180.0;
    //cout <<"thete "<<theta;
    alpha=(CV_PI*alpha)/180.0;
    //cout <<"alpha "<<alpha<<endl;
    //cout <<"tan(theta+alpha) "<<tan(theta+alpha)<<"\ttan(theta-alpha)  "<<tan(theta-alpha)<<endl;
    
    length_in_pixels=image.cols;
    width_in_pixels=image.rows;

    length_in_meters= 2.0*z*(tan(alpha)/cos(theta));
    cout<<"length_in_meters = "<<length_in_meters<<endl;
    
    width_in_meters=z*((long double)tan(theta+alpha)-(long double)tan(theta-alpha));
    cout<<"width_in_meters = "<<width_in_meters<<endl;
    
    long double yCoor_of_lowerRow,yCoor_of_upperRow;
    long double xCoor_of_leftColumn,xCoor_of_rightColumn;

    yCoor_of_lowerRow=y+(z*tan(theta-alpha));
    yCoor_of_upperRow=y+(z*tan(theta+alpha));

    xCoor_of_leftColumn=x-(z*(tan(alpha)/cos(theta)));
    xCoor_of_rightColumn=x+(z*(tan(alpha)/cos(theta)));

    cout <<"yCoor_of_lowerRow "<<yCoor_of_lowerRow<<endl;
    cout <<"yCoor_of_upperRow "<<yCoor_of_upperRow<<endl;
    cout <<"xCoor_of_leftColumn "<<xCoor_of_leftColumn<<endl;
    cout <<"xCoor_of_rightColumn "<<xCoor_of_rightColumn<<endl;

    if(yCoor_of_lowerRow<-10.0)
    {
      int row=width_in_pixels*((10.0+yCoor_of_upperRow)/width_in_meters);
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

    if(yCoor_of_upperRow>10.0)
    {
      int row=width_in_pixels*((yCoor_of_upperRow-10.0)/width_in_meters);
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
    if(xCoor_of_leftColumn<-10.0)
    {
    	int col=length_in_pixels*((-xCoor_of_leftColumn-10.0)/length_in_meters);
    	for(int i=0;i<image.rows;i++)
      	{
      	  for(int j=0;j<col;j++)
      	  {
      	  	image.at<Vec3b>(i,j)[0]=0;
      	  	image.at<Vec3b>(i,j)[1]=0;
      	  	image.at<Vec3b>(i,j)[2]=0;
      	  }
        }
    }  
    if(xCoor_of_rightColumn>10.0)
    {
    	int col=length_in_pixels*((10.0-xCoor_of_leftColumn)/length_in_meters);
        for(int i=0;i<image.rows;i++)
      	{
      	  for(int j=col;j<image.cols;j++)
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
	return 0;
}