#ifndef _BUBBLE_DETECTOR
#define _BUBBLE_DETECTOR

#include <iostream>
#include "KOCVStream.h"
#include <assert.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <memory.h>
#include <string>
#include <vector>
#include <pthread.h>
#include "IBubbleTracker.h"
#include "Calibrator.h"
#include "ProjectionEngine.h"
#include "VideoStream.h"

class IBubbleDetector{
	public:
		virtual bool init(void)=0;
		virtual bool start(void)=0;
		virtual void run(void)=0;
		virtual bool stop(void)=0;
};

class BubbleDetector: public IBubbleDetector{
	//Thread data
	pthread_mutex_t mutex;//lock while writing to avoid conflicts.
	pthread_mutex_t captureStarted;//Will be set to 1 when the first image is grabbed.
	pthread_t thread;
	float waitTime;

	protected:
		static const int ST_INIT=0;
		static const int ST_READY=1;
		static const int ST_PLAYING=2;
		int status;
		IBubbleTracker* _observer;

	public:
		vector<Bubble> bubbles;
		Filters filter;
		Kinect kinect;
		Stream* _capture;
		Mat* _homography;

		BubbleDetector(IBubbleTracker * observer){
			_observer=observer;
		}
		bool init(void);
		bool start(void);
		void run(void);
		bool stop(void);
		vector<Bubble> detectBubbles(Filters* filter, Mat* src);
		void updateFPS(bool newFrame);
		void getHomography(Mat* H);
};
#endif