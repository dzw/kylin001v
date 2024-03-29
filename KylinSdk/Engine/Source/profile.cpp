#include "Engpch.h"
#include "profile.h"
#include <windows.h>

unsigned __int64 GetCycleCount()
{
	LARGE_INTEGER counter;
	::QueryPerformanceCounter(&counter);

	return counter.QuadPart;
}

inline void Profile_Get_Ticks(_int64 * ticks)
{
	__asm
	{
		push edx;
		push ecx;
		mov ecx,ticks;
		_emit 0Fh
		_emit 31h
		mov [ecx],eax;
		mov [ecx+4],edx;
		pop ecx;
		pop edx;
	}


}

inline float Profile_Get_Tick_Rate(void)
{
	static float _CPUFrequency = -1.0f;
	
	if (_CPUFrequency == -1.0f) {
		__int64 curr_rate = 0;
		::QueryPerformanceFrequency ((LARGE_INTEGER *)&curr_rate);
		_CPUFrequency = (float)curr_rate;

	} 
	
	return _CPUFrequency;
}

/***************************************************************************************************
**
** CProfileNode
**
***************************************************************************************************/

/***********************************************************************************************
 * INPUT:                                                                                      *
 * name - pointer to a static string which is the name of this profile node                    *
 * parent - parent pointer                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 * The name is assumed to be a static pointer, only the pointer is stored and compared for     *
 * efficiency reasons.                                                                         *
 *=============================================================================================*/
CProfileNode::CProfileNode( const char * name, CProfileNode * parent ) :
	Name( name ),
	TotalCalls( 0 ),
	TotalTime( 0 ),
	StartTime( 0 ),
	RecursionCounter( 0 ),
	Parent( parent ),
	Child( NULL ),
	Sibling( NULL )
{
	Reset();
}


CProfileNode::~CProfileNode( void )
{
	delete Child;
	delete Sibling;
}


/***********************************************************************************************
 * INPUT:                                                                                      *
 * name - static string pointer to the name of the node we are searching for                   *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 * All profile names are assumed to be static strings so this function uses pointer compares   *
 * to find the named node.                                                                     *
 *=============================================================================================*/
CProfileNode * CProfileNode::Get_Sub_Node( const char * name )
{
	// Try to find this sub node
	CProfileNode * child = Child;
	while ( child ) {
		if ( child->Name == name ) {
			return child;
		}
		child = child->Sibling;
	}

	// We didn't find it, so add it
	CProfileNode * node = new CProfileNode( name, this );
	node->Sibling = Child;
	Child = node;

	return node;
}


void	CProfileNode::Reset( void )
{
	TotalCalls	= 0;
	TotalTime	= 0.0f;
    OneTime		= 0.0f;
	AveTime		= 0.0f;
	MinTime		= 100000.0f;	//set a maximum value
	MaxTime		= 0.0f;			//set a minimum value
	m_nFront	= 0;
	m_nRear		= 0;
	m_nNowSize	= 0;
	bvisited	=false;

	ZeroMemory( &this->m_afTimeQueue, sizeof(this->m_afTimeQueue) );

    ControlMaxMin=true;

	if ( Child ) {
		Child->Reset();
	}
	if ( Sibling ) {
		Sibling->Reset();
	}
}


void	CProfileNode::Call( void )
{
	TotalCalls++;
	if (RecursionCounter++ == 0) {
		StartTime=GetCycleCount();
	}
}


bool	CProfileNode::Return( void )
{
	if ( --RecursionCounter == 0 && TotalCalls != 0 ) { 
		__int64 time;
		time=GetCycleCount();
		time-=StartTime;
		OneTime=(float)time / Profile_Get_Tick_Rate();

		if (this->m_nNowSize==100)
		{
			//full
			this->m_nRear=this->m_nFront;
			float fTimeTemp=this->m_afTimeQueue[this->m_nRear];	
			this->m_afTimeQueue[this->m_nRear]=OneTime;
			this->TotalTime+=OneTime - fTimeTemp; 
			this->AveTime =this->TotalTime/this->m_nNowSize;
			this->m_nFront=(this->m_nFront+1) % 100;
		}
		else
		{	
			//not full
			this->m_nNowSize++;
			this->m_afTimeQueue[this->m_nRear]=OneTime;
			this->TotalTime+=OneTime;
			this->AveTime =this->TotalTime/this->m_nNowSize;
			this->m_nRear=(this->m_nRear+1) % 100;
		}
	
		if  (ControlMaxMin==true)
		{
			this->MaxTime=OneTime;
			this->MinTime=OneTime;
			ControlMaxMin=false;
		}

		this->TempMin=10000.0f;//a maximum value
		this->TempMax=0.0f;	//a minimum value

		for (int i=0;i<m_nNowSize;i++)
		{
			if  (this->TempMax<this->m_afTimeQueue[i])
				 this->TempMax=this->m_afTimeQueue[i];

			if  (this->TempMin>this->m_afTimeQueue[i])
				 this->TempMin=this->m_afTimeQueue[i];
		}
		this->MaxTime =this->TempMax;
		this->MinTime =this->TempMin;
	}

	return ( RecursionCounter == 0 );
}

void CProfileNode::	Set_TimeAt()
{
	if (TotalCalls != 0)
	{
		AveTime=TotalTime/TotalCalls;
	}
	else
	{
		AveTime=0.0f;
	}
	TotalTime=0.0f;
	TotalCalls = 0;
	MinTime=this->TempMin;
	MaxTime=this->TempMax;
	this->TempMin=100000.0f;//a maximum value
	this->TempMax=0.0f;	//a minimum value
}



/***************************************************************************************************
**
** CProfileIterator
**
***************************************************************************************************/
bool CProfileIterator::bProfileIterator=false;

CProfileIterator::CProfileIterator( CProfileNode * start )
{
	CurrentParent = start;
	CurrentChild = CurrentParent->Get_Child();
}


void	CProfileIterator::First(void)
{
	CurrentChild = CurrentParent->Get_Child();
}


void	CProfileIterator::Next(void)
{
	CurrentChild = CurrentChild->Get_Sibling();
}


bool	CProfileIterator::Is_Done(void)
{
	if (CurrentChild == NULL)
         return false;
	else
		 return true;
}


void	CProfileIterator::Enter_Child( int index )
{
	CurrentChild = CurrentParent->Get_Child();

    while ( (CurrentChild != NULL) && (index != 0) ) {	
	    index--;
		CurrentChild = CurrentChild->Get_Sibling();
	}

	if ( CurrentChild != NULL ) {
		CurrentParent = CurrentChild;
		CurrentChild = CurrentParent->Get_Child();
	}
}


void	CProfileIterator::Enter_Parent( void )
{
	if ( CurrentParent->Get_Parent() != NULL ) {
		CurrentParent = CurrentParent->Get_Parent();
	}
    CurrentChild = CurrentParent->Get_Child();
}


/***************************************************************************************************
**
** CProfileManager
**
***************************************************************************************************/

CProfileNode	CProfileManager::Root( "Root", NULL );
CProfileNode *	CProfileManager::CurrentNode = &CProfileManager::Root;
int				CProfileManager::FrameCounter = 0;
__int64			CProfileManager::ResetTime = 0;


/***********************************************************************************************
 * CProfileManager::Start_Profile -- Begin a named profile                                    *
 *                                                                                             *
 * Steps one level deeper into the tree, if a child already exists with the specified name     *
 * then it accumulates the profiling; otherwise a new child node is added to the profile tree. *
 *                                                                                             *
 * INPUT:                                                                                      *
 * name - name of this profiling record                                                        *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 * The string used is assumed to be a static string; pointer compares are used throughout      *
 * the profiling code for efficiency.                                                          *
 *=============================================================================================*/
void	CProfileManager::Start_Profile( const char * name )
{
	if (name != CurrentNode->Get_Name()) {
		CurrentNode = CurrentNode->Get_Sub_Node( name );
	} 
	
	CurrentNode->Call();
}


/***********************************************************************************************
 * CProfileManager::Stop_Profile -- Stop timing and record the results.                       *
 *=============================================================================================*/
void	CProfileManager::Stop_Profile( void )
{
	// Return will indicate whether we should back up to our parent (we may
	// be profiling a recursive function)
	if (CurrentNode->Return()) {
		CurrentNode = CurrentNode->Get_Parent();
	}
}
/***********************************************************************************************
* CProfileManager::Stop_Profile -- Stop timing and record the results.                       *
	
*=============================================================================================*/
void	CProfileManager::Stop_ProfileWithWatchDog(float& fOneTime)
{
	// Return will indicate whether we should back up to our parent (we may
	// be profiling a recursive function)
	if (CurrentNode->Return()) {
		fOneTime = CurrentNode->Get_Max_Time();
		CurrentNode = CurrentNode->Get_Parent();
	}
}



/***********************************************************************************************
 * CProfileManager::Reset -- Reset the contents of the profiling system                       *
 *                                                                                             *
 *    This resets everything except for the tree structure.  All of the timing data is reset.  *
 *=============================================================================================*/
void	CProfileManager::Reset( void )
{ 
	Root.Reset(); 
	FrameCounter = 0;
	ResetTime=GetCycleCount();
}


/***********************************************************************************************
 * CProfileManager::Increment_Frame_Counter -- Increment the frame counter                    *
 *=============================================================================================*/
void CProfileManager::Increment_Frame_Counter( void )
{
	FrameCounter++;
}


/***********************************************************************************************
 * CProfileManager::Get_Time_Since_Reset -- returns the elapsed time since last reset         *
 *=============================================================================================*/
float CProfileManager::Get_Time_Since_Reset( void )
{
	__int64 time;
	time= GetCycleCount();
	time -= ResetTime;
	return (float)time / Profile_Get_Tick_Rate();
}



