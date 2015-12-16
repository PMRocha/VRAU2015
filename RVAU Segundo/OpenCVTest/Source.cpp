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

bool sortByX(pair<Mat,int> vec1, pair<Mat, int> vec2) {
	return (vec1.second < vec2.second);
}

Mat preprocess(Mat img) {
	cvtColor(img, img, CV_BGR2GRAY);
	GaussianBlur(img, img, Size(1, 1), 1000);
	threshold(img, img, 150, 255, THRESH_BINARY);

	return img;
}
vector<double> compareIMG(Mat img1, Mat img2) {
	Mat hsv1, hsv2;

	cvtColor(img1, hsv1, CV_BGR2HSV);
	cvtColor(img2, hsv2, CV_BGR2HSV);

	int h_bins = 50; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };

	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };

	const float* ranges[] = { h_ranges, s_ranges };

	int channels[] = { 0, 1 };

	MatND hist1, hist2;

	calcHist(&hsv1, 1, channels, Mat(), hist1, 2, histSize, ranges, true, false);
	normalize(hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&hsv2, 1, channels, Mat(), hist2, 2, histSize, ranges, true, false);
	normalize(hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat());
	vector<double> vec;
	for (int i = 0; i < 4; i++)
	{
		int compare_method = i;
		vec.push_back(compareHist(hist1, hist2, compare_method));
	}
	return vec;
}
vector<Mat> readCards() {
	string const suit[4] = { "copas","espadas","ouros","paus" };
	vector<Mat> vec;
	for (int i = 0; i < 4; i++) {
			string str = "C:\\Users\\Asus\\Documents\\VRAU2015\\RVAU Segundo\\x64\\Release\\cards\\naipes\\"
				+ suit[i] + ".png";
			Mat image = imread(str, CV_LOAD_IMAGE_COLOR);

			vec.push_back(image);
	}
	
	return vec;
}
vector<Mat> readAllCards() {
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
	

	thresh = preprocess(cards[1]);
	findContours(thresh, vecPoint, vecInfo, CV_RETR_TREE, CHAIN_APPROX_SIMPLE);
	std::sort(vecPoint.begin(), vecPoint.end(), compareContourAreas);
	vector<pair<Mat,int>> symbols;

	for (int i = 0; i < 10; ++i) {
		
		vector<Point> symbol = vecPoint[i];
		double peri = arcLength(symbol, true);

		approxPolyDP(symbol, approximate, 0.01*peri, true);
		RotatedRect rect = minAreaRect(symbol);
		Mat r;
		boxPoints(rect, r);
	
		rectangle(cards[1], rect.boundingRect(), Scalar(0, 0, 255));

		
		Mat croppedFaceImage = cards[1](rect.boundingRect()).clone();
		symbols.push_back(pair<Mat, int>(croppedFaceImage, rect.center.x));
	}	
	std::sort(symbols.begin(), symbols.end(), sortByX);
	//---------------------------------------------------------------------------------//

	vector<Mat> allCards = readAllCards(); //getDataSet

	//Compare symbols with Dataset
/*
	double lastResult = -1;
	int card, symbol;
	
	for (int i = 0; i < allCards.size(); ++i) {
		vector<double> v1 = compareIMG(symbols[0].first, allCards[i]);
		vector<double> v2 = compareIMG(symbols[1].first, allCards[i]);

		double result = (v1[0] * v1[2]) / (v1[1] * v1[3]);
		if (result > lastResult) {
			lastResult = result;
			card = i;
			symbol = 0;
		}
		result = (v2[0] * v2[2]) / (v2[1] * v2[3]);
		if (result > lastResult) {
			lastResult = result;
			card = i;
			symbol = 1;
		}
	}


	cout << lastResult << " " << card << " " << symbol << endl;
	*/
	double lastResult = -1;
	int card;
	for (int i = 0; i < allCards.size(); ++i) {
		vector<double> v = compareIMG(cards[1], allCards[i]);
		double result = (v[0] * v[2]) / (v[1] * v[3]);
		if (result > lastResult) {
			lastResult = result;
			card = i;
		}
	}


	namedWindow("Display window1", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window1", cards[1]);
	//namedWindow("Display window2", WINDOW_AUTOSIZE);// Create a window for display.
	//imshow("Display window2", symbols[symbol].first);
	namedWindow("Display window3", WINDOW_AUTOSIZE);// Create a window for display.
	imshow("Display window3", allCards[card]);
	
	waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}