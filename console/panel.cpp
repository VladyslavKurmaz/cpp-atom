#include "./pch.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./shell.hpp"
#include "./ar.hpp"
#include "./panel.hpp"

panel::panel( logger_ptr l, pref_ptr p ) :
		wwindow( *this, INITLIST_2( &panel::onPaint, &panel::onSize ) )
	,	hListView( NULL ) {
		//
		atom::mount<panel2logger>( this, l );
		atom::mount<panel2pref>( this, p );
}

panel::~panel() {
}

bool panel::init( HWND hParent ) {
	//
	logger_ptr	l = get_slot< panel2logger >().item();
	pref_ptr	p = get_slot< panel2pref >().item(); 
	panel_ptr	w = this->shared_from_this();
	//
	struct _ {
		static bool __( WNDCLASSEX& wcex, CREATESTRUCT& cs, RECT const& rect, HWND parent ) {
			wcex.cbSize;
			wcex.style			=	/*CS_HREDRAW | CS_VREDRAW | */CS_NOCLOSE;
			wcex.lpfnWndProc;
			wcex.cbClsExtra;
			wcex.cbWndExtra;
			wcex.hInstance;
			wcex.hIcon;
			wcex.hCursor;
			wcex.hbrBackground	=	NULL;//(HBRUSH)GetStockObject( LTGRAY_BRUSH );//CreateHatchBrush( HS_DIAGCROSS, RGB( 0xFF, 0x00, 0x00 ) );
			wcex.lpszMenuName;
			wcex.lpszClassName;
			wcex.hIconSm;
			//
			cs.lpCreateParams;
			cs.hInstance;
			cs.hMenu;
			cs.hwndParent		=	parent;
			cs.cy				=	rect.bottom - rect.top;
			cs.cx				=	rect.right - rect.left;
			cs.y				=	rect.top;
			cs.x				=	rect.left;
			cs.style			=	WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_VISIBLE;
			cs.lpszName;
			cs.lpszClass;
			cs.dwExStyle		=	WS_EX_PALETTEWINDOW;
			return true;
		}
	};
	RECT rt, view;
	this->getPref()->getView( view );
	SetRect( &rt, view.left, view.top, view.left + RECT_WIDTH( view ) / 6, view.top + 2 * RECT_HEIGHT( view ) / 5 );
	OffsetRect( &rt, RECT_WIDTH( view ) - RECT_WIDTH( rt ), RECT_HEIGHT( view ) - RECT_HEIGHT( rt ) );
	if ( base_panel_t::init( boost::bind( _::__, _1, _2, boost::ref( rt ), hParent ), true ) ) {
		//
		RECT lvrt;
		this->getClientRect( lvrt );
		// Create the list-view window in report view with label editing enabled.
		hListView = CreateWindow(WC_LISTVIEW, 
			_T(""),
			WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
			0, 0,
			RECT_WIDTH( lvrt ),
			RECT_HEIGHT( lvrt ),
			this->getHWND(),
			(HMENU)10000,
			(HINSTANCE)GetModuleHandle( NULL ),
			NULL); 
		//
		LVCOLUMN lvc;
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.iSubItem	= 0;
		lvc.pszText		= _T( "Source" );
		lvc.cx = RECT_WIDTH( lvrt ) / 2;
		lvc.fmt = LVCFMT_LEFT;
		ListView_InsertColumn( hListView, 0, &lvc );

		lvc.iSubItem	= 1;
		lvc.pszText		= _T( "Result" );
		lvc.fmt = LVCFMT_LEFT;
		ListView_InsertColumn( hListView, 1, &lvc );
		//
		return true;
	}
	return false;
}

void panel::clear() {
	base_node_t::clear();
}

void panel::addRecord( atom::string_t const& s, atom::string_t const& d ) {
	//
	atom::char_t ss[4096] = { 0 };
	atom::char_t dd[4096] = { 0 };
	wcsncpy( ss, s.c_str(), 4095 );
	wcsncpy( dd, d.c_str(), 4095 );

	LVITEM lvI;
	lvI.pszText		= ss;
	lvI.mask		= LVIF_TEXT;
	lvI.stateMask	= 0;
	lvI.iItem		= ListView_GetItemCount( hListView );
	lvI.iSubItem	= 0;
	lvI.state		= 0;
	ListView_InsertItem( hListView, &lvI );
	//
	lvI.pszText		= dd;
	lvI.iSubItem	= 1;
	ListView_SetItem( hListView, &lvI );
}

void panel::onPaint( HWND hWnd ) {
	PAINTSTRUCT	ps; 
	HDC			dc = BeginPaint( hWnd, &ps );
	{
	}
	EndPaint( hWnd, &ps );
}

void  panel::onSize( HWND hWnd, UINT state, int cx, int cy ) {
	MoveWindow( hListView, 0, 0, cx, cy, TRUE );
}
