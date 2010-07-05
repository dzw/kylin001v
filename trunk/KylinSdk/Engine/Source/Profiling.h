#pragma once

#include <MyGUI.h>
#include "Singleton.h"

class CProfileIterator;
class CProfileNode;

class CProfiling 
	:public Kylin::Singleton<CProfiling>
{
public:
	CProfiling(void);
	~CProfiling(void);

	void Init();

	void Profiling(int nCommand); 
	void Profiling();

	void ReSize(int width,int height);
	void Render(float fElapsed);

	bool m_bShow; // whether to show

private:
	CProfileIterator * m_pItrParent;	// profile iterator to navigate in profile node tree. this presents current parent node
	CProfileNode* m_pRoot;				//the root profile node

	bool bFirstShow;
	bool bHelp;

	int m_nOrder;	//which child currently focused. 0 means current parent node, and the first child starts with 1

	bool m_bMined;	//whether minimized
	
	MyGUI::ListPtr	m_spContentWidget;
	//CDXUTDialog * m_pProfile_Dialog; //dialog showing profile information

	void ProfileDraw(void);

	void Show();
	void Quit();

	void SetOrder(int nCommandMark);

	void EnterParent();
	void EnterChild();

	static int FindChildIndex(CProfileNode* pChild);
	static float CalcCallRate(CProfileNode* pNode);	// Get the ratio of child's call to parent's call

};
