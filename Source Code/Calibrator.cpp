#include "Calibrator.h"

		bool homographyComputed;
		bool homogCamComputed;
		bool iHomog;
		Mat src, distortion2, result2, H, iH, faceImg;
		int dstCount;
		int srcCount;
		Point2f dstPoints[4];
		Point2f srcPoints[4];
		Stream* STREAM;

Calibrator::Calibrator(Stream* stream):s(stream){};

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
	static float offset=0;
	if( evt != CV_EVENT_LBUTTONDOWN )
        return;
	if(srcCount>=4)
		return;
	srcPoints[srcCount]=Point2f((float)x,(float)y-offset);
	// circle(img, center, radius, color, thickness, lineType, shift)
	circle(src, srcPoints[srcCount], 5, Scalar(1.0, 1.0, 0),5);
	imshow("source",src);

	if (!iHomog)
	{
		STREAM->readFrame();
		distortion2 = *STREAM->_stream;
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
	 dstPoints[dstCount]=Point2f((float)x,(float)y);
	 distortion2 = *STREAM->_stream;
	 circle(distortion2, dstPoints[dstCount], 5, Scalar(0.0, 1.0, 1.0),5);
	 imshow("distortion",distortion2);
	 dstCount++;
	 tryComputeHomography(srcPoints,srcCount,dstPoints,dstCount, src, distortion2);
};

//Function can be called to manually examine how well the homography has worked / aligns
void checkHomography()
{
	vector<Point2f> cam(4);
	vector<Point2f> proj(4);
	cam[0] = dstPoints[0];
	cam[1] = dstPoints[1];
	cam[2] = dstPoints[2];
	cam[3] = dstPoints[3];

	Mat drawing = Mat::zeros(768, 1024, CV_32FC3);
	perspectiveTransform(cam, proj, H);
	for (unsigned int i = 0; i < cam.size(); i++)
	{
		circle(drawing, proj[i], 20, Scalar(255,0,0), -1);
	}

	//Mat output(drawing.rows, drawing.cols, drawing.type());
	//warpPerspective(drawing, output, H, output.size());

	//cout << "height = " << height << ", width = " << width << "output.cols = " << output.cols << "output.rows = " << output.rows << "\n";

	namedWindow("Bubbles", CV_WINDOW_AUTOSIZE);
	moveWindow("Bubbles", 1680, 0);
	//setWindowProperty("Bubbles", CV_WND_PROP_AUTOSIZE, CV_WINDOW_NORMAL);

	imshow("Bubbles", drawing);
	waitKey(10);

}




void Calibrator::calibrateCameraProjector(){
	iHomog = false;
	STREAM = s;
	src = imread( "homographyCorrect.bmp", 1 );
	if(! src.data ) {
		cout <<  "Could not open or find the image" << std::endl ;
    }
	namedWindow("source",CV_WINDOW_AUTOSIZE);
	namedWindow("distortion",0); // was CV_WINDOW_AUTOSIZE
	imshow("source",src);
	moveWindow("source", 1680, 0);
	setWindowProperty("source", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	waitKey(10);

	int i = 0;

	STREAM->readFrame();
	//Create windows
	imshow("distortion",*(STREAM->_stream));
	
	//Add callbacks
	setMouseCallback("source", on_mouse_source, NULL );
	setMouseCallback("distortion", on_mouse_distortion, NULL );
	
	// Set the dst image the same type and size as src
	while (!homographyComputed){
		//cout<<"not computed"<<'\n';
		waitKey(10);
	}
	//checkHomography();
	srcCount = 0;
	dstCount = 0;

	destroyWindow("source");
	destroyWindow("distortion");
	_homography = H;
};

