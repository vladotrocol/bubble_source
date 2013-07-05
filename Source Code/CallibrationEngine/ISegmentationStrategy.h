#ifndef _SEGMENTATION_STRATEGY
#define _SEGMENTATION_STRATEGY
#include "./CallibrationEngine/IPlugin.h"
#include <opencv2/core/core.hpp>
class IEventProducer;

class ISegmentationStrategy: public IPlugin {
public:
	
	/**
		The Strategy will use some input parameters to tune itself (i.e. thresholding value to use). 
		It can register VariableParameters into the application so that these values will be modified when a keypress happens in the main application.
		This method allows the Strategy to register these parameters.
	*/
	virtual void config(IEventProducer& mainApp){;}
	/**
		This method performs an image processing algorithm to compute a valid stencil. 
		@param source: (Input) Image captured from the camera
		@param bgnd: (Input) Background image.
		@param frgnd: (Output) Foreground identified.  Actually, anything that you want to display in the third screen...
		@param stencil: (Output) Image that will be used as a stencil by the system.(This could change in the future as we include more layers. Instead of one image, we should return an image (stencil) for each layer. As a result, this parameter could become std::vector<cv::Mat>)
	*/
	virtual void imageProcess(const cv::Mat& source, const cv::Mat& bgnd, cv::Mat& frgnd, cv::Mat& stencil )=0;

	virtual std::vector<cv::Mat> detectStencils(const cv::Mat& stencil){
		std::vector<cv::Mat> result;
		result.push_back(stencil);
		return result;
	}

	virtual void resetDetection(){;}

	/**
		This method returns a string that explains the current status of the strategy and the keypresses that can be used to control it.
	*/

	virtual std::string getHelpString()=0;

	virtual ~ISegmentationStrategy(){;}
};

#endif
