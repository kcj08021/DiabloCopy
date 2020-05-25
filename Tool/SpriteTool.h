#pragma once
#include "afxcmn.h"
#include "afxcolorbutton.h"
#include "afxwin.h"


// CSpriteTool 대화 상자입니다.

class CDeviceManager;
class CTextureManager;

class CSpriteTool : public CDialog
{
	DECLARE_DYNAMIC(CSpriteTool)

public:
	CSpriteTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSpriteTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPRITETOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	// 트리 컨트롤 변수
	list<PATH_INFO*> m_InfoList;
	CTreeCtrl m_TreeCtrl;
private:
	virtual BOOL OnInitDialog();
	// 텍스쳐 중심 X값
	int m_iCenterX;
	// 텍스쳐 중심 Y값
	int m_iCenterY;
	// 텍스쳐 가로 길이 출력 변수
	int m_iTextureWidth;
	// 텍스쳐 세로 길이 출력 변수
	int m_iTextureHeight;
	// 색상 선택을 위한 컨트롤 변수
	CMFCColorButton m_ColorCtrl;
	// 애니메이션 재생시 인덱스 위치를 표시할 슬라이더 컨트롤
	CSliderCtrl m_SliderCtrl;
	// 미리보기를 표시할 픽처 컨트롤 변수
	CStatic m_SpritePriview;
	// 애니메이션 재생시 보관할 인덱스 값
	int m_aniIndex;

	//매니저 변수
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
