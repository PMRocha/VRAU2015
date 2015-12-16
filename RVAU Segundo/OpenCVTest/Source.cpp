#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
//#include <Gdiplus.h>

using namespace cv;
using namespace std;


bool compareContourAreas(std::vector<Point> contour1, std::vector<Point> contour2) {
	double i = fabs(contourArea(Mat(contour1)));
	double j = fabs(contourArea(Mat(contour2)));
	return (i > j);
}

Mat preprocess(Mat img) {
	cvtColor(img, img, CV_BGR2GRAY);
	GaussianBlur(img, img, Size(1, 1), 1000);
	threshold(img, img, 150, 255, THRESH_BINARY);

	return img;
}
Mat cardDiff(Mat img1, Mat img2) {
	Mat output;
	resize(img1, img1, img2.size());
	absdiff(preprocess(img1), preprocess(img2), output);
	GaussianBlur(output, output, Size(5, 5), 5);
	threshold(output, output, 200, 255, THRESH_BINARY);
	return output;
}
vector<Mat> readCards() {
	string const suit[4] = { "copas","espadas","ouros","paus" };
	vector<Mat> vec;
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j <= 13; ++j) {
			string str = "C:\\Users\\Asus\\Documents\\VRAU2015\\RVAU Segundo\\x64\\Release\\cards\\"
				+ to_string(j) + "_" + suit[i] + ".png";
			Mat image = imread(str, CV_LOAD_IMAGE_COLOR);

			vec.push_back(image);
		}
	}
	
	return vec;
}

int main( int argc, char** argv )
{
    /*if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }*/

    Mat image;
    image = imread("C:\\Users\\Asus\\Documents\\VRAU2015\\RVAU Segundo\\x64\\Release\\teste.png", CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
	
	//--------------------------------------------Get Cards-------------------------------------------//
	Mat thresh = preprocess(image);

	vector<vector<Point> > vecPoint;
	vector<Vec4i> vecInfo;
	Mat Countours;

	findContours(thresh, vecPoint, vecInfo, CV_RETR_TREE,CHAIN_APPROX_SIMPLE);

	std::sort(vecPoint.begin(), vecPoint.end(), compareContourAreas);
	
	vector<Point> r;
	vector<Vec2f> approximate;
	Point2f inputQuad[4];
	Point2f outputQuad[4];

	Mat cards[4];

	for (size_t i = 0; i < 4; i++)
	{
		vector<Point> card = vecPoint[i];
		double peri = arcLength(card, true);
		approxPolyDP(card, approximate, 0.01*peri, true);
		RotatedRect rect = minAreaRect(card);
		Mat r;
		boxPoints(rect, r);

		inputQuad[0] = approximate[0];
		inputQuad[1] = approximate[1];
		inputQuad[2] = approximate[2];
		inputQuad[3] = approximate[3];


		outputQuad[0] = Point2f(449, 0);
		outputQuad[1] = Point2f(0, 0);
		outputQuad[2] = Point2f(0, 449);
		outputQuad[3] = Point2f(449, 449);

		Mat final = getPerspectiveTransform(inputQuad, outputQuad);
		Mat output;
		warpPerspective(image, cards[i], final, Size(450, 450));	
	}
	//-----------------------------------------------------------------//

	//-------------------------------Get Symbols----------------------------------//
	

	thresh = preprocess(cards[2]);
	findContours(thresh, vecPoint, vecInfo, CV_RETR_TREE, CHAIN_APPROX_SIMPLE);
	std::sort(vecPoint.begin(), vecPoint.end(), compareContourAreas);

	for (int i = 0; i < 10; ++i) {

		//Scalar color = Scalar(0, 0,255);
		//drawContours(cards[2], vecPoint, i, color, 2, 8, vecInfo, 0, Point());

		vector<Point> symbol = vecPoint[i];
		double peri = arcLength(symbol, true);
		approxPolyDP(symbol, approximate, 0.01*peri, true);
		RotatedRect rect = minAreaRect(symbol);
		Mat croppedFaceImage = cards[2](rect.boundingRect()).clone();


		namedWindow("Display window" + to_string(i), WINDOW_AUTOSIZE);// Create a window for display.
		imshow("Display window" + to_string(i), croppedFaceImage);

	}
	namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window", cards[2]);
	/*
	vector<Mat> allCards = readCards();
	int ZeroPixels = -1;
	int best = -1;
	for (int i = 0; i < allCards.size(); ++i) {
		Mat card = cardDiff(cards[2], allCards[i]);
		int TotalNumberOfPixels = card.rows * card.cols;
		if (ZeroPixels < TotalNumberOfPixels - countNonZero(card)) {
			ZeroPixels = TotalNumberOfPixels - countNonZero(card);
			best = i;
		}
	}

	namedWindow("Display window1", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window1", cardDiff(cards[2], allCards[best]));

	namedWindow("Display window2", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window2", cards[2]);

	namedWindow("Display window3", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window3", allCards[best]);

	cout << "final step\n";
	*/
	waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}