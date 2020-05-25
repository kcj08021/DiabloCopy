#ifndef __KEY_MANAGER_H__
#define __KEY_MANAGER_H__

class CKeyManager{
	DECLARE_SINGLETON(CKeyManager);

//멤버 변수
private:
	KEYFLAG m_KeyFlag;
	KEYFLAG m_KeyDownFlag;
	KEYFLAG m_KeyUpFlag;

//생성자 및 소멸자
private:
	explicit CKeyManager();
	virtual ~CKeyManager();
//멤버 함수
public:
	void Update();
	bool IsKeyPressing(KEYFLAG _key);
	bool IsKeyDown(KEYFLAG _key);
	bool IsKeyUp(KEYFLAG _key);
	const D3DXVECTOR3 GetMousePos();
};

#define KEYMGR CKeyManager::GetInstance()
#endif // !__KEY_MANAGER_H__
