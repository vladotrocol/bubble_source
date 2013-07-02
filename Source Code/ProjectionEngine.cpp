#include "ProjectionEngine.h"

ProjectionEngine::ProjectionEngine(BubbleState* BS, Calibrator* CA):_state(BS), _calibrator(CA){;};

void ProjectionEngine::Draw(){
	int size = _state->getCurrentState()->size();
	vector<Point2f> proj(size);
	vector<Point2f> bubbles;
	for(map<unsigned int, Bubble>::iterator it = _state->getCurrentState()->begin(); it != _state->getCurrentState()->end(); ++it) {
		bubbles.push_back(it->second.center);
	}
	Mat drawing = Mat::zeros(768, 1024, CV_32FC3);
	perspectiveTransform(bubbles, proj, _calibrator->_homography);
	for (int i = 0; i < size; i++)
	{
		//circle(drawing, cam[i], 20, Scalar(255,255,255), -1);
		circle(drawing, proj[i], 20, Scalar(255,0,0), -1);
	}

	//Mat output(drawing.rows, drawing.cols, drawing.type());
	//warpPerspective(drawing, output, H, output.size());

	//cout << "height = " << height << ", width = " << width << "output.cols = " << output.cols << "output.rows = " << output.rows << "\n";

	namedWindow("Bubbles", CV_WINDOW_AUTOSIZE);
	//moveWindow("Bubbles", 1280, 0);
	//setWindowProperty("Bubbles", CV_WND_PROP_AUTOSIZE, CV_WINDOW_NORMAL);

	imshow("Bubbles", drawing);
	waitKey(10);

};