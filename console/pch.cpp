#include "./pch.hpp"

#pragma comment(lib, "winmm.lib")

// https://connect.microsoft.com/PowerShell/feedback/details/572313/powershell-exe-can-hang-if-stdin-is-redirected
// http://en.wikipedia.org/wiki/ANSI_escape_code
// http://support.microsoft.com/default.aspx?scid=kb;EN-US;q190351&wa=wsignin1.0
//SetWinEventHook.
// http://stackoverflow.com/questions/2175665/createprocess-process-do-not-terminate-when-redirecting-std-out-in-err

const TCHAR CMDID_SPLIT_NAME[]		= _T( "split" );
const TCHAR CMDID_EXPAND_NAME[]		= _T( "expand" );
const TCHAR CMDID_ROTATE_NAME[]		= _T( "rotate" );
const TCHAR CMDID_NEXT_NAME[]		= _T( "next" );
const TCHAR CMDID_PREV_NAME[]		= _T( "prev" );

const conf_cmd_t conf_cmds[] = {
	{ CMDID_SPLIT_NAME,		CMDID_SPLIT,	_T("split current frame") },
	{ CMDID_EXPAND_NAME,	CMDID_EXPAND,	_T("maximize/restore frame") },
	{ CMDID_ROTATE_NAME,	CMDID_ROTATE,	_T("rotate frames") },
	{ CMDID_NEXT_NAME,		CMDID_NEXT,		_T("focus next frame") },
	{ CMDID_PREV_NAME,		CMDID_PREV,		_T("focus prev frame") },
};
const size_t conf_cmds_cnt = sizeof( conf_cmds ) / sizeof( conf_cmds[0] );

const TCHAR DELIM1[] = _T( ";" );
const TCHAR DELIM2[] = _T( ":" );

int atom::node_cnt = 0;


/*




	void window::onKey( HWND hWnd, UINT vk, BOOL down, int repeat, UINT flags ){
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
	if ( this->current_frame ) {
		this->current_frame->process( bridge_msg::bmKbrd, &key );
	}
	this->invalidate();

}



void window::onPaint( HWND hWnd ){
	struct context {
		HWND			wnd;
		PAINTSTRUCT 	ps; 
		HDC 			dc; 
		RECT 			rect;
		paint_param_t&	pp;
		frame_ptr		current;
		bool			focused;

		context( HWND w, window& pw ) : pp( pw.paint_param ), wnd( w ) {
			dc = BeginPaint( wnd, &ps );
			GetClientRect( wnd, &rect );
			current		= pw.current_frame;
			focused		= ( GetFocus() == wnd );
			//
			FillRect( pp.mem_dc, &rect, pp.bk_brush );
		}
		~context() {
			
			EndPaint( wnd, &ps );
		}
	} c( hWnd, *this );
	//
	
}



	void window::processUI() {
	//
	this->invalidate();
}

void window::processAutostart() {
	//Registry.SetValue(@"HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run",
	//          "MyStartUp",
	//          @"C:\StartUpApp.exe");
}

void
window::parseConf( TCHAR const* conf ) {
	std::string s( conf );
	//
	for( size_t i = 0; i < conf_cmds_cnt; ++i ) {
		if ( ( s == conf_cmds[i].cmd ) && ( conf_cmds[i].id ) ) {
			SendMessage( this->get_hwnd(), WM_COMMAND, MAKEWPARAM( conf_cmds[i].id, 0 ), 0 );
			return;
		}
	}
	// check command line options
	try {
		this->getPref().parse( std::string( conf ) );
	}
	catch ( std::exception& e ) {
		//send exception description back to the console process
	}
}


void
window::closeFrame( frame_id_t const id ) {
	frame_ptr f = this->head_area->get_by_id( id );
	if ( f ) {
		//
		if ( this->current_frame == f ) {
			this->expand_mode = false;
			this->current_frame = this->head_area->find( f )->next();
			if ( this->current_frame == f ) {
				this->current_frame = frame_ptr();
			}
		}
		//
		this->head_area->find( f )->close();
		f->clear();
		//
		// exit application
		if ( !this->current_frame ) {
		}
	}
	this->invalidate();
}

*/