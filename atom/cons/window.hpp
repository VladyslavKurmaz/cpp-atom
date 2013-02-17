#pragma once

#include <atom/util/wwindow.hpp>
#include "./log.hpp"
#include "./pref.hpp"
#include "./frame.hpp"
#include "./window.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > window2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > window2pref;
typedef atom::nstorage< frame, boost::shared_ptr, atom::nlist > window2frame;
//
class window;
typedef void(window::* onchar_t)( HWND, TCHAR, int );
typedef void(window::* onhotkey_t)( HWND, int, UINT, UINT);
typedef void(window::* onpaint_t)( HWND );
typedef void(window::* onclose_t)( HWND );



typedef boost::mpl::pair< boost::mpl::int_< WM_CHAR >::type, onchar_t >::type
	onchar_pair_type_t;
typedef boost::mpl::pair< boost::mpl::int_< WM_HOTKEY >::type, onhotkey_t >::type
	onhotkey_pair_type_t;
typedef boost::mpl::pair< boost::mpl::int_< WM_PAINT >::type, onpaint_t >::type
	onpaint_pair_type_t;
typedef boost::mpl::pair< boost::mpl::int_< WM_CLOSE >::type, onclose_t >::type
	onclose_pair_type_t;

class window :	public atom::wwindow< window, LOKI_TYPELIST_4( onchar_pair_type_t, onhotkey_pair_type_t, onpaint_pair_type_t, onclose_pair_type_t ) >,
				public atom::node< LOKI_TYPELIST_3( window2logger, window2pref, window2frame ) >
{
	typedef atom::wwindow< window, LOKI_TYPELIST_4( onchar_pair_type_t, onhotkey_pair_type_t, onpaint_pair_type_t, onclose_pair_type_t ) >
		base_window_t;
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
	void onchar( HWND hWnd, TCHAR ch, int cRepeat );
	///
	void onhotkey(HWND hWnd, int idHotKey, UINT fuModifiers, UINT vk);
	///
	void onpaint( HWND hWnd );
	///
	void onclose( HWND );

protected:
	//
	logger& get_logger() {
		return ( *( get_value( boost::mpl::identity< window2logger >() ).item() ) );
	}

private:
	///
	window( logger::shared_ptr l, pref::shared_ptr p );
};

