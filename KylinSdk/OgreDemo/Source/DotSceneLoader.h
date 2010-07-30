#ifndef DOT_SCENELOADER_H
#define DOT_SCENELOADER_H

// Includes
#include <OgreString.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreResourceGroupManager.h>

#include "Hydrax.h"
#include "./Noise/Perlin/Perlin.h"
#include "./Modules/ProjectedGrid/ProjectedGrid.h"

#include "rapidxml.hpp"

#include "ClSceneLoader.h"
#include "Scene.h"
#include "rRenderable.h"


	// Forward declarations
	namespace Ogre
	{
		class SceneManager;
		class SceneNode;
		class TerrainGroup;
        class TerrainGlobalOptions;
	}

	class nodeProperty
	{
	public:
		Ogre::String nodeName;
		Ogre::String propertyNm;
		Ogre::String valueName;
		Ogre::String typeName;

		nodeProperty(const Ogre::String &node, const Ogre::String &propertyName, const Ogre::String &value, const Ogre::String &type)
			: nodeName(node), propertyNm(propertyName), valueName(value), typeName(type) {}
	};

	class DotSceneLoader : public Kylin::ClSceneLoader, public Kylin::Renderable
	{
	public:
        Ogre::TerrainGlobalOptions *mTerrainGlobalOptions;
        
        DotSceneLoader();
		~DotSceneLoader();
		
		virtual KBOOL LoadScene(KSTR mSceneFile);
		virtual KVOID Unload(Kylin::SceneHag* pHag);

		void parseDotScene(const Ogre::String &SceneName, const Ogre::String &groupName, Ogre::SceneManager *yourSceneMgr, Ogre::SceneNode *pAttachNode = NULL, const Ogre::String &sPrependNode = "");
		
		virtual KVOID Tick(KFLOAT fElapsed);
		virtual KVOID OnRenderStarted(KFLOAT fElapsed);

	protected:
		void processScene(rapidxml::xml_node<>* XMLRoot);

		void processNodes(rapidxml::xml_node<>* XMLNode);
		void processEnvironment(rapidxml::xml_node<>* XMLNode);

		void processHydrax(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
		void createWaterPlane(Ogre::SceneManager *scenemgr, Ogre::Camera *camera, Ogre::Viewport *viewport);
 		void addDepthTechnique(const Ogre::String& matname);	
// 		void removeDepthTechnique(const Ogre::String& matname); 
		void UpdataHydrax(KFLOAT fElapsed);

		void processTerrain(rapidxml::xml_node<>* XMLNode);
		void processTerrainPage(rapidxml::xml_node<>* XMLNode);
		void processBlendmaps(rapidxml::xml_node<>* XMLNode);
		//void processUserDataReference(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
		//void processUserDataReference(rapidxml::xml_node<>* XMLNode, Ogre::Entity *pEntity);
		void processLight(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
		void processCamera(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);

		void processNode(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent = 0);
		void processEntity(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
		void processCollision(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
		void processParticleSystem(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
		// 添加 新粒子系统
		void processNewParticleSystem(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
		void processBillboardSet(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
		void processPlane(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent);
		
		// 添加场景管理的阴影读取
		void processSceneShadows(rapidxml::xml_node<>* XMLNode);
		// 添加场景深度阴影纹理
		Ogre::MaterialPtr buildDepthShadowMaterial(Ogre::MaterialPtr cpyMat);
		// 添加PSSM 阴影技术
		void setupPSSM();
		// 阴影配置
		void configureShadows(rapidxml::xml_node<>* XMLNode); 

		void processFog(rapidxml::xml_node<>* XMLNode);
		void processClipping(rapidxml::xml_node<>* XMLNode);

		void processLightRange(rapidxml::xml_node<>* XMLNode, Ogre::Light *pLight);
		void processLightAttenuation(rapidxml::xml_node<>* XMLNode, Ogre::Light *pLight);

		Ogre::String getAttrib(rapidxml::xml_node<>* XMLNode, const Ogre::String &parameter, const Ogre::String &defaultValue = "");
		Ogre::Real getAttribReal(rapidxml::xml_node<>* XMLNode, const Ogre::String &parameter, Ogre::Real defaultValue = 0);
		bool getAttribBool(rapidxml::xml_node<>* XMLNode, const Ogre::String &parameter, bool defaultValue = false);

		Ogre::Vector3 parseVector3(rapidxml::xml_node<>* XMLNode);
		Ogre::Quaternion parseQuaternion(rapidxml::xml_node<>* XMLNode);
		Ogre::ColourValue parseColour(rapidxml::xml_node<>* XMLNode);
		
	protected:

		Ogre::SceneManager *	mSceneMgr;
		Ogre::SceneNode *		mAttachNode;
		Ogre::String			m_sGroupName;
		Ogre::String			m_sPrependNode;
		//Ogre::TerrainGroup* mTerrainGroup;
		Ogre::Vector3 mLightDirection;

		Ogre::ShadowCameraSetupPtr  mPSSMSetup;
		bool						mShadowsEnabled;
		
		Ogre::Terrain*						mTerrainHandle;   

		Hydrax::Hydrax*						mHydraxHandle;
		Hydrax::Module::ProjectedGrid *		mHydraxModule;
	};

#endif // DOT_SCENELOADER_H
