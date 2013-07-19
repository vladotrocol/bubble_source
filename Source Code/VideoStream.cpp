#include "VideoStream.h"

VideoStream::VideoStream(Filters* f, char* fileNam):
				filter(f),
				fileName(fileNam){
	TestON = true;
	cap = new VideoCapture(fileNam);
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
			imshow("haha", *_stream);
		waitKey(1);
	image.release();
};