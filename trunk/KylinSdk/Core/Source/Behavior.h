#pragma once

#include "Action.h"

namespace Kylin
{
	class Movement : public Action
	{
	public:
		Movement(Entity* pHost);
		virtual ~Movement();

		virtual KBOOL Init(const PropertySet& kProp);
		virtual KVOID Tick(KFLOAT fElapsed);
		
	protected:
		KFLOAT	m_fSpeed;
		KPoint3	m_kDestn;

	};
	
	class Leaping : public Action
	{
	public:
		Leaping(Entity* pHost);
		virtual ~Leaping();

		virtual KBOOL Init(const PropertySet& kProp);
		virtual KVOID Tick(KFLOAT fElapsed);

	};

	class Idlesse : public Action
	{
	public:
		Idlesse(Entity* pHost);
		virtual ~Idlesse();

		virtual KBOOL Init(const PropertySet& kProp);
		virtual KVOID Tick(KFLOAT fElapsed);


	};
}