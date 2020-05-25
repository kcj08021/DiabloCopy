#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

class CDeviceManager;
class CTextureManager;
class CTimeManager;
class CScrollManager;
class CSoundManager;


class CGameObject{
private:
protected:
	//기본 오브젝트 변수
	INFO m_tInfo;						//기본 벡터들이 담긴 구조체
	STAT_INFO m_tStat;					//스탯들이 담긴 구조체
	POINT m_ptRectSize;					//충돌체크 사각형 크기
	RECT m_tRect;						//충돌체크할 사각형
	bool m_bIsVisible;					//출력 bool값
	bool m_bIsInit;						//초기화 완료 bool값
	bool m_bIsDead;						//죽음 상태 확인 bool값

	//텍스쳐 키 변수들
	wstring m_wstrTypeKey;				//타입키
	wstring m_wstrObjectKey;			//오브젝트키
	wstring m_wstrStateKey;				//스테이트키
	wstring m_wstrDirectionKey;			//디렉션키

	//애니메이션 처리 위한 변수
	const TEXTURE_INFO* m_pTextureInfo;	//텍스쳐 정보
	size_t m_ImgCount;					//이미지 갯수
	size_t m_ImgFrame;					//현재 출력할 이미지 프레임
	float m_fDeltaTime;					//출력 후 누적 시간
	bool m_bIsImgEnd;					//출력되는 이미지가 한번 루프한 경우

	//매니저 변수
	CDeviceManager* m_pDeviceMgr;		//장치매니저
	CTextureManager* m_pTextureMgr;		//텍스쳐매니저
	CTimeManager* m_pTimeMgr;			//타임매니저
	CScrollManager* m_pScrollMgr;		//스크롤매니저
	CSoundManager* m_pSoundMgr;

public:
	explicit CGameObject();
	virtual ~CGameObject();

//일반 멤버 함수
protected:
	//방향에 따른 디렉션키 계산
	void UpdateAngle();		
	//충돌 사각형 계산
	void UpdateRect();						
	//스프라이트 애니메이션 처리
	int Animation(float _fDeltaTime = 0.066f);

//Get 멤버 함수
public:
	//객체의 기본 벡터 정보를 얻어오는 함수
	const INFO& GetInfo() const{ return m_tInfo; }
	//충돌 사각형을 얻어옴
	const RECT& GetRect() const{ return m_tRect; }
	//객체의 스테이터스 얻어옴
	STAT_INFO& GetStat(){ return m_tStat; }
	//현재 오브젝트의 죽음 상태
	bool GetIsDead() const{ return m_bIsDead; }
	//현재 오브젝트의 타입
	const wstring& GetTypeKey()const{ return m_wstrTypeKey; }
	//현재 오브젝트의 이름
	const wstring& GetObjectKey()const{ return m_wstrObjectKey; }
	//현재 출력 상태값 얻어오는 함수
	bool IsVisible() const{ return m_bIsVisible; }
	

//Set 멤버 함수
public:
	//위치 설정(INFO::vPos)
	void SetPosition(const float _x, const float _y){ m_tInfo.vPos.x = _x; m_tInfo.vPos.y = _y; }
	//위치 설정(INFO::vPos)
	void SetPosition(const D3DXVECTOR3 _vPos){ m_tInfo.vPos = _vPos; }
	//출력 상태 설정
	void SetVisible(bool _bool){ m_bIsVisible = _bool; }
	//데미지 설정
	void SetDamage(int _iDamage);
	//애니메이션 프레임 초기화
	void SetAinimationInit();
	

//기본 구동 함수
private:
	virtual HRESULT Release() = 0;
protected:
	virtual HRESULT Initialize() = 0;
	virtual HRESULT LateInit();				//모든 오브젝트들의 Initialize()가 호출된 뒤 호출되는 함수
public:
	virtual HRESULT Update() = 0;			
	virtual HRESULT LateUpdate() = 0;		//모든 오브젝트들의 Update()가 호출된 뒤 호출되는 함수
	virtual HRESULT Render() = 0;
};
#endif // !__GAMEOBJECT_H__
