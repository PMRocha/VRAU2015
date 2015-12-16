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
	return (i > j);
}


int main( int argc, char** argv )
{
    /*if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }*/

    Mat image;
    image = imread("C:\\Users\\Pedrp\\Documents\\Visual Studio 2015\\Projects\\RVAU Segundo\\x64\\Release\\cards.png", CV_LOAD_IMAGE_COLOR);   // Read the file

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
	vector<Vec2f> approximate;
	Point2f inputQuad[4];
	Point2f outputQuad[4];

	outputQuad[1] = Point2f(0, 0);
	outputQuad[0] = Point2f(485, 0);
	outputQuad[3] = Point2f(485, 681);
	outputQuad[2] = Point2f(0, 681);
	Mat card1, card2, card3, card4;
	for (size_t i = 0; i < 4; i++)
	{
		vector<Point> card = vecPoint[i];
		double peri = arcLength(card, true);
		approxPolyDP(card, approximate, 0.01*peri, true);
		RotatedRect rect = minAreaRect(card);

		inputQuad[0] = approximate[0];
		inputQuad[1] = approximate[1];
		inputQuad[2] = approximate[2];
		inputQuad[3] = approximate[3];

		Mat final = getPerspectiveTransform(inputQuad, outputQuad);
		Mat output;
		switch (i) {
		case 0:
			warpPerspective(image, card1, final, card1.size());
			break;
		case 1:
			warpPerspective(image, card2, final, card1.size());
			break;
		case 2:
			warpPerspective(image, card3, final, card1.size());
			break;
		case 3:
			warpPerspective(image, card4, final, card1.size());
			break;
		}
		
	}

	cout << "final step\n";
	

    namedWindow( "Display window1", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window1", card1);
	namedWindow("Display window2", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window2", card2);
	namedWindow("Display window3",WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window3", card3);
	namedWindow("Display window4", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window4", card4);

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}