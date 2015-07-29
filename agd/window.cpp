#include "./pch.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./window.hpp"
#include "./shell.hpp"
#include "./ad.hpp"
#include "./atlas.hpp"
#include "./am.hpp"


namespace atom {
	class wpopupmenu : boost::noncopyable {
	public:
		wpopupmenu() : menu(CreatePopupMenu()){
		}
		~wpopupmenu(){
			DestroyMenu(menu);
		}
		///
		void appendItem(atom::string_t const& caption, WORD const command, HBITMAP icon) const {
			MENUITEMINFO mi; init(mi);
			mi.fMask = MIIM_BITMAP;
			mi.hbmpItem = icon;
			append(caption, command, mi);
		}
		///
		void appendCheckbox(atom::string_t const& caption, WORD const command, bool const checked) const {
			MENUITEMINFO mi; init(mi);
			mi.fMask = MIIM_STATE;
			mi.fState = (checked) ? (MFS_CHECKED) : (0);
			append(caption, command, mi);
		}
		///
		void appendRadioButton(atom::string_t const& caption, WORD const command) const {
			MENUITEMINFO mi; init(mi);
			mi.fType = MFT_RADIOCHECK;
			mi.fMask = MIIM_STATE;
			mi.fState = MFS_CHECKED;
			append(caption, command, mi);
		}
		///
		void appendSeparator() const {
			MENUITEMINFO mi; init(mi);
			mi.cbSize = sizeof(mi);
			mi.fType = MFT_SEPARATOR;

			insert(mi);
		}
		///
		void appendSubmenu(atom::string_t const& caption, wpopupmenu& submenu) const {
			MENUITEMINFO mi; init(mi);
			mi.fMask = MIIM_SUBMENU;
			mi.hSubMenu = submenu.menu;
			append(caption, 0, mi);
		}
		///
		void popup(HWND hwnd, int const x, int const y){
			TrackPopupMenuEx(menu, TPM_LEFTALIGN | TPM_TOPALIGN, x, y, hwnd, NULL);
		}
	protected:
		void init(MENUITEMINFO& mi) const {
			memset(&mi, 0, sizeof(mi));
			mi.cbSize = sizeof(mi);
		}
		void append(atom::string_t const& caption, WORD const command, MENUITEMINFO& mi) const {
			atom::char_t name[512] = { 0 };

			mi.fMask |= MIIM_STRING | MIIM_ID | MIIM_FTYPE;
			mi.fType |= MFT_STRING;
			mi.wID = command;

			mi.cch = caption.length();
			if (mi.cch){
				wcsncpy(name, caption.c_str(), mi.cch);
				mi.dwTypeData = name;
			}

			insert(mi);
		}
		void insert(MENUITEMINFO& mi) const {
			InsertMenuItem(this->menu, GetMenuItemCount(this->menu), TRUE, &mi);
		}
	private:
		HMENU
			menu;
	};
}


enum menuItemProcessType{
	miptSkipItem,
	miptProcess,
	miptCheckboxClear,
	miptCheckboxChecked,
	miptRadioCheck
};

template< typename T >
struct menuItem {
	atom::string_t		id;
	WORD				command;
	HBITMAP				icon;
	atom::wpopupmenu*	submenu;
	T					value;
};

template <typename T, size_t N >
void buildMenu(atom::wpopupmenu& menu, menuItem<T>(&items)[N], WORD const firstId, boost::function<menuItemProcessType(menuItem<T> const&)> callback){
	WORD id = firstId;
	//
	BOOST_FOREACH(menuItem<T>& item, items) {
		if (item.id.length()){
			if (item.command || firstId){
				// string
				item.command = ((firstId) ? (id++) : (item.command));
				switch (callback(item)){
				case miptSkipItem:
					break;
				case miptProcess:
					menu.appendItem(item.id, item.command, item.icon);
					break;
				case miptCheckboxClear:
					menu.appendCheckbox(item.id, item.command, false);
					break;
				case miptCheckboxChecked:
					menu.appendCheckbox(item.id, item.command, true);
					break;
				case miptRadioCheck:
					menu.appendRadioButton(item.id, item.command);
					break;
				}
			}
			else{
				// submenu
				if (item.submenu){
					menu.appendSubmenu(item.id, *(item.submenu));
				}
			}
		}
		else{
			if (item.command){
				//
			}
			else{
				// separator
				menu.appendSeparator();
			}
		}
	}
}

template <typename T, size_t N >
bool mapMenuCommand(WORD const command, menuItem<T>(&items)[N], T& foundValue){
	//
	BOOST_FOREACH(menuItem<T>& item, items) {
		if (item.command == command){
			foundValue = item.value;
			return true;
		}
	}
	return false;
}

static menuItem<int> topItems[] = {
	{ _T("Hide"), CMDID_HIDE, HBMMENU_MBAR_MINIMIZE, NULL, 0 },
	{ _T("Maximize"), CMDID_MAXIMIZE, HBMMENU_POPUP_MAXIMIZE, NULL, 0 },
	{ _T("Restore"), CMDID_RESTORE, HBMMENU_MBAR_RESTORE, NULL, 0 },
	{ _T(""), 0, NULL, NULL, 0 },
	{ _T("Transparency"), 0, NULL, NULL, 0 },
	{ _T("Background color"), CMDID_BACKGROUND_COLOR, NULL, NULL, 0 },
	{ _T("Exclude startup toolbar"), CMDID_WORK_AREA, NULL, NULL, 0 },
	{ _T(""), 0, NULL, NULL, 0 },
	{ _T("Alignment"), 0, NULL, NULL, 0 },
	{ _T("Vertical ratio"), 0, NULL, NULL, 0 },
	{ _T("Horizontal ratio"), 0, NULL, NULL, 0 },
	{ _T("Slide timeout"), 0, NULL, NULL, 0 },
	{ _T(""), 0, NULL, NULL, 0 },
	{ _T("Exit"), CMDID_EXIT, HBMMENU_MBAR_CLOSE, NULL, 0 }
};
//
static menuItem<unsigned int> transpItems[] = {
	{ _T("90%"), 0, NULL, NULL, 230 },
	{ _T("80%"), 0, NULL, NULL, 204 },
	{ _T("70%"), 0, NULL, NULL, 179 },
	{ _T("60%"), 0, NULL, NULL, 153 },
	{ _T("50%"), 0, NULL, NULL, 128 },
	{ _T("40%"), 0, NULL, NULL, 102 },
	{ _T("30%"), 0, NULL, NULL, 77 },
	{ _T("20%"), 0, NULL, NULL, 51 },
	{ _T("10%"), 0, NULL, NULL, 26 },
	{ _T("0%"), 0, NULL, NULL, 0 }
};
//
static menuItem<unsigned int> vrationItems[] = {
	{ _T("100%"), 0, NULL, NULL, 100 },
	{ _T("75%"), 0, NULL, NULL, 75 },
	{ _T("50%"), 0, NULL, NULL, 50 },
	{ _T("25%"), 0, NULL, NULL, 25 }
};
//
static menuItem<unsigned int> hrationItems[] = {
	{ _T("100%"), 0, NULL, NULL, 100 },
	{ _T("75%"), 0, NULL, NULL, 75 },
	{ _T("50%"), 0, NULL, NULL, 50 },
	{ _T("25%"), 0, NULL, NULL, 25 }
};

static menuItem<alignment_t::type> alignmentItems[] {
	{ _T("Top + Left"), 0, NULL, NULL, alignment_t::top | alignment_t::left },
	{ _T("Top + Center horizontal"), 0, NULL, NULL, alignment_t::top | alignment_t::hcenter },
	{ _T("Top + Client horizontal"), 0, NULL, NULL, alignment_t::top | alignment_t::hclient },
	{ _T("Top + Right"), 0, NULL, NULL, alignment_t::top | alignment_t::right },
	{ _T(""), 0, NULL, NULL, 0 },
	{ _T("Center vertical + Left"), 0, NULL, NULL, alignment_t::vcenter | alignment_t::left },
	{ _T("Center vertical + Center horizontal"), 0, NULL, NULL, alignment_t::vcenter | alignment_t::hcenter },
	{ _T("Center vertical + Client horizontal"), 0, NULL, NULL, alignment_t::vcenter | alignment_t::hclient },
	{ _T("Center vertical + Right"), 0, NULL, NULL, alignment_t::vcenter | alignment_t::right },
	{ _T(""), 0, NULL, NULL, 0 },
	{ _T("Client vertical + Left"), 0, NULL, NULL, alignment_t::vclient | alignment_t::left },
	{ _T("Client vertical + Center horizontal"), 0, NULL, NULL, alignment_t::vclient | alignment_t::hcenter },
	{ _T("Client vertical + Client horizontal"), 0, NULL, NULL, alignment_t::vclient | alignment_t::hclient },
	{ _T("Client vertical + Right"), 0, NULL, NULL, alignment_t::vclient | alignment_t::right },
	{ _T(""), 0, NULL, NULL, 0 },
	{ _T("Bottom + left"), 0, NULL, NULL, alignment_t::bottom | alignment_t::left },
	{ _T("Bottom + Center horizontal"), 0, NULL, NULL, alignment_t::bottom | alignment_t::hcenter },
	{ _T("Bottom + Client horizontal"), 0, NULL, NULL, alignment_t::bottom | alignment_t::hclient },
	{ _T("Bottom + Right"), 0, NULL, NULL, alignment_t::bottom | alignment_t::right }
};

static menuItem<unsigned int> slideItems[] {
	{ _T("No"), 0, NULL, NULL, 0 },
	{ _T("0.1s"), 0, NULL, NULL, 100 },
	{ _T("0.25s"), 0, NULL, NULL, 250 },
	{ _T("0.5s"), 0, NULL, NULL, 500 }
};


#define SC_MODE_CMD         0x0010

window::window( logger_ptr l, pref_ptr p ) :
wwindow(*this, INITLIST_15(&window::onKey, &window::onKey, &window::onChar, &window::onHotkey, &window::onPaint, &window::onClose, &window::onSettingChange, &window::onTimer, &window::onCommand, &window::onLBDown, &window::onLBUp, &window::onRBDown, &window::onMouseMove, &window::onCaptureChanged, &window::onSysCommand))
	,	appearHotKey()
	,	windowPlacement()
	,	paintParam()
	,	modes()
	,	currentMode() {
		//
		atom::mount<window2logger>( this, l );
		atom::mount<window2pref>( this, p );
}

window::~window() {
}

bool window::init() {
	//
	logger_ptr	l = get_slot< window2logger >().item();
	pref_ptr	p = get_slot< window2pref >().item(); 
	window_ptr	w = this->shared_from_this();
#ifdef CONSOLE_STATE
	this->modes.push_back( boost::make_tuple( _T( "Console" ), mode::create<shell>( p->getModeConfig("ad"), l, p, w ) ) );
#endif
#ifdef AUGMENTED_DESKTOP_STATE
	this->modes.push_back(boost::make_tuple(_T("Augmented desktop"), mode::create<ad>(p->getModeConfig("console"), l, p, w)));
#endif
	//this->modes.push_back(boost::make_tuple(_T("Atlas"), mode::create<atlas>(l, p, w)));
	//this->modes.push_back( boost::make_tuple( _T( "Augmented manuals" ), mode::create<am>( l, p, w ) ) );
	//
	DWORD const style = 0;
	DWORD const ex_style = WS_EX_TOPMOST;
	struct _ {
		static bool __( WNDCLASSEX& wcex, CREATESTRUCT& cs, RECT const& rect, DWORD const style, DWORD const ex_style ) {
			wcex.cbSize;
			wcex.style			=	CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc;
			wcex.cbClsExtra;
			wcex.cbWndExtra;
			wcex.hInstance;
			wcex.hIcon;
			wcex.hCursor;
			wcex.hbrBackground	=	NULL;
			wcex.lpszMenuName;
			wcex.lpszClassName;
			wcex.hIconSm;
			//
			cs.lpCreateParams;
			cs.hInstance;
			cs.hMenu;
			cs.hwndParent;
			cs.cy				=	rect.bottom - rect.top;
			cs.cx				=	rect.right - rect.left;
			cs.y				=	rect.top;
			cs.x				=	rect.left;
			cs.style			=	style;
			cs.lpszName;
			cs.lpszClass;
			cs.dwExStyle		=	ex_style;
			return true;
		}
	};
	//
	this->updatePlacement(
		false,
		this->getPref()->get< bool >(CONFIG_MAXIMIZED),
		this->getPref()->get< bool >(CONFIG_CLIP),
		this->getPref()->get< DWORD >(CONFIG_TIMEOUT),
		this->getPref()->get< unsigned int >(CONFIG_WIDTH),
		this->getPref()->get< unsigned int >(CONFIG_HEIGHT),
		this->getPref()->getAlignment()
		);
	if ( base_window_t::init( boost::bind( _::__, _1, _2, boost::ref( this->windowPlacement.destination ), style, ex_style ), true ) ) {
		this->setStyles( WS_OVERLAPPED | WS_SYSMENU, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED );
		//
		UINT pos = 0;
		BOOST_FOREACH( mode_item_t& m, modes ) {
			this->sysMenuInsert( pos, MF_BYPOSITION | MF_STRING, SC_MODE_CMD + ( pos << 4 ), m.get<0>() );
			pos++;
		}
		this->sysMenuInsert( pos, MF_BYPOSITION | MF_SEPARATOR, 0, _T( "" ) );
		this->modeSwitch(0);
		//
		hotkey new_hk;
		if (this->getPref()->parseHotkey(CONFIG_HK_APPEAR, new_hk)) {
			//
			if ( new_hk != this->appearHotKey ) {
				new_hk.id++;
				if ( RegisterHotKey( this->getHWND(), new_hk.id, new_hk.mods, new_hk.vk )) {
					if ( this->appearHotKey.id && !UnregisterHotKey( this->getHWND(), this->appearHotKey.id ) ) {
						*(this->getLogger()) << "Hotkey unregister error" << std::endl;
					}
					this->appearHotKey = new_hk; 
				} else {
					*(this->getLogger()) << "Hotkey register error" << std::endl;
				}
			}
		}
		//
		//
		return true;
	}
	return false;
}

void window::run() {
	struct _ {
		static bool __( HWND hWnd, MSG* msg, pref_ptr pref, window& w ) {
			return pref->translateAccel( hWnd, msg );
		}
	};
	this->toggleVisibility();
	base_window_t::run( boost::bind( _::__, _1, _2, this->getPref(), boost::ref( *this ) ) );
	this->modeSwitch( std::numeric_limits<size_t>::max() );
}

void window::clear() {
	BOOST_FOREACH( mode_item_t& m, modes ) {
		m.get<1>()->clear();
	}
	base_node_t::clear();
}

void window::onKey( HWND hWnd, UINT vk, BOOL down, int repeat, UINT flags ) {
	if ( vk == 32 ) {
		//HMENU hmenu = GetSystemMenu( hWnd, false );
		//TrackPopupMenu( hmenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN, 0, 0, 0, hWnd, NULL);
	}
	KEY_EVENT_RECORD key;
	key.bKeyDown			=	down;
	key.wRepeatCount		=	repeat;
	key.wVirtualKeyCode		=	vk;
	key.wVirtualScanCode	=	MapVirtualKey( vk, MAPVK_VK_TO_VSC );
	BYTE kbrd[256] = { 0 };
	WORD c = 0;
	GetKeyboardState( kbrd );
	ToAscii(
		key.wVirtualKeyCode,
		key.wVirtualScanCode,
		kbrd,
		&c,
		0
		);
	key.uChar.UnicodeChar	=	c;
	key.uChar.AsciiChar;
	key.dwControlKeyState	=
		( ( GetKeyState( VK_CAPITAL ) & 0x01 ) ? ( CAPSLOCK_ON ) : ( 0 ) ) |
		//ENHANCED_KEY
		( ( GetKeyState( VK_LMENU ) & 0x80 ) ? ( LEFT_ALT_PRESSED ) : ( 0 ) ) |
		( ( GetKeyState( VK_LCONTROL ) & 0x80 ) ? ( LEFT_CTRL_PRESSED ) : ( 0 ) ) |
		( ( GetKeyState( VK_NUMLOCK ) & 0x01 ) ? ( NUMLOCK_ON ) : ( 0 ) ) |
		( ( GetKeyState( VK_RMENU ) & 0x80 ) ? ( RIGHT_ALT_PRESSED ) : ( 0 ) ) |
		( ( GetKeyState( VK_RCONTROL ) & 0x80 ) ? ( RIGHT_CTRL_PRESSED ) : ( 0 ) ) |
		( ( GetKeyState( VK_SCROLL ) & 0x01 ) ? ( SCROLLLOCK_ON ) : ( 0 ) ) |
		( ( GetKeyState( VK_SHIFT ) & 0x80 ) ? ( SHIFT_PRESSED ) : ( 0 ) ) ;

	//this->getLogger() << vk << ((down)?(" down"):(" up")) << std::endl;
	this->currentMode->key( key );
	this->invalidate();
}

void window::onChar( HWND hWnd, TCHAR ch, int cRepeat ) {
}

void window::onHotkey( HWND hWnd, int idHotKey, UINT fuModifiers, UINT vk ) {
	if ( idHotKey == this->appearHotKey.id ) {
		this->toggleVisibility();
	}
}


void window::onPaint( HWND hWnd ) {
	PAINTSTRUCT	ps; 
	RECT 		rect;
	HDC			dc = BeginPaint( hWnd, &ps );
	{
		GetClientRect( hWnd, &rect );
		if ( this->windowPlacement.sliding ) {
			//SIZE bmsz = { 0 };
			//GetBitmapDimensionEx( this->paintParam.bitmap, &bmsz );
			//StretchBlt( dc,
			//	0,
			//	0,
			//	rect.right,
			//	rect.bottom,
			//	this->paintParam.dc,
			//	0,
			//	0,
			//	bmsz.cx,
			//	bmsz.cy,
			//	SRCCOPY );
			//FillRect( dc, &rect, this->paintParam.bk );
		} else {
			this->currentMode->paint( this->paintParam, rect );
			//
			BitBlt( dc,
				0,
				0,
				rect.right,
				rect.bottom,
				this->paintParam.dcb.dc,
				0,
				0,
				SRCCOPY );
		}
	}
	EndPaint( hWnd, &ps );
}

void window::onClose( HWND hWnd ) {
	this->exit();
}

void window::onSettingChange( HWND hWnd, UINT uiAction, LPCTSTR lpName ) {
	switch( uiAction ) {
	case SPI_SETWORKAREA:
	case SPI_ICONVERTICALSPACING:
		this->updatePlacement(this->windowPlacement.visible, this->windowPlacement.fullScreen, this->windowPlacement.clip, this->windowPlacement.timeout, this->windowPlacement.width, this->windowPlacement.height, this->windowPlacement.alignment);
		this->slideBegin();
		break;
	}
}

void window::onTimer( HWND hWnd, UINT id ){
	if ( id == this->windowPlacement.timerId ) {
		slideUpdate();
	};
}

void window::onCommand( int id, HWND hwndCtl, UINT codeNotify ) {
	static COLORREF ccs[16] = { 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF };
	CHOOSECOLOR cc = { 0 };
	if (!currentMode->command(id)) {
		unsigned int alpha = this->currentMode->getAlpha();
		alignment_t::type aligment = 0;
		unsigned int width = 0;
		unsigned int height = 0;
		unsigned int slide = 0;
		if (mapMenuCommand<unsigned int>(id, transpItems, alpha)){
			this->currentMode->setAlpha(255-alpha);
		} else if (mapMenuCommand<alignment_t::type>(id, alignmentItems, aligment)){
			this->updatePlacement(this->windowPlacement.visible, this->windowPlacement.fullScreen, this->windowPlacement.clip, this->windowPlacement.timeout, this->windowPlacement.width, this->windowPlacement.height, aligment);
			this->slideBegin();
		} else if (mapMenuCommand<unsigned int>(id, vrationItems, height)){
			this->updatePlacement(this->windowPlacement.visible, this->windowPlacement.fullScreen, this->windowPlacement.clip, this->windowPlacement.timeout, this->windowPlacement.width, height, this->windowPlacement.alignment);
			this->slideBegin();
		} else if (mapMenuCommand<unsigned int>(id, hrationItems, width)){
			this->updatePlacement(this->windowPlacement.visible, this->windowPlacement.fullScreen, this->windowPlacement.clip, this->windowPlacement.timeout, width, this->windowPlacement.height, this->windowPlacement.alignment);
			this->slideBegin();
		} else if (mapMenuCommand<unsigned int>(id, slideItems, slide)){
			this->windowPlacement.timeout = slide;
		}
		//
		switch (id) {
		case CMDID_FULLSCREEN:
		case CMDID_MAXIMIZE:
		case CMDID_RESTORE:
			this->toggleFullScreen();
			break;
		case CMDID_EXIT:
			this->exit();
			break;
		case CMDID_HIDE:
			this->toggleVisibility();
			break;
		case CMDID_BACKGROUND_COLOR:
			cc.lStructSize = sizeof(cc);
			cc.hwndOwner = this->getHWND();
			cc.hInstance;
			cc.rgbResult = this->currentMode->getBkColor();
			cc.lpCustColors = ccs;
			cc.Flags = CC_RGBINIT;
			cc.lCustData;
			cc.lpfnHook;
			cc.lpTemplateName;
			if (ChooseColor(&cc)){
				this->currentMode->setBkColor(cc.rgbResult);
				this->invalidate();
			}
			break;
		case CMDID_WORK_AREA:
			this->toggleClip();
			break;
		}
	}
	this->invalidate();
}

void window::onLBDown( HWND, BOOL dblclick, int x , int y, UINT state ) {
	this->currentMode->mouselbdown( dblclick != 0, x, y, state );
}

void window::onLBUp( HWND hWnd, int x, int y, UINT state ) {
	this->currentMode->mouselbup( x, y, state );
}



void constructMenu(atom::wpopupmenu& menu, WORD const firstId, WORD const start, WORD const step, WORD const count, atom::char_t** arr){
	WORD value = start;
	//
	for (WORD i = 0; i < count; i++){
		if (arr){
			menu.appendItem(arr[i], firstId + i, NULL);
		}
		else{
			atom::stringstream_t ss;
			ss << value << _T("%");
			menu.appendItem(ss.str(), firstId+i, NULL);
			value += step;
		}
	}
}

void window::onRBDown(HWND, BOOL, int x, int y, UINT){
	struct _{
		static menuItemProcessType processTopItems(menuItem<int> const& item, window_ptr w){
			if ((w->windowPlacement.fullScreen && (item.command == CMDID_MAXIMIZE)) || (!w->windowPlacement.fullScreen && (item.command == CMDID_RESTORE))){
				return miptSkipItem;
			}
			if (item.command == CMDID_WORK_AREA){
				return ((w->windowPlacement.clip) ? (miptCheckboxChecked) : (miptCheckboxClear));
			}
			return miptProcess;
		}
		static menuItemProcessType processTranspItems(menuItem<unsigned int> const& item, window_ptr w, unsigned int const alpha, bool& found){
			if (!found && (alpha >= item.value)) {
				found = true;
				return miptRadioCheck;
			}
			return miptProcess;
		}
		static menuItemProcessType processVratioItems(menuItem<unsigned int> const& item, window_ptr w, unsigned int const vratio, bool& found){
			if (!found && (vratio >= item.value)) {
				found = true;
				return miptRadioCheck;
			}
			return miptProcess;
		}
		static menuItemProcessType processHratioItems(menuItem<unsigned int> const& item, window_ptr w, unsigned int const hratio, bool& found){
			if (!found && (hratio >= item.value)) {
				found = true;
				return miptRadioCheck;
			}
			return miptProcess;
		}
		static menuItemProcessType processAlignmentItems(menuItem<alignment_t::type> const& item, window_ptr w, alignment_t::type const alignment, bool& found){
			if (!found && (alignment == item.value)) {
				found = true;
				return miptRadioCheck;
			}
			return miptProcess;
		}
		static menuItemProcessType processSlideItems(menuItem<unsigned int> const& item, window_ptr w, unsigned int const slide, bool& found){
			if (!found && (slide == item.value)) {
				found = true;
				return miptRadioCheck;
			}
			return miptProcess;
		}
	};

	//
	atom::wpopupmenu transpMenu;
	bool foundTransp = false;
	buildMenu<unsigned int>(transpMenu, transpItems, CMDID_TRANSPARENT_FIRST_ID, boost::bind(_::processTranspItems, _1, this->shared_from_this(), 255-this->currentMode->getAlpha(), boost::ref(foundTransp)));
	topItems[4].submenu = &transpMenu;
	//
	atom::wpopupmenu alignmentMenu;
	bool foundAlignment = false;
	buildMenu<alignment_t::type>(alignmentMenu, alignmentItems, CMDID_ALIGNMENT_FIRST_ID, boost::bind(_::processAlignmentItems, _1, this->shared_from_this(), this->windowPlacement.alignment, boost::ref(foundAlignment)));
	topItems[8].submenu = &alignmentMenu;
	//
	atom::wpopupmenu vratio;
	bool foundVratio = false;
	buildMenu<unsigned int>(vratio, vrationItems, CMDID_VRATION_FIRST_ID, boost::bind(_::processVratioItems, _1, this->shared_from_this(), this->windowPlacement.height, boost::ref(foundVratio)));
	topItems[9].submenu = &vratio;
	//
	atom::wpopupmenu hratio;
	bool foundHratio = false;
	buildMenu<unsigned int>(hratio, hrationItems, CMDID_HRATION_FIRST_ID, boost::bind(_::processHratioItems, _1, this->shared_from_this(), this->windowPlacement.width, boost::ref(foundHratio)));
	topItems[10].submenu = &hratio;
	//
	atom::wpopupmenu slide;
	bool foundSlide = false;
	buildMenu<unsigned int>(slide, slideItems, CMDID_SLIDE_FIRST_ID, boost::bind(_::processSlideItems, _1, this->shared_from_this(), this->windowPlacement.timeout, boost::ref(foundSlide)));
	topItems[11].submenu = &slide;
	//
	atom::wpopupmenu menu;
	buildMenu<int>(menu, topItems, 0, boost::bind(_::processTopItems, _1, this->shared_from_this()));
	POINT pt;
	pt.x = x;
	pt.y = y;
	MapWindowPoints(this->getHWND(), NULL, &pt, 1);
	menu.popup(this->getHWND(), pt.x, pt.y);
}


void window::onMouseMove( HWND hWnd, int x, int y, UINT state ) {
	this->currentMode->mousemove( x, y, state );
}

void window::onCaptureChanged( HWND, HWND ) {
	this->invalidate();
}

void window::onSysCommand( HWND hWnd, UINT cmd, int x, int y ) {
	UINT pos = ( cmd - SC_MODE_CMD ) >> 4;
	if ( ( 0 <= pos ) && ( pos < this->modes.size() ) ) {
		this->modeSwitch( pos );
	} else {
		FORWARD_WM_SYSCOMMAND( hWnd, cmd, x, y, DefWindowProc );
	}
}

void window::toggleVisibility() {
	this->updatePlacement(!this->windowPlacement.visible, this->windowPlacement.fullScreen, this->windowPlacement.clip, this->windowPlacement.timeout, this->windowPlacement.width, this->windowPlacement.height, this->windowPlacement.alignment);
	this->slideBegin();
}

void window::toggleFullScreen(){
	this->updatePlacement(this->windowPlacement.visible, !this->windowPlacement.fullScreen, this->windowPlacement.clip, this->windowPlacement.timeout, this->windowPlacement.width, this->windowPlacement.height, this->windowPlacement.alignment);
	this->slideBegin();
}

void window::toggleClip(){
	this->updatePlacement(this->windowPlacement.visible, this->windowPlacement.fullScreen, !this->windowPlacement.clip, this->windowPlacement.timeout, this->windowPlacement.width, this->windowPlacement.height, this->windowPlacement.alignment);
	this->slideBegin();
}


void window::updatePlacement(bool const visible, bool const fullScreen, bool const clip, DWORD const timeout, unsigned int const width, unsigned int const height, alignment_t::type const alignment) {
	RECT rt = this->windowPlacement.destination;
	this->windowPlacement.visible = visible;
	this->windowPlacement.fullScreen = fullScreen;
	this->windowPlacement.clip = clip;
	this->windowPlacement.timeout = timeout;
	this->windowPlacement.width = width;
	this->windowPlacement.height = height;
	this->windowPlacement.alignment = alignment;
	this->getPref()->calculateDocks(((this->currentMode) ? (this->currentMode->getAlpha()) : (255)), this->windowPlacement);
	// update mem dc
	SIZE sz;
	sz.cx = RECT_WIDTH( this->windowPlacement.destination );
	sz.cy = RECT_HEIGHT( this->windowPlacement.destination );
	if ( ( RECT_WIDTH( rt ) != sz.cx ) || ( RECT_HEIGHT( rt ) != sz.cy ) ) {
		this->paintParam.updateDC( sz.cx, sz.cy );
	}
}

void window::slideBegin() {
	if ( this->windowPlacement.sliding ) {
		this->windowPlacement.startTime -= this->windowPlacement.timeout - 2 * ( timeGetTime() - this->windowPlacement.startTime );
	} else {
		this->windowPlacement.sliding = true;
		//
		this->windowPlacement.startTime =
			this->windowPlacement.lastTime = timeGetTime();
		SetTimer( this->getHWND(), this->windowPlacement.timerId, USER_TIMER_MINIMUM, NULL );
		this->show( true );
		this->activate();
	}
}

bool operator==( RECT const& l, RECT const& r ) {
	return ( ( l.left == r.left ) && ( l.top == r.top ) &&  ( l.right == r.right ) &&  ( l.bottom == r.bottom ) );
}

bool operator!=( RECT const& l, RECT const& r ) {
	return ( !( l == r ) );
}

template < typename T >
struct slider {
	slider< T > const& operator()( T &r, T const v1, T const v2, LONG const left, DWORD const dt ) const {
		float const x = (float)( left - (LONG)dt);
		float const a = (float)( v2 - v1 ) / (float)( left * left );
		r = v2 - (T)( a * x * x );
		return (*this);
	}
};

void window::slideUpdate() {
	if ( this->windowPlacement.sliding ) {
		DWORD const total = this->windowPlacement.timeout;
		bool const show = this->windowPlacement.visible;
		RECT srect;
		RECT drect = this->windowPlacement.destination;
		GetWindowRect( this->getHWND(), &srect );
		//
		BYTE salpha = 0;
		BYTE dalpha = this->windowPlacement.alpha;
		this->getAlpha( salpha );
		//
		RECT rt = drect;
		BYTE alpha = dalpha;
		//
		DWORD const ctime = timeGetTime();
		DWORD const dt = ctime - this->windowPlacement.lastTime;
		LONG const left = (LONG)this->windowPlacement.startTime + (LONG)this->windowPlacement.timeout - ctime;
		this->windowPlacement.lastTime += dt;
		//
		if ( left > 0 ) {
			slider< LONG > s;
			s	( rt.left,		srect.left,		drect.left, left, dt )
				( rt.top,		srect.top,		drect.top, left, dt )
				( rt.right,		srect.right,	drect.right, left, dt )
				( rt.bottom,	srect.bottom,	drect.bottom, left, dt );
			slider< BYTE > a;
			a	( alpha,		salpha,			dalpha,	left,	dt );
		} else {
			// stop
			this->show( show );
			if ( show ) {
				this->activate().inputCapture( keyboard );
			}
			this->currentMode->show( show );
			KillTimer( this->getHWND(), this->windowPlacement.timerId );
			this->windowPlacement.sliding = false;
			this->invalidate();
		}
		MoveWindow( this->getHWND(), rt.left, rt.top, RECT_WIDTH( rt ), RECT_HEIGHT( rt ), TRUE );
		this->setAlpha( alpha );
	}
}

void window::modeSwitch( size_t const mode ) {
	if ( this->currentMode ) {
		this->currentMode->activate( false );
	}
	this->currentMode = mode_ptr();
	size_t const sz = this->modes.size();
	if ( ( 0 <= mode ) && ( mode < sz ) ) {
		this->currentMode = this->modes[ mode ].get<1>();
		this->sysMenuCheckRadioItem( 0, sz - 1, mode, true );
		this->currentMode->activate( true );
	}
}

void window::exit(){
	PostQuitMessage(0);
}
