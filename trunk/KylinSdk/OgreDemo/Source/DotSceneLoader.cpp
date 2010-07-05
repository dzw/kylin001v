#include "stdafx.h"
#include "DotSceneLoader.h"
#include "rOgreRoot.h"

#include <Ogre.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include <Terrain/OgreTerrainMaterialGeneratorA.h>

#include "SkyXWapper.h"
#include "CameraControl.h"
#include "ParticleUniverseSystemManager.h"
#include "KylinRoot.h"
#include "GameStatus.h"
#include "Property.h"
#include "rPhyXSystem.h"
#include "rCollisionMonitor.h"
#include "rOrientedBox.h"


#pragma warning(disable:4390)
#pragma warning(disable:4305)

using namespace Kylin;


DotSceneLoader::DotSceneLoader()
: mSceneMgr(NULL)
, mTerrainHandle(NULL)
, mHydraxHandle(NULL)
, mHydraxModule(NULL)
, mAttachNode(NULL)
{
    mTerrainGlobalOptions = OGRE_NEW Ogre::TerrainGlobalOptions();
}


DotSceneLoader::~DotSceneLoader()
{
	//mSceneMgr->setSkyBox(false,"");
	//mSceneMgr->setSkyDome(false,"");
   
}

KVOID DotSceneLoader::parseDotScene(const Ogre::String &SceneName, const Ogre::String &groupName, Ogre::SceneManager *yourSceneMgr, Ogre::SceneNode *pAttachNode, const Ogre::String &sPrependNode)
{
	// set up shared object values
	m_sGroupName = groupName;
	mSceneMgr = yourSceneMgr;
	m_sPrependNode = sPrependNode;

	rapidxml::xml_document<> XMLDoc;    // character type defaults to char

	rapidxml::xml_node<>* XMLRoot;

	Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(SceneName, groupName );
	char* scene = strdup(stream->getAsString().c_str());
	XMLDoc.parse<0>(scene);

	// Grab the scene node
	XMLRoot = XMLDoc.first_node("scene");

	// Validate the File
	if( getAttrib(XMLRoot, "formatVersion", "") == "")
	{
		Ogre::LogManager::getSingleton().logMessage( "[DotSceneLoader] Error: Invalid .scene File. Missing <scene>" );
		return;
	}

	// figure out where to attach any nodes we create
	mAttachNode = pAttachNode;
	if(!mAttachNode)
		mAttachNode = mSceneMgr->getRootSceneNode();

	// Process the scene
	processScene(XMLRoot);
}

KVOID DotSceneLoader::processScene(rapidxml::xml_node<>* XMLRoot)
{
	// Process the scene parameters
	Ogre::String version = getAttrib(XMLRoot, "formatVersion", "unknown");

	Ogre::String message = "[DotSceneLoader] Parsing dotScene file with version " + version;
	if(XMLRoot->first_attribute("ID"))
		message += ", id " + Ogre::String(XMLRoot->first_attribute("ID")->value());
	if(XMLRoot->first_attribute("sceneManager"))
		message += ", scene manager " + Ogre::String(XMLRoot->first_attribute("sceneManager")->value());
	if(XMLRoot->first_attribute("minOgreVersion"))
		message += ", min. Ogre version " + Ogre::String(XMLRoot->first_attribute("minOgreVersion")->value());
	if(XMLRoot->first_attribute("author"))
		message += ", author " + Ogre::String(XMLRoot->first_attribute("author")->value());

	Ogre::LogManager::getSingleton().logMessage(message);

	rapidxml::xml_node<>* pElement;

	// Process environment (?)
	pElement = XMLRoot->first_node("environment");
	if(pElement)
		processEnvironment(pElement);

	// Process nodes (?)
	pElement = XMLRoot->first_node("nodes");
	if(pElement)
		processNodes(pElement);

	// Process externals (?)
// 	pElement = XMLRoot->first_node("externals");
// 	if(pElement)
// 		processExternals(pElement);
// 
// 	// Process octree (?)
// 	pElement = XMLRoot->first_node("octree");
// 	if(pElement)
// 		processOctree(pElement);

	// Process light (?)
	//pElement = XMLRoot->first_node("light");
	//if(pElement)
	//	processLight(pElement);

	// Process camera (?)
	pElement = XMLRoot->first_node("camera");
	if(pElement)
		processCamera(pElement);

	// Process terrain (?)
	pElement = XMLRoot->first_node("terrain");
	if(pElement)
		processTerrain(pElement);

	//Process hydrax
	pElement = XMLRoot->first_node("water");
	if(pElement)
		processHydrax(pElement);
}

KVOID DotSceneLoader::processNodes(rapidxml::xml_node<>* XMLNode)
{
	rapidxml::xml_node<>* pElement;

	// Process node (*)
	pElement = XMLNode->first_node("node");
	while(pElement)
	{
		processNode(pElement);
		pElement = pElement->next_sibling("node");
	}

	// Process position (?)
	pElement = XMLNode->first_node("position");
	if(pElement)
	{
		mAttachNode->setPosition(parseVector3(pElement));
		mAttachNode->setInitialState();
	}

	// Process rotation (?)
	pElement = XMLNode->first_node("rotation");
	if(pElement)
	{
		mAttachNode->setOrientation(parseQuaternion(pElement));
		mAttachNode->setInitialState();
	}

	// Process scale (?)
	pElement = XMLNode->first_node("scale");
	if(pElement)
	{
		mAttachNode->setScale(parseVector3(pElement));
		mAttachNode->setInitialState();
	}
}

KVOID DotSceneLoader::processEnvironment(rapidxml::xml_node<>* XMLNode)
{
	rapidxml::xml_node<>* pElement;

	// Process camera (?)
	pElement = XMLNode->first_node("camera");
	if(pElement)
		processCamera(pElement);

	// Process fog (?)
	pElement = XMLNode->first_node("fog");
	if(pElement)
		processFog(pElement);

	if (!m_pSkyWapper)
		m_pSkyWapper = KNEW SkyXWapper(mSceneMgr,OgreRoot::GetSingletonPtr()->GetCamera("$MainCamera"));//GetCameraController()->GetActiveCamera());

	// Process clipping (?)
	pElement = XMLNode->first_node("clipping");
	if(pElement)
		processClipping(pElement);

	// Process colourAmbient (?)
	pElement = XMLNode->first_node("colourAmbient");
	if(pElement)
		mSceneMgr->setAmbientLight(parseColour(pElement));

	// Process colourAmbient (?)
	pElement = XMLNode->first_node("shadows");
	//if(pElement)
	//	processSceneShadows(pElement);

	// Process colourBackground (?)
	//! @todo Set the background colour of all viewports (RenderWindow has to be provided then)
	pElement = XMLNode->first_node("colourBackground");
	if(pElement)
		;//mSceneMgr->set(parseColour(pElement));

}

KVOID DotSceneLoader::processSceneShadows(rapidxml::xml_node<>* XMLNode) 
{
	configureShadows(XMLNode);
	setupPSSM();
}

Ogre::MaterialPtr DotSceneLoader::buildDepthShadowMaterial(Ogre::MaterialPtr cpyMat) 
{
	if(mSceneMgr->getShadowTechnique() >= (int)Ogre::SHADOWTYPE_TEXTURE_ADDITIVE && 
		mSceneMgr->getShadowTechnique() <= (int)Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED)
	{
		Ogre::String matName = "DepthShadows/" + cpyMat->getName();

		Ogre::MaterialPtr ret = Ogre::MaterialManager::getSingleton().getByName(matName);
		if (ret.isNull())
		{
			ret = cpyMat->clone(matName);

			Ogre::Technique *t = ret->getTechnique(0);
			t->setShadowCasterMaterial("Ogre/shadow/depth/caster");
			Ogre::Pass *p = t->getPass(0);
			p->setVertexProgram("Ogre/shadow/receiver/depth/pssm3/vp");
			p->setFragmentProgram("Ogre/shadow/receiver/depth/pssm3/fp");

			Ogre::TextureUnitState *tu = p->createTextureUnitState();
			tu->setName("shadow0");
			tu->setContentType(Ogre::TextureUnitState::CONTENT_SHADOW);
			tu->setTextureAddressingMode(Ogre::TextureUnitState::TAM_BORDER);
			tu->setTextureBorderColour(Ogre::ColourValue(1,1,1,1));

			tu = p->createTextureUnitState();
			tu->setName("shadow1");
			tu->setContentType(Ogre::TextureUnitState::CONTENT_SHADOW);
			tu->setTextureAddressingMode(Ogre::TextureUnitState::TAM_BORDER);
			tu->setTextureBorderColour(Ogre::ColourValue(1,1,1,1));

			tu = p->createTextureUnitState();
			tu->setName("shadow2");
			tu->setContentType(Ogre::TextureUnitState::CONTENT_SHADOW);
			tu->setTextureAddressingMode(Ogre::TextureUnitState::TAM_BORDER);
			tu->setTextureBorderColour(Ogre::ColourValue(1,1,1,1));

			Ogre::Vector4 splitPoints;
			const Ogre::PSSMShadowCameraSetup::SplitPointList& splitPointList = 
				static_cast<Ogre::PSSMShadowCameraSetup*>(mPSSMSetup.get())->getSplitPoints();
			for (int i = 0; i < 3; ++i)
			{
				splitPoints[i] = splitPointList[i];
			}
			p->getFragmentProgramParameters()->setNamedConstant("pssmSplitPoints", splitPoints);
		}

		return ret;
	}
	else
		return cpyMat;
}

KVOID DotSceneLoader::setupPSSM()		
{
	if(mSceneMgr->getShadowTechnique() >= (int)Ogre::SHADOWTYPE_TEXTURE_ADDITIVE && 
		mSceneMgr->getShadowTechnique() <= (int)Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED)
	{
		std::vector<Ogre::String> _camnames;
		Ogre::SceneManager::CameraIterator cameras = mSceneMgr->getCameraIterator();
		while (cameras.hasMoreElements())
		{
			Ogre::Camera* camera = cameras.getNext();
			_camnames.push_back(camera->getName());
		}

		Ogre::String cameraName = _camnames[1];
		Ogre::Camera *mCamera = mSceneMgr->getCamera(cameraName);
		mPSSMSetup.setNull();

		// shadow camera setup
		Ogre::PSSMShadowCameraSetup* pssmSetup = OGRE_NEW Ogre::PSSMShadowCameraSetup();
		pssmSetup->setSplitPadding(mCamera->getNearClipDistance());
		pssmSetup->calculateSplitPoints(3, mCamera->getNearClipDistance(), mCamera->getFarClipDistance()/*mSceneMgr->getShadowFarDistance()*/);
		pssmSetup->setOptimalAdjustFactor(0, 2);
		pssmSetup->setOptimalAdjustFactor(1, 1);
		pssmSetup->setOptimalAdjustFactor(2, 0.5);

		mPSSMSetup.bind(pssmSetup);

		mSceneMgr->setShadowCameraSetup(mPSSMSetup);
	}
}

// 配置场景阴影
KVOID DotSceneLoader::configureShadows(rapidxml::xml_node<>* XMLNode)  
{
	mShadowsEnabled = getAttribBool(XMLNode, "castshadows");

	if(mShadowsEnabled)
	{
		// General scene setup
		int technique = getAttribReal(XMLNode, "shadowstechnique");
		mSceneMgr->setShadowTechnique((Ogre::ShadowTechnique)technique);
		mSceneMgr->setShadowFarDistance(getAttribReal(XMLNode, "shadowsrenderingdistance"));

		if(mSceneMgr->getShadowTechnique() >= (int)Ogre::SHADOWTYPE_TEXTURE_ADDITIVE && 
			mSceneMgr->getShadowTechnique() <= (int)Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED)
		{
			// 3 textures per directional light (PSSM)
			mSceneMgr->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 3);

			mSceneMgr->setShadowTextureCount(3);
			mSceneMgr->setShadowTextureConfig(0, 1024, 1024, (Ogre::PixelFormat)33);
			mSceneMgr->setShadowTextureConfig(1, 512, 512, (Ogre::PixelFormat)33);
			mSceneMgr->setShadowTextureConfig(2, 512, 512, (Ogre::PixelFormat)33);
			mSceneMgr->setShadowTextureSelfShadow(true);
			mSceneMgr->setShadowCasterRenderBackFaces(true);
			mSceneMgr->setShadowTextureCasterMaterial("PSSM/shadow_caster");
		}
	}
	else
	{
		mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
	}
}

KVOID DotSceneLoader::processTerrain(rapidxml::xml_node<>* XMLNode)
{
	Ogre::Real worldSize = getAttribReal(XMLNode, "worldSize");
	int mapSize = Ogre::StringConverter::parseInt(XMLNode->first_attribute("mapSize")->value());
	bool colourmapEnabled = getAttribBool(XMLNode, "colourmapEnabled");
	int colourMapTextureSize = Ogre::StringConverter::parseInt(XMLNode->first_attribute("colourMapTextureSize")->value());
	int compositeMapDistance = Ogre::StringConverter::parseInt(XMLNode->first_attribute("tuningCompositeMapDistance")->value());
	int maxPixelError = Ogre::StringConverter::parseInt(XMLNode->first_attribute("tuningMaxPixelError")->value());

// 	Ogre::Vector3 lightdir(0, -0.3, 0.75);
// 	lightdir.normalise();
// 	Ogre::Light* l = mSceneMgr->createLight("tstLight");
// 	l->setType(Ogre::Light::LT_DIRECTIONAL);
// 	l->setDirection(lightdir);
// 	l->setDiffuseColour(Ogre::ColourValue(1.0, 1.0, 1.0));
// 	l->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
// 	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.6, 0.6, 0.6));

	mTerrainGlobalOptions->setMaxPixelError((Ogre::Real)maxPixelError);
	mTerrainGlobalOptions->setCompositeMapDistance((Ogre::Real)compositeMapDistance);
//	mTerrainGlobalOptions->setLightMapDirection(lightdir);
//	mTerrainGlobalOptions->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
//	mTerrainGlobalOptions->setCompositeMapDiffuse(l->getDiffuseColour());

	m_pTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneMgr, Ogre::Terrain::ALIGN_X_Z, mapSize, worldSize);
	m_pTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

	m_pTerrainGroup->setResourceGroup("General");

	rapidxml::xml_node<>* pElement;
	rapidxml::xml_node<>* pPageElement;

	// Process terrain pages (*)
	pElement = XMLNode->first_node("terrainPages");
	if(pElement)
	{
		pPageElement = pElement->first_node("terrainPage");
		while(pPageElement)
		{
			processTerrainPage(pPageElement);
			pPageElement = pPageElement->next_sibling("terrainPage");
		}
	}
	m_pTerrainGroup->loadAllTerrains(true);

	m_pTerrainGroup->freeTemporaryResources();

}

KVOID DotSceneLoader::processTerrainPage(rapidxml::xml_node<>* XMLNode)
{
	Ogre::String name = getAttrib(XMLNode, "name");
	int pageX = Ogre::StringConverter::parseInt(XMLNode->first_attribute("pageX")->value());
	int pageY = Ogre::StringConverter::parseInt(XMLNode->first_attribute("pageY")->value());

	if (Ogre::ResourceGroupManager::getSingleton().resourceExists(m_pTerrainGroup->getResourceGroup(), name))
	{
		m_pTerrainGroup->defineTerrain(pageX, pageY, name);
		m_pTerrainGroup->loadTerrain(pageX, pageY,true);
		
		mTerrainHandle = m_pTerrainGroup->getTerrain(pageX,pageY);	//获得新的地形指针
		//////////////////////////////////////////////////////////////////////////
		//Ogre::Terrain* pTer = m_pTerrainGroup->getTerrain(pageX, pageY);
		//SAFE_CALL(m_pSkyWapper,AddGroundPass(pTer->getMaterial()));
	}
}

KVOID DotSceneLoader::processLight(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
	// Process attributes
	Ogre::String name = getAttrib(XMLNode, "name");
	Ogre::String id = getAttrib(XMLNode, "id");

	// Create the light
	Ogre::Light *pLight = mSceneMgr->createLight(name);
	if(pParent)
		pParent->attachObject(pLight);

	Ogre::String sValue = getAttrib(XMLNode, "type");
	if(sValue == "point")
		pLight->setType(Ogre::Light::LT_POINT);
	else if(sValue == "directional")
		pLight->setType(Ogre::Light::LT_DIRECTIONAL);
	else if(sValue == "spot")
		pLight->setType(Ogre::Light::LT_SPOTLIGHT);
	else if(sValue == "radPoint")
		pLight->setType(Ogre::Light::LT_POINT);

	pLight->setVisible(getAttribBool(XMLNode, "visible", true));
	pLight->setCastShadows(getAttribBool(XMLNode, "castShadows", true));

	rapidxml::xml_node<>* pElement;

	// Process position (?)
	pElement = XMLNode->first_node("position");
	if(pElement)
		pLight->setPosition(parseVector3(pElement));

	// Process normal (?)
	pElement = XMLNode->first_node("normal");
	if(pElement)
		pLight->setDirection(parseVector3(pElement));

	pElement = XMLNode->first_node("directionVector");
	if(pElement)
	{
		pLight->setDirection(parseVector3(pElement));
		mLightDirection = parseVector3(pElement);
	}

	// Process colourDiffuse (?)
	pElement = XMLNode->first_node("colourDiffuse");
	if(pElement)
		pLight->setDiffuseColour(parseColour(pElement));

	// Process colourSpecular (?)
	pElement = XMLNode->first_node("colourSpecular");
	if(pElement)
		pLight->setSpecularColour(parseColour(pElement));

	if(sValue != "directional")
	{
		// Process lightRange (?)
		pElement = XMLNode->first_node("lightRange");
		if(pElement)
			processLightRange(pElement, pLight);

		// Process lightAttenuation (?)
		pElement = XMLNode->first_node("lightAttenuation");
		if(pElement)
			processLightAttenuation(pElement, pLight);
	}
	// Process userDataReference (?)
	pElement = XMLNode->first_node("userDataReference");
	if(pElement)
		;//processUserDataReference(pElement, pLight);
}

KVOID DotSceneLoader::processCamera(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{

}

KVOID DotSceneLoader::processNode(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
	// Construct the node's name
	Ogre::String name = m_sPrependNode + getAttrib(XMLNode, "name");
	
	//////////////////////////////////////////////////////////////////////////
	// 设置角色出生位置及朝向
	if (name == "birthplace")
	{
		rapidxml::xml_node<>* pElement;
		// Process position (?)
		pElement = XMLNode->first_node("position");
		if(pElement)
		{
			KPoint3 kPos = parseVector3(pElement);
			KylinRoot::GetSingletonPtr()->GetCurrentGameStatus()->GetPropertyRef().SetValue("$BirthPosition",kPos);
		}

		// Process rotation (?)
		pElement = XMLNode->first_node("rotation");
		if(pElement)
		{
			KQuaternion kQua = parseQuaternion(pElement);
			KylinRoot::GetSingletonPtr()->GetCurrentGameStatus()->GetPropertyRef().SetValue("$BirthRotation",kQua);
		}

		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// Create the scene node
	Ogre::SceneNode *pNode;
	bool bNpc = false;
	if(name.empty())
	{
		// Let Ogre choose the name
		if(pParent)
			pNode = pParent->createChildSceneNode();
		else
			pNode = mAttachNode->createChildSceneNode();
	}
	else
	{
		// 初始化NPC
		int n = name.find("$NPC_");
		if (n == 0)
		{
			KSTR sGid = name.substr(5,name.length());
			KUINT uGID = atoi(sGid.data());
			// spawn(NPC OBJ)
			// pNode = pNpc->GetSceneNode();
			bNpc = true;
		}
		else
		{
			// Provide the name
			if(pParent)
				pNode = pParent->createChildSceneNode(name);
			else
				pNode = mAttachNode->createChildSceneNode(name);
		}
	}

	// Process other attributes
	Ogre::String id = getAttrib(XMLNode, "id");
	bool isTarget = getAttribBool(XMLNode, "isTarget");

	rapidxml::xml_node<>* pElement;

	// Process position (?)
	pElement = XMLNode->first_node("position");
	if(pElement)
	{
		pNode->setPosition(parseVector3(pElement));
		pNode->setInitialState();
	}

	// Process rotation (?)
	pElement = XMLNode->first_node("rotation");
	if(pElement)
	{
		pNode->setOrientation(parseQuaternion(pElement));
		pNode->setInitialState();
	}

	// Process scale (?)
	pElement = XMLNode->first_node("scale");
	if(pElement)
	{
		pNode->setScale(parseVector3(pElement));
		pNode->setInitialState();
	}

	// Process node (*)
	pElement = XMLNode->first_node("node");
	while(pElement)
	{
		processNode(pElement, pNode);
		pElement = pElement->next_sibling("node");
	}

	if (!bNpc)
	{	// 若是NPC 就不需要再创建entity
		// Process entity (*)
		pElement = XMLNode->first_node("entity");
		while(pElement)
		{
			processEntity(pElement, pNode);
			pElement = pElement->next_sibling("entity");
		}
	}

	// Process light (*)
	//pElement = XMLNode->first_node("light");
	//while(pElement)
	//{
	//	processLight(pElement, pNode);
	//	pElement = pElement->next_sibling("light");
	//}

	// Process camera (*)
	pElement = XMLNode->first_node("camera");
	while(pElement)
	{
		processCamera(pElement, pNode);
		pElement = pElement->next_sibling("camera");
	}

	// Process particleSystem (*)
	pElement = XMLNode->first_node("particleSystem");
	while(pElement)
	{
		processParticleSystem(pElement, pNode);
		pElement = pElement->next_sibling("particleSystem");
	}
	
	// 添加 新粒子特效
	pElement = XMLNode->first_node("particleExSystem");
	while(pElement)
	{
		processNewParticleSystem(pElement, pNode);
		pElement = pElement->next_sibling("particleExSystem");
	}

	// Process billboardSet (*)
	pElement = XMLNode->first_node("billboardSet");
	while(pElement)
	{
		processBillboardSet(pElement, pNode);
		pElement = pElement->next_sibling("billboardSet");
	}

	// Process plane (*)
	pElement = XMLNode->first_node("plane");
	while(pElement)
	{
		processPlane(pElement, pNode);
		pElement = pElement->next_sibling("plane");
	}
}

KVOID DotSceneLoader::processEntity(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
	// Process attributes
	Ogre::String name = getAttrib(XMLNode, "name");
	Ogre::String id = getAttrib(XMLNode, "id");
	Ogre::String meshFile = getAttrib(XMLNode, "meshFile");
	Ogre::String materialFile = getAttrib(XMLNode, "materialFile");
	bool isStatic = getAttribBool(XMLNode, "static", false);;
	bool castShadows = getAttribBool(XMLNode, "castShadows", true);
	
	// 加载碰撞
	if (name.find("collision_box") == 0)
	{	
		processCollision(XMLNode,pParent);
		return;
	}
	
	//---------------------------------------------------------------
	rapidxml::xml_node<>* pElement;

	// Create the entity
	Ogre::Entity *pEntity = 0;
	try
	{
		Ogre::MeshManager::getSingleton().load(meshFile, m_sGroupName);
		pEntity = mSceneMgr->createEntity(name, meshFile);
		pEntity->setQueryFlags(KylinRoot::KR_SCENE_OBJ);
		pEntity->setCastShadows(castShadows);
		pEntity->setRenderingDistance(VISIBLE_DISTANCE);
		pParent->attachObject(pEntity);
		
		// 读取实体数据
		Ogre::String propname;
		rapidxml::xml_node<>* pMaterials = XMLNode->first_node("materials");
		int count = getAttribReal(pMaterials, "count");

		int ix = 0;
		rapidxml::xml_node<>* pMaterial = pMaterials->first_node("material");
		while(pMaterial)
		{
			Ogre::String material = getAttrib(pMaterial,"materialsName");

// 			if(mShadowsEnabled)
// 			{
// 				Ogre::MaterialPtr matEnt = Ogre::MaterialManager::getSingletonPtr()->getByName(material);
// 				pEntity->getSubEntity(ix)->setMaterial(buildDepthShadowMaterial(matEnt));
// 			}
// 			else
				pEntity->getSubEntity(ix)->setMaterialName(material);

			pMaterial = pMaterial->next_sibling("material");
			++ix; 
		}
	}
	catch(Ogre::Exception &/*e*/)
	{
		Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error loading an entity!");
	}
	
}

KVOID DotSceneLoader::processParticleSystem(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
	// Process attributes
	Ogre::String name = getAttrib(XMLNode, "name");
	Ogre::String id = getAttrib(XMLNode, "id");
	Ogre::String file = getAttrib(XMLNode, "script");

	// Create the particle system
	try
	{
		Ogre::ParticleSystem *pParticles = mSceneMgr->createParticleSystem(name, file);
		pParticles->setKeepParticlesInLocalSpace(true);
		pParent->attachObject(pParticles);
	}
	catch(Ogre::Exception &/*e*/)
	{
		Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error creating a particle system!");
	}
}

KVOID DotSceneLoader::processBillboardSet(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
	//! @todo Implement this
}

KVOID DotSceneLoader::processPlane(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
	//! @todo Implement this
}

KVOID DotSceneLoader::processFog(rapidxml::xml_node<>* XMLNode)
{
	// Process attributes
	Ogre::Real expDensity = getAttribReal(XMLNode, "density", 0.001);
	Ogre::Real linearStart = getAttribReal(XMLNode, "start", 0.0);
	Ogre::Real linearEnd = getAttribReal(XMLNode, "end", 1.0);

	Ogre::FogMode mode = (Ogre::FogMode)(KINT)(getAttribReal(XMLNode, "mode"));

	rapidxml::xml_node<>* pElement;

	// Process colourDiffuse (?)
	Ogre::ColourValue colourDiffuse = Ogre::ColourValue::White;
	pElement = XMLNode->first_node("colour");
	if(pElement)
		colourDiffuse = parseColour(pElement);

	// Setup the fog
	mSceneMgr->setFog(mode, colourDiffuse, expDensity, linearStart, linearEnd);
}

KVOID DotSceneLoader::processNewParticleSystem(rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent)
{
	Ogre::String name = getAttrib(XMLNode, "name");
	Ogre::String id = getAttrib(XMLNode, "id");
	Ogre::String file = getAttrib(XMLNode, "file");
	Ogre::String pexname = getAttrib(XMLNode, "script");

	// Create the particle system
	try
	{
		ParticleUniverse::ParticleSystemManager* pManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr(); 
		ParticleUniverse::ParticleSystem*	mSystem =  pManager->createParticleSystem(name,pexname,mSceneMgr); 
		pParent->attachObject(mSystem);
		mSystem->start();
		mSystem->setScale(pParent->getScale());
	}
	catch(Ogre::Exception &/*e*/)
	{
		Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error creating a new particle system!");
	}
}

KVOID DotSceneLoader::processClipping(rapidxml::xml_node<>* XMLNode)
{
	//! @todo Implement this

	// Process attributes
	Ogre::Real fNear = getAttribReal(XMLNode, "near", 0);
	Ogre::Real fFar = getAttribReal(XMLNode, "far", 1);
}

KVOID DotSceneLoader::processLightRange(rapidxml::xml_node<>* XMLNode, Ogre::Light *pLight)
{
	// Process attributes
	Ogre::Real inner = getAttribReal(XMLNode, "inner");
	Ogre::Real outer = getAttribReal(XMLNode, "outer");
	Ogre::Real falloff = getAttribReal(XMLNode, "falloff", 1.0);

	// Setup the light range
	pLight->setSpotlightRange(Ogre::Angle(inner), Ogre::Angle(outer), falloff);
}

KVOID DotSceneLoader::processLightAttenuation(rapidxml::xml_node<>* XMLNode, Ogre::Light *pLight)
{
	// Process attributes
	Ogre::Real range = getAttribReal(XMLNode, "range");
	Ogre::Real constant = getAttribReal(XMLNode, "constant");
	Ogre::Real linear = getAttribReal(XMLNode, "linear");
	Ogre::Real quadratic = getAttribReal(XMLNode, "quadratic");

	// Setup the light attenuation
	pLight->setAttenuation(range, constant, linear, quadratic);
}


Ogre::String DotSceneLoader::getAttrib(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, const Ogre::String &defaultValue)
{
	if(XMLNode->first_attribute(attrib.c_str()))
		return XMLNode->first_attribute(attrib.c_str())->value();
	else
		return defaultValue;
}

Ogre::Real DotSceneLoader::getAttribReal(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, Ogre::Real defaultValue)
{
	if(XMLNode->first_attribute(attrib.c_str()))
		return Ogre::StringConverter::parseReal(XMLNode->first_attribute(attrib.c_str())->value());
	else
		return defaultValue;
}

bool DotSceneLoader::getAttribBool(rapidxml::xml_node<>* XMLNode, const Ogre::String &attrib, bool defaultValue)
{
	if(!XMLNode->first_attribute(attrib.c_str()))
		return defaultValue;

	if(Ogre::String(XMLNode->first_attribute(attrib.c_str())->value()) == "true")
		return true;

	return false;
}

Ogre::Vector3 DotSceneLoader::parseVector3(rapidxml::xml_node<>* XMLNode)
{
	return Ogre::Vector3(
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value()),
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value()),
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value())
	);
}

Ogre::Quaternion DotSceneLoader::parseQuaternion(rapidxml::xml_node<>* XMLNode)
{
	//! @todo Fix this crap!

	Ogre::Quaternion orientation;

	if(XMLNode->first_attribute("qx"))
	{
		orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qx")->value());
		orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qy")->value());
		orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qz")->value());
		orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qw")->value());
	}
	if(XMLNode->first_attribute("qw"))
	{
		orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qw")->value());
		orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qx")->value());
		orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qy")->value());
		orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("qz")->value());
	}
	else if(XMLNode->first_attribute("axisX"))
	{
		Ogre::Vector3 axis;
		axis.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("axisX")->value());
		axis.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("axisY")->value());
		axis.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("axisZ")->value());
		Ogre::Real angle = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angle")->value());;
		orientation.FromAngleAxis(Ogre::Angle(angle), axis);
	}
	else if(XMLNode->first_attribute("angleX"))
	{
		Ogre::Vector3 axis;
		axis.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angleX")->value());
		axis.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angleY")->value());
		axis.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("angleZ")->value());
		//orientation.FromAxes(&axis);
		//orientation.F
	}
	else if(XMLNode->first_attribute("x"))
	{
		orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value());
		orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value());
		orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value());
		orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("w")->value());
	}
	else if(XMLNode->first_attribute("w"))
	{
		orientation.w = Ogre::StringConverter::parseReal(XMLNode->first_attribute("w")->value());
		orientation.x = Ogre::StringConverter::parseReal(XMLNode->first_attribute("x")->value());
		orientation.y = Ogre::StringConverter::parseReal(XMLNode->first_attribute("y")->value());
		orientation.z = Ogre::StringConverter::parseReal(XMLNode->first_attribute("z")->value());
	}

	return orientation;
}

Ogre::ColourValue DotSceneLoader::parseColour(rapidxml::xml_node<>* XMLNode)
{
	return Ogre::ColourValue(
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("r")->value()),
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("g")->value()),
		Ogre::StringConverter::parseReal(XMLNode->first_attribute("b")->value()),
		XMLNode->first_attribute("a") != NULL ? Ogre::StringConverter::parseReal(XMLNode->first_attribute("a")->value()) : 1
	);
}

KBOOL DotSceneLoader::LoadScene( KSTR mSceneFile )
{
	parseDotScene(mSceneFile,"General",OgreRoot::GetSingletonPtr()->GetSceneManager());

	return true;
}

KVOID DotSceneLoader::Unload( Kylin::SceneHag* pHag )
{	
	if(mHydraxHandle)
	{
		if(mHydraxHandle->isCreated()) mHydraxHandle->remove();
		SAFE_DEL(mHydraxHandle);
	}

	Kylin::ClSceneLoader::Unload(pHag);


	OGRE_DELETE mTerrainGlobalOptions;
	mTerrainGlobalOptions = NULL;
}

KVOID DotSceneLoader::Tick( KFLOAT fElapsed )
{
	Kylin::ClSceneLoader::Tick(fElapsed);

	UpdataHydrax(fElapsed);
}

//-------------------------------------------------------------------
// 加载水
KVOID DotSceneLoader::processHydrax( rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent /*= 0*/ )
{
	std::vector<Ogre::String> _camnames;
	// Loop through all cameras and grab their name and set their debug representation
	Ogre::SceneManager::CameraIterator cameras = mSceneMgr->getCameraIterator();
	while (cameras.hasMoreElements())
	{
		Ogre::Camera* camera = cameras.getNext();
		_camnames.push_back(camera->getName());
	}

	Ogre::Camera* _temp = mSceneMgr->getCamera(_camnames[0]);
	createWaterPlane(mSceneMgr,_temp,_temp->getViewport());

	rapidxml::xml_node<>* pElement;
	pElement = XMLNode->first_node("main");
	if(pElement)
	{
		Ogre::Vector3 p;
		p.x	= getAttribReal(pElement, "x");
		p.y	= getAttribReal(pElement, "y");
		p.z	= getAttribReal(pElement, "z");
		mHydraxHandle->setPosition(p);
		Ogre::Vector3 c;
		c.x	= getAttribReal(pElement, "waterR");
		c.y	= getAttribReal(pElement, "waterG");
		c.z	= getAttribReal(pElement, "waterB");

		mHydraxHandle->setWaterColor(c);
	}
}

KVOID DotSceneLoader::createWaterPlane( Ogre::SceneManager *scenemgr, Ogre::Camera *camera, Ogre::Viewport *viewport )
{
	mHydraxHandle = KNEW Hydrax::Hydrax(scenemgr, camera, viewport);

	// Create our projected grid module
	mHydraxModule = KNEW Hydrax::Module::ProjectedGrid(// Hydrax parent pointer
		mHydraxHandle,
		// Noise module
		KNEW Hydrax::Noise::Perlin(/*Generic one*/),
		// Base plane
		Ogre::Plane(Ogre::Vector3(0,1,0), Ogre::Vector3(0,0,0)),
		// Normal mode
		Hydrax::MaterialManager::NM_RTT,// NM_VERTEX, NM_RTT  //顶点模式
		// Projected grid options
		Hydrax::Module::ProjectedGrid::Options(60));  //添加网格的数目 一排的数目数值越大 网格越多

	// Set our module
	mHydraxHandle->setModule(static_cast<Hydrax::Module::Module*>(mHydraxModule));
	mHydraxHandle->loadCfg("Hydrax.hdx");
	// Create water
	mHydraxHandle->create();
	mHydraxHandle->getMesh()->getEntity()->setQueryFlags(0);

	Ogre::String matNames = mTerrainHandle->getMaterialName();
	addDepthTechnique(matNames);
}

KVOID DotSceneLoader::addDepthTechnique( const Ogre::String& matname )
{
	if(!mHydraxHandle)
		return;

	if (mTerrainHandle)
	{
		Ogre::MaterialPtr pMat = mTerrainHandle->getMaterial();/*static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(matname));*/
		for(unsigned int i = 0;i < pMat->getNumTechniques();i++)
		{
			if(pMat->getTechnique(i)->getSchemeName() == "HydraxDepth") return;
		}

		mHydraxHandle->getMaterialManager()->addDepthTechnique(pMat->createTechnique());
	}
}

KVOID DotSceneLoader::UpdataHydrax( KFLOAT fElapsed )
{
	if (mHydraxHandle)
	{
		Ogre::Vector3 col = mHydraxHandle->getWaterColor();
		Ogre::Vector3 original = col;

		float height = mHydraxHandle->getSunPosition().y / 10.0f;

		if(height < -99.0f)
		{
			col = original * 0.1f;
			height = 9999.0f;
		}
		else if(height < 1.0f)
		{
			col = original * (0.1f + (0.009f * (height + 99.0f)));
			height = 100.0f / (height + 99.001f);
		}
		else if(height < 2.0f)
		{
			col += original;
			col	/= 2.0f;
			float percent = (height - 1.0f);
			col = (col * percent) + (original * (1.0f - percent));
		}
		else
		{
			col += original;
			col	/= 2.0f;
		}
		mHydraxHandle->setWaterColor(col);
		mHydraxHandle->setSunArea(height);

		mHydraxHandle->update(fElapsed);
	}
}

void DotSceneLoader::processCollision( rapidxml::xml_node<>* XMLNode, Ogre::SceneNode *pParent )
{
	

	// Process attributes
	Ogre::String name = getAttrib(XMLNode, "name");
	Ogre::String id = getAttrib(XMLNode, "id");
	Ogre::String meshFile = getAttrib(XMLNode, "meshFile");

	//---------------------------------------------------------------
	rapidxml::xml_node<>* pElement;

	// Create the entity
	Ogre::Entity *pEntity = 0;
	try
	{
		Ogre::MeshManager::getSingleton().setBoundsPaddingFactor(0.0);
		Ogre::MeshManager::getSingleton().load(meshFile, m_sGroupName);
		pEntity = mSceneMgr->createEntity(name, meshFile);
		pEntity->setQueryFlags(KylinRoot::KR_NO_MASK);
		pEntity->setCastShadows(false);
		pEntity->setRenderingDistance(VISIBLE_DISTANCE);
		pParent->attachObject(pEntity);

		pEntity->getParentSceneNode()->_updateBounds();
		Ogre::AxisAlignedBox box = pEntity->getWorldBoundingBox();
		KPoint3 kCenter = box.getCenter(); 
		box = pEntity->getBoundingBox();
		KPoint3 kSize = box.getSize() * pEntity->getParentSceneNode()->getScale();
		Ogre::Matrix3 kMat;
		pEntity->getParentSceneNode()->getOrientation().ToRotationMatrix(kMat);
		
		// collisionmanager add collisionbox
		PhyX::PhysicalSystem::GetSingletonPtr()->GetCollisionMonitor()->AddSceneCllsnBox(OrientedBox(kCenter,kSize,kMat));
		//////////////////////////////////////////////////////////////////////////

		pParent->removeAndDestroyAllChildren();
		mSceneMgr->destroySceneNode(pParent);
	}
	catch(Ogre::Exception &/*e*/)
	{
		Ogre::LogManager::getSingleton().logMessage("[DotSceneLoader] Error loading an entity!");
	}
}
//-------------------------------------------------------------------