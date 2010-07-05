#pragma	  once

/*
** A node in the Profile Hierarchy Tree
*/
class	CProfileNode {

public:
	CProfileNode( const char * name, CProfileNode * parent );
	~CProfileNode( void );

	CProfileNode * Get_Sub_Node( const char * name );

	CProfileNode * Get_Parent( void )		
	{ return Parent; }
	CProfileNode * Get_Sibling( void )		{ return Sibling; }
	CProfileNode * Get_Child( void )			{ return Child; }

	void				Reset( void );
	void				Call( void );
	bool				Return( void );
	bool				bvisited;
	const char *		Get_Name( void )				{ return Name; }
	int					Get_Total_Calls( void )		{ return TotalCalls; }
	
	float				Get_Total_Time( void )		{ return TotalTime;}
	float               Get_Min_Time(void)          {return  MinTime;}
	float               Get_Max_Time(void)          {return  MaxTime;}
	float               Get_Ave_Time(void)           {return  AveTime;}
	float               Get_One_Time(void)           {return  OneTime;}
	void				Set_TimeAt();
    
protected:

	const char *	Name;
	int				TotalCalls;
	__int64			StartTime;
    
	float			TotalTime;
	float           AveTime;
	float           MinTime;
	float           MaxTime;
	
	float           OneTime;

	float			TempMax;
	float			TempMin; 
	int				m_nFront;
	int				m_nRear;
	int				m_nNowSize;
	float			m_afTimeQueue[100];
    bool			ControlMaxMin;  
	int				RecursionCounter;

	CProfileNode *	Parent;
	CProfileNode *	Child;
	CProfileNode *	Sibling;
};

/*
** An iterator to navigate through the tree
*/
class CProfileIterator
{
public:
	// Access all the children of the current parent
	                    CProfileIterator(void);
	void				First(void);
	void				Next(void);
	bool				Is_Done(void);

	void				Enter_Child( int index );		// Make the given child the new parent
	void				Enter_Largest_Child( void );	// Make the largest child the new parent
	void				Enter_Parent( void );			// Make the current parent's parent the new parent

	// Access the current child
	const char *	    Get_Current_Name( void )			{ return CurrentChild->Get_Name(); }
	int				    Get_Current_Total_Calls( void )	{ return CurrentChild->Get_Total_Calls(); }
	float				Get_Current_Total_Time( void )	{ return CurrentChild->Get_Total_Time(); }
	float               Get_Current_Total_Min_Time(void)        {return  CurrentChild->Get_Min_Time();}
	float               Get_Current_Total_Max_Time(void)        {return  CurrentChild->Get_Max_Time();}
	float               Get_Current_Total_Ave_Time(void)        {return  CurrentChild->Get_Ave_Time();}
	float               Get_Current_Total_One_Time(void)        {return  CurrentChild->Get_One_Time();}
    // the next line 
	void				Set_Current_TimeAt(void)				{ CurrentChild->Set_TimeAt();}
	CProfileNode *     Get_CurrentParentNode(void)   {return  CurrentParent;}
	CProfileNode *     Get_CurrentChildNode(void)    {return  CurrentChild;}
	void               Set_CurrentParentNode(CProfileNode* CurrentParentBak) {CurrentParent=CurrentParentBak;}
	void               Set_CurrentChildNode(CProfileNode* CurrentChildBak) {CurrentChild=CurrentChildBak;}

	// Access the current parent
	const char *	    Get_Current_Parent_Name( void )			{ return CurrentParent->Get_Name(); }
	int				    Get_Current_Parent_Total_Calls( void )	{ return CurrentParent->Get_Total_Calls();}
	float				Get_Current_Parent_Total_Time( void )	{return  CurrentParent->Get_Total_Time();}
	float               Get_Current_Parent_Min_Time(void)       {return  CurrentParent->Get_Min_Time();}
	float               Get_Current_Parent_Max_Time(void)       {return  CurrentParent->Get_Max_Time();}
	float               Get_Current_Parent_Ave_Time(void)       {return  CurrentParent->Get_Ave_Time();}
	float               Get_Current_Parent_One_Time(void)       {return  CurrentParent->Get_One_Time();}

    static bool  bProfileIterator;
protected:

	CProfileNode *	CurrentParent;
	CProfileNode *	CurrentChild;
    
	CProfileIterator( CProfileNode * start );
	friend	class		CProfileManager;
	
};


/*
** The Manager for the Profile system
*/
class	CProfileManager {
public:
	static	void						Start_Profile( const char * name );
	static	void						Stop_Profile( void );
	
	static void							Stop_ProfileWithWatchDog( float& fOneTime );

	static	void						Reset( void );
	static	void						Increment_Frame_Counter( void );
	static	int							Get_Frame_Count_Since_Reset( void )		{ return FrameCounter; }
	static	float						Get_Time_Since_Reset( void );
	static								CProfileIterator *	Get_Iterator( void )	{ return new CProfileIterator( &Root ); }
	static	void						Release_Iterator( CProfileIterator * iterator ) { delete iterator; }
 	static int							Get_bProfileIterator(void) { return bProfileIterator; }
private:
	static	CProfileNode			Root;
	static	CProfileNode *			CurrentNode;
	static	int						FrameCounter;
	static	__int64					ResetTime;
    static int  bProfileIterator;
  
};


/*
** ProfileSampleClass is a simple way to profile a function's scope
** Use the PROFILE macro at the start of scope to time
*/
class	CProfileSample {
public:
	CProfileSample( const char * name )
	{ 
		if(CProfileIterator::bProfileIterator)
			CProfileManager::Start_Profile( name ); 
	}
	
	~CProfileSample( void )					
	{ 
		if(CProfileIterator::bProfileIterator)
			CProfileManager::Stop_Profile(); 
	}
};

#ifndef _USE_DEBUG_TOOL
	#define	PROFILE_S()
#else 	
    #define	PROFILE_S()	\
		CProfileSample __profile( __FUNCTION__ )
#endif	//_USE_DEBUG_TOOL

#ifndef _USE_DEBUG_TOOL
	#define	PROFILE( name )
#else
	#define	PROFILE( name )	\
		CProfileSample __profile( #name )
#endif	//_USE_DEBUG_TOOL

#ifndef _USE_DEBUG_TOOL
	#define PROFILE_START(name)
	#define PROFILE_END(onetime) { onetime = 0.0f;}
#else //_USE_DEBUG_TOOL

// optimize to make profile cost less cup cycles while not activated
#define PROFILE_START( name )				{\
												if(CProfileIterator::bProfileIterator) \
													CProfileManager::Start_Profile( name);\
											}

#define PROFILE_END(onetime)				{\
												if(CProfileIterator::bProfileIterator) \
													CProfileManager::Stop_ProfileWithWatchDog(onetime);\
											}

#define PROFILE_END_WATCH(maxtime)			{\
												float fTime = 0.0f; \
												if(CProfileIterator::bProfileIterator) \
													CProfileManager::Stop_ProfileWithWatchDog(fTime);\
												assert(fTime<maxtime); \
											}
#endif

