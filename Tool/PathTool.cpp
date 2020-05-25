// PathTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathTool.h"
#include "afxdialogex.h"


// CPathTool ��ȭ �����Դϴ�.

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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	LoadPathInfoFile();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

// CPathTool �޽��� ó�����Դϴ�.


void CPathTool::OnBnClickedPathSave(){
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	// ���� ���� Ȥ�� ���忡 ���� ��ȭ���� MFC Ŭ����
	CFileDialog Dlg(FALSE, L".txt", L"TextruePath.txt",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Text Files(*.txt)|*.txt|Data Files(*.dat)|*.dat||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	// ���� ��ȭ���ڿ��� ������ �ʱ� ��� ����.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // ����� X

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

	// �������� �⺻ �������α׷� ����.
	WinExec("notepad.exe ../Data/TextruePath.txt", SW_SHOW);
}


void CPathTool::OnBnClickedPathLoad(){
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	m_PathListBox.ResetContent();

	TCHAR szCurrentPath[MAX_STR] = L"";

	// ���� �۾� ��θ� ������ �Լ�.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// ���� ��ο��� ���ϸ� �����ϴ� �Լ�. ������ ���ϸ��� ������ ���� �������� �����Ѵ�.

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

	// ��� �� ���� Ž���� ���õ� MFC Ŭ����.
	CFileFind find;

	// �ش� ��γ� ������ �����ϴ��� �����ϴ� �Լ�. �����ϸ� TRUE �ƴϸ� FALSE ��ȯ.
	BOOL bIsFind = find.FindFile(wstrFindPath.c_str());

	// FindFile�Լ��� ȣ���ؼ� ���� ������ �˻��ߴٸ� 
	// FindNextFile�Լ��� ȣ���Ͽ� �˻��� ��� ������ �� �ִ�.
	// ����, FindFile�Լ��� ������ �˻� ����� *.*�� ��
	// FindNextFile�Լ��� ȣ���ϸ� �˻� ��� �� ���� ù��° �༮�� �����Ѵ�.		

	while(bIsFind){
		// �� �̻� ã�� ���� ���ų� ������ ����� ã���� �� FALSE�� ��ȯ.
		bIsFind = find.FindNextFile();

		if(find.IsDots()) // ã�� ���� .�̳� ..���� ��Ŀ��� �ǳʶپ��
			continue;
		else if(find.IsDirectory()) // ã���� ������� ���.
			ExtractPathInfo(find.GetFilePath());
		else{
			// ������ ã�Ҵٸ�..
			if(find.IsSystem()) // �����ִ� �ý��� �����̶�� �ǳʶپ��.
				continue;

			////////////////////////////////////////////////////////////////

			PATH_INFO* pPathInfo = new PATH_INFO;

			// ����� ����
			CString strFilePath = find.GetFilePath();

			// CString::Replace(old, new) old -> new ��ü.
			strFilePath.Replace(L"0.", L"%d.");

			TCHAR szCurrentPath[MAX_STR] = L"";
			::GetCurrentDirectory(MAX_STR, szCurrentPath);

			TCHAR szRelativePath[MAX_STR] = L"";
			::PathRelativePathTo(szRelativePath, szCurrentPath, FILE_ATTRIBUTE_DIRECTORY,
				strFilePath, FILE_ATTRIBUTE_DIRECTORY);

			pPathInfo->wstrImgPath = szRelativePath;

			/////////////////////////////////////////////////////////////////

			// �̹��� ���� ����

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
	// ���� ����Ʈ�ڽ��� ����ִ� ��ü ������ ���� ��ȯ.
	for(int i = 0; i < m_PathListBox.GetCount(); ++i){
		m_PathListBox.GetText(i, strFileName);

		// GetTextExtent: ���ڷ� �Ѱܹ��� ���ڿ��� cx, cy �ȼ������ �����ִ� �Լ�.
		stringPixel = pDC->GetTextExtent(strFileName);

		if(iCX < stringPixel.cx)
			iCX = stringPixel.cx;
	}

	// ���� ����Ʈ�ڽ��� ������ �ִ� ���� ��ũ�� �ִ� ������ �� ������ Ȯ���ض�.
	if(iCX > m_PathListBox.GetHorizontalExtent())
		m_PathListBox.SetHorizontalExtent(iCX); // ���� ��ũ�� ���� ������.

	m_PathListBox.ReleaseDC(pDC);
}
