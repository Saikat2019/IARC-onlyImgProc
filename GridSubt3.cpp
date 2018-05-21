#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

Mat img;

int low1=0;   //H_lower threshold
int high1=179; //H_higher threshold
int low2=0;    //S_lower threshold
int high2=162; //S_higher threshold
int low3=51;   //V_lower threshold
int high3=255; //V_higher threshold

void gridSubt(Point p1,Point p2)
  {
    
    cout<<"xxx p1 = "<<p1<<" p2 = "<<p2<<endl;
    Point checkpt(img.cols/2,img.rows/2);
    namedWindow("only grid",CV_WINDOW_NORMAL);
    double val,val_point;
    val=((double)(checkpt.x-p1.x)/(double)(checkpt.y-p1.y))-((double)(p2.x-p1.x)/(double)(p2.y-p1.y));
    cout<<"xxx"<<val<<endl;
    
    for (int i = 0; i < img.rows; i++)
    {
      for (int j = 0; j < img.cols; j++)
      {
        
        
         val_point=((double)(j-p1.x)/(double)(i-p1.y))-((double)(p2.x-p1.x)/(double)(p2.y-p1.y));
        
        
        if((val*val_point)<0.0)
        {
          img.at<Vec3b>(i,j)[0]=0;
          img.at<Vec3b>(i,j)[1]=0;
          img.at<Vec3b>(i,j)[2]=0;
          
        }
        
      }
    }
    
    imshow("only grid",img);


  }
  

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
    int erosion_size=5; //size of the kernel will be erosion_size*2+1
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

    double rho_horizontal=-1,rho_vertical=img.cols+5,theta_horizontal=CV_PI/2.0,theta_vertical=CV_PI;

    for( size_t i = 0; i < lines.size(); i++ )
    {
     double rho = lines[i][0], theta = lines[i][1];
     cout <<" r = "<<rho<<" theta(degree) = "<<((theta*180.0)/CV_PI)<<"\n";
     Point pt1, pt2;
     double a = cos(theta), b = sin(theta);
     double x0 = a*rho, y0 = b*rho;
     pt1.x = cvRound(x0 + 100000*(-b));
     pt1.y = cvRound(y0 + 100000*(a));
     pt2.x = cvRound(x0 - 100000*(-b));
     pt2.y = cvRound(y0 - 100000*(a));
     cout<<" pt1.x = "<<pt1.x<<" pt1.y = "<<pt1.y<<" pt2.x = "<<pt2.x<<" pt2.y = "<<pt2.y<<"\n";
     line( imgHSV, pt1, pt2, Scalar(255,0,0), 2,8);
     
     gridSubt(pt1,pt2);
     imshow("border1",imgHSV);
       
    }
    
  
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