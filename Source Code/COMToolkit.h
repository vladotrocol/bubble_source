#ifndef _COM_TOOLKIT
#define _COM_TOOLKIT
/**
	This class hides all the details related to the COM connection. The classes that use it (WDDLayer and XmosConfig) do not need to know 
	anything about the COM interface. Actually, they no longer contain the HANDLE and do not depend on windows.h

*/
class COMToolkit{
public:
	enum Config_options{ON_PERIOD, OFFSET, CYCLE_TIME, CONFIG_LAYER, LOAD_LAYER, START, STOP};
	static void connect();
	static unsigned char readByte();
	static void sendByte(unsigned char b);
	static bool anyErrors();
	static void startListeningSyncSignal();
	static void stopListeningSyncSignal();
	static void closeConnection();
	static void awaitSyncSignal();
};


#endif