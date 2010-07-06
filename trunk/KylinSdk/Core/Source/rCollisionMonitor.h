#pragma once

#include "rOrientedBox.h"

namespace Kylin
{
	namespace PhyX
	{
		class CollisionMonitor
		{
		public:
			CollisionMonitor();
			
			enum CllsnMode
			{
				CLLSN_BREAK,
				CLLSN_CONTINUE,
			};
			
			enum CllsnObjType
			{
				COT_NONE	= 0,
				COT_STATIC	= 1,
				COT_DYNAMIC = 2,
			};

			//////////////////////////////////////////////////////////////////////////
			class CollisionPair
			{
			public:
				CollisionPair()
					: m_pObj1(NULL)
					, m_pObj2(NULL)
				{
				}

				Node* m_pObj1;
				Node* m_pObj2;
			};

			//////////////////////////////////////////////////////////////////////////
			typedef KVOID(*Func)(const CollisionPair& kPair);

			class CollisionData
			{
			public:
				CollisionData(Node* pHost,KBOOL bCollider = false)
					: m_pHost(pHost)
					, m_bEnable(false)
					, m_bCollider(bCollider)
					, m_pCallback(NULL)
					, m_eMode(CLLSN_BREAK)
					, m_uSelf(COT_NONE)
					, m_uMate(COT_NONE)
				{
				}
				
				KVOID SetCallbackFunc(Func pFunc);
				KVOID SetCollsionMode(CllsnMode eMode);
				
				KVOID SetEnable(KBOOL bEnable);
				//KBOOL Enabled();

			protected:
				CllsnMode		m_eMode;
				KBOOL			m_bCollider;
				KBOOL			m_bEnable;
				
				Func			m_pCallback;
				Node*			m_pHost;
				
				KUINT			m_uSelf;
				KUINT			m_uMate;

				friend class CollisionMonitor;
			};

			//////////////////////////////////////////////////////////////////////////
			class CollisionGroup
			{
			public:
				
				virtual KVOID AddCollider(CollisionData* pData);
				virtual KVOID AddCollidee(CollisionData* pData);

				virtual KVOID Update();

			protected:
				KVEC<CollisionData*> m_kCollider;
				KVEC<CollisionData*> m_kCollidee;
		
			protected:
				void AddPair(const CollisionData& kCllsn, const CollisionData& kCllsn_1);	//order insensitive
				bool IsMarked(const CollisionData& kCllsn, const CollisionData& kCllsn_1)const;	//order insensitive
				
				typedef std::set<const CollisionData*> ResponserSet;
				typedef KMAP<const CollisionData*, ResponserSet> ColliderBag;

				ColliderBag m_kCllsnBag;
			};
			
			class CollisionBox
			{
			public:
				CollisionBox()
					: mWireBoundingBox(NULL)
				{
				}

				OrientedBox				mOBB;
				/// Pointer to a Wire Bounding Box for this Node
				Ogre::WireBoundingBox*	mWireBoundingBox;
			};
			//////////////////////////////////////////////////////////////////////////

			virtual KVOID Tick(KFLOAT fElapsed);
			//-----------------------------------------------------------------
			virtual KVOID Destroy();
			//-----------------------------------------------------------------
			virtual CollisionData* Commit(Node* pHost,KBOOL bCollider = false,KUINT uSelf = COT_DYNAMIC,KUINT uMate = COT_STATIC | COT_DYNAMIC);
			//-----------------------------------------------------------------
			// ��ѯ������ײ�� 
			// pos Ҫ��ѯ��λ�ã�dir ���� ��ѯ�뾶
			virtual KBOOL QueryScene(KPoint3 kPos, KPoint3 kDir, KFLOAT fRadius);

			// �жϸ�λ���Ƿ��ڳ�����ײ��
			virtual KBOOL QuerySceneCllsnBox(KPoint3 kPos);
			
			// ��ӳ�����ײ��
			virtual KVOID AddSceneCllsnBox(const OrientedBox& kBox);
			//-----------------------------------------------------------------
			
		protected:
			virtual KVOID TestEntities();

		protected:
			typedef KMAP<Node*, CollisionData*> CllsnObjMap;
			typedef KVEC<CollisionBox>			CllsnSceneVec;

			CllsnObjMap		m_kObjsMap;
			CllsnSceneVec	m_kSceneVec;
		};
	}
}