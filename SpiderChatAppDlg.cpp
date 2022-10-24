
// SpiderChatAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SpiderChatApp.h"
#include "SpiderChatAppDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <fstream>
#include <locale.h>
#include <codecvt>

#pragma comment (lib, "ws2_32.lib")

using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

static CSpiderChatAppDlg * pSpiderChatDlg;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CSpiderChatAppDlg dialog

CSpiderChatAppDlg::CSpiderChatAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SPIDERCHATAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
   pSpiderChatDlg = this;
   m_ServerIsRunning = false;
}

void CSpiderChatAppDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialogEx::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_EDIT_VIEW, m_EditView);
   DDX_Control(pDX, IDC_EDIT_CHAT_EDIT, m_EditChat);
   DDX_Control(pDX, IDC_BUTTON_SEND, m_btnSend);
   DDX_Control(pDX, IDC_STATIC_VIEW, m_grbChatView);
   DDX_Control(pDX, IDC_STATIC_SERVER, m_grbServer);
   DDX_Control(pDX, IDC_STATIC_CLIENT_OPTIONs, m_grbClient);
   DDX_Control(pDX, IDC_EDIT_CLIENT_IP, m_ClientIP);
   DDX_Control(pDX, IDC_EDIT_CLIENT_PORT, m_ClientPort);
   DDX_Control(pDX, IDC_EDIT_CLIENT_USER, m_ClientUser);
   DDX_Control(pDX, IDC_EDIT_SERVER_PORT, m_ServerPort);
   DDX_Control(pDX, IDC_STATIC_CLIENT_IP, m_lblClientIP);
   DDX_Control(pDX, IDC_STATIC_CLIENT_PORT, m_lblClientPort);
   DDX_Control(pDX, IDC_STATIC_CLIENT_USER, m_lblClientUser);
   DDX_Control(pDX, IDC_STATIC_SERVER_PORT, m_lblServerPort);
}

BEGIN_MESSAGE_MAP(CSpiderChatAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
   ON_BN_CLICKED(IDOK, &CSpiderChatAppDlg::OnBnClickedOk)
   ON_BN_CLICKED(IDC_BUTTON_SEND, &CSpiderChatAppDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CSpiderChatAppDlg message handlers

BOOL CSpiderChatAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
   if (m_SelectionMode == eModeClient)
   {
      HideServerControls();
      m_ClientUser.SetLimitText(10);
   }
   else if (m_SelectionMode == eModeServer)
   {
      HideClientControls();
   }

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSpiderChatAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSpiderChatAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSpiderChatAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSpiderChatAppDlg::HideServerControls()
{
   m_grbServer.ShowWindow(SW_HIDE);
   m_ServerPort.ShowWindow(SW_HIDE);
   m_lblServerPort.ShowWindow(SW_HIDE);

   GetDlgItem(IDOK)->SetWindowTextA("Connect");
}

void CSpiderChatAppDlg::HideClientControls()
{
   m_btnSend.ShowWindow(SW_HIDE);
   m_grbChatView.ShowWindow(SW_HIDE);
   m_EditView.ShowWindow(SW_HIDE);
   m_grbClient.ShowWindow(SW_HIDE);
   m_ClientIP.ShowWindow(SW_HIDE);
   m_ClientPort.ShowWindow(SW_HIDE);
   m_ClientUser.ShowWindow(SW_HIDE);
   m_lblClientIP.ShowWindow(SW_HIDE);
   m_lblClientPort.ShowWindow(SW_HIDE);
   m_lblClientUser.ShowWindow(SW_HIDE);
   m_EditChat.ShowWindow(SW_HIDE);

   GetDlgItem(IDOK)->SetWindowTextA("Listen");
}

UINT ReceiveMessageThread(LPVOID param)
{
   SOCKET* socket = (SOCKET*)param;

   for (;;)
   {
      char buf[4096];
      // Wait for response
      ZeroMemory(buf, 4096);
      int bytesReceived = recv(*socket, buf, 4096, 0);
      if (bytesReceived > 0)
      {
         WPARAM param;
         string strMessage(buf, 0, bytesReceived);
         if (strMessage.find('$') != std::string::npos)
         {
            strMessage = "USER#" + strMessage.substr(0, strMessage.find('$')) + strMessage.substr(strMessage.find('$')+1, strMessage.length());
         }

         pSpiderChatDlg->AppendLogging(strMessage);
      }
   }

   return 0;
}

UINT ServerListeningForMessages(LPVOID socketParam)
{
   
   SOCKET* listening = (SOCKET*)socketParam;

   // Create the master file descriptor set and zero it
   fd_set master;
   FD_ZERO(&master);
   
   FD_SET(*listening, &master);
   
   bool running = true;
   while (running)
   {
      fd_set copy = master;

      // See who's talking to us
      int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

      // Loop through all the current connections / potential connect
      for (int i = 0; i < socketCount; i++)
      {
         // Makes things easy for us doing this assignment
         SOCKET sock = copy.fd_array[i];

         // Is it an inbound communication?
         if (sock == *listening)
         {
            // Accept a new connection
            SOCKET client = accept(*listening, nullptr, nullptr);

            // Add the new connection to the list of connected clients
            FD_SET(client, &master);

            // Send a welcome message to the connected client
            string welcomeMsg = "Welcome to the Awesome Spider Chat!\r\n";
            send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
         }
         else // It's an inbound message
         {
            char buf[4096];
            ZeroMemory(buf, 4096);

            // Receive message
            int bytesIn = recv(sock, buf, 4096, 0);
            if (bytesIn <= 0)
            {
               // Drop the client
               closesocket(sock);
               FD_CLR(sock, &master);
            }
            else
            {
               // Check to see if we have to shot down the server
               if (pSpiderChatDlg->GetIsServerRunning() == false)
               {
                  running = false;
                  break;
               }
               
               std::wofstream StorageFile;
               try
               {
                  StorageFile.open("Storage.txt", std::ios::out | std::ios::in | std::ios::app | std::ios::binary | std::ios::ate);
               }
               catch (std::ios_base::failure  ex)
               {
                  pSpiderChatDlg->AppendLogging("Error opening Receipt.json file!");
               }

               if (StorageFile.is_open())
               {
                  StorageFile << buf << "\n";
                  StorageFile.close();
               }

               // Send message to other clients, and definiately NOT the listening socket
               string strReceivedMsg = buf;
               string strUser, strMessage;
               if (strReceivedMsg.find('$') != std::string::npos)
               {
                  strUser = strReceivedMsg.substr(0, strReceivedMsg.find('$'));
                  strMessage = strReceivedMsg.substr(strReceivedMsg.find('$') + 1, strReceivedMsg.length());
               }

               for (int i = 0; i < master.fd_count; i++)
               {
                  SOCKET outSock = master.fd_array[i];
                  if (outSock != *listening && outSock != sock)
                  {
                     ostringstream ss;
                     if (strUser.length() > 0)
                     {
                        ss << "USER #" << strUser << ": " << strMessage << "\r\n";
                     }
                     else
                     {
                        ss << "SOCKET #" << sock << ": " << buf << "\r\n";
                     }
                     string strOut = ss.str();

                     send(outSock, strOut.c_str(), strOut.size() + 1, 0);
                  }
               }
            }
         }
      }
   }

   // Remove the listening socket from the master file descriptor set and close it
   // to prevent anyone else trying to connect.
   FD_CLR(*listening, &master);
   closesocket(*listening);

   // Message to let users know what's happening.
   string msg = "Server is shutting down. Goodbye\r\n";

   while (master.fd_count > 0)
   {
      // Get the socket number
      SOCKET sock = master.fd_array[0];

      // Send the goodbye message
      send(sock, msg.c_str(), msg.size() + 1, 0);

      // Remove it from the master file list and close the socket
      FD_CLR(sock, &master);
      closesocket(sock);
   }

   WSACleanup();

   return 0;
}

void CSpiderChatAppDlg::OnBnClickedOk()
{
   if (m_SelectionMode == eModeServer)
   {
      if (!m_ServerIsRunning)
      {
         if (StartServer())
         {
            m_ServerIsRunning = true;
            GetDlgItem(IDOK)->SetWindowTextA("Stop");
         }
      }
      else
      {
         m_ServerIsRunning = false;
         GetDlgItem(IDOK)->SetWindowTextA("Listen");
         if (serverMessageThread)
         {
            WaitForSingleObject(serverMessageThread->m_hThread, INFINITE);
         }
      }
   }
   else if (m_SelectionMode == eModeClient)
   {
      if (!m_clientSock)
      {
         ConnectClient();
         if (GetSocket() != SOCKET_ERROR)
         {
            GetDlgItem(IDOK)->SetWindowTextA("Disconnect");
            int* socket = new int;
            *socket = GetSocket();
            clientReceivedMsgThread = AfxBeginThread(ReceiveMessageThread, socket);
         }
      }
      else
      {
         // Gracefully close down everything
         if (clientReceivedMsgThread)
         {
            //WaitForSingleObject(clientReceivedMsgThread->m_hThread, INFINITE);
            delete clientReceivedMsgThread;
         }
         GetDlgItem(IDOK)->SetWindowTextA("Connect");
         closesocket(m_clientSock);
         WSACleanup();
         m_clientSock = 0;
         AppendLogging("Disconnected from spider chat...");
      }
   }
}

void CSpiderChatAppDlg::OnBnClickedButtonSend()
{
   // Do-while loop to send and receive data
   char buf[4096];
   CString userInput;
   CString user;
   std::string receiveMessage;
   m_EditChat.GetWindowTextA(userInput);
   m_ClientUser.GetWindowTextA(user);
   m_EditChat.SetWindowTextA("");

   AppendLogging(string(userInput));

   if (userInput.GetLength() > 0)		// Make sure the user has typed in something
   {
      if (user.GetLength() > 0)
      {
         userInput = user + "$" + userInput;
      }
      // Send the text
      int sendResult = send(m_clientSock, userInput, userInput.GetLength() + 1, 0);
      userInput = "";
   }
}

bool CSpiderChatAppDlg::StartServer()
{
   //check if port is entered
   CString strServerPort;
   m_ServerPort.GetWindowTextA(strServerPort);

   if (atoi(strServerPort) == 0)
   {
      AfxMessageBox("Please enter first port number\n");
      return false;
   }
   // Initialze winsock
   WSADATA wsData;
   WORD ver = MAKEWORD(2, 2);

   int wsOk = WSAStartup(ver, &wsData);
   if (wsOk != 0)
   {
      AfxMessageBox("Can't Initialize winsock! Quitting");
      return false;
   }

   // Create a socket
   SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
   if (listening == INVALID_SOCKET)
   {
      AfxMessageBox("Can't create a socket! Quitting");
      return false;
   }

   // Bind the ip address and port to a socket
   sockaddr_in hint;
   hint.sin_family = AF_INET;
   hint.sin_port = htons(atoi(strServerPort));
   hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

   bind(listening, (sockaddr*)&hint, sizeof(hint));

   // Tell Winsock the socket is for listening 
   listen(listening, SOMAXCONN);

   int* socket = new int;
   *socket = listening;
   serverMessageThread = AfxBeginThread(ServerListeningForMessages, socket);
   this->ShowWindow(SW_MINIMIZE);
   return true;
}

void CSpiderChatAppDlg::ConnectClient()
{
   string ipAddress;    // IP Address of the server
   int port;				// Listening port # on the server

   CString strconnectionParam;
   m_ClientIP.GetWindowTextA(strconnectionParam);
   ipAddress = strconnectionParam;
   m_ClientPort.GetWindowTextA(strconnectionParam);
   port = atoi(strconnectionParam);

   // Initialize WinSock
   WSAData data;
   WORD ver = MAKEWORD(2, 2);
   int wsResult = WSAStartup(ver, &data);
   if (wsResult != 0)
   {
      AppendLogging("Can't start Winsock");
      return;
   }

   // Create socket
   m_clientSock = socket(AF_INET, SOCK_STREAM, 0);
   if (m_clientSock == INVALID_SOCKET)
   {
      AppendLogging("Can't create socket");
      WSACleanup();
      return;
   }

   // Fill in a hint structure
   sockaddr_in hint;
   hint.sin_family = AF_INET;
   hint.sin_port = htons(port);
   inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

   // Connect to server
   int connResult = connect(m_clientSock, (sockaddr*)&hint, sizeof(hint));
   if (connResult == SOCKET_ERROR)
   {
      AppendLogging("Can't connect to server");
      closesocket(m_clientSock);
      WSACleanup();
      return;
   }

   AppendLogging("Connected to spider chat...");
}

void CSpiderChatAppDlg::AppendLogging(std::string strMessage)
{
   std::string strDisplayText;
   
   char message[100];
   if (!strMessage.empty())
   {
      strDisplayText = strMessage + "\r\n";
      // get the initial text length
      int nLength = m_EditView.GetWindowTextLength();
      // put the selection at the end of text
      m_EditView.SetSel(nLength, nLength);
      // replace the selection
      m_EditView.ReplaceSel(strDisplayText.c_str());
      //place the horizontal scrollbar at initial position after replacing
      m_EditView.SetScrollPos(0, 0);
   }
}
