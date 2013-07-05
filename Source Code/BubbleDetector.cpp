#include "Ogre.h"
#include "BubbleDetector.h"

void* fwthreadFunction(void* a);//Helper function to create the thread


void* fwthreadFunction(void* a){
		BubbleDetector* p=((BubbleDetector*)a);
		p->run();
		return NULL;
};

bool BubbleDetector::init(){
	bool ki = kinect.initialiseKinect();
	cout<<"Kinect: "<<ki<<'\n';
	KOCVStream* s = new KOCVStream(&kinect, &filter);
	_stream = s;
	status = ST_READY;
	return true;
};

bool BubbleDetector::start(){
	if(status==ST_READY){
		status=ST_PLAYING;
		pthread_create(&thread,NULL,fwthreadFunction,(void*)this);
	}
	return true;
};

void BubbleDetector::run(){
	//Thread's main loop
	while(status==ST_PLAYING){
		//Do your processing
		_stream->readFrame('d');
		bubbles = detectBubbles(&filter, _stream->depth_src);
		if(bubbles.size()<1){
			cout<<"no bubbles detected"<<'\n';
		}
		//_stream->display("di");
		//_stream->displayBubbles(bubbles);
		char c = waitKey( 1 );
		this->updateFPS(true);

		_observer->update();
		//cout<<bubbles[0].center.x;
		
		//If escape is pressed exit
		if( (char)c == 27 ){
			break; 
		}

		//Leave the processor (do this always! You have to let other threads get the processor)
		Sleep(1);
	}
	//The status says we have to end
	Sleep(1000);//Keep it	
};


bool BubbleDetector::stop(){
	if(status!=ST_PLAYING)
		return false;
	status=ST_READY;
	//await termination
	void* result;
	pthread_join(thread,&result);
	return true;
};

vector<Bubble> BubbleDetector::detectBubbles(Filters* filter, Mat src){
	vector<vector<Point>> contours;
	vector<Vec4i> hier;
	//Apply the whole processing pipeline (threshold, erode, dilate)
	findContours(filter->applyFilter('i',src), contours, hier, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	
	//Look for bubbles. 
	vector<Bubble> bubbles (contours.size());
	vector<vector<Point>> contours_poly(contours.size());
	if(contours.size() < 1){
		cout << "No Bubbles in frame\n";
	}
	else{
		for (unsigned int i = 0; i < contours.size(); i++){
			if (contourArea(contours[i]) > 10){
				//Find minimum circle
				approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
				//Compute its radius and position
				minEnclosingCircle( (Mat)contours_poly[i], bubbles[i].center, bubbles[i].radius);
			}
		}
	}
	//Remove bubbles that do not fit min and max sizes
	for (unsigned int i = 0; i < bubbles.size(); i++){
		if (bubbles[i].radius < minBubbleSize || bubbles[i].radius > maxBubbleSize || bubbles[i].center.x < 50){
			bubbles.erase(bubbles.begin() + i);
			i--;
		}
	}
	if(bubbles.size()){
	vector<Point2f> proj(bubbles.size());
	vector<Point2f> init(bubbles.size());
	for(unsigned int i=0;i<proj.size();i++){
		init[i] = bubbles[i].center;
	}
	perspectiveTransform(init, proj, _homography);
	for(unsigned int i=0;i<proj.size();i++){
		bubbles[i].center = proj[i];
	}
	}
	return bubbles;
};

void BubbleDetector::updateFPS(bool newFrame){
	static Ogre::Timer timer;
	static bool firstTime=true;
	static float lastSecond;
	static int imagesSinceLastSecond;
	static int cyclesSinceLastSecond;
	static const int measureOverNSeconds=5; //We print it over 5 seconds....
	float curSecond;
	float curTime=((float)(timer.getMicroseconds()+1)/1000000);
	//0. Initialize clock
	if(firstTime){
		timer.reset();
		imagesSinceLastSecond=cyclesSinceLastSecond=0;
		lastSecond=curSecond=1.0f*static_cast<int>(curTime);
		firstTime=false;
	}
	//1. Update number of images and cycles since last second
	cyclesSinceLastSecond++;
	if(newFrame)imagesSinceLastSecond++;	
	//2. Determine in the second is finished and notify if necessary
	curSecond=static_cast<int>(curTime);
	if(curSecond>=lastSecond+measureOverNSeconds){
		cout<<cyclesSinceLastSecond<<'\n';
		imagesSinceLastSecond=cyclesSinceLastSecond=0;lastSecond=curSecond;
	}	
			
};

void BubbleDetector::getHomography(Mat H){
	_homography = H;
};