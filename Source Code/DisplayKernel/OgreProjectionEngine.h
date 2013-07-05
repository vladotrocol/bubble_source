#include <stdio.h>
#include <sys/timeb.h>
#include <memory.h>
#include <string>
#include <vector>
#include <pthread.h>
#include "IBubbleTracker.h"

class BubbleApplication;

class OgreProjectionEngine{
	//Thread data
	pthread_mutex_t mutex;//lock while writing to avoid conflicts.
	pthread_mutex_t captureStarted;//Will be set to 1 when the first image is rendered.
	pthread_t thread;
	float waitTime;
	BubbleApplication *ogreApp;

	protected:
		static const int ST_INIT=0;
		static const int ST_READY=1;
		static const int ST_PLAYING=2;
		int status;
	public:
		virtual bool init(void);
		virtual bool start(void);
		virtual void run(void);
		virtual bool stop(void);
};