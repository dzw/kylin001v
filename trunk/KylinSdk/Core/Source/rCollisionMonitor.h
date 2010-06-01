#pragma once


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
			
			//////////////////////////////////////////////////////////////////////////
			class CollisionPair
			{
			public:
				CollisionPair()
					: m_pObj1(NULL)
					, m_pObj2(NULL)
					, m_kNormal(KPoint3::ZERO)
				{
				}

				Node* m_pObj1;
				Node* m_pObj2;

				KPoint3 m_kNormal;
			};

			//////////////////////////////////////////////////////////////////////////
			typedef KVOID(*Func)(const CollisionPair kPair);

			class CollisionData
			{
			public:
				CollisionData()
					: m_pHost(NULL)
					, m_bEnable(false)
					, m_bCollider(false)
					, m_pCallback(NULL)
				{
				}
				
				KVOID SetCallbackFunc(Func pFunc);
				KVOID SetCollsionMode(CllsnMode eMode);
				
				KVOID SetEnable(KBOOL bEnable);
				//KBOOL Enabled();

			protected:
				CllsnMode	m_eMode;
				KBOOL		m_bCollider;
				KBOOL		m_bEnable;
				
				Func		m_pCallback;
				Node*		m_pHost;
				
				KFLOAT		m_fRadius;
				KFLOAT		m_fHeight;
				
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
			};
			
			//////////////////////////////////////////////////////////////////////////

			virtual KVOID Tick(KFLOAT fElapsed);
			//-----------------------------------------------------------------
			virtual KVOID Destroy();
			//-----------------------------------------------------------------
			virtual KVOID Commit(CollisionData* pData);
			//-----------------------------------------------------------------
			// 查询场景碰撞， 
			// pos 要查询的位置，dir 朝向， 查询半径
			virtual KBOOL QueryScene(KPoint3 kPos, KPoint3 kDir, KFLOAT fRadius);
			//-----------------------------------------------------------------
			virtual Node* Query(const Ogre::Ray& kRay);
			//-----------------------------------------------------------------

		protected:
			virtual KVOID TestEntities();

		protected:
			typedef KMAP<Node*, CollisionData*> CllsnObjMap;

			CllsnObjMap m_kObjsMap;
		};
	}
}