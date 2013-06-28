#include "Filters.h"

//Constructor
Filters::Filters():
	thresholdValue(1),
	thresholdType(1),
	erosionSize(10),
	dilationSize(10)
{};

//Applies the requested filter to the corresponding source stream
Mat Filters::applyFilter(char s, Mat src){
	if(s == 't'){
		return thresholdFilter(src);
	}else if(s == 'e'){
		return erosionFilter(thresholdFilter(src));
	}else if(s == 'i'){
		return dilationFilter(erosionFilter(thresholdFilter(src)));
	}else{
		return src;
	}
};

//Apply Threshold Filer
Mat Filters::thresholdFilter(Mat src){
	Mat src_grey, result;
	if(src.type() == CV_8U){
		threshold(src, result, thresholdValue, 255, thresholdType);
	}
	else{
		cvtColor(src, src_grey, CV_BGR2GRAY);
		threshold(src_grey, result, thresholdValue, 255, thresholdType);
	}
	return result;
};

//Apply Erosion Filter
Mat Filters::erosionFilter(Mat src){
	Mat result;
	if(erosionSize<=2){
		erosionSize=3;
	}
	Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(erosionSize, erosionSize), Point(2,2));
	erode( src, result, element );
	return result;
};

//Apply Dilation Filter
Mat Filters::dilationFilter(Mat src){
	Mat result;
	if(dilationSize<=2){
		dilationSize=3;
	}
	Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(dilationSize, dilationSize), Point(2,2));
	dilate( src, result, element );
	return result;
};
