#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

Mat img, res, element;

int main(int argc, char** argv)
{

   namedWindow( "Meanshift", 0 );
   namedWindow( "Original", 0 );
   img = imread("histBack14.png",1 );
   GaussianBlur(img, img, Size(5,5), 2, 2);
   pyrMeanShiftFiltering( img, res, 20, 45, 1);
   imwrite("histBack10m.png", res);
   imshow( "Meanshift", res );
   imshow("Original",img);
   waitKey();
   


   return 0;
}