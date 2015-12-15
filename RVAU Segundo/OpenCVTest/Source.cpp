#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
//#include <Gdiplus.h>

using namespace cv;
using namespace std;


bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2) {
	double i = fabs(contourArea(cv::Mat(contour1)));
	double j = fabs(contourArea(cv::Mat(contour2)));
	return (i < j);
}


int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
	
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);

	Mat blur;

	GaussianBlur(gray,blur,Size(1,1),1000);

	Mat thresh;

	threshold(blur, thresh, 150, 255, THRESH_BINARY);

	vector<vector<Point> > vecPoint;
	vector<Vec4i> vecInfo;
	Mat Countours;

	findContours(thresh, vecPoint, vecInfo, CV_RETR_TREE,CHAIN_APPROX_SIMPLE);

	std::sort(vecPoint.begin(), vecPoint.end(), compareContourAreas);
	
	vector<Point> r;
	vector<Point> approximate;
	for (size_t i = 0; i < 4; i++)
	{
		vector<Point> card = vecPoint[i];
		double peri = arcLength(card, true);
		approxPolyDP(card, approximate, 0.02*peri, true);
		cout << "487152\n";
		RotatedRect rect = minAreaRect(card);
		cout << "487153\n";
		boxPoints(rect,r);
		cout << "487154\n";
	}

	vector<Vec4i> h;
	h.push_back(Vec4i(0,0));
	h.push_back(Vec4i(486, 0));
	h.push_back(Vec4i(486, 682));
	h.push_back(Vec4i(0, 682));

	cout << "final step\n";
	Mat final = getPerspectiveTransform(approximate,h);

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", final);                   // Show our image inside it.

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}