#include "Ogre.h"
#include "BubbleDetector.h"
#include <math.h>
bool TESTS_ON = false;
queue<float> times;

vector<vector<Point>> contours;
vector<Vec4i> hier;

//Dump the list of times to file
//Done when the project stops such that it does not slow it down
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
	if(!TESTS_ON){
		_capture = (Stream*) (new KOCVStream());
	}
	else{
		_capture = (Stream*) (new VideoStream("_2bubbles.avi"));
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

void BubbleDetector::applyHomography(){
		//Apply the homography
		if(bubbles.size()>0){
		vector<Point2f> proj(bubbles.size());
		vector<Point2f> init(bubbles.size());
		for(unsigned int i=0;i<proj.size();i++){
			init[i] = Point2f(bubbles[i].center.x,bubbles[i].center.y);
		}
		perspectiveTransform(init, proj, *_homography);
		for(unsigned int i=0;i<proj.size();i++){
			bubbles[i].center = Point3f(proj[i].x,proj[i].y,bubbles[i].center.z);
		}
		}
};

//Detection main loop
void BubbleDetector::run(){
	//Thread's main loop
		
	while(status==ST_PLAYING){

		//--------------Do your processing--------------
		_capture->readFrame();
		bubbles = detectBubbles();
		//bubbles = simulateBubbles();
		///_capture->display("di");
		//_capture->displayBubbles(bubbles);
		for(unsigned int i = 0; i<bubbles.size(); i++){
			bubbles[i].pixelsToMilimiters();
		}
		//-----------------Display stuff----------------

		//applyHomography();
		//waitKey(1);

		//--------------Print the fps--------------
		//this->updateFPS(true);
		_capture->_stream->release();
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
	//dumpLogs();
	return true;
};

int simulateCounter;
vector<Bubble> BubbleDetector::simulateBubbles(){
	vector<Bubble> bubbles(1);
	if(simulateCounter==0){
		bubbles[0].center = Point3f(-200.0f, 0.0f, 0.0f);
	}
	else if(simulateCounter==1){
		bubbles[0].center = Point3f(0.0f, 200.0f, 0.0f);
	}
		else if(simulateCounter==1){
		bubbles[0].center = Point3f(200.0f, 0.0f, 0.0f);
	}
	else if(simulateCounter==1){
		bubbles[0].center = Point3f(200.0f, 200.0f, 0.0f);
	}
	simulateCounter++;
	Sleep(300);
	return bubbles;
};

//Main Detection Method
vector<Bubble> BubbleDetector::detectBubbles(){
	Point2f circleCentre;
	//float start = IClock::instance().getTimeMiliseconds();//Timer
	Mat* _tempStream = _capture->filter->applyFilter('i',_capture->_stream);
	//Apply the whole processing pipeline (threshold, erode, dilate)
	if(!(_tempStream)->empty())
	findContours(*(_tempStream), contours, hier, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	//drawContours(*(_capture->_stream), contours, -1, Scalar(0, 0,0), -1);
	//imshow("DepthMil", Mat(480,640, CV_8U, _capture->dataPix));
	//Look for bubbles. 
	vector<Bubble> bubbles (contours.size());
	vector<vector<Point>> contours_poly(contours.size());
	if(contours.size() < 1){
		//cout << "No Bubbles in frame\n";
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
	
	
	for (unsigned int i = 0; i < bubbles.size(); i++){
		//Remove bubbles that do not fit min and max sizes
		if (bubbles[i].radius < _capture->minBubbleSize || bubbles[i].radius > _capture->maxBubbleSize){
			bubbles.erase(bubbles.begin() + i);
			i--;
		}
		else{
		float averageDist = 0;
		float count=0;


		int rad=(int)bubbles[i].radius/3;
		int b_centre_x=(int)bubbles[i].center.x;
		int b_centre_y=(int)bubbles[i].center.y;
		for (int x = -rad; x < rad; x++) 
			for (int y = -rad; y < rad; y++){
				float aux_x=x+b_centre_x;
				float aux_y=y+b_centre_y;
				if ((x*x + y*y)<(rad*rad)&&aux_x<640&&aux_y<480&&aux_x>0&&aux_y>0){
					//drawing.at<BYTE>(Point2f(aux_x, aux_y))=(unsigned char)(this->kinect->dataMil[(int)(aux_x+640*aux_y)]*256/4000);
					float value=(float)(_capture->dataMil[(int)(aux_x+640*aux_y)]);
					if(value<4000 && value>500){//If the value is within the kinect range, we count it.
						averageDist +=(float)(_capture->dataMil[(int)(aux_x+640*aux_y)]); //real data
						count+=1;
						//averageDist +=1;//(float)(_capture->_stream->at<ushort>(aux_x,aux_y)); //test data
						//cout<<(int)(_capture->dataMil[(int)(aux_x+640*aux_y)])<<' ';
					}
				}
			}
		if(count>0)
			bubbles[i].center.z = averageDist/count+bubbles[i].radius; //We detected to frontal part of the bubble, the center is radius cm behind (Not totally correct, ASK DIEGO)
		//cout<<"average: "<<bubbles[i].center.z<<"\n\n\n";
		}
			
	}


	//Timer
	//float finish = IClock::instance().getTimeMiliseconds();
	//times.push(finish-start);

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

//Fetch the homography and optionally save it to file
void BubbleDetector::getHomography(Mat* H){
	
	_homography = H;

	string filename = "Homography.xml";
	FileStorage fs(filename, FileStorage::WRITE);
	fs<<"H"<<*H;
};

//Read homography from file
void BubbleDetector::readHomography(){
	FileStorage fs("Homography.xml", FileStorage::READ);
	_homography = new Mat();
	fs["H"]>>*_homography;
};