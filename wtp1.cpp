#include <stdio.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;
double addy,paddy;

void mouseCB(int event, int x, int y, int flags, void* userdata)
{
	if  ( event == EVENT_LBUTTONDOWN )
	{
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		addy=(double)x;
		paddy=(double)y;
	}
}
int main(int argc, char const *argv[])
{
	Mat img=imread(argv[1],CV_LOAD_IMAGE_COLOR);
	namedWindow("Show", CV_WINDOW_NORMAL );
	imshow("Show",img);
	waitKey(0);

	vector<Point2f> imagePointsPix;
	vector<Point3f> objectPointsm;

	
	imagePointsPix.push_back(Point2f(104., 161.));//\123., 160.));//110., 174.));//136.,288.));
	imagePointsPix.push_back(Point2f(299., 163.));//\305., 164.));//318., 174.));//299.,288.));
    imagePointsPix.push_back(Point2f(507., 161.));//\510., 164.));//510., 176.));//451.,281.));
	imagePointsPix.push_back(Point2f(470., 52.));//\478., 62.));//619.,276.));
    imagePointsPix.push_back(Point2f(291., 347.));//\\8., 54.));//\//28.,176.));
	imagePointsPix.push_back(Point2f(147., 54.));//\\163., 56.));//\153., 64.));//168.,176.));
	imagePointsPix.push_back(Point2f(302., 52.));//\\312., 59.));//\323., 64.));//299.,171.));
	imagePointsPix.push_back(Point2f(574., 347.));//\\633., 60.));//\470., 64.));//430.,169.));
	/*imagePointsPix.push_back(Point2f(632., 64.));//566.,171.));
	imagePointsPix.push_back(Point2f(70.,96.));
	imagePointsPix.push_back(Point2f(187.,91.));
	imagePointsPix.push_back(Point2f(307.,89.));
	imagePointsPix.push_back(Point2f(408.,96.));
	imagePointsPix.push_back(Point2f(529.,89.));
	imagePointsPix.push_back(Point2f(636.,86.));*/


	objectPointsm.push_back(Point3f(-1., 2., 0.));
	objectPointsm.push_back(Point3f(0., 2., 0.));
	objectPointsm.push_back(Point3f(1., 2., 0.));
	objectPointsm.push_back(Point3f(1., 3., 0.));
	objectPointsm.push_back(Point3f(0., 1., 0.));
	objectPointsm.push_back(Point3f(-1., 3., 0.));
	objectPointsm.push_back(Point3f(0., 3., 0.));
	objectPointsm.push_back(Point3f(1., 1., 0.));//
	/*objectPointsm.push_back(Point3f(2., 3., 0.));
	objectPointsm.push_back(Point3f(-2., 4., 0.));
	objectPointsm.push_back(Point3f(-1., 4., 0.));
	objectPointsm.push_back(Point3f(0., 4., 0.));
	objectPointsm.push_back(Point3f(1., 4., 0.));
	objectPointsm.push_back(Point3f(2., 4., 0.));
	objectPointsm.push_back(Point3f(3., 4., 0.));*/

	Mat rvec(1,3,DataType<double>::type);
	Mat tvec(1,3,DataType<double>::type);
	Mat rotationMatrix(3,3,DataType<double>::type);
	Mat rotationMatrix2(3,3,DataType<double>::type);
	Mat distCoeffs(4,1,DataType<double>::type);
	Mat cameraMatrix(3,3,DataType<double>::type);	
	Mat inlin;
	setIdentity(cameraMatrix);
	cameraMatrix.at<double>(0,0)=cameraMatrix.at<double>(1,1)=554.594;
	cameraMatrix.at<double>(0,2)=320.5;
	cameraMatrix.at<double>(1,2)=180.5;
	distCoeffs.at<double>(0) = 0;
	distCoeffs.at<double>(1) = 0;
	distCoeffs.at<double>(2) = 0;
	distCoeffs.at<double>(3) = 0;
	cout<<cameraMatrix<<endl;

	setMouseCallback("Show", mouseCB, NULL);
	#if 0
		int x=atoi(argv[2]);
		int min=31414,cont;
		cout<<"Hi"<<endl<<endl;
		for(int j=1;j<x;j++)
		{

			solvePnPRansac(objectPointsm, imagePointsPix, cameraMatrix, distCoeffs, rvec, tvec, 0, j,0.1,0.95	,inlin,CV_P3P);

			Rodrigues(rvec,rotationMatrix);
		
			vector<Point2f> projectedPoints;

			projectPoints(objectPointsm, rvec, tvec, cameraMatrix, distCoeffs, projectedPoints);
			double sum=0;
			for(int i = 0; i < projectedPoints.size(); ++i)
			{
	    		sum+=abs(projectedPoints[i].x-imagePointsPix[i].x)+abs(projectedPoints[i].y-imagePointsPix[i].y);
			}
			if(sum<min)
			{
				min=sum;
				cont=j;
			}
		}

		solvePnPRansac(objectPointsm, imagePointsPix, cameraMatrix, distCoeffs, rvec, tvec, 0, cont,0.1,0.95	,inlin,CV_P3P);
		Rodrigues(rvec,rotationMatrix);

		cout<<"Rvec:"<<rvec<<endl;
		cout<<"tvec:"<<tvec<<endl;
		cout<<"Camera Cordinates "<<-rotationMatrix.t()*tvec;
		while(1)
		{	

		    Mat uvPoint = Mat::ones(3,1,DataType<double>::type); //u,v,1
			uvPoint.at<double>(0,0) = addy; //img point for which we want its real coordinates
			uvPoint.at<double>(1,0) = paddy;
			
			Mat tempMat, tempMat2;
			
			double s;
			tempMat = rotationMatrix.inv() * cameraMatrix.inv() * uvPoint;
			tempMat2 = rotationMatrix.inv() * tvec;

			s = 0 + tempMat2.at<double>(2,0); //before 0 it was 285, which represents the height Zconst
			s /= tempMat.at<double>(2,0);
			Mat output=s*uvPoint;

			cout << "\nP = " << rotationMatrix.inv() * (s * cameraMatrix.inv() * uvPoint - tvec) << endl;
			
			setMouseCallback("Show", mouseCB, NULL);
			cout<<endl;
			imshow("Show",img);
			waitKey(1000);
		}
	#else  
	{
		int x=atoi(argv[2]);
		int min=31414,cont;
		for(int j=1;j<x;j++)
		{
			solvePnPRansac(objectPointsm, imagePointsPix, cameraMatrix, distCoeffs, rvec, tvec, 0, j,0.1,0.95	,inlin,CV_P3P);

			Rodrigues(rvec,rotationMatrix);
		
			vector<Point2f> projectedPoints;

			projectPoints(objectPointsm, rvec, tvec, cameraMatrix, distCoeffs, projectedPoints);
			double sum=0;
			for(int i = 0; i < projectedPoints.size(); ++i)
			{
    			sum+=abs(projectedPoints[i].x-imagePointsPix[i].x)+abs(projectedPoints[i].y-imagePointsPix[i].y);
			}
			if(sum<min)
			{
				min=sum;
				cont=j;
			}
			cout<<"J: "<<j<<" SUM: "<<sum<<" MIN:"<<min<<endl;
		}
		cout<<"Count:"<<cont<< " "<<"MIN:"<<min<<endl;

		solvePnPRansac(objectPointsm, imagePointsPix, cameraMatrix, distCoeffs, rvec, tvec, 0, cont,0.1,0.95	,inlin,CV_P3P);
		Rodrigues(rvec,rotationMatrix);
		cout<<"Rvec:"<<rvec<<endl;
		cout<<"tvec:"<<tvec<<endl;

		setMouseCallback("Show", mouseCB, NULL);
		cout<<"\nCamera Cordinates "<<-rotationMatrix.t()*tvec;
		setMouseCallback("Show", mouseCB, NULL);
		cout<<endl;

		imshow("Show",img);
		waitKey(0000);	
	}
	#endif
}
/*
XYZ=002
Camera cord=[0,0,1.84]
Rvec:[2.318419431785053;
 -0.00478896311331089;
 -0.006750331072318203]
tvec:[-0.03472525185495923;
 1.317421558392364;
 1.298610749483372]

XYZ=003
Rvec:[2.364785323361222;
 -0.03029360811772977;
 0.01079772901899972]
tvec:[-0.08459587385352263;
 2.068372584492771;
 2.025855824555771]

Camera Cordinates [0.08930678410844242;
 0.05319219390911245;
 2.894582581454576]


*/

/*
xyz=032
Rvec:[2.331353671047926;
 0.007637632398161886;
 -0.02033632001669571]
tvec:[-0.1163827094899424;
 1.2815609876509;
 1.253036048585058]
Camera Cordinates [0.1388082264145865;
 -0.02271448085649863;
 1.790603017932697]

 xyz=332
Rvec:[2.361380557278175;
 0.03585195128657504;
 -0.04159854745688536]
tvec:[-0.1451636698561342;
 1.342772747383592;
 1.418824672083986]
Camera Cordinates [0.1846726668864952;
 -0.03644627736698769;
 1.94980569987051]


xyz=003
Rvec:[2.346975187329746;
 -0.03371934166280662;
 0.01006128359568509]
tvec:[-0.08431258616405968;
 2.037342258359937;
 2.000515787566498]

Camera Cordinates [0.09273646511228767;
 -0.00219295275640663;
 2.855053527551512]

xyz=002
Rvec:[2.343173489304827;
 0.01317351942790935;
 -0.02467797740484011]
tvec:[-0.1083151113648527;
 1.365333759416382;
 1.335093607654932]
Camera Cordinates [0.1348007416026795;
 -0.001182918392697574;
 1.907923148326619]

*/