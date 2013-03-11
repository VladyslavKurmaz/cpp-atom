#pragma once

#include <atom/util/wwindow.hpp>
#include <atom/util/waccel.hpp>
#include <atom/util/ptr.hpp>
#include "./log.hpp"
#include "./pref.hpp"
#include "./frame.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > window2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > window2pref;
typedef atom::nstorage< frame, boost::shared_ptr, atom::nlist > window2frame;
//
class window;

typedef void(window::* onchar_t)( HWND, TCHAR, int );
typedef boost::mpl::pair< boost::mpl::int_< WM_CHAR >::type, onchar_t >::type
	onchar_pair_type_t;

typedef void(window::* onhotkey_t)( HWND, int, UINT, UINT);
typedef boost::mpl::pair< boost::mpl::int_< WM_HOTKEY >::type, onhotkey_t >::type
	onhotkey_pair_type_t;

typedef void(window::* onpaint_t)( HWND );
typedef boost::mpl::pair< boost::mpl::int_< WM_PAINT >::type, onpaint_t >::type
	onpaint_pair_type_t;

typedef void(window::* onclose_t)( HWND );
typedef boost::mpl::pair< boost::mpl::int_< WM_CLOSE >::type, onclose_t >::type
	onclose_pair_type_t;

typedef void(window::* onsettingchange_t)( HWND, UINT, LPCTSTR );
typedef boost::mpl::pair< boost::mpl::int_< WM_SETTINGCHANGE >::type, onsettingchange_t >::type
	onsettingchange_pair_type_t;

typedef void(window::* ontimer_t)( HWND, UINT );
typedef boost::mpl::pair< boost::mpl::int_< WM_TIMER >::type, ontimer_t >::type
	ontimer_pair_type_t;

typedef void(window::* oncommand_t)( HWND hWnd, int id, HWND hwndCtl, UINT codeNotify );
typedef boost::mpl::pair< boost::mpl::int_< WM_COMMAND >::type, oncommand_t >::type
	oncommand_pair_type_t;


class window :	public atom::wwindow< window, LOKI_TYPELIST_7( onchar_pair_type_t, onhotkey_pair_type_t, onpaint_pair_type_t, onclose_pair_type_t, onsettingchange_pair_type_t, ontimer_pair_type_t, oncommand_pair_type_t ) >,
				public atom::node< LOKI_TYPELIST_3( window2logger, window2pref, window2frame ) >,
				public boost::enable_shared_from_this< window > {
	typedef atom::wwindow< window, LOKI_TYPELIST_7( onchar_pair_type_t, onhotkey_pair_type_t, onpaint_pair_type_t, onclose_pair_type_t, onsettingchange_pair_type_t, ontimer_pair_type_t, oncommand_pair_type_t ) >
		base_window_t;
	typedef atom::node< LOKI_TYPELIST_3( window2logger, window2pref, window2frame ) >
		base_node_t;
public:
	///
	typedef boost::shared_ptr< window >
		shared_ptr;
	///
	static shared_ptr create( logger::shared_ptr l, pref::shared_ptr p ) {
		return shared_ptr( new window( l, p ) );
	}
	///
	~window();
	///
	bool
		init();
	///
	void
		run();
	///
	void
		clear();
	///
	void onchar( HWND hWnd, TCHAR ch, int cRepeat );
	///
	void onhotkey(HWND hWnd, int idHotKey, UINT fuModifiers, UINT vk);
	///
	void onpaint( HWND hWnd );
	///
	void onclose( HWND );
	///
	void onsettingchange( HWND hWnd, UINT uiAction, LPCTSTR lpName );
	///
	void ontimer( HWND hWnd, UINT id );
	///
	void oncommand( HWND hWnd, int id, HWND hwndCtl, UINT codeNotify );
	///
	window& operator()( atom::po::id_t const opt );

protected:
	//
	logger&
	get_logger() { return ( *( get_value( boost::mpl::identity< window2logger >() ).item() ) ); }
	//
	pref& get_pref()
	{ return ( *( get_value( boost::mpl::identity< window2pref >() ).item() ) ); }
	//
	void
	update_hotkeys();
	//
	void
	update_placement();
	//
	void
	update_position( HWND hWnd, bool dir, float mult );

private:
	///
	window( logger::shared_ptr l, pref::shared_ptr p );
	//
	frame::shared_ptr
		current_frame;
	//
	frame::shared_ptr
		head_frame;
	//
	bool
		expand_mode;
	//
	struct hotkey_t {
		int id;
		UINT mods;
		UINT vk;
		hotkey_t() : id(0), mods(), vk() {}   
		int operator==( hotkey_t const& r ) const { return ( ( this->mods == r.mods ) && ( this->vk == r.vk ) ); }
	} appear_hk;
	//
	atom::accel
		accel;
	//
	POINT
		anchor;
	RECT
		in_rect;
	///
	int
		slide_dir;
	DWORD
		slide_start_time;
	UINT_PTR
		slide_timer_id;
	//
	atom::shared_dc
		mem_dc;
	atom::shared_gdiobj< HBITMAP >
		mem_bitmap;
	atom::shared_gdiobj< HBRUSH >
		bk_brush;
	atom::shared_gdiobj< HBRUSH >
		border_brush;
	atom::shared_gdiobj< HBRUSH >
		scroll_brush;
	atom::shared_gdiobj< HBRUSH >
		font_brush;
	atom::shared_gdiobj< HFONT >
		font;
};

