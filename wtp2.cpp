#include <stdio.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/imgproc/imgproc.hpp>
//#include <fstream>
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

	
	imagePointsPix.push_back(Point2f(315., 176.));//300., 163.));//304., 163.));//\/\/\/
	imagePointsPix.push_back(Point2f(318., 65.));//302., 52.));//315., 57.));//\/\/\/
	imagePointsPix.push_back(Point2f(155., 64.));//148., 52.));//166., 57.));//\/\/\/
	imagePointsPix.push_back(Point2f(470., 65.));//468., 49.));//478., 60.));//\/\/\/
    imagePointsPix.push_back(Point2f(1., 60.));//291., 347.));//292., 344.));//\/\/\/
	imagePointsPix.push_back(Point2f(632., 64.));//569., 350.));//\/\/
	imagePointsPix.push_back(Point2f(510., 176.));//510., 164.));//\/
	imagePointsPix.push_back(Point2f(112., 174.));//120., 161.));
	/*imagePointsPix.push_back(Point2f(632., 64.));//566.,171.));
	imagePointsPix.push_back(Point2f(70.,96.));
	imagePointsPix.push_back(Point2f(187.,91.));
	imagePointsPix.push_back(Point2f(307.,89.));
	imagePointsPix.push_back(Point2f(408.,96.));
	imagePointsPix.push_back(Point2f(529.,89.));
	imagePointsPix.push_back(Point2f(636.,86.));*/


	objectPointsm.push_back(Point3f(2., 0., 0.));//\/\/\/
	objectPointsm.push_back(Point3f(3., 0., 0.));//\/\/\/
	objectPointsm.push_back(Point3f(3., 1., 0.));//\/\/\/
	objectPointsm.push_back(Point3f(3., -1., 0.));//\/\/\/
	objectPointsm.push_back(Point3f(3., 2., 0.));//\/\/\/
	objectPointsm.push_back(Point3f(3., -2., 0.));//\/\/
	objectPointsm.push_back(Point3f(2., -1., 0.));//\/
	objectPointsm.push_back(Point3f(2., 1., 0.));//
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
	#if 1
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
xyz=002
Rvec:[1.73866871509916;
 -1.771659772958356;
 0.7995910146081044]
tvec:[0.04974079739266;
 1.338806103760195;
 1.269537008195808]
Camera Cordinates [-0.0004134853254840198;
 0.1139899249766496;
 1.842174332576392]

xyz=003
Rvec:[1.722909631545703;
 -1.767384227835314;
 0.8168058039150952]
tvec:[-0.04387764686365012;
 1.945588316345559;
 1.799196441407201]

Camera Cordinates [-0.01573150369014265;
 0.05242164031823755;
 2.649783287513332]

xyz=332
Rvec:[1.758928668991228;
 -1.701225200631459;
 0.6874064227077716]
tvec:[-0.158542349508817;
 1.295597930151757;
 1.353918665657058]
Camera Cordinates [-0.06875343725016236;
 -0.2298379662732959;
 1.865275556195793]

 xyz=032
 Rvec:[1.753434642557427;
 -1.747396416440388;
 0.7154248234971822]
tvec:[-0.1062764706399676;
 1.307500432462404;
 1.313059399333799]
Camera Cordinates [-0.01454544545689818;
 -0.1332095353315967;
 1.851221408960338]

xyz=002
Rvec:[1.761585339076719;
 -1.753583316776769;
 0.7435300149316865]
tvec:[-0.02917608384629279;
 1.36978479564406;
 1.349752644576806]
Camera Cordinates [-0.0003109386336228592;
 -0.01163685756089857;
 1.923241617335585]

*/