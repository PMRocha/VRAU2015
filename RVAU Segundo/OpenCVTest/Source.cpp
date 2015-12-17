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

double dist(Point2f p1, Point2f p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

Mat preprocess(Mat img) {
	cvtColor(img, img, CV_BGR2GRAY);
	GaussianBlur(img, img, Size(1, 1), 1000);
	threshold(img, img, 150, 255, THRESH_BINARY);

	return img;
}
double compareIMG(Mat img1, Mat img2) {
	Mat hsv1, hsv2;

	cvtColor(img1, hsv1, CV_BGR2HSV);
	cvtColor(img2, hsv2, CV_BGR2HSV);

	int h_bins = 50; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };

	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };

	const float* ranges[] = { h_ranges, s_ranges };

	int channels[] = { 0, 1, 2 };

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
	return (vec[0] * vec[2]) / (vec[1] * vec[3]);
}
vector<Mat> readCards() {
	string const suit[4] = { "copas","espadas","ouros","paus" };
	string const num[13] = { "2","3","4","5","6","7","8","9","10","J","Q","K","A" };
	vector<Mat> vec;

	for (int i = 0; i < 13; i++) {
		string str = "C:\\Users\\Asus\\Documents\\VRAU2015\\RVAU Segundo\\x64\\Release\\cards\\naipes\\"
			+ num[i] + ".png";
		Mat image = imread(str, CV_LOAD_IMAGE_COLOR);

		vec.push_back(image);
	}

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
vector<Mat> readAllNewCards() {
	string const suit[4] = { "hearts","spades","diamonds","clubs" };
	string const num[13] = { "2","3","4","5","6","7","8","9","10","jack","queen","king","ace" };
	string path = "C:\\Users\\Asus\\Documents\\VRAU2015\\RVAU Segundo\\x64\\Release\\PNG-cards-1.3\\";
	vector<Mat> vec;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 9; ++j) {
			string str = path + num[j] + "_of_" + suit[i] + ".png";
			Mat image = imread(str, CV_LOAD_IMAGE_COLOR);

			vec.push_back(image);
		}
		for (int j = 9; j < 12; ++j) {
			string str = path + num[j] + "_of_" + suit[i] + ".png";
			Mat image = imread(str, CV_LOAD_IMAGE_COLOR);

			vec.push_back(image);

			str = path + num[j] + "_of_" + suit[i] + "2.png";
			image = imread(str, CV_LOAD_IMAGE_COLOR);

			vec.push_back(image);
		}

		string str = path + num[12] + "_of_" + suit[i] + ".png";
		Mat image = imread(str, CV_LOAD_IMAGE_COLOR);

		vec.push_back(image);
	}
	string str = path + "ace_of_spades2.png";
	Mat image = imread(str, CV_LOAD_IMAGE_COLOR);

	vec.push_back(image);

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
		Point2f points[4];
		rect.points(points);

		if (dist(approximate[0], approximate[1]) < dist(approximate[1], approximate[2])) {
			inputQuad[0] = approximate[0];
			inputQuad[1] = approximate[1];
			inputQuad[2] = approximate[2];
			inputQuad[3] = approximate[3];
		}
		else {
			inputQuad[0] = approximate[1];
			inputQuad[1] = approximate[2];
			inputQuad[2] = approximate[3];
			inputQuad[3] = approximate[0];
		}
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
	/*
	vector<pair<Mat, Mat>> allSymbols;
	for (int index = 0; index < 4 ; ++index) {
		vector<pair<Mat, int>> symbols;
		thresh = preprocess(cards[index]);
		findContours(thresh, vecPoint, vecInfo, CV_RETR_TREE, CHAIN_APPROX_SIMPLE);
		std::sort(vecPoint.begin(), vecPoint.end(), compareContourAreas);
		
		for (int i = 0; i < 15 && i < vecPoint.size(); ++i) {
			vector<Point> symbol = vecPoint[i];
			double peri = arcLength(symbol, true);
			approxPolyDP(symbol, approximate, 0.01*peri, true);
			RotatedRect rect = minAreaRect(symbol);
			//rectangle(cards[index], rect.boundingRect(), Scalar(0, 0, 255));

			//cout << cards[index].size() << endl;
			//cout << rect.boundingRect() << endl;
			Mat cropped;
			if (rect.boundingRect().x + rect.boundingRect().width < cards[index].size().width &&
				rect.boundingRect().y + rect.boundingRect().height < cards[index].size().height &&
				rect.boundingRect().x >= 0 && rect.boundingRect().y >= 0) {
				cropped = cards[index](rect.boundingRect()).clone();
			}
			else {
				int x = rect.boundingRect().x, 
					y = rect.boundingRect().y, 
					width = rect.boundingRect().width, 
					height = rect.boundingRect().height;
				if (rect.boundingRect().x + rect.boundingRect().width > cards[index].size().width)
					width = cards[index].size().width - rect.boundingRect().x;
				if (rect.boundingRect().y + rect.boundingRect().height > cards[index].size().height)
					height = cards[index].size().height - rect.boundingRect().y;
				if (rect.boundingRect().x < 0)
					x = 0;
				if (rect.boundingRect().y < 0)
					y = 0;

				cropped = cards[index](Rect(x,y,width,height));
			}
			symbols.push_back(pair<Mat, int>(cropped, rect.center.x));
		}
		std::sort(symbols.begin(), symbols.end(), sortByX);
		allSymbols.push_back(pair<Mat, Mat>(symbols[0].first,symbols[1].first));
	}

	*/
	//---------------------------------------------------------------------------------//

	vector<Mat> allCards = readAllNewCards(); //getDataSet
	vector<Mat> aux = readAllCards();
	allCards.insert(allCards.end(),aux.begin(), aux.end());
	//Compare symbols with Dataset

	/*
	for (int index = 0; index < allSymbols.size(); ++index) {
		double lastResult1 = -1, lastResult2 = -1;
		int card1, card2;
		for (int i = 0; i < allCards.size(); ++i) {

			double result = compareIMG(allSymbols[index].first, allCards[i]);
			if (result > lastResult1) {
				lastResult1 = result;
				card1 = i;
			}
			result = compareIMG(allSymbols[index].second, allCards[i]);
			if (result > lastResult2) {
				lastResult2 = result;
				card2 = i;
			}

		}

		namedWindow("Display window1" + to_string(index), WINDOW_AUTOSIZE);// Create a window for display.
		imshow("Display window1" + to_string(index), allSymbols[index].first);
		namedWindow("Display window2" + to_string(index), WINDOW_AUTOSIZE);// Create a window for display.
		imshow("Display window2" + to_string(index), allSymbols[index].second);


		string const num[17] = { "2","3","4","5","6","7","8","9","10","J","Q","K","A","copas","espadas","ouros","paus" };

		cout << "card: " << num[card1] << " " << num[card2] << endl;
	}
	*/

	
	for (int index = 0; index < 4; ++index) {
		double lastResult = -1;
		int card;
		for (int i = 0; i < allCards.size(); ++i) {
			double result = compareIMG(cards[index], allCards[i]);
			if (result > lastResult) {
				lastResult = result;
				card = i;
			}
		}
		namedWindow("Display window1" + to_string(index), WINDOW_AUTOSIZE);// Create a window for display.
		imshow("Display window1" + to_string(index), cards[index]);
		namedWindow("Display window2" + to_string(index), WINDOW_AUTOSIZE);// Create a window for display.
		imshow("Display window2" + to_string(index), allCards[card]);
		}
		


		

	waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}