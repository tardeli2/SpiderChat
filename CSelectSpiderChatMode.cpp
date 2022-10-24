// CSelectSpiderChatMode.cpp : implementation file
//

#include "pch.h"
#include "SpiderChatApp.h"
#include "CSelectSpiderChatMode.h"
#include "afxdialogex.h"


// CSelectSpiderChatMode dialog

IMPLEMENT_DYNAMIC(CSelectSpiderChatMode, CDialog)

CSelectSpiderChatMode::CSelectSpiderChatMode(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_PROMPT, pParent)
{
   m_radioButtonModeSelection = eModeInvalid;
}

CSelectSpiderChatMode::~CSelectSpiderChatMode()
{
}

void CSelectSpiderChatMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelectSpiderChatMode, CDialog)
   ON_BN_CLICKED(IDC_RADIO_SERVER, &CSelectSpiderChatMode::OnBnClickedRadioServer)
   ON_BN_CLICKED(IDC_RADIO_CLIENT, &CSelectSpiderChatMode::OnBnClickedRadioClient)
END_MESSAGE_MAP()

BOOL CSelectSpiderChatMode::OnInitDialog()
{
   CDialog::OnInitDialog();

   CButton* pRadioButton = (CButton*)GetDlgItem(IDC_RADIO_SERVER);
   pRadioButton->SetCheck(BST_CHECKED);
   m_radioButtonModeSelection = static_cast<int>(eModeServer);

   return TRUE;
}

const SpiderChatMode CSelectSpiderChatMode::GetSelectedMode()
{
   return static_cast<SpiderChatMode>(m_radioButtonModeSelection);
}
// CSelectSpiderChatMode message handlers


void CSelectSpiderChatMode::OnBnClickedRadioServer()
{
   m_radioButtonModeSelection = static_cast<int>(eModeServer);
   // TODO: Add your control notification handler code here
}


void CSelectSpiderChatMode::OnBnClickedRadioClient()
{
   m_radioButtonModeSelection = static_cast<int>(eModeClient);
   // TODO: Add your control notification handler code here
}
