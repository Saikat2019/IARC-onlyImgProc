#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include<iostream>

using namespace cv;
using namespace std;

RNG rng(12345);
Mat image;
Mat checkit(Mat ,Mat);
int maximum(int a,int b);

int main(int argc, char const *argv[])
   {
      char Model1[]="botColors/botColor";
      char Model2[]=".png";

      string Model=string(Model1)+string(argv[2])+string(Model2);
   

      namedWindow("detectedBot",WINDOW_NORMAL);
      
      Mat greenBot=imread(Model);
      
      VideoCapture vid(argv[1]);
      while(1)
      {
         vid >> image;
      
         Mat findGreenBot = checkit(greenBot, image);
      
        imshow("detectedBot",findGreenBot);
      
        waitKey(0);//if(waitKey(1000)>=0)waitKey(0);
      }
    
    waitKey(0);
   
   return 0;
   }

Mat checkit(Mat model ,Mat image1)
  {
   namedWindow("ratio histogram backprojection onto the image",WINDOW_NORMAL);
   namedWindow("convolution with circular HSV mask",WINDOW_NORMAL);
   namedWindow("convolution with circular BGR mask",WINDOW_NORMAL);
   namedWindow("location of the peak",WINDOW_NORMAL);
   namedWindow("probable bot position",WINDOW_NORMAL);
   namedWindow("contours",WINDOW_NORMAL);
   namedWindow("model",WINDOW_NORMAL);


   int bins = 16;
   double d, max;
   Point maxPt;

   Mat histModel, histImage, histRatio;
   Mat model32fc3, image32fc3, backprj, kernel, conv;

   Mat temp,temp1, colorHSV,colorBGR,colorBinary;//bot;

   const int channels[] = {0, 1, 2};
   const int histSize[] = {bins, bins, bins};
   const float rgbRange[] = {0, 256};
   const float* ranges[] = {rgbRange, rgbRange, rgbRange};

   model.convertTo(model32fc3, CV_32FC3);
   calcHist(&model32fc3, 1, channels, Mat(), histModel, 3, histSize, ranges, true, false);

   image.convertTo(image32fc3, CV_32FC3);
   calcHist(&image32fc3, 1, channels, Mat(), histImage, 3, histSize, ranges, true, false);

   divide(histModel, histImage, histRatio, 1.0, CV_32F);
   
   
  
   calcBackProject(&image32fc3, 1, channels, histRatio, backprj, ranges);

   d =maximum(model.rows,model.cols)/2;   
   kernel = getStructuringElement(MORPH_ELLIPSE,Size(20,20));// Size((int)d, (int)d));
   
   filter2D(backprj, conv, CV_32F, kernel);
   
   minMaxLoc(conv, NULL, &max, NULL, &maxPt);

   cout <<"\n\n>>>>>>>>   max value\t" << max<<endl;
   
   backprj.convertTo(temp, CV_8U, 255);
   applyColorMap(temp, colorHSV, COLORMAP_JET); //BONE//WINTER//RAINBOW 've to think in a new way'
   imshow("ratio histogram backprojection onto the image", colorHSV);
   
   conv.convertTo(temp, CV_8U, 255/max);
   applyColorMap(temp, colorHSV, COLORMAP_JET);//OCEAN//SUMMER//COOL//PINK//HOT
   cvtColor(colorHSV, colorBGR,COLOR_HSV2BGR);
   cvtColor(colorBGR, colorBGR,COLOR_BGR2GRAY);
   threshold(colorBGR, colorBinary, 1, 255,THRESH_BINARY);
   Mat element=getStructuringElement(MORPH_ELLIPSE,Size(5,5),Point(2,2));
   erode(colorBinary,colorBinary,element);

   
   Mat canny_output;
   vector< vector<Point> > contours;
   vector<Vec4i> hierarchy;

   Canny(colorBinary,canny_output,1,255,3);
   findContours(canny_output,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0,0));
   Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
   int no_of_contours=contours.size();

       for(int i=0;i<no_of_contours;i++)
           {
              Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
              drawContours(drawing,contours,i,color,2,8,hierarchy,0,Point());
           }
   imshow("convolution with circular HSV mask", colorHSV);
   imshow("convolution with circular BGR mask", colorBGR);
   imshow("probable bot position", colorBinary);
   imshow("contours", drawing);
   imshow("model",model);

   cout <<">>>> no_of_contours = "<<no_of_contours<<endl;
   
   image.copyTo(image1);
   if(no_of_contours<3 && max>50.0)
   {
       
       circle(image1, Point(maxPt.x, maxPt.y), (int)(d/2), Scalar(0, 0, 255), 3);
       
   }
   imshow("location of the peak", image1);       
   return image1;

   }

   int maximum(int a,int b)
   {
      if(a>b)return a;
      else return b;
   }