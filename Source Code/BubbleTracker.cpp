#include "BubbleTracker.h"

BubbleTracker::BubbleTracker(BubbleState* BS):_state(BS){};

bool BubbleTracker::init(){
	_detector = new BubbleDetector(this);
	_detector->init();
	_calibrator = new Calibrator(_detector->_stream);
	_calibrator->calibrateCameraProjector();
	return true;
};

bool BubbleTracker::start(){
	_detector->start();
	return true;
};

bool BubbleTracker::stop(){
	_detector->stop();
	return true;
};

void BubbleTracker::update(float x, float y, float radius){
	unsigned int untrID = _state->getUnknownBubble();
	if(untrID){
		_state->updateBubble(untrID, x, y, radius);
	}
};

//How many bubbles have popped? And which bubbles?
//Whether there are any new bubbles? Which bubbles?
//Once bubbles are identified, track them.
double distanceBetweenPoints(Point2f a, Point2f b){
	double res = pow((double)b.x - a.x, 2.0) + pow((double)b.y - a.y, 2.0);
	return sqrt(res);
}

pair<double, Point> distanceBetweenPoints(Point2f a, Point2f b, int j, int i){
	double res = pow((double)b.x - a.x, 2.0) + pow((double)b.y - a.y, 2.0);
	pair<double, Point> result; result.first = sqrt(res); result.second.y = j; result.second.x = i;
	return result;
}


//Sort functions for vectors
bool sortDistance(pair<double, Point> a, pair<double, Point> b) { return a.first > b.first; }
bool sortBubbleSize(Bubble a, Bubble b) { return a.radius > b.radius; }
bool sortIndexPre(pair<double, Point> a, pair<double, Point> b) { return a.second.y < b.second.y; }
bool sortIndexCurr(pair<double, Point> a, pair<double, Point> b) { return a.second.x < b.second.x; }

//Given a high frame rate, bubbles will not move that far between frames.
//Therefore, finding the nearest bubble is most likely to track the same bubble.
//Obviously, this is subject to error and some double checking must be conducted.
vector<pair<double, Point>> BubbleTracker::findNearestBubbles(){
	vector<pair<double, Point>> nearestBubbles(bubbles.size());
	for (int i = 0; i < bubbles.size(); i++)
	{
		vector<pair<double, Point>> temp;
		for (int j = 0; j < prevBubbles.size(); j++)
		{
			temp.push_back(distanceBetweenPoints(bubbles[i].center, prevBubbles[j].center, j, i));
		}
		sort(temp.begin(), temp.end(), sortDistance);
		nearestBubbles[i] = temp[0];
	}

	sort(nearestBubbles.begin(), nearestBubbles.end(), sortDistance);

	return nearestBubbles;
}

//Identify change in bubbles from previous frame. Assuming a high frame rate, bubbles will
//not have moved too significantly. Therefore, any bubbles in current frame not near a 
//bubble in previous frame, must be a new bubble. Any bubbles in previous frame not near a 
//bubble in current frame must have popped.
int BubbleTracker::guessBubblePoppedandBorn(vector<pair<double, Point>> bubbleComps)
{
	//Distance travelled greater than treshold - assume new bubble
	int born = 0;
	for (int i = 0; i < bubbleComps.size() && bubbleComps[i].first > travelMax; i++)
	{
		born++;
		bubbles[bubbleComps[i].second.x].ID = bubbles.size();
	}
	cout << "Estimate " << born << " bubbles born\n";

	//determine popped - sort by bubble number in previous frame
	//As findNearestBubble returned matches between frames, if
	//index is missing then previous bubble does not match current bubble
	//so assume popped. (Questionable whether it works - probably need to consider
	//vector of travel)
	int popped = 0;
	sort(bubbleComps.begin(), bubbleComps.end(), sortIndexPre);

	for (int i = 0; i < prevBubbles.size(); i++)
	{
		if (bubbleComps[i].second.y != (i + popped))
		{
			cout << "Bubble " << i << " popped!\n";
			popped++;
		}

		if (bubbleComps[i].first < travelMax)
		{
			//Close bubble, assume the same id
			bubbles[bubbleComps[i].second.x].ID = prevBubbles[bubbleComps[i].second.y].ID;
		}
	}
	return 0;
}