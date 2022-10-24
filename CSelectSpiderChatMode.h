#pragma once


// CSelectSpiderChatMode dialog

enum SpiderChatMode
{
   eModeInvalid = 0,
   eModeServer = 1,
   eModeClient = 2
};

class CSelectSpiderChatMode : public CDialog
{
	DECLARE_DYNAMIC(CSelectSpiderChatMode)

public:
	CSelectSpiderChatMode(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSelectSpiderChatMode();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROMPT };
#endif

   virtual BOOL OnInitDialog();

   const SpiderChatMode GetSelectedMode();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
   int m_radioButtonModeSelection;
public:
   afx_msg void OnBnClickedRadioServer();
   afx_msg void OnBnClickedRadioClient();
};
