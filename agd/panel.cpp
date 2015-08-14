#include "./pch.hpp"
#include "./resource.h"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./window.hpp"
#include "./shell.hpp"
#include "./ad.hpp"
#include "./langs.hpp"
#include "./panel.hpp"

panel::panel(logger_ptr l, pref_ptr p, ad_ptr a, langs_ptr lg) :
		wwindow( *this, INITLIST_4( &panel::onPaint, &panel::onSize, &panel::onNotify, &panel::onCommand ) )
		, adActive(false)
		, adVisible(false)
		, imageList()
		, listView()
		, toolbar() {
		//
		atom::mount<panel2logger>( this, l );
		atom::mount<panel2pref>(this, p);
		atom::mount<panel2ad>(this, a);
		atom::mount<panel2langs>(this, lg);
}

panel::~panel() {
}

bool panel::init( HWND hParent ) {
	assert(hParent);
	//this->getAD()->
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
			cs.dwExStyle		= WS_EX_PALETTEWINDOW | WS_EX_TOPMOST;
			return true;
		}
	};
	RECT rt, view;
	this->getPref()->getView(this->getPref()->get< bool >(CONFIG_CLIP), view);
	SetRect( &rt, view.left, view.top, view.left + RECT_WIDTH( view ) / 5, view.top + 2 * RECT_HEIGHT( view ) / 5 );
	OffsetRect( &rt, RECT_WIDTH( view ) - RECT_WIDTH( rt ), RECT_HEIGHT( view ) - RECT_HEIGHT( rt ) );
	if ( base_panel_t::init( boost::bind( _::__, _1, _2, boost::ref( rt ), hParent ), true ) ) {
		//
		HINSTANCE hInst = (HINSTANCE)GetModuleHandle( NULL );
		RECT lvrt;
		this->getClientRect( lvrt );
		//
		//
		size_t const btnSz = 32;
		size_t const lngsCount = this->getLangs()->count() + 3;
		COLORREF maskColor = RGB(255, 255, 255);

		this->imageList.create((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_IMAGES), btnSz, 0, maskColor);
#if 0
		//
		dcb_t dcb;
		dcb.updateDC(btnSz * lngsCount, btnSz);
		RECT rt;
		SetRect(&rt, 0, 0, btnSz * lngsCount, btnSz);
		FillRect(dcb.dc, &rt, reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
		SetBkMode(dcb.dc, TRANSPARENT);
		//
		SetRect(&rt, 0, 0, btnSz, btnSz);
		OffsetRect(&rt, btnSz * 3, 0);
		//
		atom::stringstream_t ss;
		struct _{
			static bool __(language_t const& lang, dcb_t& dcb, RECT&rt, size_t const sz, atom::stringstream_t& ss) {
				atom::string_t c2 = lang.c2;
				boost::to_upper(c2);
				DrawText(dcb.dc, c2.c_str(), c2.length(), &rt, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				OffsetRect(&rt, sz, 0);
				//
				ss << lang.name << _T(", ");
				return true;
			}
		};
		this->getLangs()->foreach(boost::bind(&_::__, _1, boost::ref(dcb), boost::ref(rt), btnSz, boost::ref(ss)));
		//int index = -1;
		//this->imageList.append(dcb.bitmap, maskColor, index);
		//bitmapSave(_T("lang.bmp"), dcb.dc, dcb.bitmap);
		std::wofstream outFile;
		outFile.open(_T("lang.txt"));
		outFile << ss.str();
#endif

		//
		// Create the list-view window in report view with label editing enabled.
		this->listView.create( 0, WS_CHILD | WS_BORDER | LVS_REPORT | LVS_EDITLABELS, lvrt, AD_PANEL_LISTVIEW, *this ).
			updateColumn( true, 0, LVCFMT_LEFT, ( RECT_WIDTH( lvrt ) ) / 2, _T( "Source" ), 0 ).
			updateColumn( true, 1, LVCFMT_LEFT, ( RECT_WIDTH( lvrt ) ) / 2, _T( "Translation" ), 1 ).
			show();
		//
		// create toolbar
		langspair_t const langs = this->getLangs()->getPair();
		RECT tbrt;
		SetRect( &tbrt, 0, 0, 0, 40 );
		this->toolbar.
			create( 0, WS_CHILD | CCS_NORESIZE | TBSTYLE_WRAPABLE | TBSTYLE_FLAT, tbrt, AD_PANEL_TOOLBAR, this->imageList, *this ).
			setButtonSize(btnSz, btnSz).
			setIndent(8).
			addButton(AD_PANEL_IMAGE_PIN, AD_PANEL_TB_PIN, TBSTATE_ENABLED | TBSTATE_WRAP, BTNS_CHECK | BTNS_AUTOSIZE, NULL, atom::string_t()).
			addButton( -1, 0, TBSTATE_ENABLED | TBSTATE_WRAP, BTNS_SEP | BTNS_AUTOSIZE, NULL, atom::string_t() ).
			addButton( langs.first.img, AD_PANEL_TB_LANG_FROM, TBSTATE_ENABLED | TBSTATE_WRAP, BTNS_BUTTON | TBSTYLE_AUTOSIZE, NULL, atom::string_t() ).
			addButton( AD_PANEL_IMAGE_SWAP, AD_PANEL_TB_LANG_SWAP, TBSTATE_ENABLED | TBSTATE_WRAP, BTNS_BUTTON | BTNS_AUTOSIZE, NULL, atom::string_t() ).
			addButton( langs.second.img, AD_PANEL_TB_LANG_TO, TBSTATE_ENABLED | TBSTATE_WRAP, BTNS_BUTTON | TBSTYLE_AUTOSIZE, NULL, atom::string_t() ).
			addButton( -1, 0, TBSTATE_ENABLED | TBSTATE_WRAP, BTNS_SEP | BTNS_AUTOSIZE, NULL, atom::string_t() ).
			addButton( AD_PANEL_IMAGE_DELETE, AD_PANEL_TB_DELETE, TBSTATE_ENABLED | TBSTATE_WRAP, BTNS_BUTTON | BTNS_AUTOSIZE, NULL, atom::string_t() ).
			build().
			show();
		this->updateLangsImages();
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
			this->show(this->adActive && this->adVisible);
			break;
		}
	case AD_PANEL_TB_LANG_FROM:
		{
			this->popupLangMenu( id );
			break;
		}
	case AD_PANEL_TB_LANG_SWAP:
		{
			langspair_t langs = this->getLangs()->getPair();
			std::swap( langs.first, langs.second );
			this->getLangs()->setPair(langs);
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
	}
}

bool panel::isLocked() const {
	return this->toolbar.isBottonChecked(AD_PANEL_TB_PIN);
}

void panel::popupLangMenu( int const id ) {
	langspair_t langs = this->getLangs()->getPair();
	language_t clang = langs.first;

	if ( id == AD_PANEL_TB_LANG_TO ) {
		clang = langs.second;
	}

	RECT rc;
	SendMessage( this->toolbar.getHWND(), TB_GETRECT, (WPARAM)id, (LPARAM)&rc);

	// Convert to screen coordinates.            
	MapWindowPoints( this->toolbar.getHWND(), HWND_DESKTOP, (LPPOINT)&rc, 2);                         

	atom::wmenu menu;
	menu.create();
	//
	struct _{
		static bool __(language_t const& lang, language_t const& clang, atom::wmenu& mn) {
			if (lang.enable){
				atom::stringstream_t ss;
				ss << lang.name;
				mn.appendItem(MF_ENABLED | MF_STRING | MFT_RADIOCHECK | ((lang == clang) ? (MF_CHECKED) : (0)), lang.command, ss.str());
			}
			return true;
		}
	};
	this->getLangs()->foreach( boost::bind( &_::__, _1, boost::ref( clang ), boost::ref( menu ) ) );
	UINT command = menu.popup(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_NONOTIFY | TPM_RETURNCMD, rc.left, rc.bottom, *this, rc);
	if (command){
		this->getLangs()->setLang(id == AD_PANEL_TB_LANG_FROM, (size_t)(command - AD_PANEL_TB_LANGUAGE));
		this->updateLangsImages();
	}
}

void panel::updateLangsImages() {
	langspair_t const langs = this->getLangs()->getPair();
	//
	atom::stringstream_t ss;
	ss << langs.first.name << _T(" > ") << langs.second.name;
	this->setTitle(ss.str());

	this->toolbar.updateButtonImage( AD_PANEL_TB_LANG_FROM, langs.first.img );
	this->toolbar.updateButtonImage( AD_PANEL_TB_LANG_TO, langs.second.img );
	this->toolbar.invalidate();
}
