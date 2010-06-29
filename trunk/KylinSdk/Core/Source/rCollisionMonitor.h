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
					, m_eSelf(COT_NONE)
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

				typedef std::set<CollisionData*> ResponserSet;
				typedef KMAP<CollisionData*, ResponserSet> ColliderBag;

				ColliderBag m_kCllsnBag;
			};
			
			//////////////////////////////////////////////////////////////////////////

			virtual KVOID Tick(KFLOAT fElapsed);
			//-----------------------------------------------------------------
			virtual KVOID Destroy();
			//-----------------------------------------------------------------
			virtual CollisionData* Commit(Node* pHost,KBOOL bCollider,KUINT uSelf,KUINT uMate);
			//-----------------------------------------------------------------
			// 查询场景碰撞， 
			// pos 要查询的位置，dir 朝向， 查询半径
			virtual KBOOL QueryScene(KPoint3 kPos, KPoint3 kDir, KFLOAT fRadius);
			//-----------------------------------------------------------------

		protected:
			virtual KVOID TestEntities();

		protected:
			typedef KMAP<Node*, CollisionData*> CllsnObjMap;

			CllsnObjMap m_kObjsMap;
		};
	}
}