#ifndef _INPUT_STREAM
#define _INPUT_STREAM

class InputStream{
public:
	virtual void reset()=0;
	virtual bool next()=0;
	virtual char read()=0;
};

#endif
