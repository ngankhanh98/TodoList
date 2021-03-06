// TodoList.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TodoList.h"
#include <windowsx.h>
#include <CommCtrl.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND txtTask;
HWND lsbTasks;
HWND btnUpdate;
HWND btnDeleteAll;
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL				OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void				OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void				OnDestroy(HWND hwnd);
void				OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TODOLIST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TODOLIST));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TODOLIST));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TODOLIST);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 600, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_CONTEXTMENU, OnContextMenu);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	HWND lbl = CreateWindowEx(NULL, L"BUTTON", L"Tên công việc:",
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		10, 30, 365, 100, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lbl, hFont, TRUE);

	txtTask = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL,
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		20, 58, 345, 25, hwnd,
		(HMENU)IDC_TXTTASK, lpCreateStruct->hInstance, NULL);
	SetWindowFont(txtTask, hFont, TRUE);

	lsbTasks = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTBOX, NULL, WS_CHILD | WS_VISIBLE | LBS_MULTIPLESEL | LBS_NOTIFY,
		20, 150, 345, 380, hwnd,
		(HMENU)IDC_LSB_TASKS, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lsbTasks, hFont, TRUE);

	HWND btnAdd = CreateWindowEx(NULL, L"BUTTON", L"Thêm",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 75, 30, hwnd,
		(HMENU)IDC_BTNADD, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnAdd, hFont, TRUE);

	lbl = CreateWindowEx(NULL, L"BUTTON", L"Danh sách công việc:",
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		10, 130, 365, 400, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lbl, hFont, TRUE);

	btnUpdate = CreateWindowEx(NULL, L"BUTTON", L"Cập nhật",
		WS_CHILD | BS_PUSHBUTTON,
		0, 0, 75, 30, hwnd,
		(HMENU)IDC_BTN_UPDATE, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnUpdate, hFont, TRUE);

	btnDeleteAll = CreateWindowEx(NULL, L"BUTTON", L"Xóa toàn bộ",
		WS_CHILD | BS_PUSHBUTTON,
		0, 0, 75, 30, hwnd,
		(HMENU)IDC_BTN_DELALL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnDeleteAll, hFont, TRUE);

	// chỉnh vị trí 3 button
	MoveWindow(btnAdd, 35, 90, 75, 30, 0);
	MoveWindow(btnUpdate, 145, 90, 75, 30, 0);
	MoveWindow(btnDeleteAll, 255, 90, 100, 30, 0);

	return TRUE;
}

const int BUFFER_SIZE = 255;
WCHAR buffer[BUFFER_SIZE];
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	// thêm công việc
	if (IDC_BTNADD == id) {
		GetWindowText(txtTask, buffer, BUFFER_SIZE);
		ListBox_AddString(lsbTasks, buffer);

		if(*buffer) 
			ShowWindow(btnDeleteAll, SW_SHOW);	// khi có công việc được thêm vào thì btn "Xóa toàn bộ" mới hiện

		Edit_SetText(txtTask, L"");
	}
	// xóa nhiều công việc
	else if (IDC_DELETE == id) {
		int count = ListBox_GetSelCount(lsbTasks);  
		int* indices = new int[count];				
		ListBox_GetSelItems(lsbTasks, count, indices);
		for (auto i = 0; i < count; i++) { 
			ListBox_DeleteString(lsbTasks, indices[i]-i); // code mẫu của thầy: ListBox_DeleteString(lsbTasks, indices[i]) --> lỗi vì mỗi lần xóa thì list sẽ dồn lên, xóa tiếp là xóa bậy. Code sửa: tham khảo của Võ Đăng Khoa
		}
		delete[] indices; 
	}
	// hiện công việc được chọn trong list lên "Tên công việc"
	else if (IDC_LSB_TASKS == id) {
		switch (codeNotify) {
		case LBN_SELCHANGE:
			int selectedIndex = ListBox_GetCurSel(lsbTasks);  // Lấy chỉ mục của item được click
			ListBox_GetText(lsbTasks, selectedIndex, buffer); // Lấy text
			Edit_SetText(txtTask, buffer);
			ShowWindow(btnUpdate, SW_SHOW);
			break;
		}
	}
	// update công việc vào list
	else if (IDC_BTN_UPDATE == id) {
		int selectedIndex = ListBox_GetCurSel(lsbTasks);  // Lấy chỉ mục của item được click
		Edit_GetText(txtTask, buffer, BUFFER_SIZE);

		
		ListBox_DeleteString(lsbTasks, selectedIndex); // xóa đi dòng bị thay đổi
		ListBox_InsertString(lsbTasks, selectedIndex, buffer); // chèn ngược lại		
		ShowWindow(btnUpdate, SW_HIDE); // Cập nhật xong rồi thì ẩn nút Cập nhật nào
		Edit_SetText(txtTask, L"");
	}
	// xóa toàn bộ
	else if (IDC_BTN_DELALL == id) {
		ListBox_ResetContent(lsbTasks);
		ShowWindow(btnDeleteAll, SW_HIDE);	// xóa xong > không còn công việc nào > btn "Xóa toàn bộ" + "Cập nhật" ẩn
		ShowWindow(btnUpdate, SW_HIDE);

	}
}

void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}

void OnContextMenu(HWND hwnd, HWND hwndContext, UINT xPos, UINT yPos)
{
	if (hwndContext == lsbTasks) { // Nếu bấm chuột phải vào ListBox
		HMENU popup = CreatePopupMenu();
		InsertMenu(popup, 0, MF_BYCOMMAND | MF_STRING | MF_ENABLED, IDC_DELETE, L"Xóa");
		// Hiển thị menu vừa tạo ra
		TrackPopupMenu(popup, TPM_TOPALIGN | TPM_LEFTALIGN, xPos, yPos, 0, hwnd, NULL);
	}
}
