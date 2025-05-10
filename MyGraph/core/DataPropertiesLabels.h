#pragma once


class CGraphCtrl;
class CGraphView;

// CDataPropertiesLabels dialog

class CDataPropertiesLabels : public CPropertyPage
{
	DECLARE_DYNAMIC(CDataPropertiesLabels)

public:
	CDataPropertiesLabels();
	virtual ~CDataPropertiesLabels();

// Dialog Data
	enum { IDD = IDD_LABELS_PROPERTIES };

	CGraphCtrl* m_pGraphCtrl;
	CGraphView* m_pView;

	BOOL m_XLabels;
	BOOL m_YLabels;
	int m_XLabelAngle;
	int m_YLabelAngle;
	FONT m_XFont;
	FONT m_YFont;
	int m_XLabelFormat;
	int m_YLabelFormat;
	int m_XLabelPrecision;
	int m_YLabelPrecision;

	afx_msg void OnEnChange();
	afx_msg void OnBnClickedChange();
	afx_msg void OnBnClickedXFont();
	afx_msg void OnBnClickedYFont();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void UpdateControlsState();
	BOOL Validate(void);
	void FillDataFormatList(CComboBox* pCombo);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	afx_msg void OnCbnSelendokLabelFormat();
};
