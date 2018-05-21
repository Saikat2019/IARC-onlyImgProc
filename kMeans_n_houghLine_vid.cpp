#include "cv.h"
#include "highgui.h"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


#include <iostream>

using namespace cv;
using namespace std;

int thMin=25;  //thresholds for canny
int thMax=95;
int kernal_size=2;
int intersects=150;


int main() {
    Mat src; 
    namedWindow("original",WINDOW_NORMAL);
    namedWindow("blurred",WINDOW_NORMAL);
    namedWindow("clustered",WINDOW_NORMAL);
    namedWindow("detectedLines",WINDOW_NORMAL);

    VideoCapture vid("botDetect1.avi");

    while(1)
    {

    vid >> src;
    imshow("original", src);

    blur(src, src, Size(11,11));
    imshow("blurred", src);

    Mat p = Mat::zeros(src.cols*src.rows, 5, CV_32F);
    Mat bestLabels, centers, clustered;
    vector<Mat> bgr;
    cv::split(src, bgr);
    // i think there is a better way to split pixel bgr color
    for(int i=0; i<src.cols*src.rows; i++) {
        p.at<float>(i,0) = (i/src.cols) / src.rows;
        p.at<float>(i,1) = (i%src.cols) / src.cols;
        p.at<float>(i,2) = bgr[0].data[i] / 255.0;
        p.at<float>(i,3) = bgr[1].data[i] / 255.0;
        p.at<float>(i,4) = bgr[2].data[i] / 255.0;
    }

    int K = 2;
    kmeans(p, K, bestLabels,
            TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0),
            3, KMEANS_PP_CENTERS, centers);

    int colors[K];
    for(int i=0; i<K; i++) {
        colors[i] = 255/(i+1);
    }
    // i think there is a better way to do this mayebe some Mat::reshape?
    clustered = Mat(src.rows, src.cols, CV_32F);
    for(int i=0; i<src.cols*src.rows; i++) {
        clustered.at<float>(i/src.cols, i%src.cols) = (float)(colors[bestLabels.at<int>(0,i)]);
//      cout << bestLabels.at<int>(0,i) << " " << 
//              colors[bestLabels.at<int>(0,i)] << " " << 
//              clustered.at<float>(i/src.cols, i%src.cols) << " " <<
//              endl;
    }

    clustered.convertTo(clustered, CV_8U);
    imshow("clustered", clustered);
    //imwrite("twoCluster.png",clustered);
    Mat detectedLines(src.rows,src.cols,CV_8UC1,Scalar(0));
    Mat canny1; 
    Canny(src,canny1,thMin,thMax,3);
        vector<Vec2f> lines;
            HoughLines(canny1, lines, 1, CV_PI/360, intersects, 0, 0 );
            for( size_t i = 0; i < lines.size(); i++ )
               {
                   float rho = lines[i][0], theta = lines[i][1];
                   Point pt1, pt2;
                   double a = cos(theta), b = sin(theta);
                   double x0 = a*rho, y0 = b*rho;
                   pt1.x = cvRound(x0 + (src.cols)*(-b));
                   pt1.y = cvRound(y0 + (src.rows)*(a));
                   pt2.x = cvRound(x0 - (src.cols)*(-b));
                   pt2.y = cvRound(y0 - (src.rows)*(a));
                   line(detectedLines, pt1, pt2, Scalar(255), 3,8);// CV_AA);
                }
    imshow("detectedLines",detectedLines);

    if(waitKey(30)>=0)break;
    
    }
    waitKey(0);
    return 0;
}