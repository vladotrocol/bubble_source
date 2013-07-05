#ifndef _INPUT_CHANNELS
#define _INPUT_CHANNELS


namespace InputChannels {
	static const int INVALID_CHANNEL=0;
	static const int VISUAL_CHANNEL=1;
	static const int PHYSICAL_CHANNEL=2;
	static const int PICK_CHANNEL=3;
	static const int DROP_CHANNEL=4;
	static const int DROP_OBJECT_CHANNEL=5;
	static const int MOVE_CHANNEL=6;
	static const int POINT_CHANNEL=7;
	static const int NEAR_CHANNEL=8;
	static const int FAR_CHANNEL=9;
	//New channels for version 0.2.5
	static const int TOGGLE_INVALID_CHANNEL=10;
	static const int TOGGLE_VISUAL_CHANNEL=11;
	static const int TOGGLE_PHYSICAL_CHANNEL=12;
	static const int TOGGLE_PICK_CHANNEL=13;
	static const int TOGGLE_DROP_CHANNEL=14;
	static const int TOGGLE_DROP_OBJECT_CHANNEL=15;
	static const int TOGGLE_MOVE_CHANNEL=16;
	static const int TOGGLE_POINT_CHANNEL=17;
	//New channels for version 0.3
	static const int ATTACH_CHANNEL=18;
	static const int DETACH_CHANNEL=19;
	static const int FEED_BACK_ALIVE_CHANNEL=20;
	static const int TRY_MOVE_CHANNEL=21;
	static const int SOUND_CHANNEL=22;

}


#endif



