#include "stdafx.h"
#include "KeyManager.h"

IMPLEMENT_SINGLETON(CKeyManager);

CKeyManager::CKeyManager()
	: m_KeyFlag(KEY_NULL), m_KeyDownFlag(KEY_NULL), m_KeyUpFlag(KEY_NULL){

}

CKeyManager::~CKeyManager(){

}

void CKeyManager::Update(){
	m_KeyFlag = KEY_NULL;

	// 방향키
	if(GetAsyncKeyState(VK_UP) & 0x8000)
		m_KeyFlag |= KEY_UP;
	if(GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_KeyFlag |= KEY_DOWN;
	if(GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_KeyFlag |= KEY_LEFT;
	if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_KeyFlag |= KEY_RIGHT;

	// 마우스
	if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_KeyFlag |= KEY_LBUTTON;
	if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_KeyFlag |= KEY_RBUTTON;
	
	// 문자키
	if(GetAsyncKeyState(CHAR_S) & 0x8000)
		m_KeyFlag |= KEY_S;
	if(GetAsyncKeyState(CHAR_D) & 0x8000)
		m_KeyFlag |= KEY_D;
	if(GetAsyncKeyState(CHAR_C) & 0x8000)
		m_KeyFlag |= KEY_C;
	if(GetAsyncKeyState(CHAR_I) & 0x8000)
		m_KeyFlag |= KEY_I;
	if(GetAsyncKeyState(CHAR_Q) & 0x8000)
		m_KeyFlag |= KEY_Q;
	if(GetAsyncKeyState(CHAR_B) & 0x8000)
		m_KeyFlag |= KEY_B;
	if(GetAsyncKeyState(CHAR_Z) & 0x8000)
		m_KeyFlag |= KEY_Z;
	if(GetAsyncKeyState(CHAR_X) & 0x8000)
		m_KeyFlag |= KEY_X;

	// 특수키
	if(GetAsyncKeyState(VK_SHIFT) & 0x8000)
		m_KeyFlag |= KEY_SHIFT;

	// 펑션키
	if(GetAsyncKeyState(VK_F5) & 0x8000)
		m_KeyFlag |= KEY_F5;
	if(GetAsyncKeyState(VK_F6) & 0x8000)
		m_KeyFlag |= KEY_F6;
	if(GetAsyncKeyState(VK_F7) & 0x8000)
		m_KeyFlag |= KEY_F7;
	if(GetAsyncKeyState(VK_F8) & 0x8000)
		m_KeyFlag |= KEY_F8;
	if(GetAsyncKeyState(CHAR_1) & 0x8000)
		m_KeyFlag |= KEY_1;
	if(GetAsyncKeyState(CHAR_2) & 0x8000)
		m_KeyFlag |= KEY_2;
	if(GetAsyncKeyState(CHAR_3) & 0x8000)
		m_KeyFlag |= KEY_3;
	if(GetAsyncKeyState(CHAR_4) & 0x8000)
		m_KeyFlag |= KEY_4;
	if(GetAsyncKeyState(CHAR_5) & 0x8000)
		m_KeyFlag |= KEY_5;
	if(GetAsyncKeyState(CHAR_6) & 0x8000)
		m_KeyFlag |= KEY_6;
	if(GetAsyncKeyState(CHAR_7) & 0x8000)
		m_KeyFlag |= KEY_7;
	if(GetAsyncKeyState(CHAR_8) & 0x8000)
		m_KeyFlag |= KEY_8;
	if(GetAsyncKeyState(VK_TAB) & 0x8000)
		m_KeyFlag |= KEY_TAB;
	if(GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_KeyFlag |= KEY_ENTER;
	
}

bool CKeyManager::IsKeyPressing(KEYFLAG _key){
	if(m_KeyFlag & _key)
		return true;
		
	return false;
}

bool CKeyManager::IsKeyDown(KEYFLAG _key ){
	if(!(m_KeyDownFlag & _key) && (m_KeyFlag & _key)){
		m_KeyDownFlag |= _key;
		return true;
	}

	if((m_KeyDownFlag & _key) && !(m_KeyFlag & _key)){
		m_KeyDownFlag ^= _key;
		return false;
	}

	return false;
}

bool CKeyManager::IsKeyUp(KEYFLAG _key){
	if((m_KeyUpFlag & _key) && !(m_KeyFlag & _key)){
		m_KeyUpFlag ^= _key;
		return true;
	}

	if(!(m_KeyUpFlag & _key) && (m_KeyFlag & _key)){
		m_KeyUpFlag |= _key;
		return false;
	}

	return false;
}

const D3DXVECTOR3 CKeyManager::GetMousePos(){
	POINT pt = {};
	::GetCursorPos(&pt);
	::ScreenToClient(g_hWnd, &pt);

	return D3DXVECTOR3(static_cast<float>(pt.x), static_cast<float>(pt.y), 0.f);
}
