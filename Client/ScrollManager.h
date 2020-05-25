#ifndef __SCROLL_MANAGER_H__
#define __SCROLL_MANAGER_H__

class CScrollManager{
	DECLARE_SINGLETON(CScrollManager)

	// 멤버 변수
private:
	D3DXVECTOR3 m_vScroll;
	D3DXVECTOR3 m_vMaxScroll;

	// 생성자 및 소멸자
private:
	explicit CScrollManager();
	virtual ~CScrollManager();

	// Get 멤버 함수
public:
	const D3DXVECTOR3 GetScroll() const { return m_vScroll; }
	const D3DXVECTOR3 GetMaxScroll() const { return m_vMaxScroll; };

	// Set 멤버 함수
public:
	void SetScroll(float _fX, float _fY){ m_vScroll.x = _fX; m_vScroll.y = _fY; }
	void SetMaxScroll(float _fMaxX, float _fMaxY){ m_vMaxScroll.x = _fMaxX; m_vMaxScroll.y = _fMaxY; }
	
	// 일반 멤버 함수
private:
public:
	void IntiScroll();
	void CheckScroll();
	void MoveScroll(float _fAddX, float _fAddY){ m_vScroll.x += _fAddX; m_vScroll.y += _fAddY; }

};

#define SCROLLMGR CScrollManager::GetInstance()

#endif // !__SCROLL_MANAGER_H__
