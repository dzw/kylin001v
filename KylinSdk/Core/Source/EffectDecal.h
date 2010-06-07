#pragma once

#include "KylinRoot.h"

/************************************************************************/
// Ìù»¨                                                                   
/************************************************************************/
namespace Kylin
{
	class EffectDecal
	{
	public:
		EffectDecal( Ogre::SceneManager *sceneMgr, KCSTR &matName, const KFLOAT &radius, KFLOAT heightOffset = 0.2, KINT resolution = 4 )
			: mSceneMgr(sceneMgr)
			, mRadius(radius)
			, mHeightOffset(heightOffset)
			, mSize(resolution)
		{
			mDecal = mSceneMgr->createManualObject(matName+"_game_decal");

			mDecal->setCastShadows(false);
			mDecal->setQueryFlags(0);
			mDecal->setDynamic(true);

			mDecal->begin(matName , RenderOperation::OT_TRIANGLE_LIST); 

			for (int i=0; i<mSize; i++)
			{
				for (int j=0; j<=mSize; j++)
				{
					mDecal->position(Ogre::Vector3(i, 0, j));
					mDecal->textureCoord((float)i / (float)mSize, (float)j / (float)mSize);
				}
			}

			for (int i=0; i<mSize; i++)
			{
				for (int j=0; j<mSize; j++)
				{
					mDecal->quad( i * (mSize+1) + j, i * (mSize+1) + j + 1,
						(i + 1) * (mSize+1) + j + 1,(i + 1) * (mSize+1) + j);
				}
			}

			mDecal->end();

			mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mDecal);
			mDecalNode = mDecal->getParentSceneNode();

			SetVisible(false);
		}

		~EffectDecal(void)
		{
			mDecalNode->getParent()->removeChild(mDecalNode);
			mSceneMgr->destroySceneNode(mDecalNode);
			mSceneMgr->destroyManualObject(mDecal);
		}

		void MoveTo(const KPoint3& pos)
		{
			Ogre::Real x1 = pos.x - mRadius;
			Ogre::Real z1 = pos.z - mRadius;

			Ogre::Real step = mRadius * 2/ mSize;

			// ¸üÐÂ
			mDecal->beginUpdate(0);
			for (KINT i = 0; i <= mSize; i++)
			{
				for (KINT j = 0; j <= mSize; j++)
				{
					KFLOAT y = getHeight(x1,z1) + mHeightOffset;

					mDecal->position(x1, y, z1);
					mDecal->textureCoord(i / (KFLOAT)mSize, j / (KFLOAT)mSize);

					z1 += step;
				}
				
				x1 += step;
				z1 = pos.z - mRadius;
			}

			for (KINT i = 0; i < mSize; i++)
			{
				for (KINT j = 0; j < mSize; j++)
				{
					mDecal->quad(i * (mSize + 1) + j,
						i * (mSize + 1) + j + 1,
						(i + 1) * (mSize + 1) + j + 1,
						(i + 1) * (mSize + 1) + j);
				}
			}
			mDecal->end();
		}

		KVOID SetVisible(KBOOL bFlag)
		{
			mDecal->setVisible(bFlag);
		}

		KBOOL IsVisible()
		{
			return mDecal->getVisible();
		}

	private:
		KFLOAT	getHeight(KFLOAT x, KFLOAT z)
		{
			KPoint3 vPos;
			if ( KylinRoot::GetSingletonPtr()->HitTest(KPoint3(x, 5000, z),KPoint3::NEGATIVE_UNIT_Y,vPos) )
				return vPos.y;

			return 0;
		}

	private:
		Ogre::SceneManager	*mSceneMgr;
		Ogre::ManualObject	*mDecal;
		Ogre::SceneNode		*mDecalNode;
		KFLOAT				mRadius;
		KFLOAT				mHeightOffset;		
		KINT				mSize;
	};
}
