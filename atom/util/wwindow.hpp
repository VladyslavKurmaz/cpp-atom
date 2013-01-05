/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.02.24 - Created                                           |
|              2012.01.03 - has been moved to github/atom/util                |
|-----------------------------------------------------------------------------|
| TODO:		                                                                  |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_UTIL_WWINDOW_HPP
#define ATOM_UTIL_WWINDOW_HPP
#ifdef _WINDOWS
//
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
//
#include <sstream>
#include <vector>
//
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

#define	ATOM_UTIL_WWINDOW_PROP	_T("wwindow")
namespace atom {

	class subclass
	{
	private:
		///
		HWND
			wnd;
		///
		WNDPROC
			proc;

	protected:
		///
		bool is_valid( HWND w, WNDPROC p ) const
		{
			return ( ( w != 0 ) && ( p != 0 ) );
		}
		///
		WNDPROC swap( HWND w, WNDPROC p )
		{
			WNDPROC result = reinterpret_cast< WNDPROC >( GetWindowLong( w,
#ifndef Z3D_OS_X64
				GWL_WNDPROC
#else
				GWLP_WNDPROC
#endif
				) );
			SetClassLong( w,
#ifndef Z3D_OS_X64
				GWL_WNDPROC
#else
				GWLP_WNDPROC
#endif
				, reinterpret_cast< LONG >( p ) );
			return result;
		}

	public:
		///
		subclass() : 
				wnd( 0 )
			  ,	proc( NULL )
		  {
		  }
		  ///
		  ~subclass()
		  {
		  }
		  ///
		  WNDPROC sub( HWND w, WNDPROC p )
		  {
			  WNDPROC result = NULL;
			  if ( is_valid( w, p ) )
			  {
				  result = unsub();
				  this->proc = swap( ( this->wnd = w ), p );
			  }
			  return result;
		  }
		  ///
		  WNDPROC unsub()
		  {
			  WNDPROC result = NULL;
			  if ( is_valid( this->wnd, this->proc ) )
			  {
				  result = swap( this->wnd, this->proc );
				  this->wnd		= 0;
				  this->proc	= NULL;
			  }
			  return result;
		  }
		  ///
		  LRESULT call( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
		  {
			  if ( this->proc != NULL )
			  {
				  return ( CallWindowProc( this->proc, hWnd, uMsg, wParam, lParam ) );
			  }
			  return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
		  }
	};

	//------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------
	class wwindow :	public boost::noncopyable
	{
		typedef TCHAR
			char_t;
		typedef std::basic_string< char_t >
			string_t;
		typedef std::basic_stringstream< char_t >
			stringstream_t;
		
	private:
		///
		ATOM
			class_atom;
		///
		HWND
			wnd;
		///
		WNDCLASSEX
			wcex;
		///
		CREATESTRUCT
			cs;
		///
		subclass
			subcl;
		///
		bool
			auto_destroy;

	public:
		///
		wwindow() :
				class_atom( 0 )
			  ,	wnd( 0 )
			  ,	wcex()
			  ,	cs()
			  ,	subcl()
			  ,	auto_destroy( false )
		  {
			  memset( &wcex, 0, sizeof( wcex ) );
			  memset( &cs, 0, sizeof( cs ) );
		  }
		  ///
		  ~wwindow()
		  {
			  deinit();
		  }
		  ///
		  wwindow const& show( bool const s ) const
		  {
			  ShowWindow( this->wnd, ( ( s )?( SW_SHOW ):( SW_HIDE ) ) );
			  return ( *this );
		  }
		  ///
		  bool init( boost::function< bool ( WNDCLASSEX&, CREATESTRUCT& )> configure, bool const ad )
		  {
			  deinit();
			  {
				  stringstream_t ss;
				  ss << ATOM_UTIL_WWINDOW_PROP << rand() << rand();
				  string_t className( ss.str() );
				  //
				  this->wcex.cbSize			= sizeof( this->wcex );
				  this->wcex.style			= 0;
				  this->wcex.lpfnWndProc	= proc;
				  this->wcex.cbClsExtra		= 0;
				  this->wcex.cbWndExtra		= 0;
				  this->wcex.hInstance		= GetModuleHandle( NULL );
				  this->wcex.hIcon			= 0;
				  this->wcex.hCursor		= LoadCursor( NULL, MAKEINTRESOURCE( IDC_ARROW ) );
				  this->wcex.hbrBackground	= reinterpret_cast< HBRUSH >( 0 );
				  this->wcex.lpszMenuName	= NULL;
				  this->wcex.lpszClassName	= className.c_str();
				  this->wcex.hIconSm		= 0;
				  //
				  this->cs.lpCreateParams	= 0;
				  this->cs.hInstance		= this->wcex.hInstance;
				  this->cs.hMenu			= 0;
				  this->cs.hwndParent		= 0;
				  this->cs.cy				= 0;
				  this->cs.cx				= 0;
				  this->cs.y				= 0;
				  this->cs.x				= 0;
				  this->cs.style			= 0;
				  this->cs.lpszName			= 0;
				  this->cs.lpszClass		= 0;
				  this->cs.dwExStyle		= 0;
				  //
				  if ( configure( this->wcex, this->cs ) )
				  {
					  this->wnd = CreateWindowEx(
						  this->cs.dwExStyle,
						  reinterpret_cast< LPCTSTR >( this->class_atom = RegisterClassEx( &this->wcex ) ),
						  this->cs.lpszName,
						  this->cs.style,
						  this->cs.x,
						  this->cs.y,
						  this->cs.cx,
						  this->cs.cy,
						  this->cs.hwndParent,
						  this->cs.hMenu,
						  this->wcex.hInstance,
						  this->cs.lpCreateParams );
					  if ( this->wnd != 0 )
					  {
						  if ( this->wcex.lpfnWndProc != proc )
						  {
							  this->subcl.sub( this->wnd, proc );
						  }
						  SetProp( this->wnd, ATOM_UTIL_WWINDOW_PROP, this );
						  this->auto_destroy = ad;
						  return true;
					  }
				  }
			  }
			  return false;
		  }
		  ///
		  bool init( HWND w, bool const ad )
		  {
			  deinit();
			  {
				  if ( w != 0 )
				  {
					  wwindow* win = NULL;
					  if ( get_window_object( w, win ) == 0 )
					  {
						  subcl.sub( ( this->wnd = w ), proc );
						  SetProp( this->wnd, ATOM_UTIL_WWINDOW_PROP, this );
						  this->auto_destroy = ad;
						  return true;
					  }
				  }
			  }
			  return false;
		  }
		  ///
		  static void run( boost::function< bool() > tick )
		  {
			  MSG msg = { 0 };
			  bool cont = true;
			  do
			  {
				  while ( cont && PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
				  {
					  if ( ( cont = ( msg.message != WM_QUIT ) ) == true )
					  {
						  if ( !process_dlg_msgs( msg.hwnd, msg ) )
						  {
							  TranslateMessage( &msg );
							  DispatchMessage( &msg );
						  }
					  }
				  }
			  }
			  while( cont && tick() );
		  }
		  ///
		  static void run()
		  {
			  MSG msg;
			  BOOL bRet;
			  while( ( bRet = GetMessage( &msg, 0, 0, 0 ) ) != 0 )
			  {
				  if ( bRet == -1 )
				  {
				  }
				  else
				  {
					  if ( !process_dlg_msgs( msg.hwnd, msg ) )
					  {
						  TranslateMessage(&msg); 
						  DispatchMessage(&msg); 
					  }
				  }
			  }
		  }
		  ///
		  static void calc_rect( RECT& rect, DWORD const style, DWORD const ex_style, bool const menu, bool const center )
		  {
			  AdjustWindowRectEx( &rect, style, (menu)?(TRUE):(FALSE), ex_style );
			  if ( center )
				  OffsetRect( 
				  &rect, 
				  ( GetSystemMetrics( SM_CXFULLSCREEN ) - ( rect.right - rect.left ) ) / 2,
				  ( GetSystemMetrics( SM_CYFULLSCREEN ) - ( rect.bottom - rect.left ) ) /2 );
			  else
				  OffsetRect( &rect, -rect.left, -rect.top );
		  }
		  ///
		  static void exit()
		  {
			  PostQuitMessage( 0 );
		  }

	protected:
		  ///
		  void deinit()
		  {
			  if ( this->wnd != 0 )
			  {
				  RemoveProp( this->wnd, ATOM_UTIL_WWINDOW_PROP );
				  this->subcl.unsub();
				  if ( this->auto_destroy )
				  {
					  DestroyWindow( this->wnd );
				  }
				  if ( this->class_atom != 0 )
				  {
					  UnregisterClass( reinterpret_cast< LPCTSTR >( this->class_atom ), this->wcex.hInstance );
				  }
				  this->class_atom	= 0;
				  this->wnd			= 0;
				  this->auto_destroy= false;
			  }
		  }
		///
		static bool process_dlg_msgs( HWND hDlg, MSG& msg )
		{ return ( hDlg && IsDialogMessage( hDlg, &msg ) ); }
		///
		static bool get_window_object( HWND w, wwindow*& win )
		{
			return ( ( win = reinterpret_cast< wwindow* >( GetProp( w, ATOM_UTIL_WWINDOW_PROP ) ) ) != NULL );
		}
		///
		static void on_close( HWND /*hWnd*/ )
		{
			PostQuitMessage( 0 );
		}
		//
		static LRESULT CALLBACK proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
		{
			switch( uMsg )
			{
			//	// window
			//	HANDLE_MSG(	hWnd,	WM_GETMINMAXINFO,	OnGetMinMaxInfo );
				HANDLE_MSG(	hWnd,	WM_CLOSE,			on_close );
			//	HANDLE_MSG(	hWnd,	WM_DESTROY,			OnDestroy );
			//	HANDLE_MSG(	hWnd,	WM_PAINT,			OnPaint );
			//	HANDLE_MSG(	hWnd,	WM_MOVE,			OnMove );
			//	// keyboard
			//	HANDLE_MSG(	hWnd,	WM_CHAR,			OnChar );
			//	// mouse
			//	HANDLE_MSG(	hWnd,	WM_LBUTTONDOWN,		OnLButtonDown );
			//	HANDLE_MSG(	hWnd,	WM_LBUTTONUP,		OnLButtonUp );
			//	HANDLE_MSG(	hWnd,	WM_MOUSEMOVE,		OnMouseMove );
			//	HANDLE_MSG(	hWnd,	WM_CAPTURECHANGED,	OnReleaseCapture );
			//case WM_SETTINGCHANGE:
			//	{
			//		break;
			//	}
			}
			wwindow* win = NULL;
			if ( get_window_object( hWnd, win ) )
				return ( win->subcl.call( hWnd, uMsg, wParam, lParam ) );
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
		}

	};
}


#endif
#endif // ATOM_UTIL_WWINDOW_HPP


#if 0

#ifndef Z3D_UTIL_WND_HPP
#define Z3D_UTIL_WND_HPP

#ifdef Z3D_WINDOWS
//
#include <windows.h>
#include <windowsx.h>
//
#include <sstream>
#include <vector>
//
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
//
#include <z3d/chain_of_resp.hpp>
#include <z3d/util/msgs.hpp>

namespace z3d { namespace util {
	//------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------
	inline LONG rwidth( RECT const & _rect )
	{
		return ( _rect.right - _rect.left );
	}
	//------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------
	inline LONG rheight( RECT const & _rect )
	{
		return ( _rect.bottom - _rect.top );
	}
	//------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------
	template < class T >
	class subclass;
	///
	template <>
	class subclass<HWND> : public boost::noncopyable
	{
	private:
		///
		HWND
			wnd;
		///
		WNDPROC
			old_wnd_proc;

	protected:
		///
		bool is_valid( HWND _wnd, WNDPROC _wnd_proc )
		{
			return ( ( _wnd != 0 ) && ( _wnd_proc != 0 ) );
		}
		///
		WNDPROC swap( HWND _wnd, WNDPROC _wnd_proc )
		{
			WNDPROC result = reinterpret_cast< WNDPROC >( GetWindowLong( _wnd,
#ifndef Z3D_OS_X64
				GWL_WNDPROC
#else
				GWLP_WNDPROC
#endif
				) );
			SetClassLong( _wnd,
#ifndef Z3D_OS_X64
				GWL_WNDPROC
#else
				GWLP_WNDPROC
#endif
				, reinterpret_cast< LONG >( _wnd_proc ) );
			return result;
		}

	public:
		///
		subclass() : 
		  wnd( 0 )
			  ,	old_wnd_proc( 0 )
		  {
		  }
		  ///
		  ~subclass()
		  {
		  }
		  ///
		  WNDPROC sub( HWND _wnd, WNDPROC _wnd_proc )
		  {
			  WNDPROC result = unsub();
			  if ( is_valid( _wnd, _wnd_proc ) )
			  {
				  this->old_wnd_proc = swap( ( this->wnd = _wnd ), _wnd_proc );
			  }
			  return result;
		  }
		  ///
		  WNDPROC unsub()
		  {
			  WNDPROC result = 0;
			  if ( is_valid( this->wnd, this->old_wnd_proc ) )
			  {
				  result = swap( this->wnd, this->old_wnd_proc );
				  this->wnd			= 0;
				  this->old_wnd_proc = 0;
			  }
			  return result;
		  }
		  ///
		  LRESULT call( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
		  {
			  if ( this->old_wnd_proc != 0 )
			  {
				  if ( !::CallWindowProc( this->old_wnd_proc, hWnd, uMsg, wParam, lParam ) )
					  return 0;
			  }
			  return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
		  }
	};

	//------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------
	typedef boost::function< void( HWND ) >
		window_onpaint_t;
	typedef boost::function< void( HWND, BOOL, int, int, UINT ) >
		window_onlbuttondown_t;
	typedef boost::function< void( HWND, int, int, UINT ) >
		window_onlbuttonup_t;
	typedef boost::function< void( HWND, int, int, UINT ) >
		window_onmousemove_t;
	typedef boost::function< void( HWND, WPARAM, HWND ) >
		window_onreleasecapture_t;
	typedef boost::function< void( HWND, int, int ) >
		window_onmove_t;
	typedef boost::function< void( HWND, TCHAR, int ) >
		window_onchar_t;

	//------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------
	///
	enum resizing_mode_t
	{
		rmNormal = 0,
		rmTabularResizing = 2,
	};
	///
	enum placementmode_t
	{
		pmNormal = 0,
		pmWorkAreaWithMenu = 1,
		pmWorkArea = 2,
		pmDisplayWithMenu = 3,
		pmDisplay = 4
	};
	const TCHAR mPropName[] = _T( "window" );

	template < typename charT >
	class window :	public boost::noncopyable,
		public z3d::chain_of_resp< LOKI_TYPELIST_7( 
		window_onpaint_t,
		window_onlbuttondown_t,
		window_onlbuttonup_t,
		window_onmousemove_t,
		window_onreleasecapture_t,
		window_onmove_t,
		window_onchar_t ) >
	{
		typedef charT
			char_t;
		typedef window< char_t >
			window_t;
		typedef std::basic_string< char_t >
			string_t;

	private:
		///
		ATOM
			class_atom;
		///
		HWND
			wnd;
		///
		WNDCLASSEX
			wcex;
		///
		CREATESTRUCT
			cs;
		///
		WINDOWPLACEMENT
			placement;
		///
		subclass<HWND>
			subcl;
		///
		bool
			portrait;
		///
		bool
			client_area_mode;
		///
		placementmode_t
			placementmode;
		///
		resizing_mode_t
			resizing_mode;
		///
		struct window_size
		{
			int width;
			int height;
		};
		typedef std::vector<window_size>
			sizes_t;
		sizes_t
			sizes;

	public:
		///
		window() :
		  class_atom( 0 )
			  ,	wnd( 0 )
			  ,	wcex()
			  ,	cs()
			  ,	placement()
			  ,	subcl()
			  ,	portrait( false )
			  ,	client_area_mode( true )
			  ,	placementmode( pmNormal )
			  ,	resizing_mode( rmNormal )
			  ,	sizes()
		  {
			  memset( &wcex, 0, sizeof( wcex ) );
			  memset( &cs, 0, sizeof( cs ) );
			  memset( &placement, 0, sizeof( placement ) );
			  placement.length = sizeof( placement );
		  }
		  ///
		  ~window()
		  {
			  deinit();
		  }
		  ///
		  window_t& set_client_area_mode( bool const _client_area_mode )
		  {
			  this->client_area_mode = _client_area_mode;
			  return *this;
		  }
		  ///
		  bool get_client_area_mode() const
		  { return ( this->client_area_mode ); }
		  ///
		  window_t& set_placementmode( placementmode_t const _placementmode )
		  {
			  const bool changed = ( this->placementmode != _placementmode );
			  bool menu = is_menu_exist();
			  RECT rect;
			  SetRectEmpty( &rect );
			  switch( _placementmode )
			  {
			  case pmNormal:
				  {
					  break;
				  }
			  case pmWorkAreaWithMenu:
				  {
					  menu = false;
				  }
			  case pmWorkArea:
				  {
					  SystemParametersInfo( SPI_GETWORKAREA, 0, &rect, 0 );
					  break;
				  }
			  case pmDisplayWithMenu:
				  {
					  menu = false;
				  }
			  case pmDisplay:
				  {
					  SetRect( &rect, 0, 0, GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ) );
					  break;
				  }
			  }
			  if ( _placementmode != pmNormal )
			  {
				  if ( _placementmode == pmNormal )
				  {
					  GetWindowPlacement( this->wnd, &this->placement );
				  }
				  //RECT r = rect;
				  AdjustWindowRectEx( &rect, get_style(), menu, get_style_ex() );
				  MoveWindow( this->wnd, rect.left, rect.top, rwidth( rect ), rheight( rect ), TRUE );
			  }
			  else
			  {
				  if ( changed )
				  {
					  SetWindowPlacement( this->wnd, &this->placement );
				  }
			  }
			  this->placementmode = _placementmode;
			  InvalidateRect( this->wnd, 0, TRUE );
			  return ( *this );
		  }
		  ///
		  placementmode_t get_placementmode() const
		  { return this->placementmode; }
		  ///
		  window_t& set_resizing_mode( resizing_mode_t const _resizing_mode )
		  {
			  this->resizing_mode = _resizing_mode;
			  return *this;
		  }
		  ///
		  resizing_mode_t get_resizing_mode() const
		  { return ( this->resizing_mode ); }
		  ///
		  HWND get_handle()
		  { return ( this->wnd ); }
		  ///
		  window_t& add_size( int const width, int const height )
		  {
			  window_size sz;
			  sz.width = width;
			  sz.height = height;
			  this->sizes.push_back( sz );
			  return *this;
		  }
		  ///
		  window_t& show( bool const _show )
		  {
			  ShowWindow( this->wnd, ( ( _show )?( SW_SHOW ):( SW_HIDE ) ) );
			  return ( *this );
		  }
		  ///
		  window_t& center( bool const _client_area, bool const _work_area )
		  {
			  RECT rt;
			  RECT wa;
			  if ( _client_area )
			  {
				  GetClientRect( this->wnd, &rt );
			  }
			  else
			  {
				  GetWindowRect( this->wnd, &rt );
			  }
			  //
			  if ( _work_area )
			  {
				  SystemParametersInfo( SPI_GETWORKAREA, 0, &wa, 0 );
			  }
			  else
			  {
				  SetRect( &wa, 0, 0, GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ) );
			  }
			  //
			  OffsetRect( &rt, -rt.left + ( rwidth( wa ) - rwidth( rt ) ) / 2, -rt.top + ( rheight( wa ) - rheight( rt ) ) / 2 );
			  if ( _client_area )
			  {
				  AdjustWindowRectEx( &rt, get_style(), is_menu_exist(), get_style_ex() );
				  MapWindowPoints( this->wnd, 0, reinterpret_cast<LPPOINT>(&rt), 2 );
			  }
			  MoveWindow( this->wnd, rt.left, rt.top, rwidth( rt ), rheight( rt ), TRUE );
			  GetWindowPlacement( this->wnd, &this->placement );
			  return ( *this );
		  }
		  ///
		  bool init( boost::function< bool ( WNDCLASSEX&, CREATESTRUCT& )> configure )
		  {
			  deinit();
			  {
				  std::basic_stringstream<TCHAR> ss;
				  ss << _T("CLASS") << rand() << rand();
				  string_t className( ss.str() );
				  //
				  this->wcex.cbSize			= sizeof( this->wcex );
				  this->wcex.style			= 0;
				  this->wcex.lpfnWndProc		= WindowProc;
				  this->wcex.cbClsExtra		= 0;
				  this->wcex.cbWndExtra		= 0;
				  this->wcex.hInstance		= 0;
				  this->wcex.hIcon			= 0;
				  this->wcex.hCursor			= 0;
				  this->wcex.hbrBackground	= reinterpret_cast< HBRUSH >( 0 );
				  this->wcex.lpszMenuName	= NULL;
				  this->wcex.lpszClassName	= className.c_str();
				  this->wcex.hIconSm			= 0;
				  //
				  this->cs.lpCreateParams	= 0;
				  this->cs.hInstance			= 0;
				  this->cs.hMenu				= 0;
				  this->cs.hwndParent		= 0;
				  this->cs.cy				= 0;
				  this->cs.cx				= 0;
				  this->cs.y					= 0;
				  this->cs.x					= 0;
				  this->cs.style				= 0;
				  this->cs.lpszName			= 0;
				  this->cs.lpszClass			= 0;
				  this->cs.dwExStyle			= 0;
				  //
				  if ( configure( this->wcex, this->cs ) )
				  {
					  calc_size( this->cs.cx, this->cs.cy );
					  this->wnd = CreateWindowEx(
						  this->cs.dwExStyle,
						  reinterpret_cast< LPCTSTR >( this->class_atom = RegisterClassEx( &this->wcex ) ),
						  this->cs.lpszName,
						  this->cs.style,
						  this->cs.x,
						  this->cs.y,
						  this->cs.cx,
						  this->cs.cy,
						  this->cs.hwndParent,
						  this->cs.hMenu,
						  this->wcex.hInstance,
						  this->cs.lpCreateParams );
					  if ( this->wnd != 0 )
					  {
						  GetWindowPlacement( this->wnd, &this->placement );
						  if ( this->wcex.lpfnWndProc != WindowProc )
						  {
							  this->subcl.sub( this->wnd, WindowProc );
						  }
						  SetProp( this->wnd, mPropName, this );
						  return true;
					  }
				  }
			  }
			  return false;
		  }
		  ///
		  bool init( HWND _wnd )
		  {
			  deinit();
			  {
				  if ( _wnd != 0 )
				  {
					  if ( get_window_object( _wnd ) == 0 )
					  {
						  subcl.sub( ( this->wnd = _wnd ), WindowProc );
						  SetProp( this->wnd, mPropName, this );
						  return true;
					  }
				  }
			  }
			  return false;
		  }
		  ///
		  bool run( HWND hDlg, boost::function< bool() > tick )
		  {
			  MSG msg = { 0 };
			  bool cont = true;
			  do
			  {
				  while ( cont && PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
				  {
					  if ( ( cont = ( msg.message != WM_QUIT ) ) == true )
					  {
						  if ( !process_dlg_msgs( hDlg, msg ) )
						  {
							  TranslateMessage(&msg);
							  DispatchMessage(&msg);
						  }
					  }
				  }
			  }
			  while( cont && tick() );
			  return true; 
		  }
		  ///
		  bool run( HWND hDlg )
		  {
			  MSG msg;
			  BOOL bRet;
			  while( ( bRet = GetMessage( &msg, 0, 0, 0 ) ) != 0 )
			  {
				  if ( bRet == -1 )
				  {
				  }
				  else
				  {
					  if ( !process_dlg_msgs( hDlg, msg ) )
					  {
						  TranslateMessage(&msg); 
						  DispatchMessage(&msg); 
					  }
				  }
			  }
			  return true;
		  }
		  ///
		  void exit()
		  {
			  PostMessage( this->wnd, WM_CLOSE, 0, 0 ) ;
		  }
		  ///
		  void deinit()
		  {
			  if ( this->wnd != 0 )
			  {
				  RemoveProp( this->wnd, mPropName );
				  this->subcl.unsub();
				  if ( this->class_atom != 0 )
				  {
					  DestroyWindow( this->wnd );
					  UnregisterClass( reinterpret_cast< LPCTSTR >( this->class_atom ), this->wcex.hInstance );
				  }
				  this->class_atom	= 0;
				  this->wnd			= 0;
			  }
		  }

	protected:
		///
		bool process_dlg_msgs( HWND hDlg, MSG& msg )
		{ return ( hDlg && IsDialogMessage( hDlg, &msg ) ); }
		///
		DWORD get_style() const
		{
			//??????check current placement mode
			return this->cs.style; }
		///
		DWORD get_style_ex() const
		{
			//??????check current placement mode
			return this->cs.dwExStyle; }
		///
		bool is_menu_exist() const
		{
			return ( GetMenu( this->wnd ) != 0 );
		}
		///
		void calc_size( int& width, int& height )
		{
			if ( get_client_area_mode() )
			{
				RECT rt;
				SetRect( &rt, 0, 0, width, height );
				AdjustWindowRectEx( &rt, get_style(), ( GetMenu( this->wnd ) != 0 ), get_style_ex() );
				OffsetRect( &rt, -rt.left, -rt.top );
				width = rt.right;
				height = rt.bottom;
			}
		}
		///
		static bool get_window_object( HWND _wnd, window_t*& wnd )
		{
			wnd = reinterpret_cast< window* >( GetProp( _wnd, mPropName ) );
			return ( wnd != NULL );
		}
		///
		static void OnGetMinMaxInfo( HWND hWnd, LPMINMAXINFO lpMinMaxInfo )
		{
			RECT rt;
			GetWindowRect( hWnd, &rt );
			Sleep( 0 );
			lpMinMaxInfo->ptMaxTrackSize.x += 150;
			lpMinMaxInfo->ptMaxTrackSize.y += 150;
		}
		///
		static void OnClose( HWND hWnd )
		{
			DestroyWindow( hWnd );
		}
		///
		static void OnDestroy( HWND /*hWnd*/ )
		{
			PostQuitMessage( 0 );
		}
		///
		static void OnPaint( HWND hWnd )
		{
			window_t* window = NULL;
			if ( get_window_object( hWnd, window ) )
			{
				window_onpaint_t f = window->call<window_onpaint_t>();
				if ( !f.empty() )
					f( hWnd );
			}
		}
		static void OnMove( HWND hWnd, int x, int y )
		{
			window_t* window = NULL;
			if ( get_window_object( hWnd, window ) )
			{
				window_onmove_t f = window->call<window_onmove_t>();
				if ( !f.empty() )
					f( hWnd, x, y );
			}
		}
		///
		static void OnChar( HWND hWnd, TCHAR ch, int cRepeat )
		{
			window_t* window = NULL;
			if ( get_window_object( hWnd, window ) )
			{
				window_onchar_t f = window->call<window_onchar_t>();
				if ( !f.empty() )
					f( hWnd, ch, cRepeat );
			}
		}
		///
		static void OnLButtonDown( HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags )
		{
			window_t* window = NULL;
			if ( get_window_object( hWnd, window ) )
			{
				window_onlbuttondown_t f = window->call<window_onlbuttondown_t>();
				if ( !f.empty() )
					f( hWnd, fDoubleClick, x, y, keyFlags );
			}
		}
		///
		static void OnLButtonUp( HWND hWnd, int x, int y, UINT keyFlags )
		{
			window_t* window = NULL;
			if ( get_window_object( hWnd, window ) )
			{
				window_onlbuttonup_t f = window->call<window_onlbuttonup_t>();
				if ( !f.empty() )
					f( hWnd, x, y, keyFlags );
			}
		}
		///
		static void OnMouseMove( HWND hWnd, int x, int y, UINT keyFlags )
		{
			window_t* window = NULL;
			if ( get_window_object( hWnd, window ) )
			{
				window_onmousemove_t f = window->call<window_onmousemove_t>();
				if ( !f.empty() )
					f( hWnd, x, y, keyFlags );
			}
		}
		///
		static void OnReleaseCapture( HWND hWnd, WPARAM wParam, HWND hWndGaining )
		{
			window_t* window = NULL;
			if ( get_window_object( hWnd, window ) )
			{
				window_onreleasecapture_t f = window->call<window_onreleasecapture_t>();
				if ( !f.empty() )
					f( hWnd, wParam, hWndGaining );
			}
		}

		static LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
		{
			switch( uMsg )
			{
				// window
				HANDLE_MSG(	hWnd,	WM_GETMINMAXINFO,	OnGetMinMaxInfo );
				HANDLE_MSG(	hWnd,	WM_CLOSE,			OnClose );
				HANDLE_MSG(	hWnd,	WM_DESTROY,			OnDestroy );
				HANDLE_MSG(	hWnd,	WM_PAINT,			OnPaint );
				HANDLE_MSG(	hWnd,	WM_MOVE,			OnMove );
				// keyboard
				HANDLE_MSG(	hWnd,	WM_CHAR,			OnChar );
				// mouse
				HANDLE_MSG(	hWnd,	WM_LBUTTONDOWN,		OnLButtonDown );
				HANDLE_MSG(	hWnd,	WM_LBUTTONUP,		OnLButtonUp );
				HANDLE_MSG(	hWnd,	WM_MOUSEMOVE,		OnMouseMove );
				HANDLE_MSG(	hWnd,	WM_CAPTURECHANGED,	OnReleaseCapture );
			case WM_SETTINGCHANGE:
				{
					break;
				}
			}
			window_t* window = NULL;
			if ( get_window_object( hWnd, window ) )
				return ( window->subcl.call( hWnd, uMsg, wParam, lParam ) );
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
		}

	};
} }
#endif
#endif//Z3D_UTIL_WND_HPP



#endif
