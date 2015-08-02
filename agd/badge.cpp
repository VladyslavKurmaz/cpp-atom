#include "./pch.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./window.hpp"
#include "./badge.hpp"

badge::badge(logger_ptr l, pref_ptr p, window_ptr w) :
		wwindow(*this, INITLIST_4(&badge::onPaint, &badge::onLButtonDown, &badge::onMouseMove, &badge::onLButtonUp))
	,	badgeImg() {
	//
	atom::mount<badge2logger>(this, l);
	atom::mount<badge2pref>(this, p);
	atom::mount<badge2window>(this, w);
}

badge::~badge() {
}



bool badge::init(HWND hParent) {
	assert(hParent);
	//
	logger_ptr  l = get_slot< badge2logger >().item();
	pref_ptr  p = get_slot< badge2pref >().item();
	badge_ptr w = this->shared_from_this();
	//
	this->badgeImg.loadBitmap((HINSTANCE)GetModuleHandle(NULL), IDR_IMG_BADGE);
	//
	struct _ {
		static bool __(WNDCLASSEX& wcex, CREATESTRUCT& cs, RECT const& rect, HWND parent) {
			wcex.cbSize;
			wcex.style = /*CS_HREDRAW | CS_VREDRAW | */CS_NOCLOSE;
			wcex.lpfnWndProc;
			wcex.cbClsExtra;
			wcex.cbWndExtra;
			wcex.hInstance;
			wcex.hIcon;
			wcex.hCursor;
			wcex.hbrBackground = NULL;//(HBRUSH)GetStockObject( LTGRAY_BRUSH );//CreateHatchBrush( HS_DIAGCROSS, RGB( 0xFF, 0x00, 0x00 ) );
			wcex.lpszMenuName;
			wcex.lpszClassName;
			wcex.hIconSm;
			//
			cs.lpCreateParams;
			cs.hInstance;
			cs.hMenu;
			cs.hwndParent = parent;
			cs.cy = rect.bottom - rect.top;
			cs.cx = rect.right - rect.left;
			cs.y = rect.top;
			cs.x = rect.left;
			cs.style = WS_OVERLAPPED | WS_VISIBLE;
			cs.lpszName;
			cs.lpszClass;
			cs.dwExStyle = WS_EX_PALETTEWINDOW | WS_EX_TOPMOST;
			return true;
		}
	};
	RECT rt, view;
	this->getPref()->getView(this->getPref()->get< bool >(CONFIG_CLIP), view);
	SetRect(&rt, view.left, view.top, 48, view.top + 48);
	OffsetRect(&rt, RECT_WIDTH(view) - RECT_WIDTH(rt), 0);
	if (base_badge_t::init(boost::bind(_::__, _1, _2, boost::ref(rt), hParent), true)) {
		this->setStyles(WS_OVERLAPPED, WS_EX_PALETTEWINDOW | WS_EX_TOPMOST);
		return true;
	}
	return false;
}

void badge::clear() {
	base_node_t::clear();
}

void badge::onPaint(HWND hWnd) {
	PAINTSTRUCT ps;
	HDC     dc = BeginPaint(hWnd, &ps);
	{
		BitBlt(dc, 0, 0, 48, 48, badgeImg.dc, 0, 0, SRCCOPY);
	}
	EndPaint(hWnd, &ps);
}


RECT startRect;
POINT startPoint;
void badge::onLButtonDown(HWND hWnd, BOOL dblClickint, int x, int y, UINT state){
	GetWindowRect(this->getHWND(), &startRect);
	startPoint.x = x;
	startPoint.y = y;
	MapWindowPoints(this->getHWND(), NULL, &startPoint, 1);
	this->inputCapture(mouse);
}

void badge::onMouseMove(HWND hWnd, int x, int y, UINT state){
	if (this->inputIsCaptured(mouse)){
		POINT cPoint;
		cPoint.x = x;
		cPoint.y = y;
		MapWindowPoints(this->getHWND(), NULL, &cPoint, 1);
		//
		MoveWindow(this->getHWND(), startRect.left + (cPoint.x - startPoint.x), startRect.top + (cPoint.y - startPoint.y), RECT_WIDTH(startRect), RECT_HEIGHT(startRect), TRUE);
	}
}

void badge::onLButtonUp(HWND hWnd, int x, int y, UINT state){
	if (this->inputIsCaptured(mouse)){
		this->inputRelease(mouse);
		this->invalidate();
	}
	POINT cPoint;
	cPoint.x = x;
	cPoint.y = y;
	MapWindowPoints(this->getHWND(), NULL, &cPoint, 1);
	if ((cPoint.x == startPoint.x) && (cPoint.y == startPoint.y)){
		this->getWindow()->toggleVisibility();
	}

}
