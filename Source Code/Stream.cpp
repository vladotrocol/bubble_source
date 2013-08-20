#include "Stream.h"
#include <string.h>
using namespace std;
//Display streams in windows
//FLAG grammar is: (d(t||e||i)*)*
void Stream::display(char* s){
	string b = "_window_";
	generateControls();
	 
	unsigned int j=0;
	while(j<strlen(s)){
		unsigned int i=j;
		do{
			circle(  *(filter->applyFilter((char)s[i], _stream)), Point2f(320, 240), 2, Scalar(0,0,0), 2, 8, 0 );
			imshow(s[j]+b+s[i], *(filter->applyFilter((char)s[i], _stream)));
			waitKey( 1 );
			i++;
		}while((char)s[i]!='d'&&i<strlen(s));
		j=i;
	}	
};

//Display detected bubbles as circles
void Stream::displayBubbles(vector<Bubble> &bubbles){
	RNG rng(12345);
	Mat drawing = Mat::zeros(_stream->size(), CV_8U );
	//std::vector<Mat> RGB;
   // split(*_stream, RGB);

	for( unsigned int i = 0; i< bubbles.size(); i++ ){
		//draw the contour
		if(bubbles[i].state){
			Scalar color = Scalar( 255, 255, 255 );
			if((int)bubbles[i].radius>0){
				//circle( drawing, Point2f(bubbles[i].center.x, bubbles[i].center.y), (int)bubbles[i].radius, color, 2, 8, 0 );
				//std::stringstream ss;
				//ss << bubbles[i].ID;
				//std::string s(ss.str());
				//cv::putText(drawing, s , cvPoint(bubbles[i].center.x-bubbles[i].radius/5, bubbles[i].center.y+bubbles[i].radius/5), FONT_HERSHEY_SCRIPT_SIMPLEX, 0.6, Scalar( 255, 255, 255 ), 2,3);
			
				int rad=(int)bubbles[i].radius/3;
				int b_centre_x=(int)bubbles[i].center.x;
				int b_centre_y=(int)bubbles[i].center.y;
				for (int x = -rad; x < rad; x++) 
					for (int y = -rad; y < rad; y++){
					float aux_x=x+b_centre_x;
					float aux_y=y+b_centre_y;
					if ((x*x + y*y)<(rad*rad)&&aux_x<640&&aux_y<480&&aux_x>0&&aux_y>0){
						drawing.at<BYTE>(Point2f(aux_x, aux_y))=255; //real data
					}
			}
			}
		}
    }
    imshow( "Bubbles", drawing );
    cvWaitKey(1);
};

//Display detected bubbles as circles
void Stream::displayTrackedBubbles(BubbleState* _state){
	RNG rng(12345);
	Mat drawing = Mat::zeros(_stream->size(), CV_8U );
	//std::vector<Mat> RGB;
   // split(*_stream, RGB);
	map<unsigned int, Bubble>* _curState = _state->getCurrentState();
		float radius;
	if(_curState->size()>0){
		for(map<unsigned int, Bubble>::iterator it = _curState->begin(); it != _curState->end(); ++it) {
			radius = it->second.radius;
			if(radius>0){
				Scalar color = Scalar( 255, 255, 255 );
				circle( drawing, Point2f(it->second.center.x, it->second.center.y), it->second.radius, color, 2, 8, 0 );
				std::stringstream ss;
				ss << it->second.ID;
				std::string s(ss.str());
				cv::putText(drawing, s , cvPoint(it->second.center.x-it->second.radius/5, it->second.center.y+it->second.radius/5), FONT_HERSHEY_SCRIPT_SIMPLEX, 0.6, Scalar( 255, 255, 255 ), 2,3);
			}
		}
	}
    imshow( "Contours", drawing );
    cvWaitKey(1);
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