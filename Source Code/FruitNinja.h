#ifndef _FRUIT_NINJA
#define _FRUIT_NINJA

#include <pthread.h>
#include "Bubble.h"
#include "BubbleState.h"
#include "BubbleGenerator.h"

class FruitNinja{
	protected:
		static const int ST_INIT=0;
		static const int ST_READY=1;
		static const int ST_PLAYING=2;
		int status;
	public: 
		//Thread data
		pthread_mutex_t mutex;//lock while writing to avoid conflicts.
		pthread_mutex_t captureStarted;//Will be set to 1 when the first image is grabbed.
		pthread_t thread;
		BubbleState* _state;
		BubbleGenerator* _generator;
		FruitNinja(BubbleState* BS, BubbleGenerator* BG);
		bool init();
		bool start();
		void run();
		bool stop();
		void createBubble(unsigned int ID, char* material);
};
#endif