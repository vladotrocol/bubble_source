#include "Ogre.h"

namespace OgreUtils{

class CameraUtils{

	static void reorientCamera(Ogre::SceneNode* upperNode, Ogre::Matrix4 screenPos ){
		//1. Calculate new values
		Ogre::Matrix4 headPos=upperNode->_getFullTransform();
		//Use virtual position
		Ogre::Vector3 position=headPos.getTrans();
		//Orient camera towards the screen
		Ogre::Quaternion screenQuat=screenPos.extractQuaternion();
		Ogre::Quaternion cameraOrientation=screenQuat*Ogre::Quaternion(Ogre::Radian(3.1415f),Ogre::Vector3(0,1,0));
		//Store the position of the camera
		Ogre::Matrix4 cameraPosition;
		cameraPosition.makeTransform(position,Ogre::Vector3(1,1,1),cameraOrientation);
		//...we have to express it in node's coordinates:
		cameraPosition=upperNode->getParentSceneNode()->_getFullTransform().inverse()*cameraPosition;
		//2. Apply new values		
		Ogre::Vector4 pos=cameraPosition*Ogre::Vector4(0,0,0,1);
		//upperNode->setPosition(pos[0],pos[1],pos[2]);
		upperNode->setOrientation(cameraPosition.extractQuaternion());
		upperNode->setScale(Ogre::Vector3(1,1,1));
	}

	static bool calculateProjectionData(Ogre::Matrix4 headPos,Ogre::Matrix4 screenPos,float width,float height,float *FOVy,float *dx,float *dy){
		Ogre::Vector4 hPos=screenPos.inverse() * headPos*Ogre::Vector4(0,0,0,1);
		if(hPos[0]==0 &&  hPos[1]==0 && hPos[2]==0)
			return false;
		double tgAngle= height/(2*hPos[2]);
		*FOVy=static_cast<float>( 2* atan(tgAngle));
		*dx=-hPos[0]/hPos[2];
		*dy=-hPos[1]/hPos[2];
		return true;
		//printf("headToScreen(%f, %f, %f), x=%f, y=%f, FOVy=%f\n", hPos[0], hPos[1], hPos[2], *dx, *dy, *FOVy);
		
	}

	static bool recalculateFrustum(Ogre::Camera* camera, Ogre::SceneNode* upperNode, Ogre::Matrix4 screenPos, float width, float height){
		float FOVy, x_offset, y_offset;		
		//1. Calculate new values:
		if (!calculateProjectionData(upperNode->_getFullTransform(),screenPos,width, height,&FOVy,&x_offset,&y_offset))
			return false;
		if(FOVy<=0) 
			return false;
		//2. Apply new values
		camera->setAspectRatio(width/height);
		camera->setFOVy(Ogre::Radian(FOVy));
		static bool testOffsets=true;
		if(testOffsets)
			camera->setFrustumOffset(x_offset,y_offset);
		else 
			camera->setFrustumOffset(0,0);
		return true;
	}

	


public:
	static bool updateCameraFrustum(Ogre::Camera* camera, Ogre::SceneNode* upperNode, Ogre::Matrix4 screenPos, float width, float height){
		//Set the position/orientation/projection volume of the camera.
		reorientCamera(upperNode,screenPos);
		return recalculateFrustum(camera,upperNode, screenPos,width,height);
	}

	static Ogre::TexturePtr configureRtt(std::string textureName, int bufferWidth, int bufferHeight, Ogre::Camera* recordFromCam, Ogre::RenderTargetListener* rtl){
		//Function body
		Ogre::TexturePtr rttTex = Ogre::TextureManager::getSingleton().createManual(textureName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, bufferWidth, bufferHeight, 0, Ogre::PF_R8G8B8A8, Ogre::TU_RENDERTARGET);
		//rttTex->getBuffer
		Ogre::RenderTexture *renderTexture = rttTex->getBuffer()->getRenderTarget();
		renderTexture->addViewport(recordFromCam);
		renderTexture->getViewport(0)->setClearEveryFrame(true);
		renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
		renderTexture->getViewport(0)->setOverlaysEnabled(false);
		renderTexture->addListener(rtl);
		return rttTex;
	}

	static Ogre::SceneNode* createMiniScreen(float x, float y, float w, float h, std::string materialName, Ogre::SceneNode* parentNode, Ogre::Rectangle2D** outEntity){
		Ogre::Rectangle2D *mMiniScreen = new Ogre::Rectangle2D(true);
		//Transform from UV coordinates to (-1,1)x(-1,1) coordinates
		float l=(x-0.5f)*2,t=-(y-0.5f)*2,r=(x-0.5f)*2+w*2,b=-(y-0.5f)*2-h*2;
		mMiniScreen->setCorners(l, t, r, b);
		mMiniScreen->setBoundingBox(Ogre::AxisAlignedBox(-100000.0f * Ogre::Vector3::UNIT_SCALE, 100000.0f * Ogre::Vector3::UNIT_SCALE));
		Ogre::SceneNode* miniScreenNode = parentNode->createChildSceneNode();
		miniScreenNode->attachObject(mMiniScreen);
		mMiniScreen->setMaterial(materialName);
		*outEntity=mMiniScreen;
		return miniScreenNode;
	}

	static void createMaterialFromTexture(std::string materialName,std::string textureName){
		Ogre::MaterialPtr renderMaterial = Ogre::MaterialManager::getSingleton().create(materialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		Ogre::Technique* matTechnique = renderMaterial->createTechnique();
		matTechnique->createPass();
		renderMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
		renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(textureName);	
	}

};

}