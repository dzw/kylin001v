#include "engpch.h"
#include "EffectManager.h"
#include "rOgreRoot.h"
#include "CameraControl.h"


KBOOL Kylin::EffectManager::Initialize(Ogre::Root* pRoot, Ogre::SceneManager* pSceneMngr)
{
	// particle system needs to be initialised here
	//Ogre::ParticleSystem* particle_system = pSceneMngr->createParticleSystem("Spark", "Spark");
	//pSceneMngr->getRootSceneNode()->attachObject(particle_system);

	// do not implement if not supported
	const Ogre::RenderSystemCapabilities* caps = pRoot->getRenderSystem()->getCapabilities();
	if ( caps->hasCapability(Ogre::RSC_VERTEX_PROGRAM) 
		&& (caps->hasCapability(Ogre::RSC_FRAGMENT_PROGRAM))
		)
	{
		// just experimenting with compositors here
		CreateCompositors();
	}

	return true;
}

KVOID Kylin::EffectManager::CreateCompositors()
{
	Ogre::Viewport* pVP = OgreRoot::GetSingletonPtr()->GetMainWindow()->getViewport(0);

	// bloom
	Ogre::CompositorInstance *compositor_instance = 
		Ogre::CompositorManager::getSingletonPtr()->addCompositor(pVP, "Bloom");
	compositor_instance->setEnabled(false);

	/// Motion blur effect
	Ogre::CompositorPtr comp3 = Ogre::CompositorManager::getSingleton().create(
		"MotionBlur", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
		);
	{
		Ogre::CompositionTechnique *t = comp3->createTechnique();
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("scene");
			def->width = 0;
			def->height = 0;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("sum");
			def->width = 0;
			def->height = 0;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		{
			Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("temp");
			def->width = 0;
			def->height = 0;
			def->formatList.push_back(Ogre::PF_R8G8B8);
		}
		/// Render scene
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
			tp->setOutputName("scene");
		}
		/// Initialisation pass for sum texture
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
			tp->setOutputName("sum");
			tp->setOnlyInitial(true);
		}
		/// Do the motion blur
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			tp->setOutputName("temp");
			{ Ogre::CompositionPass *pass = tp->createPass();
			pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
			pass->setMaterialName("Ogre/Compositor/Combine");
			pass->setInput(0, "scene");
			pass->setInput(1, "sum");
			}
		}
		/// Copy back sum texture
		{
			Ogre::CompositionTargetPass *tp = t->createTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			tp->setOutputName("sum");
			{ Ogre::CompositionPass *pass = tp->createPass();
			pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
			pass->setMaterialName("Ogre/Compositor/Copyback");
			pass->setInput(0, "temp");
			}
		}
		/// Display result
		{
			Ogre::CompositionTargetPass *tp = t->getOutputTargetPass();
			tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			{ Ogre::CompositionPass *pass = tp->createPass();
			pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
			pass->setMaterialName("Ogre/Compositor/MotionBlur");
			pass->setInput(0, "sum");
			}
		}
	}

	compositor_instance = 
		Ogre::CompositorManager::getSingletonPtr()->addCompositor(pVP, "MotionBlur");
	compositor_instance->setEnabled(false);
}

// KVOID Kylin::EffectManager::CreateLightning()
// {
// 	//instance = new Lightning(scene_manager);
// }

KVOID Kylin::EffectManager::SetEffectEnable( KSTR sName, KBOOL bFlag )
{
	Ogre::Viewport* pVP = OgreRoot::GetSingletonPtr()->GetCameraController()->GetActiveCamera()->getViewport();

	Ogre::CompositorManager::getSingletonPtr()->setCompositorEnabled(pVP,sName,bFlag);

	//Lightning *lightning = dynamic_cast<Lightning*>(mGameObjectManager->AddObjectToCollection("lightning", mSceneManager));
	//lightning->AddToScene(mPaddle2->GetSceneNode(), mBall->GetSceneNode());
}