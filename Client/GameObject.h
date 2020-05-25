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
	//�⺻ ������Ʈ ����
	INFO m_tInfo;						//�⺻ ���͵��� ��� ����ü
	STAT_INFO m_tStat;					//���ȵ��� ��� ����ü
	POINT m_ptRectSize;					//�浹üũ �簢�� ũ��
	RECT m_tRect;						//�浹üũ�� �簢��
	bool m_bIsVisible;					//��� bool��
	bool m_bIsInit;						//�ʱ�ȭ �Ϸ� bool��
	bool m_bIsDead;						//���� ���� Ȯ�� bool��

	//�ؽ��� Ű ������
	wstring m_wstrTypeKey;				//Ÿ��Ű
	wstring m_wstrObjectKey;			//������ƮŰ
	wstring m_wstrStateKey;				//������ƮŰ
	wstring m_wstrDirectionKey;			//�𷺼�Ű

	//�ִϸ��̼� ó�� ���� ����
	const TEXTURE_INFO* m_pTextureInfo;	//�ؽ��� ����
	size_t m_ImgCount;					//�̹��� ����
	size_t m_ImgFrame;					//���� ����� �̹��� ������
	float m_fDeltaTime;					//��� �� ���� �ð�
	bool m_bIsImgEnd;					//��µǴ� �̹����� �ѹ� ������ ���

	//�Ŵ��� ����
	CDeviceManager* m_pDeviceMgr;		//��ġ�Ŵ���
	CTextureManager* m_pTextureMgr;		//�ؽ��ĸŴ���
	CTimeManager* m_pTimeMgr;			//Ÿ�ӸŴ���
	CScrollManager* m_pScrollMgr;		//��ũ�ѸŴ���
	CSoundManager* m_pSoundMgr;

public:
	explicit CGameObject();
	virtual ~CGameObject();

//�Ϲ� ��� �Լ�
protected:
	//���⿡ ���� �𷺼�Ű ���
	void UpdateAngle();		
	//�浹 �簢�� ���
	void UpdateRect();						
	//��������Ʈ �ִϸ��̼� ó��
	int Animation(float _fDeltaTime = 0.066f);

//Get ��� �Լ�
public:
	//��ü�� �⺻ ���� ������ ������ �Լ�
	const INFO& GetInfo() const{ return m_tInfo; }
	//�浹 �簢���� ����
	const RECT& GetRect() const{ return m_tRect; }
	//��ü�� �������ͽ� ����
	STAT_INFO& GetStat(){ return m_tStat; }
	//���� ������Ʈ�� ���� ����
	bool GetIsDead() const{ return m_bIsDead; }
	//���� ������Ʈ�� Ÿ��
	const wstring& GetTypeKey()const{ return m_wstrTypeKey; }
	//���� ������Ʈ�� �̸�
	const wstring& GetObjectKey()const{ return m_wstrObjectKey; }
	//���� ��� ���°� ������ �Լ�
	bool IsVisible() const{ return m_bIsVisible; }
	

//Set ��� �Լ�
public:
	//��ġ ����(INFO::vPos)
	void SetPosition(const float _x, const float _y){ m_tInfo.vPos.x = _x; m_tInfo.vPos.y = _y; }
	//��ġ ����(INFO::vPos)
	void SetPosition(const D3DXVECTOR3 _vPos){ m_tInfo.vPos = _vPos; }
	//��� ���� ����
	void SetVisible(bool _bool){ m_bIsVisible = _bool; }
	//������ ����
	void SetDamage(int _iDamage);
	//�ִϸ��̼� ������ �ʱ�ȭ
	void SetAinimationInit();
	

//�⺻ ���� �Լ�
private:
	virtual HRESULT Release() = 0;
protected:
	virtual HRESULT Initialize() = 0;
	virtual HRESULT LateInit();				//��� ������Ʈ���� Initialize()�� ȣ��� �� ȣ��Ǵ� �Լ�
public:
	virtual HRESULT Update() = 0;			
	virtual HRESULT LateUpdate() = 0;		//��� ������Ʈ���� Update()�� ȣ��� �� ȣ��Ǵ� �Լ�
	virtual HRESULT Render() = 0;
};
#endif // !__GAMEOBJECT_H__
