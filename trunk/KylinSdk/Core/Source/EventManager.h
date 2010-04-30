#pragma once
#include "Event.h"

namespace Kylin
{
	class EventManager
	{
	public:
		EventManager(EntityManager *);
		virtual ~EventManager(KVOID);

	protected:
		struct EventAssign
		{
			EventAssign(EventPtr spEv, KUINT id): 
		spEvent(spEv), entityID(id), fExecuteTime(0.0f), bRemoved(false){}
		EventPtr spEvent;
		KUINT	entityID;
		KFLOAT	fExecuteTime;
		KBOOL	bRemoved;

		};
		typedef boost::shared_ptr<EventAssign> EventAssignPtr;
		KVOID DumpEventAssignInfo(EventAssignPtr ea, KINT nDetail);

		KVEC<EventAssignPtr> m_tempEventQue;
		KVEC<EventAssignPtr> m_nextFrameEventQue;

		//timing event priority_queue
		class EventComp
		{
		public:
			KBOOL operator() (const EventAssignPtr& p1, const EventAssignPtr& p2) const
			{
				return p1->fExecuteTime > p2->fExecuteTime;
			}
		};
		std::priority_queue<EventAssignPtr, KVEC<EventAssignPtr>, EventComp> m_timingEventQue; 
		
		EntityManager * m_pEntMgr;
		
	public:
		typedef EventAssignPtr EventHandle;

		EventHandle SendEvent(KUINT entityID, const EventPtr spEvent);

		//!send event to entity, return true if entity responsed. The query result will be put into spEvent, so it's not const
		KBOOL SendQueryEvent(KUINT entityID, EventPtr spEvent);

		//!Should be called per-tick
		KVOID HandleEvents(KFLOAT fElapsed);

		//!remove all the events associated with entity ID and has template ev
		//TODO This is low efficiency right now and should not be used too often
		KVOID RemoveEvent(KUINT entityID, const EventTemplate* ev);

		//!remove event with given handle
		KVOID RemoveEvent(EventHandle evHandle);

		//!remove all events in event manager
		KVOID RemoveAllEvents();

		//!Return the EventPtr with given handle
		EventPtr GetEvent(EventHandle evHandle);

		//Dump event manager state, used for debug
		KVOID DumpState(KINT nDetail);
		KVOID DumpState(KCHAR sBuf[256]);
	};

}
