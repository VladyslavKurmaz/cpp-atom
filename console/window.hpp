#pragma once
#include "./classes.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > window2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > window2pref;
//
class window;

ATOM_DEF_ONKEYDOWN( window )
ATOM_DEF_ONKEYUP( window )
ATOM_DEF_ONCHAR( window )
ATOM_DEF_ONHOTKEY( window )
ATOM_DEF_ONPAINT( window )
ATOM_DEF_ONCLOSE( window )
ATOM_DEF_ONSETTINGCHANGE( window )
ATOM_DEF_ONTIMER( window )
ATOM_DEF_ONCOMMAND( window )

ATOM_DEF_ONLBUTTONDOWN( window )
ATOM_DEF_ONLBUTTONUP( window )
ATOM_DEF_ONMOUSEMOVE( window )
ATOM_DEF_CAPTURECHANGED( window )
ATOM_DEF_ONSYSCOMMAND( window )


class window :	public atom::wwindow< window, LOKI_TYPELIST_14( onkeydown_pair_t, onkeyup_pair_t, onchar_pair_t, onhotkey_pair_t, onpaint_pair_t, onclose_pair_t, onsettingchange_pair_t, ontimer_pair_t, oncommand_pair_t, onlbuttondown_pair_t, onlbuttonup_pair_t, onmousemove_pair_t, oncapturechanged_pair_t, onsyscommand_pair_t ) >,
				public atom::node< LOKI_TYPELIST_2( window2logger, window2pref ) >,
				public boost::enable_shared_from_this< window > {
	typedef atom::wwindow< window, LOKI_TYPELIST_14( onkeydown_pair_t, onkeyup_pair_t, onchar_pair_t, onhotkey_pair_t, onpaint_pair_t, onclose_pair_t, onsettingchange_pair_t, ontimer_pair_t, oncommand_pair_t, onlbuttondown_pair_t, onlbuttonup_pair_t, onmousemove_pair_t, oncapturechanged_pair_t, onsyscommand_pair_t ) >
		base_window_t;
	typedef atom::node< LOKI_TYPELIST_2( window2logger, window2pref ) >
		base_node_t;
public:
	///
	static window_ptr create( logger_ptr l, pref_ptr p ) {
		return window_ptr( new window( l, p ) );
	}
	///
	~window();
	///
	bool
	init();
	///
	void
	run();
	//
	void getPlacement( placement& p ) const { p = this->windowPlacement; }
	//
	void
	clear();
	///
	void onKey( HWND hWnd, UINT vk, BOOL down, int repeat, UINT flags );
	//
	void onChar( HWND hWnd, TCHAR ch, int cRepeat );
	///
	void onHotkey(HWND hWnd, int idHotKey, UINT fuModifiers, UINT vk);
	///
	void onPaint( HWND hWnd );
	///
	void onClose( HWND );
	///
	void onSettingChange( HWND hWnd, UINT uiAction, LPCTSTR lpName );
	///
	void onTimer( HWND hWnd, UINT id );
	///
	void onCommand( HWND hWnd, int id, HWND hwndCtl, UINT codeNotify );
	//
	void onLBDown( HWND, BOOL, int, int, UINT );
	//
	void onLBUp( HWND, int, int, UINT );
	//
	void onMouseMove( HWND, int, int, UINT );
	//
	void onCaptureChanged( HWND, HWND );
	//
	void onSysCommand( HWND hWnd, UINT cmd, int x, int y );

protected:
	//
	logger&
	getLogger() { return ( *( get_slot< window2logger >().item() ) ); }
	logger_ptr
	getLoggerPtr() { return ( get_slot< window2logger >().item() ); }
	//
	pref&
	getPref() { return ( *( get_slot< window2pref >().item() ) ); }
	pref_ptr
	getPrefPtr() { return ( get_slot< window2pref >().item() ); }
	//
	void
	toggleVisibility();
	//
	void
	toggleFullScreen();
	//
	void updatePlacement( bool const visible, bool const fullScreen );
	//
	void slideBegin();
	//
	void slideUpdate();
	///
	void modeSwitch( size_t const mode );

private:
	///
	window( logger_ptr l, pref_ptr p );
	//
	hotkey
		appearHotKey;
	//
	atom::accel
		accel;
	//
	placement
		windowPlacement;
	//
	paint_param_t
		paintParam;
	//
	typedef boost::tuple< std::string, mode_ptr >
		mode_item_t;
	typedef std::vector< mode_item_t >
		modes_t;
	modes_t
		modes;
	mode_ptr
		currentMode;
};

