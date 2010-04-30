/*
-----------------------------------------------------------------------------
Filename:    HeatShimmerManager.h
-----------------------------------------------------------------------------

This is the source file to create and manage multiple instances of the heat
shimmer particle shader.

Uses OGRE
(Object-oriented Graphics Rendering Engine)

Copyright (c) 2000-2009 The OGRE Team
For the latest info, see http://www.ogre3d.org/

While OGRE is under the LGPL, this file is in the public domain, and may be
used for whatever you like.

If your SceneManager or loader can have user data attached to nodes,
you can modify the addHeat and setHeatParticle functions to load user data
to modify the particle system. I did not do this because from what I can tell,
there are different user data systems that store it differently.

By Shadowking97 @ Ogre 3d forums
-----------------------------------------------------------------------------
*/

#ifndef __HeatShimmerManager_H__
#define __HeatShimmerManager_H__

#include "Ogre.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

using namespace Ogre;

//Renderable Listener combined with RenderTargetListener to black out non-heat related
//renderables
class HeatRenderableListener: public RenderTargetListener, RenderQueue::RenderableListener
{
private:
   SceneManager* mSceneMgr;
   Camera* cam;
   //Number of heat particle emmiters.
   int numParticles;
   RenderQueue* queue;
public:
   HeatRenderableListener(SceneManager* scene, Camera* c, int p): cam(c)
   {
      numParticles = p;
      mSceneMgr = scene;
      queue = mSceneMgr->getRenderQueue();
   }
   void preRenderTargetUpdate(const RenderTargetEvent& evt)
   {
      // Todo: Set non-heat related soft particles to black while keeping alpha
      
      //set heat particle camera to the same loc/rot of the default camera
      mSceneMgr->getCamera("HeatCam")->setPosition(cam->getPosition());
      mSceneMgr->getCamera("HeatCam")->setOrientation(cam->getOrientation());

      //Set heat particles visible and set renderablelistener
      for(int i = 0;i<numParticles;i++)
         mSceneMgr->getParticleSystem("HeatParticleSystem_"+StringConverter::toString(i))->setVisible(true);
      queue->setRenderableListener(this);
   }
   void postRenderTargetUpdate(const RenderTargetEvent& evt)
   {
      // Reset the heat particles and RenderableListener
      for(int i = 0;i<numParticles;i++)
         mSceneMgr->getParticleSystem("HeatParticleSystem_"+StringConverter::toString(i))->setVisible(false);
      queue->setRenderableListener(0);
   }
   bool renderableQueued(Renderable* rend, uint8 groupID, 
            ushort priority, Technique** ppTech, RenderQueue* pQueue)
   {
      //If renderable's technique isn't the Technique from Particle/Heat,
      //Renderable is not a heat particle so set the technique to Darkness
      Technique* tech = *ppTech;   
      if(tech->getParent()->getName()!="Particle/Heat")
      {
         MaterialPtr mat = MaterialManager::getSingleton().getByName("Heat/Darkness");
         tech = mat->getTechnique(0);
         *ppTech = tech;
      }
      return true;
   }
};

//Main manager for heat particles.
class HeatShimmerManager
{
private:
   //Main render camera
   Camera* mCamera;
   //Camera created for heat rendering
   Camera* mHeatCam;
   SceneManager* mSceneMgr;
   RenderTexture* heatRender;
   RenderWindow* mWindow;
   //Number of particle systems
   int particleCount;
public:
   HeatShimmerManager(Camera* MainCam, SceneManager* MainScene, RenderWindow* win): mCamera(MainCam), mSceneMgr(MainScene), mWindow(win)
   {
      particleCount=0;
   }
   /*  HeatShimmerManager::Initialize(bool)
      
      Used to initialize Heat Shimmer Manager.
      Sets up the heat camera, viewport, render texture
      Adds Renderable/RenderTarget Lisstener to the texture
      Adds Heat Compositor

      Call this function after the scene has been set up inbetween the compositors
      you want the heat wave to go.

      @setUpParticles
      If true, calls functions to search for nodes and bones
      named for particle system attachment.      
   */
   void initialize(bool setUpParticles)
   {
      mHeatCam = mSceneMgr->createCamera("HeatCam");
      mHeatCam->setNearClipDistance(mCamera->getNearClipDistance());
      mHeatCam->setFarClipDistance(mCamera->getFarClipDistance());
      mHeatCam->setAspectRatio(mCamera->getAspectRatio());
      mHeatCam->setFOVy (mCamera->getFOVy());
      mHeatCam->setOrientation(mCamera->getOrientation());
      mHeatCam->setPosition(mCamera->getPosition());

      //TexturePtr heatTex = TextureManager::getSingleton().createManual("heat",
      //   ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, mWindow->getWidth(),
      //   mWindow->getHeight(), 0, PF_R8G8B8, TU_RENDERTARGET);
	TexturePtr heatTex = TextureManager::getSingleton().createManual("heat",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, mWindow->getWidth()/2,
		mWindow->getHeight()/2, 0, PF_R8G8B8, TU_RENDERTARGET);

      heatRender = heatTex->getBuffer()->getRenderTarget();
      heatRender->addViewport(mHeatCam);
      heatRender->getViewport(0)->setClearEveryFrame(true);
      heatRender->getViewport(0)->setBackgroundColour(ColourValue::Black);
      heatRender->getViewport(0)->setOverlaysEnabled(false);
      heatRender->setAutoUpdated(true);
    
      if(setUpParticles)
      {
         setHeatParticles(mSceneMgr->getRootSceneNode());
         setBonedHeatParticles();
      }
     
      HeatRenderableListener * heatListener = new HeatRenderableListener(mSceneMgr,mCamera,particleCount);
      heatRender->addListener(heatListener);

      CompositorManager::getSingleton().addCompositor(mCamera->getViewport(), "heat_waves");
      CompositorManager::getSingleton().setCompositorEnabled(mCamera->getViewport(),"heat_waves",true);
   }

   /*  HeatShimmerManager::reset()
      
      Clears the Heat Shimmer and Scene Managers of all particle systems.
   */
   void reset()
   {
      for(int i = 0; i<particleCount;i++)
      {
         mSceneMgr->destroyParticleSystem("HeatParticleSystem_"+StringConverter::toString(i));
      }
      particleCount=0;
   }

   //Manual heat particle creation on SceneNode
   void addHeat(SceneNode* n)
   {
      ParticleSystem* heat_Particles = mSceneMgr->createParticleSystem("HeatParticleSystem_"+StringConverter::toString(particleCount), "Render/Heat");
      particleCount++;
      n->attachObject(heat_Particles);
      heat_Particles->setVisible(false);
   }
   
   //Manual heat particle creation on SceneNode
   void addHeat(String sceneNodeName)
   {
      ParticleSystem* heat_Particles = mSceneMgr->createParticleSystem("HeatParticleSystem_"+StringConverter::toString(particleCount), "Render/Heat");
      particleCount++;
      mSceneMgr->getSceneNode(sceneNodeName)->attachObject(heat_Particles);
      heat_Particles->setVisible(false);
   }
   
   //Manual heat particle creation on Entity bone
   void addHeatToBone(Bone* b, Entity* e)
   {
      ParticleSystem* heat_Particles = mSceneMgr->createParticleSystem("HeatParticleSystem_"+StringConverter::toString(particleCount), "Render/Heat");
      particleCount++;
      e->attachObjectToBone(b->getName(),heat_Particles);
      heat_Particles->setVisible(false);
   }
   
   //Manual heat particle creation on Entity bone
   void addHeatToBone(String boneName, Entity* e)
   {
      ParticleSystem* heat_Particles = mSceneMgr->createParticleSystem("HeatParticleSystem_"+StringConverter::toString(particleCount), "Render/Heat");
      particleCount++;
      e->attachObjectToBone(boneName,heat_Particles);
      heat_Particles->setVisible(false);
   }
protected:

   /*
      setHeatParticles(SceneNode*)
      Goes through the scene recursivly via root scene node
      attaching ParticleSystems for heat to the apropriate
      nodes.
   */
   void setHeatParticles(SceneNode* node)
   {
      if(node->getName().find("HeatSystem_")!=-1)
      {
         ParticleSystem* heat_Particles = mSceneMgr->createParticleSystem("HeatParticleSystem_"+StringConverter::toString(particleCount), "Render/Heat");
         particleCount++;
         node->attachObject(heat_Particles);
         heat_Particles->setVisible(false);
      }
      Node::ChildNodeIterator iterator = node->getChildIterator();
      while(iterator.hasMoreElements())
      {
         SceneNode* n = dynamic_cast<SceneNode*>(iterator.getNext());
         if(n)
            setHeatParticles(n);
      }
   }

   /*
      setBonedHeatParticles()
      Goes through Entities in a scene and attaches heat particles
      to the appropriate bones.
   */
   void setBonedHeatParticles()
   {
      SceneManager::MovableObjectIterator iterator = mSceneMgr->getMovableObjectIterator("Entity");
      while(iterator.hasMoreElements())
      {
         Entity* e = static_cast<Entity*>(iterator.getNext());
         if(e->hasSkeleton()&&e->isInScene())
         {
            SkeletonInstance* s = e->getSkeleton();
            Skeleton::BoneIterator bones = s->getBoneIterator();
            while(bones.hasMoreElements())
            {
               Bone* bone = static_cast<Bone*>(bones.getNext());
               if(bone->getName().find("HeatSystem_")!=-1)
               {
                  ParticleSystem* heat_Particles = mSceneMgr->createParticleSystem("HeatParticleSystem_"+StringConverter::toString(particleCount), "Render/Heat");
                  particleCount++;
                  e->attachObjectToBone(bone->getName(),heat_Particles);
                  heat_Particles->setVisible(false);
               }
            }
         }
      }
   }
};


#endif
