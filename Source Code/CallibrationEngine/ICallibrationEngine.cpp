#include ".\CallibrationEngine\ICallibrationEngine.h"

ICallibrationEngine::~ICallibrationEngine(){;}
ICallibrationEngine* ICallibrationEngine::_instance=NULL;

boost::shared_ptr<WaterDisplayInfo> fake_read_config(){
	boost::shared_ptr<WaterDisplayInfo> product(new WaterDisplayInfo());
		//Create first layer:
		WaterLayerInfo first;
		first.srcPoints[0][0]=0;first.srcPoints[0][1]=0;
		first.srcPoints[1][0]=0;first.srcPoints[1][1]=1;
		first.srcPoints[2][0]=1;first.srcPoints[2][1]=1;
		first.srcPoints[3][0]=1;first.srcPoints[3][1]=0;
		first.dstPoints[0][0]=0;first.dstPoints[0][1]=0;
		first.dstPoints[1][0]=0;first.dstPoints[1][1]=1;
		first.dstPoints[2][0]=1;first.dstPoints[2][1]=1;
		first.dstPoints[3][0]=1;first.dstPoints[3][1]=0;
		first.curFrame=0;
		first.framesPerCycle=1;
		first.framesPerSecond=60;
		first.inputTexture="10points.png";
		first.outputTexture="layer1Texture";
		first.px_height=640;
		first.px_width=608;
		for(int i=0;i<60;i++){
			std::vector<std::string> v;
			std::ostringstream os;
			os<<((i%10)+1);
			v.push_back("stencil"+os.str()+".png");
			first.stencil.push_back(v);
		}
		//Create second layer:
		WaterLayerInfo second;
		second.srcPoints[0][0]=0;second.srcPoints[0][1]=0;
		second.srcPoints[1][0]=0;second.srcPoints[1][1]=1;
		second.srcPoints[2][0]=1;second.srcPoints[2][1]=1;
		second.srcPoints[3][0]=1;second.srcPoints[3][1]=0;
		second.dstPoints[0][0]=0;second.dstPoints[0][1]=0;
		second.dstPoints[1][0]=0;second.dstPoints[1][1]=1;
		second.dstPoints[2][0]=1;second.dstPoints[2][1]=1;
		second.dstPoints[3][0]=1;second.dstPoints[3][1]=0;
		second.curFrame=0;
		second.framesPerCycle=1;
		second.framesPerSecond=60;
		second.inputTexture="spheremap.png";
		second.outputTexture="layer2Texture";
		second.px_height=640;
		second.px_width=608;
		for(int i=0;i<60;i++){
			std::vector<std::string> v;
			std::ostringstream os;
			os<<(((i+1)%10)+1);
			v.push_back("stencil"+os.str()+".png");
			second.stencil.push_back(v);
		}
		//Create third layer:
		WaterLayerInfo third;
		third.srcPoints[0][0]=0;third.srcPoints[0][1]=0;
		third.srcPoints[1][0]=0;third.srcPoints[1][1]=1;
		third.srcPoints[2][0]=1;third.srcPoints[2][1]=1;
		third.srcPoints[3][0]=1;third.srcPoints[3][1]=0;
		third.dstPoints[0][0]=0;third.dstPoints[0][1]=0;
		third.dstPoints[1][0]=0;third.dstPoints[1][1]=1;
		third.dstPoints[2][0]=1;third.dstPoints[2][1]=1;
		third.dstPoints[3][0]=1;third.dstPoints[3][1]=0;
		third.curFrame=0;
		third.framesPerCycle=1;
		third.framesPerSecond=60;
		third.inputTexture="spheremap.png";
		third.outputTexture="layer3Texture";
		third.px_height=640;
		third.px_width=608;
		for(int i=0;i<60;i++){
			std::vector<std::string> v;
			std::ostringstream os;
			os<<(((i+2)%10)+1);
			v.push_back("stencil"+os.str()+".png");
			third.stencil.push_back(v);
		}
		//Create fourth layer:
		WaterLayerInfo fourth;
		fourth.srcPoints[0][0]=0;fourth.srcPoints[0][1]=0;
		fourth.srcPoints[1][0]=0;fourth.srcPoints[1][1]=1;
		fourth.srcPoints[2][0]=1;fourth.srcPoints[2][1]=1;
		fourth.srcPoints[3][0]=1;fourth.srcPoints[3][1]=0;
		fourth.dstPoints[0][0]=0;fourth.dstPoints[0][1]=0;
		fourth.dstPoints[1][0]=0;fourth.dstPoints[1][1]=1;
		fourth.dstPoints[2][0]=1;fourth.dstPoints[2][1]=1;
		fourth.dstPoints[3][0]=1;fourth.dstPoints[3][1]=0;
		fourth.curFrame=0;
		fourth.framesPerCycle=1;
		fourth.framesPerSecond=60;
		fourth.inputTexture="spheremap.png";
		fourth.outputTexture="layer4Texture";
		fourth.px_height=640;
		fourth.px_width=608;
		for(int i=0;i<60;i++){
			std::vector<std::string> v;
			std::ostringstream os;
			os<<(((i+3)%10)+1);
			v.push_back("stencil"+os.str()+".png");
			fourth.stencil.push_back(v);
		}
		//ADd the layers!
		product->layers.push_back(first);
		product->layers.push_back(second);
		product->layers.push_back(third);
		product->layers.push_back(fourth);
		return product;
}
