#ifndef _IBUBBLE_TRACKER_C
#define _IBUBBLE_TRACKER_C

class IBubbleTracker{
	public:
		virtual bool init(void)=0;
		virtual bool start(void)=0;
		virtual void update(float x, float y, float radius)=0;
		virtual bool stop(void)=0;
		virtual void printBubbles(void)=0;
};

#endif