#include "stdafx.h"

#include <iostream>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/core/core.hpp>

//#include <Gdiplus.h>

using namespace cv;
using namespace std;

void filterMatchesByAbsoluteValue(std::vector<DMatch> &matches, float maxDistance);
Mat filterMatchesRANSAC(std::vector<DMatch> &matches, std::vector<KeyPoint> &keypointsA, std::vector<KeyPoint> &keypointsB);
void showResult(string name, Mat &imgA, std::vector<KeyPoint> &keypointsA, Mat &imgB, std::vector<KeyPoint> &keypointsB, std::vector<DMatch> &matches, Mat &homography);

vector<int> cardNames;
vector<string> suitNames;

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
	return (vec[0] * vec[2]);
}
bool sortCards(pair<int, string> p1, pair<int, string> p2) {
	return p1.first > p2.first;
}
pair<int, string> mySort(vector<pair<int, string>> vec, string strong, string trump) {
	vector<pair<int, string>> newVec;
	for (int i = 0; i < vec.size(); ++i) {
		if (vec[i].second == trump)
			newVec.push_back(vec[i]);
	}
	if (!newVec.empty()) {
		sort(newVec.begin(), newVec.end(), sortCards);
		return newVec[0];
	}
	for (int i = 0; i < vec.size(); ++i) {
		if (vec[i].second == strong)
			newVec.push_back(vec[i]);
	}

	sort(newVec.begin(), newVec.end(), sortCards);
	return newVec[0];
}

int compareSIFT(Mat img1, vector<Mat> img2) {
	cout << "Running SIFT" << endl;
	int minHessian = 400;

	Ptr<FeatureDetector> detector = xfeatures2d::SIFT::create();
	Ptr<DescriptorExtractor> extractor = xfeatures2d::SIFT::create();
	FlannBasedMatcher* matcher = new FlannBasedMatcher();

	Mat descriptor, homography;
	vector<Mat>  descriptors(img2.size());

	std::vector<KeyPoint> keypoints_1;
	vector<vector<KeyPoint>>keypoints = vector<vector<KeyPoint>>();

	detector->detect(img1, keypoints_1);
	extractor->compute(img1, keypoints_1, descriptor);

	double maxPoints = -1;
	int bestIndex;
	vector<DMatch> bestMatches;

	for (int i = 0; i < img2.size();i++)
	{
		resize(img2[i], img2[i], img1.size());
		vector<DMatch> matches = vector<DMatch>();

		vector<KeyPoint> keypoint;
		detector->detect(img2[i], keypoint);
		extractor->compute(img2[i], keypoint, descriptors[i]);
		keypoints.push_back(keypoint);
		matcher->match(descriptor, descriptors[i], matches);

		filterMatchesByAbsoluteValue(matches, 125);

		double d = compareIMG(img1, img2[i]);
		double points = 100 * matches.size() * d;

		if (points > maxPoints) {
			maxPoints = points;
			bestIndex = i;
			bestMatches = matches;
		}
	}
	homography = filterMatchesRANSAC(bestMatches, keypoints_1, keypoints[bestIndex]);
	showResult("Window" + to_string(maxPoints),img1, keypoints_1, img2[bestIndex], keypoints[bestIndex], bestMatches, homography);

	return bestIndex;
}

void filterMatchesByAbsoluteValue(std::vector<DMatch> &matches, float maxDistance)
{
	std::vector<DMatch> filteredMatches;
	for (size_t i = 0; i < matches.size(); i++)
	{
		if (matches[i].distance < maxDistance)
			filteredMatches.push_back(matches[i]);
	}
	matches = filteredMatches;
}	

Mat filterMatchesRANSAC(std::vector<DMatch> &matches, std::vector<KeyPoint> &keypointsA, std::vector<KeyPoint> &keypointsB)
{
	Mat homography;
	std::vector<DMatch> filteredMatches;
	if (matches.size() >= 4)
	{
		vector<Point2f> srcPoints = vector<Point2f>();
		vector<Point2f> dstPoints;
		for (size_t i = 0; i < matches.size(); i++)
		{

			srcPoints.push_back(keypointsA[matches[i].queryIdx].pt);
			dstPoints.push_back(keypointsB[matches[i].trainIdx].pt);
		}

		Mat mask;
		homography = findHomography(srcPoints, dstPoints, CV_RANSAC, 1.0, mask);

		for (int i = 0; i < mask.rows; i++)
		{
			if (mask.ptr<uchar>(i)[0] == 1)
				filteredMatches.push_back(matches[i]);
		}
	}
	//matches = filteredMatches;
	return homography;
}

void showResult(string name, Mat &imgA, std::vector<KeyPoint> &keypointsA, Mat &imgB, std::vector<KeyPoint> &keypointsB, std::vector<DMatch> &matches, Mat &homography)
{
	// Draw matches
	Mat imgMatch;
	drawMatches(imgA, keypointsA, imgB, keypointsB, matches, imgMatch, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	if (!homography.empty())
	{
		//-- Get the corners from the image_1 ( the object to be "detected" )
		std::vector<Point2f> obj_corners(4);
		obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(imgA.cols, 0);
		obj_corners[2] = cvPoint(imgA.cols, imgA.rows); obj_corners[3] = cvPoint(0, imgA.rows);
		std::vector<Point2f> scene_corners(4);

		perspectiveTransform(obj_corners, scene_corners, homography);

		float cols = (float)imgA.cols;
		line(imgMatch, scene_corners[0] + Point2f(cols, 0), scene_corners[1] + Point2f(cols, 0), Scalar(0, 255, 0), 4);
		line(imgMatch, scene_corners[1] + Point2f(cols, 0), scene_corners[2] + Point2f(cols, 0), Scalar(0, 255, 0), 4);
		line(imgMatch, scene_corners[2] + Point2f(cols, 0), scene_corners[3] + Point2f(cols, 0), Scalar(0, 255, 0), 4);
		line(imgMatch, scene_corners[3] + Point2f(cols, 0), scene_corners[0] + Point2f(cols, 0), Scalar(0, 255, 0), 10);
	}



	namedWindow(name, CV_WINDOW_KEEPRATIO);
	imshow(name, imgMatch);
}

vector<Mat> readAllNewCards() {
	string const suit[4] = { "hearts","spades","diamonds","clubs" };
	string const num[13] = { "2","3","4","5","6","7","8","9","10","jack","queen","king","ace" };
	string path = "cards\\";
	vector<Mat> vec;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; ++j) {
			string str = path + num[j] + "_of_" + suit[i] + ".png";
			
			Mat image = imread(str, CV_LOAD_IMAGE_COLOR);

			vec.push_back(image);
			cardNames.push_back(j);
			suitNames.push_back(suit[i]);
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 9; j < 12; ++j) {
			string str = path + num[j] + "_of_" + suit[i] + "2.png";

			Mat image = imread(str, CV_LOAD_IMAGE_COLOR);

			vec.push_back(image);
			cardNames.push_back(j);
			suitNames.push_back(suit[i]);
		}
	}
	return vec;
}

Mat showFinal(Mat src1, Mat src2)
{
	Mat gray, gray_inv, src1final, src2final;
	cvtColor(src2, gray, CV_BGR2GRAY);
	threshold(gray, gray, 0, 255, CV_THRESH_BINARY);
	bitwise_not(gray, gray_inv);
	src1.copyTo(src1final, gray_inv);
	src2.copyTo(src2final, gray);
	Mat finalImage = src1final + src2final;
	return finalImage;
}

Mat WriteOnImage(string message, Mat image, vector<Point2f> CardCoords, Scalar color) {
	int temp;
	vector<Point2f> left_image;

	Mat textImg = Mat::zeros(image.cols, image.rows, image.type());
	putText(textImg, message, Point(image.cols / 2 - getTextSize(message, FONT_HERSHEY_SIMPLEX, 5.0, 20, &temp).width / 2, image.rows / 2), FONT_HERSHEY_SIMPLEX, 5.0, color, 20);

	left_image.push_back(Point2f(float(0), float(0)));
	left_image.push_back(Point2f(float(0), float(textImg.rows)));
	left_image.push_back(Point2f(float(textImg.cols), float(textImg.rows)));
	left_image.push_back(Point2f(float(textImg.cols), float(0)));

	Mat H = findHomography(left_image, CardCoords, 0);
	Mat logoWarped;
	// Warp the logo image to change its perspective
	warpPerspective(textImg, logoWarped, H, textImg.size());
	return showFinal(image, logoWarped);

}

int main( int argc, char** argv )
{
	
	vector<vector<Point2f> > CardsCoords;
	vector<Mat> allCards = readAllNewCards(); //getDataSet	

    Mat image;
    image = imread("C:\\Users\\Asus\\Documents\\VRAU2015\\RVAU Segundo\\x64\\Release\\TestImages\\TestImage5.png", CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

	namedWindow("Original", CV_WINDOW_KEEPRATIO);
	imshow("Original", image);
	string trump;
	cout << "Select the Trump: hearts, spades, diamonds, clubs" << endl;
	cin >> trump;
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
		vector<Point2f> temp;

		double peri = arcLength(card, true);
		approxPolyDP(card, approximate, 0.01*peri, true);
		RotatedRect rect = minAreaRect(card);
		Point2f points[4];
		rect.points(points);

		temp.push_back(approximate[0]);
		temp.push_back(approximate[1]);
		temp.push_back(approximate[2]);
		temp.push_back(approximate[3]);

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

		CardsCoords.push_back(temp);
	}
	//-----------------------------------------------------------------//
	
	vector<pair<int, string>> cardsTable;
	for (int index = 0; index < 4; ++index) {
		int i = compareSIFT(cards[index], allCards);
		cardsTable.push_back(pair<int, string>(cardNames[i], suitNames[i]));
	}

	string strong = cardsTable[0].second;

	
	
	cout << "Trump: " << trump << endl;

	pair<int, string> winner = mySort(cardsTable, strong, trump);

	for (int i = 0; i < 4;i++) {
			switch (cardsTable[i].first) {
			case 9:
				cout << "Card " << i << ": Jack of " << cardsTable[i].second << endl;
				break;
			case 10:
				cout << "Card " << i << ": Queen of " << cardsTable[i].second << endl;
				break;
			case 11:
				cout << "Card " << i << ": King of " << cardsTable[i].second << endl;
				break;
			case 12:
				cout << "Card " << i << ": Ace of " << cardsTable[i].second << endl;
				break;
			default:
				cout << "Card " << i << ": " << cardsTable[i].first + 2 << " of " << cardsTable[i].second << endl;
				break;
			}
		if (cardsTable[i] == winner) {
			image = WriteOnImage("Winner", image, CardsCoords[i], Scalar(66, 210, 38));
			drawContours(image, vecPoint, i, Scalar(66, 210, 38), 2, 8, vecInfo, 0, Point());
		}
		else {
			image = WriteOnImage("Loser", image, CardsCoords[i], Scalar(210, 66, 38));
			drawContours(image, vecPoint, i, Scalar(210, 66, 38), 2, 8, vecInfo, 0, Point());
		}
	}
	namedWindow("Final", CV_WINDOW_KEEPRATIO);
	imshow("Final", image);

	waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}