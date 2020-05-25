#ifndef __FLAGDEFINE_H__
#define __FLAGDEFINE_H__

//키보드
typedef DWORD KEYFLAG;		//플래그 저장위한 타입 재정의(2바이트)
#define KEY_NULL		0x00000000
#define KEY_UP			0x00000001
#define KEY_DOWN		0x00000002
#define KEY_LEFT		0x00000004
#define KEY_RIGHT		0x00000008
#define KEY_LBUTTON		0x00000010
#define KEY_RBUTTON		0x00000020
#define KEY_TAB			0x00000040

#define KEY_S			0x00000080
#define KEY_D			0x00000100
#define KEY_C			0x00000200
#define KEY_I			0x00000400
#define KEY_Q			0x00000800
#define KEY_B			0x00001000
#define KEY_SHIFT		0x00002000


#define KEY_F5			0x00004000
#define KEY_F6			0x00008000
#define KEY_F7			0x00010000
#define KEY_F8			0x00020000

#define KEY_1			0x00040000
#define KEY_2			0x00080000
#define KEY_3			0x00100000
#define KEY_4			0x00200000
#define KEY_5			0x00400000
#define KEY_6			0x00800000
#define KEY_7			0x01000000
#define KEY_8			0x02000000
#define KEY_9			0x04000000

#define KEY_Z			0x08000000
#define KEY_X			0x10000000

#define KEY_ENTER		0x20000000








#endif // !__FLAGDEFINE_H__
