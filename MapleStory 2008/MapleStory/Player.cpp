#include "StdAfx.h"
#include "Player.h"
#include "KeyMgr.h"
#include "Factory.h"
#include "Skill.h"
#include "UI.h"
#include "Inventory.h"
#include "Equipment.h"
#include "SkillPanel.h"
#include "QuickSlot.h"
#include "Item.h"
#include "Weapon.h"

CPlayer::CPlayer(void)
:m_pSkill(NULL)
,m_iBeyond(0)
{
}

CPlayer::~CPlayer(void)
{
	Release();
}

void CPlayer::Initialize(void)
{
	m_cTimer.TimeSetting();
	m_tInfo = INFO(WINCX / 2.f, WINCY / 2.f, 70.f, 90.f);
	m_tStat = STAT(10.f, 10.f, 10.f, 10.f);
	m_tSprite = SPRITE(0, 5, 0, 80);
	m_pUI[UI_INVENTORY] = CFactory<CInventory>::CreateUI(600.f, 300.f);
	m_pUI[UI_EQUIPMENT] = CFactory<CEquipment>::CreateUI(500.f, 300.f);
	m_pUI[UI_SKILLPANEL] = CFactory<CSkillPanel>::CreateUI(600.f, 400.f);
	m_pUI[UI_QUICKSLOT] = CFactory<CQuickSlot>::CreateUI(730.f, 480.f);
	

	CItem*	pWeapon = new CWeapon("Weapon", 10, 1, 0, 100);
	pWeapon->Initialize();

	CItem*	pWeapon2 = new CWeapon("Weapon2", 10, 1, 0, 100);
	pWeapon2->Initialize();

	CItem*	pWeapon3 = new CWeapon("Weapon3", 10, 2, 0, 100);
	pWeapon3->Initialize();

	CItem*	pWeapon4 = new CWeapon("Weapon4", 10, 3, 0, 100);
	pWeapon4->Initialize();

	((CInventory*)m_pUI[UI_INVENTORY])->AddItem(pWeapon);
	((CInventory*)m_pUI[UI_INVENTORY])->AddItem(pWeapon2);
	((CInventory*)m_pUI[UI_INVENTORY])->AddItem(pWeapon3);
	((CInventory*)m_pUI[UI_INVENTORY])->AddItem(pWeapon4);
	
	for (int i = 0; i < UI_END; ++i)
	{
		m_bUIOnOff[i] = false;
	}
	m_bUIOnOff[UI_QUICKSLOT] = true;

	m_dwTime = GetTickCount();
	m_strKey = "Player_LEFT";
	m_dwKey = 0;

	m_ptOffset.x = WINCX / 2;
	m_ptOffset.y = WINCY / 2;
}

void CPlayer::Progress(DWORD _delta)
{
	KeyInput(_delta);
	Rotation();
	Gravity();
	ScrollX();
	ScrollY();

	if (m_dwTime + m_tSprite.dwTime < GetTickCount())
	{
		m_dwTime = GetTickCount();

		++m_tSprite.iStart;
	}

	if (m_tSprite.iStart >= m_tSprite.iLast)
	{
 		if ((m_dwState != ST_STAND) && (m_dwState != ST_PROSTRATE) && (m_dwState != ST_JUMP) && (m_dwState != ST_UP))
			m_dwState = ST_STAND;

		m_tSprite.iStart = 0;
	}

	for (int i = 0; i < UI_END; ++i)
	{
		if (m_bUIOnOff[i] == true)
		{
			m_pUI[i]->Progress(_delta);
		}
	}

	SetState(ST_STAND, 5, 0, 80);
	SetState(ST_WALK, 3, 1, 100);
	SetState(ST_JUMP, 1, 6, 100);
	SetState(ST_ATTACK, 3, 2, 150);
	SetState(ST_UP, 1, 8, 100);
	SetState(ST_PROSTRATE, 1, 5, 100);
}

void CPlayer::Render(HDC hdc)
{
		TransparentBlt(hdc, 
		int(m_tInfo.fX - m_tInfo.fCX / 2.f + m_ptScroll.x),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f + m_ptScroll.y),
		int(m_tInfo.fCX), 
		int(m_tInfo.fCY), 
		(*m_pBitMap)[m_strKey]->GetMemdc(),
		int(m_tInfo.fCX * m_tSprite.iStart), 
		int(m_tInfo.fCY * m_tSprite.iMotion),
		(int)m_tInfo.fCX, 
		(int)m_tInfo.fCY, 
		RGB(255, 255, 250));

		for (int i = 0; i < UI_END; ++i)
		{
			if (m_bUIOnOff[i] == true)
			{
				m_pUI[i]->Render(hdc);
			}
		}
}

void CPlayer::Release(void)
{
}

void CPlayer::KeyInput(DWORD _delta)
{
	m_dwKey = CKeyMgr::GetInstance()->GetKey();

	if (!m_dwKey && (m_dwState != ST_ATTACK) && (m_dwState != ST_UP) && (m_bLand == true))
	{
   		m_dwState = ST_STAND;
	}

	if (m_dwKey && (m_dwKey != KEY_UP) && (m_dwState != ST_ATTACK) && (m_dwState != ST_PROSTRATE) && (m_dwState != ST_UP) && (m_bLand == true))
	{
		m_dwState = ST_WALK;
	}

	if (m_dwKey & KEY_LEFT && (m_dwState != ST_UP) && (m_dwState != ST_PROSTRATE))
	{
		if (m_bLand && m_dwState == ST_ATTACK)
			return;

		m_tInfo.fX -= m_tStat.fSpeed;
	}

	if (m_dwKey & KEY_RIGHT && (m_dwState != ST_UP) && (m_dwState != ST_PROSTRATE))
	{
		if (m_bLand && m_dwState == ST_ATTACK)
			return;

		m_tInfo.fX += m_tStat.fSpeed;
	}

	if ((m_dwKey & KEY_UP) && (m_dwState & ST_UP))
	{
		m_bLand = true;
		m_tInfo.fY -= m_tStat.fSpeed;
	}

	if (m_dwKey & KEY_DOWN)
	{
		if (m_dwState & ST_UP)
		{
			m_bLand = true;
			m_tInfo.fY += m_tStat.fSpeed;
		}

		if (m_dwKey & KEY_SPACE)
		{
			m_dwState = ST_JUMP;
			m_tInfo.fY += 25.f;

			if (m_bLand == true)
			{
				m_fJpower = 5.f;
				m_bLand = false;
			}
		}
	}

	if (m_dwKey & KEY_CONTROL)
	{
		m_dwState = ST_ATTACK;
	}

	if (m_dwKey & KEY_SPACE)
	{
		m_dwState = ST_JUMP;

		if (m_bLand == true)
		{
			m_fJpower = -12.f;
			m_bLand = false;
		}
	}

	if (m_dwKey & KEY_I)	// 인벤토리
	{
		if (m_bUIOnOff[UI_INVENTORY])
			m_bUIOnOff[UI_INVENTORY] = false;
		
		else
			m_bUIOnOff[UI_INVENTORY] = true;
	}

	if (m_dwKey & KEY_U)	// 장비창
	{
		if (m_bUIOnOff[UI_EQUIPMENT])
			m_bUIOnOff[UI_EQUIPMENT] = false;
		
		else
			m_bUIOnOff[UI_EQUIPMENT] = true;
	}

	if (m_dwKey & KEY_K)	// 스킬창
	{
		if (m_bUIOnOff[UI_SKILLPANEL])
			m_bUIOnOff[UI_SKILLPANEL] = false;
		
		else
			m_bUIOnOff[UI_SKILLPANEL] = true;
	}

	if ((m_cTimer.m_fRemainTime[1] -= _delta) <= 0)
	{
		if (m_dwKey & KEY_Q)
		{
			if (m_strKey == "Player_LEFT") 
				m_pSkill->push_back(CreateSkill(m_tInfo.fX - 150, m_tInfo.fY - 15,"Annihilation_LEFT"));

			else if (m_strKey == "Player_RIGHT")
				m_pSkill->push_back(CreateSkill(m_tInfo.fX + 150, m_tInfo.fY - 15,"Annihilation_RIGHT"));

			m_cTimer.m_fRemainTime[1] = 2000.f;
		}
	}
	if (m_dwKey & KEY_W)
	{
		if (m_strKey == "Player_LEFT") 
			m_pSkill->push_back(CreateSkill(m_tInfo.fX, m_tInfo.fY - 15,"Ascend_LEFT"));

		else if (m_strKey == "Player_RIGHT")
			m_pSkill->push_back(CreateSkill(m_tInfo.fX, m_tInfo.fY - 15,"Ascend_RIGHT"));

		return;
	}

	if (m_dwKey & KEY_E)
	{
		if (m_strKey == "Player_LEFT") 
			m_pSkill->push_back(CreateSkill(m_tInfo.fX - 150, m_tInfo.fY - 45,"Typhoon_LEFT"));

		else if (m_strKey == "Player_RIGHT")
			m_pSkill->push_back(CreateSkill(m_tInfo.fX + 150, m_tInfo.fY - 45,"Typhoon_RIGHT"));

		return;
	}

	if (m_dwKey & KEY_R)
	{
		switch (m_iBeyond)
		{
		case 0:
			if (m_strKey == "Player_LEFT") 
				m_pSkill->push_back(CreateSkill(m_tInfo.fX - 50, m_tInfo.fY,"Beyond_LEFT"));

			else if (m_strKey == "Player_RIGHT")
				m_pSkill->push_back(CreateSkill(m_tInfo.fX + 50, m_tInfo.fY,"Beyond_RIGHT"));
		
			++m_iBeyond;
			break;
		case 1:
			if (m_strKey == "Player_LEFT") 
				m_pSkill->push_back(CreateSkill(m_tInfo.fX - 50, m_tInfo.fY,"Beyond2_LEFT"));

			else if (m_strKey == "Player_RIGHT")
				m_pSkill->push_back(CreateSkill(m_tInfo.fX + 50, m_tInfo.fY,"Beyond2_RIGHT"));
			
			++m_iBeyond;
			break;
		case 2:
			if (m_strKey == "Player_LEFT") 
				m_pSkill->push_back(CreateSkill(m_tInfo.fX - 50, m_tInfo.fY,"Beyond3_LEFT"));

			else if (m_strKey == "Player_RIGHT")
				m_pSkill->push_back(CreateSkill(m_tInfo.fX + 50, m_tInfo.fY,"Beyond3_RIGHT"));
			
			m_iBeyond = 0;
			break;

		}
	}
}

void CPlayer::Rotation(void)
{
	m_dwKey = CKeyMgr::GetInstance()->GetKey();

	if ((m_dwKey & KEY_LEFT) && (m_dwState != ST_ATTACK) && (m_dwState != ST_PROSTRATE))
		m_strKey = "Player_LEFT";

	if ((m_dwKey & KEY_RIGHT) && (m_dwState != ST_ATTACK) && (m_dwState != ST_PROSTRATE))
		m_strKey = "Player_RIGHT";

	if (m_dwKey & KEY_UP)
	{
	}

	if ((m_dwKey & KEY_DOWN) && (m_dwState != ST_JUMP) && (m_dwState != ST_UP))
	{
		m_dwState = ST_PROSTRATE;
	}
}

void CPlayer::SetState(DWORD _dwState, int _iLast, int _iMotion, DWORD _dwTime)
{
	if (m_dwState & _dwState)
	{
		if (m_tSprite.iMotion != _iMotion)
			m_tSprite.iStart = 0;

		m_tSprite.iLast = _iLast;
		m_tSprite.iMotion = _iMotion;
		m_tSprite.dwTime = _dwTime;
	}
}

void CPlayer::ScrollX(void)
{
// 좌측 끝
	if(m_tInfo.fX < m_ptOffset.x - 100.f)
	{
		if(m_ptScroll.x > 0 - m_tStat.fSpeed)
		{
			if(m_tInfo.fX < 0)
				m_tInfo.fX = 0.f;

			return;
		}

		m_ptScroll.x += (long)m_tStat.fSpeed;
		m_ptOffset.x -= (long)m_tStat.fSpeed;
	}

	// 우측 끝
	if(m_tInfo.fX > m_ptOffset.x + 100.f)
	{
		if(m_ptScroll.x < WINCX - m_ptMapSize.x + m_tStat.fSpeed)
		{
			if(m_tInfo.fX > m_ptMapSize.x)
				m_tInfo.fX = (float)m_ptMapSize.x;

			return;
		}

		m_ptScroll.x -= (long)m_tStat.fSpeed;
		m_ptOffset.x += (long)m_tStat.fSpeed;
	}
}


void CPlayer::ScrollY(void)
{
	// 상단 끝
	if(m_tInfo.fY < m_ptOffset.y)
	{
		if(m_ptScroll.y > 0 - m_tStat.fSpeed)
		{
			if(m_tInfo.fY < 0)
				m_tInfo.fY = 0.f;

			return;
		}

		m_ptScroll.y += (long)m_tStat.fSpeed;
		m_ptOffset.y -= (long)m_tStat.fSpeed;
	}

	// 하단 끝
	if(GetRect().bottom > m_ptOffset.y)
	{
		if(m_ptScroll.y < WINCY - m_ptMapSize.y + m_tStat.fSpeed)
		{
			if(m_tInfo.fY > m_ptMapSize.y)
				m_tInfo.fY = (float)m_ptMapSize.y;

			return;
		}

		m_ptScroll.y -= (long)m_tStat.fSpeed;
		m_ptOffset.y += (long)m_tStat.fSpeed;
	}
}

CParent* CPlayer::CreateSkill(float _fX, float _fY, string _strKey)
{

	CParent* pSkill = CFactory<CSkill>::CreateParent(_fX, _fY, _strKey);

	return pSkill;
}

void CPlayer::SetSkill(vector<CParent*>* _pSkill)
{
	m_pSkill = _pSkill;
}

void CPlayer::SetMapSize(float _fX, float _fY)
{
	m_ptMapSize.x = (long)_fX;
	m_ptMapSize.y = (long)_fY;
}
