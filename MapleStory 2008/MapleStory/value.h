#pragma once

const int WINCX = 800;
const int WINCY = 600;

const int TILECX = 30;
const int TILECY = 30;

const int INVENSIZE = 24;

// Key ��

const	DWORD		KEY_LEFT	= 0x00000001;
const	DWORD		KEY_RIGHT	= 0x00000002;
const	DWORD		KEY_UP		= 0x00000004;
const	DWORD		KEY_DOWN	= 0x00000008;

const	DWORD		KEY_CONTROL	= 0x00000010;
const	DWORD		KEY_SPACE	= 0x00000020;
const	DWORD		KEY_Q		= 0x00000040;
const	DWORD		KEY_W		= 0x00000080;

const	DWORD		KEY_E		= 0x00000100;
const	DWORD		KEY_R		= 0x00000200;
const	DWORD		KEY_I		= 0x00000400;
const	DWORD		KEY_U		= 0x00000800;

const	DWORD		KEY_K		= 0x00001000;
const	DWORD		KEY_J		= 0x00002000;

const	DWORD		KEY_F5		= 0x00010000;
const	DWORD		KEY_F6		= 0x00020000;
const	DWORD		KEY_F7		= 0x00040000;
const	DWORD		KEY_F8		= 0x00080000;

const	DWORD		KEY_A		= 0x00100000;
const	DWORD		KEY_S		= 0x00200000;
const	DWORD		KEY_D		= 0x00400000;
const	DWORD		KEY_F		= 0x00800000;



// state ��

const	DWORD		ST_STAND		= 0x00000001;
const	DWORD		ST_WALK			= 0x00000002;
const	DWORD		ST_PROSTRATE	= 0x00000004;
const	DWORD		ST_UP			= 0x00000008;
const	DWORD		ST_ATTACK		= 0x00000010;
const	DWORD		ST_ATTACK2		= 0x00000020;
const	DWORD		ST_JUMP			= 0x00000040;
const	DWORD		ST_HIT			= 0x00000080;
const	DWORD		ST_DEATH		= 0x00000100;
