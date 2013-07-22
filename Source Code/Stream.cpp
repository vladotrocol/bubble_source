#include "Stream.h"

//Display streams in windows
//FLAG grammar is: (d(t||e||i)*)*
void Stream::display(char* s){
	string b = "_window_";
	generateControls();
	unsigned int j=0;
	while(j<strlen(s)){
		unsigned int i=j;
		do{
			imshow(s[j]+b+s[i], *(filter->applyFilter(s[i], _stream)));
			waitKey( 20 );
			i++;
		}while((char)s[i]!='d'&&i<strlen(s));
		j=i;
	}	
	char c = waitKey( 20 );
};

//Display detected bubbles as circles
void Stream::displayBubbles(vector<Bubble> &bubbles){
	RNG rng(12345);
	Mat drawing = Mat::zeros(_stream->size(), CV_8UC3 );
	for( unsigned int i = 0; i< bubbles.size(); i++ ){
		Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		if((int)bubbles[i].radius>0)
        circle( drawing, Point2f(bubbles[i].center.x, bubbles[i].center.y), (int)bubbles[i].radius, color, 2, 8, 0 );
    }
    imshow( "Contours", drawing );
    cvWaitKey(10);
};

//Generates the window with the filter sliders
void Stream::generateControls(){
	namedWindow("Controls", CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar( "ThreValue", "Controls",
                  &filter->thresholdValue, 255,
                  NULL);
	cvCreateTrackbar("ThreType","Controls",
				  &filter->thresholdType,
                  4, NULL);
	cvCreateTrackbar("EroSize","Controls",
				  &filter->erosionSize,
                  200, NULL);
	cvCreateTrackbar("DilSize","Controls",
				  &filter->dilationSize,
                  200, NULL);
	cvCreateTrackbar("MinRadius","Controls",
				  &minBubbleSize,
                  100, NULL);
	cvCreateTrackbar("MaxRadiu","Controls",
				  &maxBubbleSize,
                  500, NULL);
};
