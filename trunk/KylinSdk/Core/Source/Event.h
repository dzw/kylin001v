#pragma once

#include "standard.h"
#include "archive.h"
#include "boost/variant.hpp"
#include "boost/shared_ptr.hpp"

#pragma warning(disable : 4996) //hash_map

typedef boost::variant<int, unsigned  int, float, const char *, bool, void *> EventArg; 

class EventTemplate
{
public:
	EventTemplate(KCCHAR * name);
	KCCHAR * GetName() const {return m_sName.data();}
	static const EventTemplate * Find(KCCHAR * sName);

private:
	KSTR m_sName;
};

class Event;

typedef boost::shared_ptr<Event const> EventPtr;

class Event
{
public:
	enum Flag
	{
		ev_immediate,	//立刻发生
		ev_nextframe,	//下一帧时发生
		ev_timing,		//在游戏时间超过fTime时发生
		ev_recursive,	//从当前时刻开始，每隔fTime时间发生一次
	};
	Event():m_pTemplate(NULL), m_fTime(0.0f), m_flag(ev_immediate){};
	Event(EventPtr spOther);
	Event(const EventTemplate* tp, Flag flag = ev_nextframe, float fTime = 0.0f, int nArgCount = 0, ...);
	template<typename T>
	const T& GetArg(int nIndex)const;
	std::vector<EventArg> args;
	const EventTemplate * m_pTemplate;
	float	m_fTime;
	Flag	m_flag;
};

template<typename T>
const T& Event::GetArg(int nIndex)const
{
	return boost::get<T>(args[nIndex]);
}

Kylin::Archive& operator<<(Kylin::Archive& ar, Event& ev);
Kylin::Archive& operator<<(Kylin::Archive& ar, EventArg& earg);

template<class T>
struct EventResponse
{
	typedef void (T::*MemberFunction)(EventPtr);
	EventTemplate * pTemplate;
	MemberFunction responseFunc;
};

class EventHandler
{
public:
	virtual bool HandleEvent(EventPtr ev) {return false;}
	virtual bool IsValidEvent(EventPtr ev) {return false;}
};

#define Declare_Event_Handler(CLASS, PARENT) \
	typedef stdext::hash_map<const EventTemplate*, EventResponse<CLASS>::MemberFunction> FuncMap;	\
static EventResponse<CLASS> m_response_declare[];											\
static  FuncMap m_funcmap;																	\
virtual bool HandleEvent(EventPtr ev);													\
virtual bool IsValidEvent(EventPtr ev);													\
static void ConstructFuncMap();

#define Implement_Event_Handler(CLASS, PARENT)\
	bool CLASS::HandleEvent(EventPtr ev)	{												\
	FuncMap::iterator it = m_funcmap.find(ev->m_pTemplate);									\
	if( it == m_funcmap.end()) {															\
	return PARENT::HandleEvent(ev);														\
	}																						\
	else {																					\
	(this->*(it->second))(ev);															\
	return true;																		\
	}																						\
	}																							\
	bool CLASS::IsValidEvent(EventPtr ev) {												\
	FuncMap::iterator it = m_funcmap.find(ev->m_pTemplate);									\
	if( it == m_funcmap.end()) {															\
	return PARENT::IsValidEvent(ev);												\
	}																						\
	return false;																			\
	}																							\
	void CLASS::ConstructFuncMap(){																\
	EventResponse<CLASS> * pEntry = m_response_declare;									\
	while(pEntry->pTemplate != NULL)														\
{																						\
	m_funcmap.insert(std::pair<const EventTemplate*, EventResponse<CLASS>::MemberFunction>(pEntry->pTemplate, pEntry->responseFunc));																				\
	pEntry ++;																			\
}																						\
	}\
	CLASS::FuncMap CLASS::m_funcmap; \
	EventResponse<CLASS> CLASS::m_response_declare[] = 
