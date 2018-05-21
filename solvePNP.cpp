#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
 
#include <iostream>
#include <string>

using namespace std;
using namespace cv;
 
vector<Point2f> Generate2DPoints();
vector<Point3f> Generate3DPoints();
 
int main( int argc, char* argv[])
{
  
  vector<Point2f> imagePoints = Generate2DPoints();
  vector<Point3f> objectPoints = Generate3DPoints();
                                                   
  cout << "There are " << imagePoints.size() << " imagePoints and " << objectPoints.size() << " objectPoints." <<endl;
  Mat cameraMatrix(3,3,DataType<double>::type);
  setIdentity(cameraMatrix);
 
  cout << "Initial cameraMatrix: " << cameraMatrix <<endl;
 
  Mat distCoeffs(4,1,DataType<double>::type);
  distCoeffs.at<double>(0) = 0;
  distCoeffs.at<double>(1) = 0;
  distCoeffs.at<double>(2) = 0;
  distCoeffs.at<double>(3) = 0;
 
  Mat rvec(3,1,DataType<double>::type);
  Mat tvec(3,1,DataType<double>::type);
 
  solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec);
 
  cout << "rvec: " << rvec <<endl;
  cout << "tvec: " << tvec <<endl;
 
  vector<Point2f> projectedPoints;
  projectPoints(objectPoints, rvec, tvec, cameraMatrix, distCoeffs, projectedPoints);
 
  for(unsigned int i = 0; i < projectedPoints.size(); ++i)
    {
     cout << "Image point: " << imagePoints[i] << " Projected to " << projectedPoints[i] <<endl;
    }
 
  return 0;
}
 
 
vector<Point2f> Generate2DPoints()
{
  vector<Point2f> points;
 
  float x,y;
 
  x=282;y=274;
  points.push_back(cv::Point2f(x,y));
 
  x=397;y=227;
  points.push_back(cv::Point2f(x,y));
 
  x=577;y=271;
  points.push_back(cv::Point2f(x,y));
 
  x=462;y=318;
  points.push_back(cv::Point2f(x,y));
 
  x=270;y=479;
  points.push_back(cv::Point2f(x,y));
 
  x=450;y=523;
  points.push_back(cv::Point2f(x,y));
 
  x=566;y=475;
  points.push_back(cv::Point2f(x,y));
 
  for(unsigned int i = 0; i < points.size(); ++i)
    {
      cout << points[i] <<endl;
    }
 
  return points;
}
 
 
vector<Point3f> Generate3DPoints()
{
  vector<Point3f> points;
 
 
  float x,y,z;
 
  x=.5;y=.5;z=-.5;
  points.push_back(cv::Point3f(x,y,z));
 
  x=.5;y=.5;z=.5;
  points.push_back(cv::Point3f(x,y,z));
 
  x=-.5;y=.5;z=.5;
  points.push_back(cv::Point3f(x,y,z));
 
  x=-.5;y=.5;z=-.5;
  points.push_back(cv::Point3f(x,y,z));
 
  x=.5;y=-.5;z=-.5;
  points.push_back(cv::Point3f(x,y,z));
 
  x=-.5;y=-.5;z=-.5;
  points.push_back(cv::Point3f(x,y,z));
 
  x=-.5;y=-.5;z=.5;
  points.push_back(cv::Point3f(x,y,z));
 
  for(unsigned int i = 0; i < points.size(); ++i)
    {
    cout << points[i] <<endl;
    }
 
  return points;
}