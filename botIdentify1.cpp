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
   char  Image1[]="ImagesToCompare/Image";
   char  Image2[]=".png";

   string Image=string(Image1)+string(argv[1])+string(Image2);

   char Model1[]="botColors/botColor";
   char Model2[]=".png";

   string Model=string(Model1)+string(argv[2])+string(Model2);
   
   Mat image = imread(Image);
   Mat model = imread(Model);
   


   namedWindow("model",WINDOW_NORMAL);
   namedWindow("location of the peak",WINDOW_NORMAL);
   


   int bins = 16; //no of bins for histogram

   double d, Min,max,max1;

   Point maxPt;

   Mat histModel, histImage, histRatio;
   Mat model32fc3, image32fc3, backprj, kernel, conv;
   Mat temp, color;

   const int channels[] = {0, 1, 2};
   const int histSize[] = {bins, bins, bins};
   const float rgbRange[] = {0, 256};
   const float* ranges[] = {rgbRange, rgbRange, rgbRange};

   //converting 8UC3 to 32FC3 for precision in calculation
   model.convertTo(model32fc3, CV_32FC3);

   calcHist(&model32fc3, 1, channels, Mat(), histModel, 3, histSize, ranges, true, false);

   //converting 8UC3 to 32FC3 for precision in calculation
   image.convertTo(image32fc3, CV_32FC3);

   calcHist(&image32fc3, 1, channels, Mat(), histImage, 3, histSize, ranges, true, false);
   
   divide(histModel, histImage, histRatio, 1.0, CV_32F);
   min(histRatio, 1.0, histRatio);
   calcBackProject(&image32fc3, 1, channels, histRatio, backprj, ranges);
   d = sqrt(4*model.rows*model.cols/CV_PI);
   kernel = getStructuringElement(MORPH_ELLIPSE,Size(50,50)); //Size((int)d, (int)d));
   filter2D(backprj, conv, CV_32F, kernel);
   minMaxLoc(conv, &Min, &max, NULL, &maxPt);

   cout <<sizeof(CV_32F)<<endl;
   
   cout <<"\n\n>>>>>>>>   max value\t" << max<<"min val "<<Min<<endl;

   namedWindow("ratio histogram backprojection onto the image",WINDOW_NORMAL);
   namedWindow("convolution with circular mask",WINDOW_NORMAL);
   backprj.convertTo(temp, CV_8U, 255);
   applyColorMap(temp, color, COLORMAP_JET);//BONE//WINTER//RAINBOW 've to think 
   imshow("ratio histogram backprojection onto the image", color);
   conv.convertTo(temp, CV_8U, 255/max);
   minMaxLoc(temp, NULL, &max1, NULL, &maxPt);
   cout <<"\n\n>>>>>>>>   max1 value\t" << max1<<endl;

   applyColorMap(temp, color, COLORMAP_JET);//OCEAN//SUMMER//COOL//PINK//HOT
   imshow("convolution with circular mask", color);
     
   if(max>10.0)
   {
     image.copyTo(color);
     circle(color, Point(maxPt.x, maxPt.y), (int)100,/*(d/2),*/ Scalar(0, 0, 255), 10);
     imshow("location of the peak", color);
   }
   else
   {
     imshow("location of the peak", image);      
    cout << "\n\n\n>>>>>>>   bot not detected\n\n\n"<<endl;
   }
   imshow("model",model);
   waitKey(0);
   return 0;      
}  