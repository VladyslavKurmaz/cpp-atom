#include "./pch.hpp"
#include "./resource.h"
#include "./pref.hpp"
#include "./help.hpp"



DWORD const slidingTime = 500;
int const dialogYOffset = 52;

HWND hTopics = NULL;
UINT_PTR const timerId = 1;
UINT timeout = 1;
size_t currentPage = 0;
size_t prevPage = 0;
DWORD startTime = 0;
bool sliding = false;
bool showHelpDialogOnStartup = false;

int const pages[] = {
	0,
	-562,
	-562*2
};
size_t const pagesCount = sizeof(pages) / sizeof(pages[0]);

struct externalLink_t {
	int id;
	TCHAR* link;
} const externalLinks[] = {
	{ IDC_STATIC_FACEBOOK, _T("https://www.facebook.com/pages/Augmented-Desktop/943800872330049") },
	{ IDC_STATIC_WWW, _T("http://augmenteddesktop.com/") },
	{ IDC_STATIC_EMAIL, _T("mailto:vladislav.kurmaz@gmail.com") }
};

INT_PTR CALLBACK helpDlgTopicsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	return FALSE;
}

WNDPROC oldStaticWindowProc = NULL;
LRESULT CALLBACK staticWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){
	switch (uMsg){
	case WM_SETCURSOR:
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		return TRUE;
	}
	return CallWindowProc(oldStaticWindowProc, hwnd, uMsg, wParam, lParam);
}
BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam){
	if (showHelpDialogOnStartup){
		CheckDlgButton(hwnd, IDC_CHECK_SHOW_HELP_ONSTARTUP, BST_CHECKED);
	}
	//
	BOOST_FOREACH(externalLink_t const& l, externalLinks){
		oldStaticWindowProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(GetDlgItem(hwnd, l.id), GWLP_WNDPROC, reinterpret_cast<LONG>(staticWindowProc)));
	}
	//
	hTopics = CreateDialog(reinterpret_cast<HINSTANCE>(GetModuleHandle(NULL)), MAKEINTRESOURCE(IDD_DIALOG_HELP_TOPICS), hwnd, helpDlgTopicsProc);
	RECT rt;
	GetWindowRect(hTopics, &rt);
	OffsetRect(&rt, -rt.left, -rt.top);
	SetWindowPos(hTopics, HWND_TOP, rt.left, rt.top + dialogYOffset, RECT_WIDTH(rt), RECT_HEIGHT(rt), SWP_SHOWWINDOW);
	//
	currentPage = 0;
	UINT_PTR id = SetTimer(hwnd, timerId, timeout, NULL);
	return TRUE;
}

void Cls_OnEndDialog(HWND hwnd){
	showHelpDialogOnStartup = (IsDlgButtonChecked(hwnd, IDC_CHECK_SHOW_HELP_ONSTARTUP) == TRUE);
	KillTimer(hwnd, timerId);
	EndDialog(hwnd, FALSE);
}

void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {
	switch (id){
	case IDC_BUTTON_GOTIT:
		Cls_OnEndDialog(hwnd);
		break;
	case IDC_BUTTON_NEXT:
		prevPage = currentPage;
		currentPage++;
		if (currentPage >= pagesCount){
			currentPage = 0;
		}
		startTime = timeGetTime();
		sliding = true;
		break;
	case IDC_STATIC_FACEBOOK:
	case IDC_STATIC_WWW:
	case IDC_STATIC_EMAIL:
		if (codeNotify == STN_CLICKED){
			BOOST_FOREACH(externalLink_t const& l, externalLinks){
				if (l.id == id){
					ShellExecute(NULL, _T("open"), l.link, NULL, NULL, SW_NORMAL);
				}
			}
		}
		break;
	}
}

void Cls_OnTimer(HWND hwnd, UINT id){
	if ((id == timerId) && sliding){
		int pos = pages[currentPage];
		DWORD dt = timeGetTime() - startTime;
		if (dt < slidingTime){
			pos = pages[prevPage] + (int)((float)(pages[currentPage] - pages[prevPage]) * (float)(dt) / (float)(slidingTime));
			//
		}else{
			sliding = false;
		}
		RECT rt;
		GetWindowRect(hTopics, &rt);
		MapWindowPoints(NULL, hwnd, (LPPOINT)&rt, 2);
		OffsetRect(&rt, pos - rt.left, 0);
		MoveWindow(hTopics, rt.left, rt.top, RECT_WIDTH(rt), RECT_HEIGHT(rt), TRUE);
	}
}

INT_PTR CALLBACK helpDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg){
		HANDLE_MSG(hwndDlg, WM_INITDIALOG, Cls_OnInitDialog);
		HANDLE_MSG(hwndDlg, WM_COMMAND, Cls_OnCommand);
		HANDLE_MSG(hwndDlg, WM_TIMER, Cls_OnTimer);
	case WM_NOTIFY:
		Sleep(0);
		break;
	}
	return FALSE;
}

void showHelpDialog(HINSTANCE hInst, HWND hParent, pref_ptr p){
	showHelpDialogOnStartup = p->get< bool >(CONFIG_SHOW_HELP_ON_STARTUP);
	DialogBox(hInst, MAKEINTRESOURCE(IDD_HELP), hParent, helpDlgProc);
	//?????? back showHelpDialogOnStartup to pref
}
