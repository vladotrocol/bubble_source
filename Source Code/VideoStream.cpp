#include "VideoStream.h"

VideoStream::VideoStream(char* fileNam):
				fileName(fileNam){
	cap = new VideoCapture(fileNam);
	filter = new Filters();
	_stream  = new Mat();
};

void VideoStream::readFrame(){
	if(!cap->isOpened())  // check if we succeeded
        cerr<<"cannot open video file";
	Mat image;
	cap->read(image);
	if(image.empty()){
		cap = new VideoCapture(fileName);
	}else{
		image.copyTo(*_stream);
	}
	image.release();
};