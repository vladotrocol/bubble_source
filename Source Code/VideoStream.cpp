#include "VideoStream.h"

//Constructor
VideoStream::VideoStream(char* fileNam):
				fileName(fileNam){
	cap = new VideoCapture(fileNam);
	filter = new Filters();
	_stream  = new Mat();
};

//Read a frame from file
void VideoStream::readFrame(){

	//Check if valid reader
	if(!cap->isOpened()){
        cerr<<"cannot open video file";
	}

	//Read the next frame from video
	Mat image;
	cap->read(image);

	if(image.empty()){
		//The last frame is always empty..
		//If so, reset the reader
		cap = new VideoCapture(fileName);
	}else{
		//Read the frame
		image.copyTo(*_stream);
	}

	//No memory leaks
	image.release();
};