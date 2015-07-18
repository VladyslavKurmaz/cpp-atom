#include "./pch.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./shell.hpp"
#include "./ad.hpp"
#include "./panel.hpp"

panel::panel( logger_ptr l, pref_ptr p ) :
		wwindow( *this, INITLIST_4( &panel::onPaint, &panel::onSize, &panel::onNotify, &panel::onCommand ) )
	,	imageList()
	,	listView()
	,	toolbar() {
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
	SetRect( &rt, view.left, view.top, view.left + RECT_WIDTH( view ) / 5, view.top + 2 * RECT_HEIGHT( view ) / 5 );
	OffsetRect( &rt, RECT_WIDTH( view ) - RECT_WIDTH( rt ), RECT_HEIGHT( view ) - RECT_HEIGHT( rt ) );
	if ( base_panel_t::init( boost::bind( _::__, _1, _2, boost::ref( rt ), hParent ), true ) ) {
		//
		HINSTANCE hInst = (HINSTANCE)GetModuleHandle( NULL );
		RECT lvrt;
		this->getClientRect( lvrt );
		//
		//
		this->imageList.create( (HINSTANCE)GetModuleHandle( NULL ), MAKEINTRESOURCE( IDR_IMAGES ), 24, 0, RGB( 255, 255, 255 ) ); 
		//
		// Create the list-view window in report view with label editing enabled.
		this->listView.create( 0, WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS, lvrt, AD_PANEL_LISTVIEW, *this ).
			updateColumn( true, 0, LVCFMT_LEFT, ( RECT_WIDTH( lvrt ) ) / 2, _T( "Source" ), 0 ).
			updateColumn( true, 1, LVCFMT_LEFT, ( RECT_WIDTH( lvrt ) ) / 2, _T( "Translation" ), 1 ).
			show();
		//
		// create toolbar
		pref::langspair_t const langs = this->getPref()->langGetPair();
		RECT tbrt;
		SetRect( &tbrt, 0, 0, 0, 32 );
		this->toolbar.
			create( 0, WS_CHILD | CCS_NORESIZE | TBSTYLE_WRAPABLE | TBSTYLE_FLAT, tbrt, AD_PANEL_TOOLBAR, this->imageList, *this ).
			setButtonSize( 24, 24 ).
			addButton(-1, 0, TBSTATE_ENABLED | TBSTATE_WRAP, BTNS_SEP | BTNS_AUTOSIZE, NULL, atom::string_t()).
			addButton(AD_PANEL_IMAGE_PIN, AD_PANEL_TB_PIN, /*TBSTATE_ENABLED | */TBSTATE_WRAP, BTNS_CHECK | BTNS_AUTOSIZE, NULL, atom::string_t()).
			addButton( -1, 0, TBSTATE_ENABLED | TBSTATE_WRAP, BTNS_SEP | BTNS_AUTOSIZE, NULL, atom::string_t() ).
			addButton( langs.first.get<3>(), AD_PANEL_TB_LANG_FROM, TBSTATE_ENABLED | TBSTATE_WRAP, BTNS_BUTTON | TBSTYLE_AUTOSIZE, NULL, atom::string_t() ).
			addButton( AD_PANEL_IMAGE_SWAP, AD_PANEL_TB_LANG_SWAP, TBSTATE_ENABLED | TBSTATE_WRAP, BTNS_BUTTON | BTNS_AUTOSIZE, NULL, atom::string_t() ).
			addButton( langs.second.get<3>(), AD_PANEL_TB_LANG_TO, TBSTATE_ENABLED | TBSTATE_WRAP, BTNS_BUTTON | TBSTYLE_AUTOSIZE, NULL, atom::string_t() ).
			addButton( -1, 0, TBSTATE_ENABLED | TBSTATE_WRAP, BTNS_SEP | BTNS_AUTOSIZE, NULL, atom::string_t() ).
			addButton( AD_PANEL_IMAGE_DELETE, AD_PANEL_TB_DELETE, TBSTATE_ENABLED | TBSTATE_WRAP, BTNS_BUTTON | BTNS_AUTOSIZE, NULL, atom::string_t() ).
			build().
			show();
		//
		this->onSize( this->getHWND(), 0, lvrt.right, lvrt.bottom );
		//
		return true;
	}
	return false;
}

void panel::clear() {
	base_node_t::clear();
}

void panel::addRecord( atom::string_t const& s, atom::string_t const& d ) {
	int const icnt = 0;//this->listView.getItemCount();
	this->listView.insertItem( icnt, 0, s );
	this->listView.setItem( icnt, 1, d );
}

void panel::onPaint( HWND hWnd ) {
	PAINTSTRUCT	ps; 
	HDC			dc = BeginPaint( hWnd, &ps );
	{
	}
	EndPaint( hWnd, &ps );
}

void panel::onSize( HWND hWnd, UINT state, int cx, int cy ) {
	RECT trt;
	GetWindowRect( this->toolbar.getHWND(), &trt );
	//
	int const tbcx = RECT_WIDTH( trt );
	int const tbcy = RECT_HEIGHT( trt );

	MoveWindow( this->toolbar.getHWND(), 0, 0, cx, tbcy, TRUE );
	MoveWindow( this->listView.getHWND(), 0, tbcy, cx, cy - tbcy, TRUE );
	//
	int const cw = ( tbcx / 2 ) - 1;
	this->listView.
		updateColumn( false, 0, 0, cw, _T(""), -1 ).
		updateColumn( false, 1, 0, cw, _T(""), -1 );
}

void panel::onNotify( int id, LPNMHDR lpnm ) {
	//switch( lpnm->code )
	//{
	//case NM_CLICK:
	//	{
	//		LPNMMOUSE lpnmBtn = reinterpret_cast<LPNMMOUSE>(lpnm);
	//		// toolbar click
	//		if ( lpnm->hwndFrom == this->toolbar.getHWND() ) {
	//			switch( lpnmBtn->dwItemSpec ) {
	//			case AD_PANEL_TB_PIN:
	//				{
	//					break;
	//				}
	//			case AD_PANEL_TB_LANG_SWAP:
	//				{
	//					//pref::langspair_t langs = this->getPref()->langGetPair();
	//					//std::swap( langs.first, langs.second );
	//					//this->getPref()->langSetPair( langs );
	//					break;
	//				}
	//			case AD_PANEL_TB_DELETE:
	//				{
	//					break;
	//				}
	//			}
	//		}
	//		break;
	//	}
	//case TBN_DROPDOWN:
	//	{
	//		LPNMTOOLBAR lpnmTB = reinterpret_cast<LPNMTOOLBAR>(lpnm);
	//		// toolbar dropdown
	//		if ( lpnm->hwndFrom == this->toolbar.getHWND() ) {
	//			this->popupLangMenu( lpnmTB->iItem );
	//		}
	//		break;
	//	}
	//}
}
	
void  panel::onCommand( int id, HWND hwndCtl, UINT codeNotify ) {
	switch( id ) {
	case AD_PANEL_TB_PIN:
		{
			break;
		}
	case AD_PANEL_TB_LANG_FROM:
		{
			this->popupLangMenu( id );
			break;
		}
	case AD_PANEL_TB_LANG_SWAP:
		{
			pref::langspair_t langs = this->getPref()->langGetPair();
			std::swap( langs.first, langs.second );
			this->getPref()->langSetPair( langs );
			this->updateLangsImages();
			break;
		}
	case AD_PANEL_TB_LANG_TO:
		{
			this->popupLangMenu( id );
			break;
		}
	case AD_PANEL_TB_DELETE:
		{
			this->listView.deleteAllItems();
			break;
		}
	default:
		{
			bool update = false;
			bool from = true;

			int ind = id - AD_PANEL_TB_LANGUAGE;
			if ( ind >= 0 ) {
				if ( ind < AD_PANEL_IMAGE_LANG_COUNT ) {
					// from
					update = true;
				} else {
					ind -= AD_PANEL_IMAGE_LANG_COUNT;
					if ( ind < AD_PANEL_IMAGE_LANG_COUNT ) {
						// to
						from = false;
						update = true;
					}
				}
			}
			if ( update ) {
				this->getPref()->langSetLang( from, (size_t)ind );
				this->updateLangsImages();
			}
			break;
		}
	}
}


void panel::popupLangMenu( int const id ) {
	pref::langspair_t langs = this->getPref()->langGetPair();
	pref::lang_t clang = langs.first;
	unsigned int langId = AD_PANEL_TB_LANGUAGE;

	if ( id == AD_PANEL_TB_LANG_TO ) {
		clang = langs.second;
		langId += AD_PANEL_IMAGE_LANG_COUNT;
	}

	RECT rc;
	SendMessage( this->toolbar.getHWND(), TB_GETRECT, (WPARAM)id, (LPARAM)&rc);

	// Convert to screen coordinates.            
	MapWindowPoints( this->toolbar.getHWND(), HWND_DESKTOP, (LPPOINT)&rc, 2);                         

	atom::wmenu menu;
	menu.create();
	//
	struct _{
		static bool __( pref::lang_t const& lang, pref::lang_t const& clang, unsigned int& id, atom::wmenu& mn ) {
			atom::stringstream_t ss;
			ss << _T("[") << lang.get<1>() << _T("] ") << lang.get<2>();
				mn.appendItem( MF_ENABLED | MF_STRING | MFT_RADIOCHECK | (( lang == clang )?( MF_CHECKED ):( 0 )), id, ss.str() );
			id++;
			return true;
		}
	};
	this->getPref()->langEnum( boost::bind( &_::__, _1, boost::ref( clang ), boost::ref( langId ), boost::ref( menu ) ) );
	menu.popup( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, *this, rc );
}

void panel::updateLangsImages() {
	pref::langspair_t const langs = this->getPref()->langGetPair();
	//
	this->toolbar.updateButtonImage( AD_PANEL_TB_LANG_FROM, langs.first.get<3>() );
	this->toolbar.updateButtonImage( AD_PANEL_TB_LANG_TO, langs.second.get<3>() );
	this->toolbar.invalidate();
}
