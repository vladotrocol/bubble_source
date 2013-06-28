#include "Calibrator.h"

		bool homographyComputed;
		bool homogCamComputed;
		bool iHomog;
		Mat src, distortion2, result2, H, iH, faceImg;
		int dstCount;
		int srcCount;
		Point2f dstPoints[4];
		Point2f srcPoints[4];
		KOCVStream* STREAM;

Calibrator::Calibrator(KOCVStream& stream):s(&stream){};

void tryComputeHomography(Point2f srcPoints[],int srcCount, Point2f dstPoints[], int dstCount,Mat source, Mat distortion){
	if(srcCount<4 || dstCount<4)
		return;
	if (!iHomog){
		H=getPerspectiveTransform(dstPoints,srcPoints); // Calculate the perspective transform from 4 pairs of corresponding points
		cout << "H=" <<H << "\n";
		result2=Mat::zeros( distortion.rows, distortion.cols, distortion.type() ); // Create a zeroed matrix the same size as the distortion matrix
		warpPerspective( distortion, result2, H, result2.size() ); // Apply the perspective transformation to the 'result' matrix
		cvSetWindowProperty("distortion", CV_WND_PROP_AUTOSIZE, CV_WINDOW_NORMAL);
		cvSetWindowProperty("source", CV_WND_PROP_AUTOSIZE, CV_WINDOW_NORMAL);
		imshow("distortion", result2); // Display the perspective transformation image
		homographyComputed = true;
	}
	else{
		iH=getPerspectiveTransform(dstPoints,srcPoints);
		result2=Mat::zeros( distortion.rows, distortion.cols, distortion.type() ); // Create a zeroed matrix the same size as the distortion matrix
		warpPerspective( distortion, result2, iH, result2.size() );
		imshow("distortion", result2);
		homogCamComputed = true;
	}
	return;
 }

void on_mouse_source(int evt, int x, int y, int flags, void* param){
	static int offset=0;
	if( evt != CV_EVENT_LBUTTONDOWN )
        return;
	if(srcCount>=4)
		return;
	srcPoints[srcCount]=Point2f(x,y-offset);
	// circle(img, center, radius, color, thickness, lineType, shift)
	circle(src, srcPoints[srcCount], 5, Scalar(1.0, 1.0, 0),5);
	imshow("source",src);

	if (!iHomog)
	{
		STREAM->readFrame('d');
		distortion2 = STREAM->depth_src;
		imshow("distortion", distortion2);
	}
	srcCount++;
	tryComputeHomography(srcPoints, srcCount, dstPoints, dstCount, src, distortion2);
};


void on_mouse_distortion(int evt, int x, int y, int flags, void* param){
	 if( evt != CV_EVENT_LBUTTONDOWN )
        return;
	 if(dstCount>=4)
		return;
	 dstPoints[dstCount]=Point2f(x,y);
	 distortion2 = STREAM->depth_src;
	 circle(distortion2, dstPoints[dstCount], 5, Scalar(0.0, 1.0, 1.0),5);
	 imshow("distortion",distortion2);
	 dstCount++;
	 tryComputeHomography(srcPoints,srcCount,dstPoints,dstCount, src, distortion2);
};


void Calibrator::calibrateCameraProjector(){
	iHomog = false;
	STREAM = s;
	src = imread( "homographyCorrect.bmp", 1 );
	namedWindow("source",0  );
	namedWindow("distortion",0); // was CV_WINDOW_AUTOSIZE
	imshow("source",src);
	//moveWindow("source", 1280, 0);
	//setWindowProperty("source", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	waitKey(10);

	Mat res;

	int i = 0;

	STREAM->readFrame('d');
	res = STREAM->depth_src;

	res.copyTo(distortion2);// = readFrameRGB(); //imread("homographyDistortion.bmp",1);
	//Create windows
	imshow("distortion",distortion2);
	//Add callbacks
	setMouseCallback("source", on_mouse_source, NULL );
	setMouseCallback("distortion", on_mouse_distortion, NULL );
	// on_mouse();
	/// Set the dst image the same type and size as src
	while (!homographyComputed)
		waitKey(10);

	srcCount = 0;
	dstCount = 0;

	destroyWindow("source");
	destroyWindow("distortion");
};


