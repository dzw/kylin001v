#ifndef GameApp_h__
#define GameApp_h__

//#include "BaseApplication.h"
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#include "GameClient.h"

class DotSceneLoader;

using namespace Kylin;

class GameApp : public Kylin::GameClient
{
public:
	GameApp(void);
	virtual ~GameApp(void);
	

protected:
	virtual KBOOL	LoadResources();
	virtual KVOID	OnIdle(KFLOAT fElapsed);
	
	virtual Kylin::SceneLoader*	CreateSceneLoader();

private:
	
	Ogre::TerrainGroup* mTerrainGroup;
	//DotSceneLoader* mLoader;
	bool mTerrainImported;
	Ogre::String mSceneFile;
	Ogre::String mHelpInfo;
	
	Ogre::Real mFallVelocity;
};


#endif // GameApp_h__