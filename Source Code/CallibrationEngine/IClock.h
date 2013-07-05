#ifndef _ICLOCK
#define _ICLOCK


class IClock{
protected:
	static IClock* _instance;
	IClock(){;}
public:
	static IClock& instance();
	virtual void nextSecond();
	virtual float getTimeMiliseconds();
};

#endif