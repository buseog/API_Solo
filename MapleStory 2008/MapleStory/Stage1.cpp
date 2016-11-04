#include "StdAfx.h"
#include "Stage1.h"
#include "Player.h"
#include "Monster.h"
#include "Factory.h"
#include "UI.h"
#include "Portal.h"
#include "CollisionMgr.h"

CStage1::CStage1(void)
{
	m_strKey = "Stage1";
	LoadMap();
	LoadBmp();

	m_vecPortal.push_back(CFactory<CPortal>::CreateParent(50.f, 1140.f, "Portal"));
	((CPortal*)m_vecPortal.back())->SetPortal(1);

	m_vecPortal.push_back(CFactory<CPortal>::CreateParent(1650.f, 1140.f, "Portal"));
	((CPortal*)m_vecPortal.back())->SetPortal(3);
}

CStage1::~CStage1(void)
{
	Release();
}


void CStage1::Initialize(void)
{
	ParentClear();

	((CPlayer*)m_vecParent[PAR_PLAYER].back())->SetMapSize(1773.f, 1464.f);

	CParent::SetBitMap(&m_BitMap);
	CUI::SetBitMap(&m_BitMap);

	m_pLoading = new CLoading();
}

void CStage1::Progress(DWORD _delta)
{
		KeyInput();
	UIDrag();

	for (size_t i = 0; i < PAR_END; ++i)
	{
		for (vector<CParent*>::iterator iter = m_vecParent[i].begin(); iter != m_vecParent[i].end();)
		{
			(*iter)->Progress(_delta);

			if ((*iter)->GetDestroy())
			{
				::Safe_Delete(*iter);
				iter = m_vecParent[i].erase(iter);

				if (iter == m_vecParent[i].end())
					break;
			}
			else
				++iter;
		}
	}
	
	float fHp = m_vecParent[PAR_PLAYER].back()->GetStat().fHp / m_vecParent[PAR_PLAYER].back()->GetStat().fFullHp;
	float fExp = m_vecParent[PAR_PLAYER].back()->GetStat().fExp / 1000.f;

	for (size_t i = 0; i < UI_END; ++i)
	{
		if (m_bUIView[i])
		{
			for (vector<CUI*>::iterator iter = m_vecUI[i].begin(); iter != m_vecUI[i].end(); ++iter)
			{
				(*iter)->SetPercent(fHp, fExp);
				(*iter)->Progress(_delta);
			}
		}
	}

	for (size_t i = 0; i < m_vecPortal.size(); ++i)
	{
		m_vecPortal[i]->Progress(_delta);
	}

	if (m_pLoading)
	{
		m_pLoading->Progress(_delta);
		
		if (m_pLoading->GetDestroy())
			::Safe_Delete(m_pLoading);
	}

	if (GetAsyncKeyState(VK_UP))
		CCollisionMgr::CollisionPortal(&m_vecParent[PAR_PLAYER], &m_vecPortal);

	CCollisionMgr::CollisionPTile(&m_vecParent[PAR_PLAYER], &m_vecTile);
	CCollisionMgr::CollisionMTile(&m_vecParent[PAR_MONSTER], &m_vecTile);
	m_vecParent[PAR_PLAYER].back()->SetExp(CCollisionMgr::CollisionSKill(&m_vecParent[PAR_SKILL], &m_vecParent[PAR_MONSTER]));
	CCollisionMgr::CollisionBodyButt(&m_vecParent[PAR_PLAYER], &m_vecParent[PAR_MONSTER]);
}

void CStage1::Render(HDC hdc)
{
	BitBlt(m_BitMap["Back"]->GetMemdc(), 
		0 + m_vecParent[PAR_PLAYER].back()->GetScroll().x,
		0 + m_vecParent[PAR_PLAYER].back()->GetScroll().y, 
			1773, 1464, 
			m_BitMap[m_strKey]->GetMemdc(),
			0, 0, SRCCOPY);

	for (size_t i = 0; i < PAR_END; ++i)
	{
		for (vector<CParent*>::iterator iter = m_vecParent[i].begin(); iter != m_vecParent[i].end(); )
		{
			(*iter)->Render(m_BitMap["Back"]->GetMemdc());

			if (iter == m_vecParent[i].end())
				break;

			else
				++iter;
		}
	}

	for (size_t i = 0; i < UI_END; ++i)
	{
		if (m_bUIView[i])
		{
			for (vector<CUI*>::iterator iter = m_vecUI[i].begin(); iter != m_vecUI[i].end(); ++iter)
			{
				(*iter)->Render(m_BitMap["Back"]->GetMemdc());
			}
		}
	}
	
	for (size_t i = 0; i < m_vecPortal.size(); ++i)
	{
		m_vecPortal[i]->Render(m_BitMap["Back"]->GetMemdc());
	}

	if (m_pLoading)
		m_pLoading->Render(m_BitMap["Back"]->GetMemdc());

	BitBlt(hdc, 
			0, 0, 
			WINCX, WINCY, 
			m_BitMap["Back"]->GetMemdc(),
			0, 0, SRCCOPY);
}

void CStage1::Release(void)
{
	for (map<string, CBitBmp*>::iterator iter = m_BitMap.begin(); iter != m_BitMap.end(); ++iter)
	{
		::Safe_Delete(iter->second);
	}
	m_BitMap.clear();

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		::Safe_Delete(m_vecTile[i]);
	}
	m_vecTile.clear();

	for (size_t i = 0; i < m_vecPortal.size(); ++i)
	{
		::Safe_Delete(m_vecPortal[i]);
	}
	m_vecPortal.clear();
}