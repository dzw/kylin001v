#pragma once

#include "standard.h"

#include <Ogre.h>
#include <OIS.h>
#include <Terrain/OgreTerrainGroup.h>
#include "boost/any.hpp"

typedef boost::any	KANY;

#define KPoint2		Ogre::Vector2 
#define KPoint3		Ogre::Vector3 
#define KMatrix3	Ogre::Matrix3 
#define KMatrix4	Ogre::Matrix4 
#define KColor		Ogre::ColourValue

#define KKeyCode	OIS::KeyCode

//---------------------------------------------------------
namespace Kylin
{
	class Node;
	class XmlStream;
	class ScriptVM;
	class AppFrame;
	class InputListener;
	class InputManager;
	class GuiManager;
	class CameraControl;
	class SceneLoader;
	
	class CommandHandler;
	class CursorEx;
	
	class DataUnit;
	class DataLoader;

	extern AppFrame* g_theApp;
}

namespace OgreBites
{
	class SdkTrayManager;
	class SdkTrayListener;
}

namespace MyGUI 
{ 
	class OgrePlatform; 
}

struct lua_State;
//---------------------------------------------------------

using namespace Ogre;
