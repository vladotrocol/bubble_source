#include "Ogre.h"
#include "BubbleDetector.h"

ofstream myfile;
ifstream myfile2;
bool TESTS_ON = false;
queue<float> times;

vector<vector<Point>> contours;
vector<Vec4i> hier;


void dumpLogs(){
	ofstream file1;
	file1.open ("DetectionLog.txt");
	while(!times.empty()){
		file1<<times.front()<<"\n";
		times.pop();
	}
	file1.close();
};

//Helper function to create the thread
void* fwthreadFunction(void* a);


void* fwthreadFunction(void* a){
		BubbleDetector* p=((BubbleDetector*)a);
		p->run();
		return NULL;
};

//Detection Initialisation
bool BubbleDetector::init(){
	//myfile.open ("bubblelog.txt");
	_homography = new Mat(); 
	if(!TESTS_ON){
		_capture = (Stream*) (new KOCVStream());
	}
	else{
		_capture = (Stream*) (new VideoStream("_inOutBubbles.avi"));
	}
	status = ST_READY;
	return true;
};


//Detection trigger
bool BubbleDetector::start(){
	if(status==ST_READY){
		status=ST_PLAYING;
		pthread_create(&thread,NULL,fwthreadFunction,(void*)this);
	}
	return true;
};

//Detection main loop
void BubbleDetector::run(){
	//Thread's main loop
		
	while(status==ST_PLAYING){
		//Do your processing
		_capture->readFrame();
		bubbles = detectBubbles();
		_capture->_stream->release();
		//-----------------Display stuff----------------
		//_capture->display("d");
		//_capture->displayBubbles(bubbles);
		//_capture->generateControls();
		waitKey(1);

		//--------------Print bubble positions to file----------------
		//for(int i=0;i<bubbles.size();i++){
		//	myfile<<bubbles[i].center.x<<" "<<bubbles[i].center.y<<'\n';
		//}
		//myfile<<"\n\n";

		//--------------Print the fps--------------
		//this->updateFPS(true);

		_observer->update();

		//Leave the processor (do this always! You have to let other threads get the processor)
		Sleep(1);
	}
	//The status says we have to end
	Sleep(1000);//Keep it	
};

//Detection kill
bool BubbleDetector::stop(){
	if(status!=ST_PLAYING)
		return false;
	status=ST_READY;
	//await termination
	void* result;
	pthread_join(thread,&result);
	dumpLogs();
	return true;
};

//Main Detection Method
vector<Bubble> BubbleDetector::detectBubbles(){
	Point2f circleCentre;
	float start = IClock::instance().getTimeMiliseconds();
	//Apply the whole processing pipeline (threshold, erode, dilate)
	findContours(*(_capture->filter->applyFilter('i',_capture->_stream)), contours, hier, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	
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
				minEnclosingCircle( (Mat)contours_poly[i], circleCentre, bubbles[i].radius);
				bubbles[i].center = Point3f(circleCentre.x, circleCentre.y, 0);
			}
		}
	}
	//Remove bubbles that do not fit min and max sizes
	for (unsigned int i = 0; i < bubbles.size(); i++){
		if (bubbles[i].radius < _capture->minBubbleSize || bubbles[i].radius > _capture->maxBubbleSize || bubbles[i].center.x < 50){
			bubbles.erase(bubbles.begin() + i);
			i--;
		}
	}
	if(bubbles.size()>0){
		vector<Point2f> proj(bubbles.size());
		vector<Point2f> init(bubbles.size());
		for(unsigned int i=0;i<proj.size();i++){
			init[i] = Point2f(bubbles[i].center.x,bubbles[i].center.y);
		}
		perspectiveTransform(init, proj, *_homography);
		for(unsigned int i=0;i<proj.size();i++){
			bubbles[i].center = Point3f(proj[i].x,proj[i].y,0);
		}
	}
	float finish = IClock::instance().getTimeMiliseconds();
	times.push(finish-start);
	return bubbles;
};

//A basic fps retriever
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

//Fetch the homography
void BubbleDetector::getHomography(Mat* H){
	_homography = H;
	//-------------Save the homography----------
	string filename = "Homography.xml";
	FileStorage fs(filename, FileStorage::WRITE);
	fs<<"H"<<*H;
};

/*void BubbleDetector::readHomography(){
	FileStorage fs.open(filename, FileStorage::READ);
	myfile2.open("homography.txt");
	myfile2>>readH;

	_homography = readH;
};*/