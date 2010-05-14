#pragma once

#include "Node.h"

#define QUERYFLAG_SHOWGIRL 1

namespace Kylin
{
	struct AnimUnit
	{
		AnimUnit()
		{
			mAnim		= "";
			mLoopFlag = false;
			mTime		= .0f;
		}

		KSTR	mAnim;
		KBOOL	mLoopFlag;
		KFLOAT	mTime;		// m_fTime = 0 时不受时间控制
	};
	
	typedef KVEC<AnimUnit> AnimQueue;

	class Showgirl : public Node
	{
	public:
		Showgirl(KUINT uGid);
		virtual ~Showgirl();
		
		KVOID Spawn();
		KBOOL Initialize();
		KVOID Tick(KFLOAT fElapsed);
		
		KVOID Showing(KBOOL bFlag);

		enum ShowFlag
		{
			SF_NODE_,
			SF_PREPARE_,
			SF_SHOWING_,
		};

	protected:
		KVOID	OnPrepare(KFLOAT fElapsed);
		KVOID	OnShowing(KFLOAT fElapsed);

	protected:
		friend class Stage;
		
		KSTR		m_sIdleAnim;
		KUINT		m_uGid;
		ShowFlag	m_eShowFlag;
		AnimQueue	m_kAnimQueue;
		KUINT		m_uCurAnimIndex;
		KFLOAT		m_fTempTime;
	};
}