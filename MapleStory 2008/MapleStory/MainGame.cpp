#include "StdAfx.h"
#include "MainGame.h"
#include "Player.h"
#include "Factory.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "RenderMgr.h"

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
	m_pSceneMgr->SetScene(SC_MENU);
}

void CMainGame::Progress(DWORD _delta)
{
	CKeyMgr::GetInstance()->KeyCheck();

	m_pSceneMgr->Progress(_delta);
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
	CSceneMgr::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->Release();
	//::Safe_Delete(m_pSceneMgr);
}