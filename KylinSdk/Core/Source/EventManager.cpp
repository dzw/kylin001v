#include "corepch.h"
#include ".\eventmanager.h"
#include "EntityManager.h"
#include "singleton.h"
#include "entity.h"
#include "Event.h"
#include "Utility.h"


Event::Event(const EventTemplate* tp, Flag flag /* = ev_nextframe*/, KFLOAT fTime /* =0.0f*/, KINT nArgCount /* =0 */, ...)
	: m_pTemplate(tp)
	, m_flag(flag)
	, m_fTime(fTime)
{
	va_list marker;
	va_start(marker, nArgCount);
	for(KINT i=0; i< nArgCount; i++)
	{
		args.push_back(va_arg( marker, EventArg));
	}
	va_end( marker );              /* Reset variable arguments.      */
}

Event::Event(EventPtr spOther)
{
	m_pTemplate = spOther->m_pTemplate;
	m_fTime		= spOther->m_fTime;
	m_flag		= spOther->m_flag;
	unsigned uSize = spOther->args.size();
	for(unsigned i = 0; i<uSize; i++)
	{
		args.push_back(spOther->args[i]);
	}
}

extern KMAP<KSTR, const EventTemplate*> g_kEventMap;
EventTemplate::EventTemplate(KCCHAR * name)
: m_sName(name)
{
	g_kEventMap.clear();
	g_kEventMap.insert(std::pair<KSTR, const EventTemplate*>(m_sName,this));
}

const EventTemplate * EventTemplate::Find(KCCHAR * sName)
{
	return g_kEventMap.find(sName)->second;
}

Kylin::Archive& operator<<(Kylin::Archive& ar, Event& ev)
{
	ar << ev.m_fTime;
	ar << (KBYTE&)(ev.m_flag);

	if(ar.IsReading())
	{
		KBYTE by;
		ar << by;
		for(KINT i=0; i<by; i++)
		{
			EventArg earg;
			ar << earg;
			ev.args.push_back(earg);
		}
	}
	else
	{
		KBYTE by = ev.args.size();
		ar << by;
		for(KINT i=0; i<by; i++)
		{
			//???
			EventArg earg = ev.args[i];
			ar << earg;
		}
	}

	return ar;
}

/*
*	EvenArg serialization.
This is ugly, but ...
*/
class save_variant
	: public boost::static_visitor<>
{
public:
	save_variant(Kylin::Archive& ar):m_ar(ar){}

	KVOID operator()(KINT & i) const	{	KBYTE n= 0; m_ar << n; m_ar << i;}
	KVOID operator()(KUINT & i) const	{	KBYTE n = 1; m_ar << n; m_ar << i;}
	KVOID operator()(KFLOAT & i) const		
	{	
		KBYTE n = 2; 
		m_ar << n ; 

		unsigned short data = Kylin::floatToHalf(i);
		m_ar << data;
	}
	KVOID operator()(KBOOL & i) const		{	KBYTE n = 3; m_ar << n; m_ar << i;}
	KVOID operator()(KVOID * & i) const		
	{ 
		assert(!"Can't Serialize type of 'KVOID *'");
	}
	KVOID operator()(KCCHAR * & i) const	{	KBYTE n = 4; m_ar << n; m_ar << i;}
	Kylin::Archive& m_ar;
};

class load_variant
	: public boost::static_visitor<>
{
public:
	load_variant(Kylin::Archive& ar):m_ar(ar){}

	KVOID operator()(KINT & i) const		{	m_ar << i;}
	KVOID operator()(KUINT & i) const		{	m_ar << i;}
	KVOID operator()(KFLOAT & i) const		
	{	
		unsigned short data = 0;
		m_ar << data;
		i = Kylin::halfToFloat(data);
	}

	KVOID operator()(KBOOL & i) const		{	m_ar << i;}
	KVOID operator()(KVOID * & i) const		{assert(!"Can't Serialize type of 'KVOID *'");}
	KVOID operator()(KCCHAR * & i) const	{	m_ar << i;}
	Kylin::Archive& m_ar;
};

Kylin::Archive& operator<<(Kylin::Archive& ar, EventArg& earg)
{
	if( ar.IsReading())
	{
		KBYTE n;
		ar << n;
		switch(static_cast<KINT>(n))
		{
		case 0: earg = (KINT)0; break;
		case 1: earg = (KUINT)0; break;
		case 2: earg = (KFLOAT)0; break;
		case 3: earg = (KBOOL)0; break;
		case 4: earg = (KCCHAR *)0; break;
		default: 
			assert(false);
		}
		boost::apply_visitor(load_variant(ar), earg);
	}
	else
		boost::apply_visitor(save_variant(ar), earg);
	return ar;
}

namespace Kylin
{

	//!let given entity handle the given event immediately, return true if success.
	static KBOOL TryToHandleEvent( Kylin::Entity * pEnt, EventPtr spEvt );

	//////////////////////////////////////////////////////////////////////////
	EventManager::EventManager(EntityManager * pEntMgr)
		: m_pEntMgr(pEntMgr)
	{
	}

	EventManager::~EventManager(KVOID)
	{
	}

	EventManager::EventHandle EventManager::SendEvent(KUINT entityID, const EventPtr spEvent)
	{
		EventAssignPtr evHandle;
		switch(spEvent->m_flag)
		{
		case Event::ev_immediate:
			{
				Kylin::Entity * pEnt = m_pEntMgr->GetEntityPtr(entityID);
				if(pEnt)
				{
					// 				if(CmConfig::log_event_id == entityID)
					// 					syslog->debug("Event %s sent to %d\n", spEvent->m_pTemplate->GetName(), entityID);

					TryToHandleEvent(pEnt, spEvent);
				}
				else
				{
					//syslog->debug("Found event sent to non-exist entity%u\n", entityID);
				}
			}
			break;
		case Event::ev_nextframe:
		case Event::ev_timing:
		case Event::ev_recursive:
			//we need to put the events in a "temp" event que because there might be new events generated while we are handling events
			evHandle = EventAssignPtr(new EventAssign(spEvent, entityID));

			m_tempEventQue.push_back(evHandle);
			break;
		default:
			assert(!"unknown flag found in event");
		}

		return evHandle;
	}

	//send event to entity, return true if entity responsed. The query result will be put into spEvent, so it's not const
	KBOOL EventManager::SendQueryEvent(KUINT entityID, EventPtr spEvent)
	{
		assert(spEvent->m_flag == Event::ev_immediate);
		Kylin::Entity * pEnt = m_pEntMgr->GetEntityPtr(entityID);
		if(pEnt)
			return pEnt->HandleEvent( spEvent);
		return false;
	}

	KVOID EventManager::RemoveEvent(EventHandle evHandle)
	{
		assert(evHandle != NULL);
		evHandle->bRemoved = true;
	}

	EventPtr EventManager::GetEvent(EventHandle evHandle)
	{
		return evHandle->spEvent;
	}

	KVOID EventManager::HandleEvents(KFLOAT fElapsed)
	{
		//PROFILE("EventManager::HandleEvents()");
		//KFLOAT fTime = .0f;//g_pGame->GetGameTime();
		{
			//PROFILE("EventManager::DispatchEvent");
			//dispatch new events
			std::vector<EventAssignPtr>::iterator it;
			for(it=m_tempEventQue.begin(); it!=m_tempEventQue.end(); ++it)
			{
				EventAssignPtr ea = *it;
				if(ea->bRemoved)
					continue;
				switch(ea->spEvent->m_flag)
				{
				case Event::ev_immediate:
					assert(!"Found ev_immediate in tempEventQue");
					break;
				case Event::ev_nextframe:
					//push it to the next frame eventQue without handling it. otherwise handling events might generate new events into the temp event que.
					m_nextFrameEventQue.push_back(ea);
					break;
				case Event::ev_timing:
				case Event::ev_recursive:
					//ea->fExecuteTime = fTime + ea->spEvent->m_fTime;
					ea->fExecuteTime = ea->spEvent->m_fTime;

					m_timingEventQue.push(ea);
					break;
				default:
					assert(!"unknown event flag type");
				}
			}
			m_tempEventQue.clear();
		}

		//take care of the "next frame" events
		{
			//PROFILE("EventManager::NextFrameEvents");
			KINT nDebugNextFrameEvents = 0;
			std::vector<EventAssignPtr>::iterator it;
			for( it = m_nextFrameEventQue.begin(); it != m_nextFrameEventQue.end(); ++it)
			{
				nDebugNextFrameEvents ++;
				EventAssignPtr ea = *it;
				Kylin::Entity * pEnt = m_pEntMgr->GetEntityPtr( ea->entityID);
				if(pEnt)
				{
					// 				if(CmConfig::log_event_id == pEnt->GetID())
					// 					syslog->debug("Event %s sent to %d\n", ea->spEvent->m_pTemplate->GetName(), pEnt->GetID());

					TryToHandleEvent(pEnt, ea->spEvent);
				}
				else
				{
					//syslog->debug("Found event sent to non-exist entity%u\n", ea->entityID);	
				}
			}
			m_nextFrameEventQue.clear();
		}

		{
			//PROFILE("EventManager::Timing events");
			KINT nDebugTimingEvents = 0;
			//take care of all the timing events and recursive events
			while(!m_timingEventQue.empty())
			{
				nDebugTimingEvents++;
				//get the first most recent events
				EventAssignPtr ea = m_timingEventQue.top();
				ea->fExecuteTime -= fElapsed;

				if(ea->bRemoved)
				{
					//do nothing but remove it
					m_timingEventQue.pop();
				}
				//else if( ea->fExecuteTime <= fTime) //if the event supposed to happen
				else if( ea->fExecuteTime <= .0f) //if the event supposed to happen
				{
					m_timingEventQue.pop();

					Kylin::Entity * pEnt = m_pEntMgr->GetEntityPtr(ea->entityID);
					EventPtr spEvt = ea->spEvent;
					if(spEvt->m_flag == Event::ev_recursive)
					{
						//reinsert into eventque
						//ea->fExecuteTime = fTime + spEvt->m_fTime;
						ea->fExecuteTime = spEvt->m_fTime;
						m_timingEventQue.push(ea);
					}

					if(pEnt)
					{
						// 					if(CmConfig::log_event_id == pEnt->GetID())
						// 						syslog->debug("Event %s sent to %d\n", ea->spEvent->m_pTemplate->GetName(), pEnt->GetID());
						TryToHandleEvent(pEnt, spEvt);

					}
					else
					{
						//syslog->debug("Found event sent to non-exist entity%u\n", ea->entityID);
					}
				}
				else //no more events need to be executed
					break;
			} 
		}
	}

	//remove all the events associated with entity KUINT and has template ev
	//TODO This is low efficiency right now and should not be used too often
	KVOID EventManager::RemoveEvent(KUINT entityID, const EventTemplate* ev)
	{
		//syslog->debug("RemoveEvent(KUINT, const EventTemplate*) has very low performance, make sure you need to call this function\n");
		//PROFILE("EventManager::RemoveEvent");
		std::vector<EventAssignPtr>::iterator tempit;
		for(tempit = m_tempEventQue.begin(); tempit != m_tempEventQue.end(); ++ tempit)
		{
			EventAssignPtr ea = *tempit;
			if( ea->entityID == entityID && ea->spEvent->m_pTemplate == ev)
				ea->bRemoved = true;
		}
		for(tempit = m_nextFrameEventQue.begin(); tempit != m_nextFrameEventQue.end(); ++ tempit)
		{
			EventAssignPtr ea = *tempit;
			if( ea->entityID == entityID && ea->spEvent->m_pTemplate == ev)
				ea->bRemoved = true;
		}
	}

	//dumpstate for debuging
	KVOID EventManager::DumpEventAssignInfo(EventAssignPtr ea, KINT nDetail)
	{
		// 	if(nDetail == 0)
		// 		syslog->debug("EventTemplate:%s, ClEntity:%d, ExecuteTime:%f\n",
		// 			ea->spEvent->m_pTemplate->GetName(),
		// 			ea->entityID,
		// 			ea->fExecuteTime);
		// 	else
		// 	{
		// 	
		// 	}
	}

	KVOID EventManager::DumpState(KINT nDetail)
	{
		// 	syslog->debug("---Start EventManager State Dumping---\n");
		// 	syslog->debug("CurrentTime: %f\n", GF::GetGameTime());
		// 	syslog->debug("In TempEventQue:\n");
		std::vector<EventAssignPtr>::iterator tempit;
		for(tempit = m_tempEventQue.begin(); tempit != m_tempEventQue.end(); ++ tempit)
		{
			EventAssignPtr ea = *tempit;
			DumpEventAssignInfo(ea, nDetail);
		}
		//	syslog->debug("In NextFrameEventQue:\n");
		for(tempit = m_nextFrameEventQue.begin(); tempit != m_nextFrameEventQue.end(); ++ tempit)
		{
			EventAssignPtr ea = *tempit;
			DumpEventAssignInfo(ea, nDetail);
		}

		//	syslog->debug("---End EventManager State Dumping---\n");
	}

	KVOID EventManager::DumpState( char sBuf[256] )
	{
		sprintf(sBuf, "temp event:%d\nnextframe event:%d\ntiming event:%d", m_tempEventQue.size(), m_nextFrameEventQue.size(), m_timingEventQue.size());
	}

	KVOID EventManager::RemoveAllEvents()
	{
		m_tempEventQue.clear();
		m_nextFrameEventQue.clear();
		while(!m_timingEventQue.empty())
			m_timingEventQue.pop();
	}

	KBOOL TryToHandleEvent( Kylin::Entity * pEnt, EventPtr spEvt )
	{
		try
		{
			pEnt->HandleEvent( spEvt);
		}
		catch(boost::bad_get)
		{
			assert(!"event arguments error"); 
			//syslog->debug("Event Arguments Error:%s\n", spEvt->m_pTemplate->GetName());
			return false;
		}
		return true;
	}
}

