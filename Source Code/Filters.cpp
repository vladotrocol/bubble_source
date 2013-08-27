#include "Filters.h"

static Mat src_grey,
		resultThres, 
		resultErosion, 
		resultDilate;

//Constructor
Filters::Filters():
	thresholdValue(130), //was 60 /182
	thresholdType(0),
	erosionSize(4),
	dilationSize(10)
{;};

//Applies the requested filter to the corresponding source stream
Mat* Filters::applyFilter(char s, Mat* src){
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
Mat* Filters::thresholdFilter(Mat* src){
	if(src->type() == CV_8U){
		threshold(*src, resultThres, thresholdValue, 255, thresholdType);
	}
	else{
		cvtColor(*src, src_grey, CV_BGR2GRAY);
		threshold(src_grey, resultThres, thresholdValue, 255, thresholdType);
	}
	return &resultThres;
};

//Apply Erosion Filter
Mat* Filters::erosionFilter(Mat* src){
	if(erosionSize<=2){
		erosionSize=3;
	}
	Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(erosionSize, erosionSize), Point(2,2)); 
	erode( *src, resultErosion, element );
	return &resultErosion;
};

//Apply Dilation Filter
Mat* Filters::dilationFilter(Mat* src){
	if(dilationSize<=2){
		dilationSize=3;
	}
	Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(dilationSize, dilationSize), Point(2,2));
	dilate( *src, resultDilate, element );
	return &resultDilate;
};
