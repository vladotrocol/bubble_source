#ifndef _OGRE_PRIMITIVES
#define _OGRE_PRIMITIVES

#include "Ogre.h"

class Primitives{
public:
static void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16);
static void createInvertedSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16);
};
#endif
