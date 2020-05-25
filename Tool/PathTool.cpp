// PathTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathTool.h"
#include "afxdialogex.h"


// CPathTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPathTool, CDialog)

CPathTool::CPathTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHTOOL, pParent)
{

}

CPathTool::~CPathTool()
{
	for(auto elem : m_PathList)
		SafeDelete(elem);
}

void CPathTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_PathListBox);
}


BEGIN_MESSAGE_MAP(CPathTool, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathTool::OnBnClickedPathSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CPathTool::OnBnClickedPathLoad)
END_MESSAGE_MAP()


BOOL CPathTool::OnInitDialog(){
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	LoadPathInfoFile();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// CPathTool 메시지 처리기입니다.


void CPathTool::OnBnClickedPathSave(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	// 파일 열기 혹은 저장에 관한 대화상자 MFC 클래스
	CFileDialog Dlg(FALSE, L".txt", L"TextruePath.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

	if(IDOK == Dlg.DoModal()){
		wofstream fout;

		fout.open(Dlg.GetPathName());

		if(fout.fail())
			return;

		wstring wstrPreString = L"Type|Object|Image_Count||StateKey|DirectionKey|A|R|G|B|CenterX|CenterXY|FilePath";

		fout << wstrPreString.c_str() << endl;

		int iCount = m_PathListBox.GetCount();

		for(int i = 0; i < iCount; ++i){
			CString listString = L"";
			wstring wstrOutString = L"";
			m_PathListBox.GetText(i, listString);

			wstrOutString = listString.GetString();

			fout << wstrOutString.c_str() << endl;
		}

		fout.close();
	}

	UpdateData(FALSE);

	// 윈도우즈 기본 응용프로그램 실행.
	WinExec("notepad.exe ../Data/TextruePath.txt", SW_SHOW);
}


void CPathTool::OnBnClickedPathLoad(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_PathListBox.ResetContent();

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.

	::PathRemoveFileSpec(szCurrentPath);

	lstrcat(szCurrentPath, L"\\Texture");

	CFolderPickerDialog folderPicker(szCurrentPath, OFN_FILEMUSTEXIST, NULL, 0);

	if(IDOK == folderPicker.DoModal()){
		ExtractPathInfo(folderPicker.GetPathName().GetString());
	}

	for(auto elem : m_PathList){
		wstring String;
		TCHAR szNum[MIN_STR];

		String += elem->wstrTypeKey + L"|";
		String += elem->wstrObjectKey + L"|";

		swprintf_s(szNum, L"%d", elem->iImgCount);
		String += wstring(szNum) + L"|";
		
		String += elem->wstrStateKey + L"|";
		String += elem->wstrDirectionKey + L"|";

		swprintf_s(szNum, L"%d", elem->iColorA);
		String += wstring(szNum) + L"|";
		swprintf_s(szNum, L"%d", elem->iColorR);
		String += wstring(szNum) + L"|"; 
		swprintf_s(szNum, L"%d", elem->iColorG);
		String += wstring(szNum) + L"|"; 
		swprintf_s(szNum, L"%d", elem->iColorB);
		String += wstring(szNum) + L"|";

		swprintf_s(szNum, L"%d", elem->iCenterX);
		String += wstring(szNum) + L"|";
		swprintf_s(szNum, L"%d", elem->iCenterY);
		String += wstring(szNum) + L"|";

		String += elem->wstrImgPath;

		m_PathListBox.AddString(String.c_str());
	}
	InitHorizontalScroll();
	UpdateData(FALSE);
}


void CPathTool::ExtractPathInfo(const TCHAR* _szPath){
	wstring wstrFindPath = wstring(_szPath) + L"\\*.*";

	// 경로 및 파일 탐색에 관련된 MFC 클래스.
	CFileFind find;

	// 해당 경로나 파일이 존재하는지 조사하는 함수. 존재하면 TRUE 아니면 FALSE 반환.
	BOOL bIsFind = find.FindFile(wstrFindPath.c_str());

	// FindFile함수를 호출해서 존재 유무를 검사했다면 
	// FindNextFile함수를 호출하여 검색된 대상에 접근할 수 있다.
	// 만약, FindFile함수에 전달한 검색 대상이 *.*일 때
	// FindNextFile함수를 호출하면 검색 대상 중 제일 첫번째 녀석에 접근한다.		

	while(bIsFind){
		// 더 이상 찾을 것이 없거나 마지막 대상을 찾았을 때 FALSE를 반환.
		bIsFind = find.FindNextFile();

		if(find.IsDots()) // 찾은 것이 .이나 ..같은 마커라면 건너뛰어라
			continue;
		else if(find.IsDirectory()) // 찾은게 폴더라면 재귀.
			ExtractPathInfo(find.GetFilePath());
		else{
			// 파일을 찾았다면..
			if(find.IsSystem()) // 숨어있는 시스템 파일이라면 건너뛰어라.
				continue;

			////////////////////////////////////////////////////////////////

			PATH_INFO* pPathInfo = new PATH_INFO;

			// 상대경로 추출
			CString strFilePath = find.GetFilePath();

			// CString::Replace(old, new) old -> new 교체.
			strFilePath.Replace(L"0.", L"%d.");

			TCHAR szCurrentPath[MAX_STR] = L"";
			::GetCurrentDirectory(MAX_STR, szCurrentPath);

			TCHAR szRelativePath[MAX_STR] = L"";
			::PathRelativePathTo(szRelativePath, szCurrentPath, FILE_ATTRIBUTE_DIRECTORY,
				strFilePath, FILE_ATTRIBUTE_DIRECTORY);

			pPathInfo->wstrImgPath = szRelativePath;

			/////////////////////////////////////////////////////////////////

			// 이미지 개수 추출

			TCHAR szFullPath[MAX_STR] = L"";
			lstrcpy(szFullPath, strFilePath);
			::PathRemoveFileSpec(szFullPath);

			pPathInfo->iImgCount = CountImgFiles(szFullPath);

			vector<wstring> vecString;

			do{
				vecString.push_back(::PathFindFileName(szFullPath));
				::PathRemoveFileSpec(szFullPath);
			} while(vecString.back() != L"Texture");

			vecString.pop_back();

			auto iter = vecString.end();
			
			pPathInfo->wstrTypeKey = *--iter;
			pPathInfo->wstrObjectKey = *--iter;
			if(vecString.begin() != iter)
				pPathInfo->wstrStateKey = *--iter;
			if(vecString.begin() != iter)
				pPathInfo->wstrDirectionKey = *--iter;

			m_PathList.push_back(pPathInfo);
			break;
		}
	}
}

size_t CPathTool::CountImgFiles(const TCHAR * _szPath){
	TCHAR szPath[MAX_STR];

	::lstrcpy(szPath, _szPath);
	::lstrcat(szPath, L"\\*.*");
	
	CFileFind find;
	BOOL bIsFind = find.FindFile(szPath);

	size_t count = 0;

	while(bIsFind){
		bIsFind = find.FindNextFile();

		if(find.IsDots() || find.IsDirectory() || find.IsSystem())
			continue;

		++count;
	}

	return count;
}

void CPathTool::LoadPathInfoFile(){
	wifstream fin;

	fin.open(L"../Data/TextruePath.txt");

	if(fin.fail())
		return;


	TCHAR szString[MAX_STR] = {};

	fin.getline(szString, MAX_STR);

	m_PathListBox.ResetContent();
	while(true){
		fin.getline(szString,MAX_STR);

		m_PathListBox.AddString(szString);

		if(fin.eof())
			break;
	}

	fin.close();
	m_PathListBox.DeleteString(0);

	InitHorizontalScroll();
}

void CPathTool::InitHorizontalScroll(){
	CDC* pDC = m_PathListBox.GetDC();
	CSize stringPixel;
	int iCX = 0;

	CString strFileName = L"";

	// CListBox::GetCount
	// 현재 리스트박스에 들어있는 전체 데이터 개수 반환.
	for(int i = 0; i < m_PathListBox.GetCount(); ++i){
		m_PathListBox.GetText(i, strFileName);

		// GetTextExtent: 인자로 넘겨받은 문자열의 cx, cy 픽셀사이즈를 구해주는 함수.
		stringPixel = pDC->GetTextExtent(strFileName);

		if(iCX < stringPixel.cx)
			iCX = stringPixel.cx;
	}

	// 현재 리스트박스가 가지고 있는 수평 스크롤 최대 범위가 더 적으면 확장해라.
	if(iCX > m_PathListBox.GetHorizontalExtent())
		m_PathListBox.SetHorizontalExtent(iCX); // 수평 스크롤 범위 재지정.

	m_PathListBox.ReleaseDC(pDC);
}
