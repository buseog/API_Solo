#include "StdAfx.h"
#include "MainGame.h"
#include "Player.h"
#include "Factory.h"
#include "SceneMgr.h"
#include "KeyMgr.h"

CMainGame::CMainGame(void)
:m_pSceneMgr(CSceneMgr::GetInstance())
{
}

CMainGame::~CMainGame(void)
{
	Release();
}


void CMainGame::Initialize(void)
{
	m_hdc = GetDC(g_hWnd);
	m_pSceneMgr->SetScene(SC_LOADING);
}

void CMainGame::Progress(void)
{
	CKeyMgr::GetInstance()->KeyCheck();

	m_pSceneMgr->Progress();
}

void CMainGame::Render(void)
{
	m_pSceneMgr->Render(m_hdc);
}

void CMainGame::Release(void)
{
	ReleaseDC(g_hWnd, m_hdc);
	m_pSceneMgr->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInst();


}