#include "engpch.h"
#include "profiling.h"
#include "profile.h"	

#define  FORMAT_COUNT 30
//control id
static const int nTextBox = 9;	
enum eControls
{
	IDC_TEXTBOX = nTextBox,
	IDC_BACKGROUND
};

//default text color
static const Ogre::ColourValue g_kTxtColor = Ogre::ColourValue(0, 0, 0, 255);
static const Ogre::ColourValue g_kFocusTxtColor = Ogre::ColourValue(255, 0, 255, 255);
static const Ogre::ColourValue g_kAlertTxtColor = Ogre::ColourValue(255, 0, 0, 255);

//format
static const char* g_pHeadFormat = "%-40s%-13s%-13s%-13s%-13s\n";
static const char* g_pDataFormat = "%-40s%-13.3f%-13.3f%-13.3f%-13.1f\n";

//position
static const int g_nX = 30;
static const int g_nDlgMoveOffset = 10;

//time scale
static const float g_fTimeScale = 1000.0f;

CProfiling::CProfiling(void)
:m_pItrParent(0)	
,m_pRoot(0)	
,m_bMined(false)
{
	m_nOrder		= 0;
	m_bShow			= false;
	bHelp			= false;
	m_nOrder		= 0;
	bFirstShow		= true;
}

CProfiling::~CProfiling(void)
{
	if(m_pItrParent)
		CProfileManager::Release_Iterator(m_pItrParent);
	
	if (m_spContentWidget)
	{
		MyGUI::Gui::getInstance().destroyChildWidget(m_spContentWidget);
		m_spContentWidget = NULL;
	}
}

void  CProfiling::Init()
{  

	const MyGUI::IntSize& rViewSize = MyGUI::Gui::getInstance().getViewSize();
	int nWidth = rViewSize.width;
	int nHeight= rViewSize.height/3;
	m_spContentWidget = MyGUI::Gui::getInstance().createWidget<MyGUI::List>(
		"List", 
		MyGUI::IntCoord(0, 0, nWidth, nHeight), 
		MyGUI::Align::Center, 
		"Main"
		);

	CProfileIterator::bProfileIterator=false; 
	
	m_spContentWidget->setVisible(false);
} 
// 
// void CProfiling::OnKeyDown(const unsigned char nVKey)	
// {
// 	if(!m_bShow)
// 		return;
// 
// 	if(nVKey == VK_ESCAPE)	//minimize or revert
// 		m_bMined = !m_bMined;
// 
// 	CDXUTTextBox *pTextBox = NULL;
// 	int x = 0, y = 0;
// 
// 	if(!m_bMined)	//not minimized, handle input
// 	{
// 		switch(nVKey)
// 		{
// 		case VK_UP:
// 			{
// 				if(m_nOrder > 0)
// 					--m_nOrder;
// 			}
// 			break;
// 		case VK_DOWN:
// 			{
// 				CProfileIterator itrNext = *m_pItrParent;
// 
// 				assert(m_nOrder >= 0);
// 
// 				//try to enter the next child
// 				const int nNextOrder = m_nOrder + 1;
// 				itrNext.Enter_Child(nNextOrder - 1);
// 
// 				if(itrNext.Get_CurrentParentNode() != m_pItrParent->Get_CurrentParentNode())
// 					m_nOrder = nNextOrder;
// 			}
// 			break;
// 		case VK_LEFT:
// 			{
// 				pTextBox = m_pProfile_Dialog->GetTextBox(IDC_TEXTBOX);
// 				pTextBox->GetLocation(x, y);
// 				pTextBox->SetLocation(x - g_nDlgMoveOffset, y);
// 			}
// 			break;
// 		case VK_RIGHT:
// 			{
// 				pTextBox = m_pProfile_Dialog->GetTextBox(IDC_TEXTBOX);
// 				pTextBox->GetLocation(x, y);
// 				pTextBox->SetLocation(x + g_nDlgMoveOffset, y);
// 			}
// 			break;
// 		case VK_RETURN:
// 			{
// 				assert(m_nOrder >= 0);
// 
// 				if(m_nOrder == 0)	//enter parent
// 				{
// 					EnterParent();
// 				}
// 				else	//enter a child
// 				{
// 					EnterChild();
// 				}
// 			}
// 			break;
// 		case VK_BACK:
// 			//enter parent
// 			EnterParent();
// 			break;
// 		}
// 	}
// }

void CProfiling::EnterParent()
{
	if(m_pItrParent->Get_CurrentParentNode()->Get_Parent())
	{
		CProfileNode* pOldChild = m_pItrParent->Get_CurrentParentNode();
		m_pItrParent->Enter_Parent();

		//set order at the old child
		m_nOrder = FindChildIndex(pOldChild) + 1;
	}
}

void CProfiling::EnterChild()
{
	const unsigned int nChild = m_nOrder - 1;

	m_pItrParent->Enter_Child(nChild);
	assert(m_pItrParent->Get_CurrentParentNode());

	m_nOrder = 0;
}

int CProfiling::FindChildIndex(CProfileNode* pChild)	//NieXu:
{
	CProfileNode* pHead = pChild->Get_Parent()->Get_Child();

	for(int i = 0; pHead; ++i)
	{
		if(pHead == pChild)
			return i;
		else
			pHead = pHead->Get_Sibling();
	}

	assert(!"CProfiling::FindChildIndex(): can't find the child.");

	return -1;
}

//从新设置控制台的大小
void CProfiling::ReSize(int width,int height)
{

}

void CProfiling::Render(float fElapsed)
{
    ProfileDraw();

}

void CProfiling::ProfileDraw(void)
{
	int nCharPosition = 0;
    char acOut[1024] = "";
	WCHAR acOutW[1024] = L"";

	if (CProfileIterator::bProfileIterator)
	{
		unsigned int uiLineNo = 0;  
		m_spContentWidget->removeAllItems();

		m_spContentWidget->addItem("Name                                    Ave          Max          Min          Freq         ");
		
		//iterator node
		if(!m_bMined)
		{
			int nNodeNum = 0;
			CProfileIterator temp = *m_pItrParent;

			//print parent node
			if (m_pItrParent->Get_CurrentParentNode() != m_pRoot)	//skip root because its data is invalid
			{  
				char name[128] = "";
				sprintf(name, "(%d)%s", nNodeNum, temp.Get_Current_Parent_Name());
				sprintf(acOut, g_pDataFormat, 
					name,
					temp.Get_Current_Parent_Ave_Time() * g_fTimeScale,
					temp.Get_Current_Parent_Max_Time() * g_fTimeScale,
					temp.Get_Current_Parent_Min_Time() * g_fTimeScale,
					temp.Get_Current_Parent_One_Time() * g_fTimeScale,
                    CalcCallRate(temp.Get_CurrentParentNode())
					);
				MultiByteToWideChar(CP_ACP, 0, acOut, -1, acOutW, sizeof(acOutW));
				{
					MultiByteToWideChar(CP_ACP, 0, acOut, -1, acOutW, sizeof(acOutW));
					//pTxtBox->AddText(acOutW,m_nOrder == 0 ? g_kFocusTxtColor : g_kTxtColor);
					int nCount	= m_spContentWidget->getItemCount();
					m_spContentWidget->addItem(acOutW, nCount);
				}

			}

			//print all children
			while (temp.Is_Done())
			{   
				++nNodeNum;

				char cCurrentName[128]="";
				sprintf(cCurrentName," (%d)%s",nNodeNum,temp.Get_Current_Name());
				sprintf(acOut, g_pDataFormat,
					cCurrentName,
					temp.Get_Current_Total_Ave_Time() * g_fTimeScale,
					temp.Get_Current_Total_Max_Time() * g_fTimeScale,
					temp.Get_Current_Total_Min_Time() * g_fTimeScale,
					temp.Get_Current_Total_One_Time() * g_fTimeScale,
					CalcCallRate(temp.Get_CurrentChildNode())
					);

				MultiByteToWideChar(CP_ACP, 0, acOut, -1, acOutW, sizeof(acOutW));

				if (temp.Get_Current_Total_One_Time()*1000>=temp.Get_Current_Total_Ave_Time()*1000*1.5)
				{
					//pTxtBox->AddText(acOutW, g_kAlertTxtColor);
					int nCount	= m_spContentWidget->getItemCount();
					m_spContentWidget->addItem(acOutW, nCount);
				}
				else
				{
					//const Ogre::ColourValue& textColor = nNodeNum == m_nOrder ? g_kFocusTxtColor : g_kTxtColor;	//highlight focused line
					//pTxtBox->AddText(acOutW,textColor);	
					int nCount	= m_spContentWidget->getItemCount();
					m_spContentWidget->addItem(acOutW, nCount);
				}
				temp.Next();
			}

			//if order is too large, make the last child focused
			assert(m_nOrder <= nNodeNum);
		}

		if (bFirstShow==true)
		{
			SetOrder(0); 
			bFirstShow=false; 
		}
	}
}

void CProfiling::Profiling(void)
{
	if(m_bShow==true)
	{
		Quit(); 
	}
	else
	{
		m_nOrder=0;
		Show(); 
	}
}


void CProfiling::Profiling(int nCommand)
{    
	bHelp=false;
	if ( m_bShow==false)
	{
		Show();
	}
	else
	{
		SetOrder(nCommand);
	}
}

void CProfiling::SetOrder(int nCommandMark)
{
	const unsigned int nOldOrder = m_nOrder;
	m_nOrder= nCommandMark;

	//set order
	bHelp=false;
	m_bShow=true;

	if (CProfileIterator::bProfileIterator!=true)
	{     
		CProfileIterator::bProfileIterator=true;
	}

	if (nCommandMark==0)
	{
		if (CProfileIterator::bProfileIterator==true)
			m_pItrParent->Enter_Parent();
	}
	else
	{
		if (CProfileIterator::bProfileIterator==true)
		{
			CProfileIterator itrTarget = *m_pItrParent;
			itrTarget.Enter_Child(nCommandMark - 1);

			// if failed, revert operation
			if(itrTarget.Is_Done())
				*m_pItrParent = itrTarget;
			else
				m_nOrder = nOldOrder;
		}
	}
}

void CProfiling::Quit()
{
	m_bShow=false;
    if (CProfileIterator::bProfileIterator==true)
	{
		m_nOrder=0;
		//m_pProfile_Dialog->GetTextBox( IDC_TEXTBOX)->Clear();
		m_spContentWidget->removeAllItems();
		
		CProfileIterator::bProfileIterator=false;
		m_spContentWidget->setVisible(false);
	}
}

void CProfiling::Show()
{
	bHelp=false;
	m_bShow=true;

	if(!m_pItrParent)
	{
		m_pItrParent = CProfileManager::Get_Iterator();

		m_pRoot = m_pItrParent->Get_CurrentParentNode();
		assert(!m_pRoot->Get_Parent());

		m_pItrParent->First();
		m_pItrParent->Enter_Parent();
	}
	
	m_spContentWidget->setVisible(true);
	CProfileIterator::bProfileIterator = true;
}

float CProfiling::CalcCallRate(CProfileNode* pNode)
{
	const unsigned int nChildCall = pNode->Get_Total_Calls();
	const unsigned int nParentCall = pNode->Get_Parent() ? pNode->Get_Parent()->Get_Total_Calls() : 0;

	return (float)nChildCall / nParentCall;
}