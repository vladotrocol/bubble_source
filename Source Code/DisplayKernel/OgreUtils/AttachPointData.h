#ifndef _ATTACH_POINT_DATA
#define _ATTACH_POINT_DATA
#include <string>
#include <Ogre.h>
namespace OgreUtils{

class AttachPointData{
	std::string nodeName;
public:
	Ogre::Matrix4 position;
	
	AttachPointData(std::string nodeName)
	: nodeName(nodeName)
	, position(Ogre::Matrix4::IDENTITY)
	{;}

	inline std::string getNodeName(){
		return nodeName;
	}


};
}

#endif
