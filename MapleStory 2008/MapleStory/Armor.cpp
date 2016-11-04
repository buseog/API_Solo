#include "StdAfx.h"
#include "Armor.h"

CArmor::CArmor(void)
{
}

CArmor::CArmor(wstring _strName, int _iOption, int _iCount, int _iPrice, int _iType)
:CItem(_strName, _iOption, _iCount, _iPrice, _iType)
{
	Initialize();
}
CArmor::~CArmor(void)
{
}

void CArmor::Initialize(void)
{
	m_tInfo = INFO(0, 0, 30.f, 30.f);
}

void CArmor::Progress(DWORD _delta)
{
	if(PtInRect(&GetRect(), GetMouse()))
	{
		m_DrawId = 1;
	}
	else
		m_DrawId = 0;
}

void CArmor::Render(HDC hdc)
{
	TransparentBlt(hdc,
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		(*m_pBitMap)["Armor"]->GetMemdc(),
		0,
		0,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 255, 250));

	


	if (m_DrawId == 1)
	{
		BitBlt(hdc,
		int(m_tInfo.fX - m_tInfo.fCX / 2.f + 30),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f + 30),
		150,
		120,
		(*m_pBitMap)["ItemStat"]->GetMemdc(),
		0,
		0,
		SRCCOPY);

		TCHAR szName[128] = L"";
		TCHAR szOption[128] = L"";
		TCHAR szPrice[128] = L"";

		wsprintf(szName, L"�̸� : %s", m_tItem.m_strName.c_str());
					TextOut(hdc,
					m_tInfo.fX + 30, m_tInfo.fY + 30,
					szName, lstrlen(szName));

		wsprintf(szOption, L"���� : %d", (int)m_tItem.m_iOption);
					TextOut(hdc, 
						m_tInfo.fX + 30, m_tInfo.fY + 45,
						szOption, lstrlen(szOption));

		wsprintf(szPrice, L"���� : %d", (int)m_tItem.m_iPrice);
					TextOut(hdc, 
							m_tInfo.fX + 30, m_tInfo.fY + 60,
						szPrice, lstrlen(szPrice));

		if (m_tItem.m_iType == IT_WEAPON)
		{
			TextOut(hdc, m_tInfo.fX + 30, m_tInfo.fY + 75,	L"���� : ����", 7);
		}
		if (m_tItem.m_iType == IT_ARMOR)
		{
			TextOut(hdc, m_tInfo.fX + 30, m_tInfo.fY + 75,	L"���� : ��", 8);
		}
		if (m_tItem.m_iType == IT_POTION)
		{
			TextOut(hdc, m_tInfo.fX + 30, m_tInfo.fY + 75,	L"���� : ����", 7);
		}

		SetBkMode((*m_pBitMap)["Back"]->GetMemdc(),TRANSPARENT);
	}

}

void CArmor::Release(void)
{

}