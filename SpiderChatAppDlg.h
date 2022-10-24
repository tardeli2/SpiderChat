
// SpiderChatAppDlg.h : header file
//

#pragma once
#include "CSelectSpiderChatMode.h"
#include <string>

// CSpiderChatAppDlg dialog
class CSpiderChatAppDlg : public CDialogEx
{
// Construction
public:
	CSpiderChatAppDlg(CWnd* pParent = nullptr);	// standard constructor

   void AppendLogging(std::string strMessage);
   SOCKET GetSocket() { return m_clientSock; };
   bool GetIsServerRunning() { return m_ServerIsRunning; };
   

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPIDERCHATAPP_DIALOG };
#endif

   void SetSelectionMode(const SpiderChatMode eSelectionMode) { m_SelectionMode = eSelectionMode; };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

   void HideServerControls();
   void HideClientControls();

   bool StartServer();
   void ConnectClient();

private:

   SpiderChatMode m_SelectionMode;
   SOCKET m_clientSock;
   CWinThread * clientReceivedMsgThread;
   CWinThread * serverMessageThread;
   bool m_ServerIsRunning;
   
public:
   CEdit m_EditView;
   CEdit m_EditChat;
   CButton m_btnSend;
   CStatic m_grbChatView;
   CStatic m_grbServer;
   CStatic m_grbClient;
   CEdit m_ClientIP;
   CEdit m_ClientPort;
   CEdit m_ClientUser;
   CEdit m_ServerPort;
   CButton m_lblServerPort;
   CStatic m_lblClientIP;
   CStatic m_lblClientPort;
   CStatic m_lblClientUser;
   afx_msg void OnBnClickedOk();
   afx_msg void OnBnClickedButtonSend();
};
