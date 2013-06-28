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
	status=ST_READY;
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
	KOCVStream STREAM(kinect,filter);
	namedWindow("Contours", CV_WINDOW_AUTOSIZE );
	//Thread's main loop
	while(status==ST_PLAYING){
		//Do your processing
		STREAM.readFrame('d');
		Bubbles = detectBubbles(&filter, STREAM.depth_src);
		STREAM.display("d");
		STREAM.displayBubbles(Bubbles);
		char c = waitKey( 1 );
		this->updateFPS(true);
		_observer->update(0,0,0);
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
	findContours(filter->applyFilter('t',src), contours, hier, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	vector<Bubble> bubbles (contours.size());
	vector<vector<Point>> contours_poly(contours.size());

	if(contours.size() < 1){
		cout << "No Bubbles in frame\n";
	}
	else{
		for (unsigned int i = 0; i < contours.size(); i++){
			if (contourArea(contours[i]) > 10){
				approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
				minEnclosingCircle( (Mat)contours_poly[i], bubbles[i].center, bubbles[i].radius);
			}
		}
	}

	for (unsigned int i = 0; i < bubbles.size(); i++){
		if (bubbles[i].radius < minBubbleSize || bubbles[i].radius > maxBubbleSize || bubbles[i].center.x < 50){
			bubbles.erase(bubbles.begin() + i);
			i--;
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