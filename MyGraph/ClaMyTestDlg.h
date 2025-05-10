#pragma once
#include "afxdialogex.h"


// ClaMyTestDlg dialog

class ClaMyTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ClaMyTestDlg)

public:
	ClaMyTestDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ClaMyTestDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
