#pragma once
#include "afxcmn.h"
#include "afxcolorbutton.h"
#include "afxwin.h"


// CSpriteTool ��ȭ �����Դϴ�.

class CDeviceManager;
class CTextureManager;

class CSpriteTool : public CDialog
{
	DECLARE_DYNAMIC(CSpriteTool)

public:
	CSpriteTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSpriteTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPRITETOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
private:
	// Ʈ�� ��Ʈ�� ����
	list<PATH_INFO*> m_InfoList;
	CTreeCtrl m_TreeCtrl;
private:
	virtual BOOL OnInitDialog();
	// �ؽ��� �߽� X��
	int m_iCenterX;
	// �ؽ��� �߽� Y��
	int m_iCenterY;
	// �ؽ��� ���� ���� ��� ����
	int m_iTextureWidth;
	// �ؽ��� ���� ���� ��� ����
	int m_iTextureHeight;
	// ���� ������ ���� ��Ʈ�� ����
	CMFCColorButton m_ColorCtrl;
	// �ִϸ��̼� ����� �ε��� ��ġ�� ǥ���� �����̴� ��Ʈ��
	CSliderCtrl m_SliderCtrl;
	// �̸����⸦ ǥ���� ��ó ��Ʈ�� ����
	CStatic m_SpritePriview;
	// �ִϸ��̼� ����� ������ �ε��� ��
	int m_aniIndex;

	//�Ŵ��� ����
	CDeviceManager* m_pDeviceMgr;
	CTextureManager* m_pTextureMgr;

private:
	void Render(const TEXTURE_INFO* pTextureInfo);

public:
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedApply();
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedPlay();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedSaveSetting();

	afx_msg void OnBnClickedStop();
};
