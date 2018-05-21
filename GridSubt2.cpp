#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

Mat img;

int low1=75;   //H_lower threshold
int high1=179; //H_higher threshold
int low2=0;    //S_lower threshold
int high2=162; //S_higher threshold
int low3=51;   //V_lower threshold
int high3=255; //V_higher threshold

void gridSubt_vertical_right(Point p1,Point p2)
  {
    
    cout<<"xxx p1 = "<<p1<<" p2 = "<<p2<<endl;
    Point checkpt(img.cols/2,img.rows/2);
    namedWindow("only grid",CV_WINDOW_NORMAL);
    float val,val_point;
    val=((float)(checkpt.x-p1.x)/(float)(checkpt.y-p1.y))-((float)(p2.x-p1.x)/(float)(p2.y-p1.y));
    cout<<"xxx"<<val<<endl;
    
    for (int i = 0; i < img.rows; i++)
    {
      for (int j = 0; j < img.cols; j++)
      {
        
        if(j!=p1.x && p2.x!=p1.x)
        {
          
        val_point=((float)(j-p1.x)/(float)(i-p1.y))-((float)(p2.x-p1.x)/(float)(p2.y-p1.y));
        
        
        if((val*val_point)<0.0)
        {
          img.at<Vec3b>(i,j)[0]=0;
          img.at<Vec3b>(i,j)[1]=0;
          img.at<Vec3b>(i,j)[2]=0;
        }
        }
      }
    }
    
    imshow("only grid",img);


  }
void gridSubt_horizontal_up(Point p1,Point p2)
  {
    cout<<"*** p1 = "<<p1<<" p2 = "<<p2<<endl;
    Point checkpt(img.cols,img.rows);
    namedWindow("only grid1",CV_WINDOW_NORMAL);
    float val,val_point;
    val=((float)(checkpt.y-p1.y)/(float)(checkpt.x-p1.x))-((float)(p2.y-p1.y)/(float)(p2.x-p1.x));
    cout<<"***"<<val<<endl;
    
    for (int i = 0; i < img.rows; i++)
    {
      for (int j = 0; j < img.cols; j++)
      {
        
        if(j!=p1.x && p2.x!=p1.x)
        {
          
        val_point=((float)(i-p1.y)/(float)(j-p1.x))-((float)(p2.y-p1.y)/(float)(p2.x-p1.x));
        
        
        if((val*val_point)<0.0)
        {
          img.at<Vec3b>(i,j)[0]=0;
          img.at<Vec3b>(i,j)[1]=0;
          img.at<Vec3b>(i,j)[2]=0;
        }
        }
      }
    }
    
    imshow("only grid1",img);

  }  

void borderLines()
  {
  	namedWindow("HSV",CV_WINDOW_NORMAL);
  	namedWindow("border",CV_WINDOW_NORMAL);
    namedWindow("border1",CV_WINDOW_NORMAL);
    namedWindow("border2",CV_WINDOW_NORMAL);
  	
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

    float rho_horizontal=-1,rho_vertical=img.cols+5,theta_horizontal=CV_PI/2.0,theta_vertical=CV_PI;

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
     cout<<" pt1.x = "<<pt1.x<<" pt1.y = "<<pt1.y<<" pt2.x = "<<pt2.x<<" pt2.y = "<<pt2.y<<"\n";
     line( imgHSV, pt1, pt2, Scalar(255,0,0), 2,8);
     imshow("border1",imgHSV);
     
     gridSubt_vertical_right(pt1,pt2);

     if((90-((theta*180.0)/CV_PI))<50)
     {
      if(rho_horizontal<abs(rho))
        {
          rho_horizontal=rho;
          theta_horizontal=theta;
        }
     }

     if((180-((theta*180.0)/CV_PI))<50)
     {
       if(rho_vertical>abs(rho))
       {
        rho_vertical=rho;
        theta_vertical=theta;
       }
     }
  
    }
    Point pt1_horizontal, pt2_horizontal;
    double a_horizontal = cos(theta_horizontal), b_horizontal = sin(theta_horizontal);
    double x0_horizontal = a_horizontal*rho_horizontal, y0_horizontal = b_horizontal*rho_horizontal;
    pt1_horizontal.x = cvRound(x0_horizontal + 1000*(-b_horizontal));
    pt1_horizontal.y = cvRound(y0_horizontal + 1000*(a_horizontal));
    pt2_horizontal.x = cvRound(x0_horizontal - 1000*(-b_horizontal));
    pt2_horizontal.y = cvRound(y0_horizontal - 1000*(a_horizontal));
    //cout<<" pt1.x = "<<pt1.x<<" pt1.y = "<<pt1.y<<" pt2.x = "<<pt2.x<<" pt2.y = "<<pt2.y<<"\n";
    line( img, pt1_horizontal, pt2_horizontal, Scalar(255,0,0), 2,8);
    //gridSubt_horizontal_up(pt1_horizontal,pt2_horizontal);
    

    Point pt1_vertical, pt2_vertical;
    double a_vertical = cos(theta_vertical), b_vertical = sin(theta_vertical);
    double x0_vertical = a_vertical*rho_vertical, y0_vertical = b_vertical*rho_vertical;
    pt1_vertical.x = cvRound(x0_vertical + 1000*(-b_vertical));
    pt1_vertical.y = cvRound(y0_vertical + 1000*(a_vertical));
    pt2_vertical.x = cvRound(x0_vertical - 1000*(-b_vertical));
    pt2_vertical.y = cvRound(y0_vertical - 1000*(a_vertical));
    //cout<<" pt1.x = "<<pt1.x<<" pt1.y = "<<pt1.y<<" pt2.x = "<<pt2.x<<" pt2.y = "<<pt2.y<<"\n";
    line( img, pt1_vertical, pt2_vertical, Scalar(255,0,0), 2,8);

   // gridSubt_vertical_right(pt1_vertical,pt2_vertical);
  	
    imshow("border2",img);
 	
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