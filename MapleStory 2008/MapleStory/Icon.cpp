#include "StdAfx.h"
#include "Icon.h"
#include "Parent.h"

CIcon::CIcon(void)
{
}

CIcon::CIcon(ICONID _eID, string _strName)
:m_eID(_eID)
,m_strName(_strName)
,m_pBit(NULL)
{
	m_dwTime = GetTickCount();
	m_tInfo = INFO(0.f, 0.f, 32.f, 32.f);
}

CIcon::CIcon(const CIcon &_Icon)
{
	m_tInfo.fX = _Icon.m_tInfo.fX;
	m_tInfo.fY = _Icon.m_tInfo.fY;
	m_tInfo.fCX = _Icon.m_tInfo.fCX;
	m_tInfo.fCY = _Icon.m_tInfo.fCY;
	m_eID = _Icon.m_eID;
	m_strName = _Icon.m_strName;

	if (m_strName == "Typhoon_ON")
	{
		m_pBit = (new CBitBmp)->LoadBmp(L"../Texture/Skill/Icon/Typhoon_ON.bmp");
	}
	else if (m_strName == "Bolt_ON")
	{
		m_pBit = (new CBitBmp)->LoadBmp(L"../Texture/Skill/Icon/Bolt_ON.bmp");
	}
	else if (m_strName == "Beyond_ON")
	{
		m_pBit = (new CBitBmp)->LoadBmp(L"../Texture/Skill/Icon/Beyond_ON.bmp");
	}
	else if (m_strName == "Annihilation_ON")
	{
		m_pBit = (new CBitBmp)->LoadBmp(L"../Texture/Skill/Icon/Annihilation_ON.bmp");
	}
}

CIcon::~CIcon(void)
{
	Release();
}

void	CIcon::Initialize(void)
{
	switch (m_eID)
	{
	case IC_SKILL:
		if (m_strName == "Typhoon_ON")
		{
			m_pBit = (new CBitBmp)->LoadBmp(L"../Texture/Skill/Icon/Typhoon_ON.bmp");
		}
		else if (m_strName == "Bolt_ON")
		{
			m_pBit = (new CBitBmp)->LoadBmp(L"../Texture/Skill/Icon/Bolt_ON.bmp");
		}
		else if (m_strName == "Beyond_ON")
		{
			m_pBit = (new CBitBmp)->LoadBmp(L"../Texture/Skill/Icon/Beyond_ON.bmp");
		}
		else if (m_strName == "Annihilation_ON")
		{
			m_pBit = (new CBitBmp)->LoadBmp(L"../Texture/Skill/Icon/Annihilation_ON.bmp");
		}
		break;

	case IC_ITEM:
		break;
	}

}
void	CIcon::Progress(DWORD _dwDelta)
{
	Picking();
}
void	CIcon::Render(HDC hdc)
{
	TransparentBlt(hdc,
			int(m_tInfo.fX - m_tInfo.fCX / 2.f),
			int(m_tInfo.fY - m_tInfo.fCY / 2.f),
			int(m_tInfo.fCX),
			int(m_tInfo.fCY),
			m_pBit->GetMemdc(),
			0,
			0,
			(int)m_tInfo.fCX, 
			(int)m_tInfo.fCY,
			RGB(255, 255, 250));
}
void	CIcon::Release(void)
{
	::Safe_Delete(m_pBit);
}

void	CIcon::SetPos(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}
void	CIcon::Picking(void)
{
	
}

RECT CIcon::GetRect(void)
{
	RECT rc = {
		long(m_tInfo.fX - m_tInfo.fCX / 2.f),
		long(m_tInfo.fY - m_tInfo.fCY / 2.f),
		long(m_tInfo.fX + m_tInfo.fCX / 2.f),
		long(m_tInfo.fY + m_tInfo.fCY / 2.f)
	};

	return rc;
}