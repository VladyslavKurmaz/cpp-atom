/*
/-----------------------------------------------------------------------------\
| Copyright � 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.02.24 - Created                                           |
|              2012.01.03 - has been moved to github/atom/util                |
|              2012.02.16 - mpl like message handler                          |
|-----------------------------------------------------------------------------|
| TODO:		                                                                  |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_UTIL_WWINDOW_HPP
#define ATOM_UTIL_WWINDOW_HPP
#if defined(_WIN32)
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
#include <boost/mpl/int.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/pair.hpp>
//
#include <loki/Typelist.h>
//
#include <atom/node/tldefs.hpp>
#include <atom/util/wctrls.hpp>
//

#define	ATOM_UTIL_WWINDOW_PROP	_T("wwindow")
namespace atom {

	class rectCtrl {
	public:
		//
		rectCtrl() : s(), e() {
		}
		//
		void start( int const x, int const y ) {
			this->s.x = x;
			this->s.y = y;
			this->e = this->s; 
		}
		//
		void update( int const x, int const y, bool const move, RECT const& r ) {
			if ( move ) {
				if ( ( r.left <= x ) && ( x < r.right ) ) {
					this->s.x += x - this->e.x;
				}
				if ( ( r.top <= y ) && ( y < r.bottom ) ) {
					this->s.y += y - this->e.y;
				}
			}
			this->e.x = x;
			this->e.y = y;
			checkPt( this->s, r );
			checkPt( this->e, r );
		}
		//
		void getRect(RECT& r) {
			SetRect(&r, min(this->s.x, this->e.x), min(this->s.y, this->e.y), max(this->s.x, this->e.x), max(this->s.y, this->e.y));
		}
		//
		void draw(HWND wnd) {
			RECT r;
			this->getRect(r);
			HDC dc = GetDC(wnd);
			SetTextColor(dc, RGB(128,128,128));
			DrawFocusRect(dc, &r);
			ReleaseDC(wnd, dc);
		}
	protected:
		//
		void checkPt( POINT& p, RECT const& r ) {
			if ( p.x < r.left ) {
				p.x = r.left;
			}
			if ( p.x > r.right ) {
				p.x = r.right;
			}
			if ( p.y < r.top ) {
				p.y = r.top;
			}
			if ( p.y > r.bottom ) {
				p.y = r.bottom;
			}
		}
	private:
		POINT	s;
		POINT	e;
	};

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
		bool is_valid( HWND w, WNDPROC p ) const {
			return ( ( w != 0 ) && ( p != 0 ) );
		}
		///
		WNDPROC swap( HWND w, WNDPROC p ) {
			return reinterpret_cast< WNDPROC >( SetWindowLongPtr( w, GWLP_WNDPROC, reinterpret_cast< ULONG >( p ) ) );
		}

	public:
		///
		subclass() : 
		  wnd( 0 )
			  ,	proc( NULL ) {
		  }
		  ///
		  ~subclass() {
		  }
		  ///
		  WNDPROC sub( HWND w, WNDPROC p ) {
			  WNDPROC result = NULL;
			  if ( is_valid( w, p ) ) {
				  result = unsub();
				  this->proc = swap( ( this->wnd = w ), p );
			  }
			  return result;
		  }
		  ///
		  WNDPROC unsub() {
			  WNDPROC result = NULL;
			  if ( is_valid( this->wnd, this->proc ) ) {
				  result = swap( this->wnd, this->proc );
				  this->wnd		= 0;
				  this->proc	= NULL;
			  }
			  return result;
		  }
		  ///
		  LRESULT call( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
			  if ( this->proc != NULL ) {
				  return ( CallWindowProc( this->proc, hWnd, uMsg, wParam, lParam ) );
			  }
			  return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
		  }
	};


	//------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------
	template < UINT, typename, typename >
	struct handle_msg;

#define	ATOM_DEF_ONNCHITTEST( c ) typedef LRESULT( c::* c ## _onnchittest_t )( HWND, int, int ); typedef boost::mpl::pair< boost::mpl::int_< WM_NCHITTEST >::type, c ## _onnchittest_t >::type c ## _onnchittest_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_NCHITTEST, T, U > {
		static LRESULT call(T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam) {
			return ((t.*u)((hWnd), GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		}
	};

#define	ATOM_DEF_ONNCLBUTTONDOWN( c ) typedef void( c::* c ## _onnclbuttondown_t )( HWND, int, int, int ); typedef boost::mpl::pair< boost::mpl::int_< WM_NCLBUTTONDOWN >::type, c ## _onnclbuttondown_t >::type c ## _onnclbuttondown_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_NCLBUTTONDOWN, T, U > {
		static LRESULT call(T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam) {
			return ((t.*u)((hWnd), (int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)), 0L);
		}
	};

#define	ATOM_DEF_ONNCLBUTTONUP( c ) typedef void( c::* c ## _onnclbuttonup_t )( HWND, int, int, int ); typedef boost::mpl::pair< boost::mpl::int_< WM_NCLBUTTONUP >::type, c ## _onnclbuttonup_t >::type c ## _onnclbuttonup_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_NCLBUTTONUP, T, U > {
		static LRESULT call(T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam) {
			return ((t.*u)((hWnd), (int)wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)), 0L);
		}
	};

#define	ATOM_DEF_ONSETTINGCHANGE( c ) typedef void( c::* c ## _onsettingchange_t )( HWND, UINT, LPCTSTR ); typedef boost::mpl::pair< boost::mpl::int_< WM_SETTINGCHANGE >::type, c ## _onsettingchange_t >::type c ## _onsettingchange_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_SETTINGCHANGE, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)((hWnd), (UINT)(wParam), (LPCTSTR)(lParam) ), 0L);
		}
	};

#define	ATOM_DEF_ONTIMER( c ) typedef void( c::* c ## _ontimer_t )( HWND, UINT ); typedef boost::mpl::pair< boost::mpl::int_< WM_TIMER >::type, c ## _ontimer_t >::type c ## _ontimer_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_TIMER, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)((hWnd), (UINT)(wParam)), 0L);
		}
	};

	template < typename T, typename U >
	struct handle_msg< WM_CREATE, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)((hWnd), (LPCREATESTRUCT)(lParam)) ? 0L : (LRESULT)-1L);
		}
	};


#define	ATOM_DEF_ONKEYDOWN( c ) typedef void( c::* c ## _onkeydown_t )( HWND, UINT, BOOL, int, UINT ); typedef boost::mpl::pair< boost::mpl::int_< WM_KEYDOWN >::type, c ## _onkeydown_t >::type c ## _onkeydown_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_KEYDOWN, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)((hWnd), (UINT)(wParam), TRUE, (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L);
		}
	};

#define	ATOM_DEF_ONKEYUP( c ) typedef void( c::* c ## _onkeyup_t )( HWND, UINT, BOOL, int, UINT ); typedef boost::mpl::pair< boost::mpl::int_< WM_KEYUP >::type, c ## _onkeyup_t >::type c ## _onkeyup_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_KEYUP, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)((hWnd), (UINT)(wParam), FALSE, (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L);
		}
	};


#define	ATOM_DEF_ONCHAR( c ) typedef void( c::* c ## _onchar_t )( HWND, TCHAR, int ); typedef boost::mpl::pair< boost::mpl::int_< WM_CHAR >::type, c ## _onchar_t >::type c ## _onchar_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_CHAR, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)((hWnd), (TCHAR)(wParam), (int)(short)LOWORD(lParam)), 0L);
		}
	};

#define	ATOM_DEF_ONLBUTTONDOWN( c )	typedef void( c::* c ## _onlbuttondown_t )( HWND, BOOL, int, int, UINT ); typedef boost::mpl::pair< boost::mpl::int_< WM_LBUTTONDOWN >::type, c ## _onlbuttondown_t >::type c ## _onlbuttondown_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_LBUTTONDOWN, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)((hWnd), FALSE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
		}
	};

#define	ATOM_DEF_ONLBUTTONUP( c )	typedef void( c::* c ## _onlbuttonup_t )( HWND, int, int, UINT ); typedef boost::mpl::pair< boost::mpl::int_< WM_LBUTTONUP >::type, c ## _onlbuttonup_t >::type c ## _onlbuttonup_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_LBUTTONUP, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)((hWnd), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
		}
	};

#define	ATOM_DEF_ONRBUTTONDOWN( c )	typedef void( c::* c ## _onrbuttondown_t )( HWND, BOOL, int, int, UINT ); typedef boost::mpl::pair< boost::mpl::int_< WM_RBUTTONDOWN >::type, c ## _onrbuttondown_t >::type c ## _onrbuttondown_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_RBUTTONDOWN, T, U > {
		static LRESULT call(T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam) {
			return ((t.*u)((hWnd), FALSE, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
		}
	};


#define	ATOM_DEF_ONMOUSEMOVE( c )	typedef void( c::* c ## _onmousemove_t )( HWND, int, int, UINT ); typedef boost::mpl::pair< boost::mpl::int_< WM_MOUSEMOVE >::type, c ## _onmousemove_t >::type c ## _onmousemove_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_MOUSEMOVE, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)((hWnd), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L);
		}
	};

#define	ATOM_DEF_CAPTURECHANGED( c )	typedef void( c::* c ## _oncapturechanged_t )( HWND, HWND ); typedef boost::mpl::pair< boost::mpl::int_< WM_CAPTURECHANGED >::type, c ## _oncapturechanged_t >::type c ## _oncapturechanged_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_CAPTURECHANGED, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)((hWnd), (HWND)(lParam)), 0L);
		}
	};

#define	ATOM_DEF_ONHOTKEY( c )	typedef void( c::* c ## _onhotkey_t )( HWND, int, UINT, UINT); typedef boost::mpl::pair< boost::mpl::int_< WM_HOTKEY >::type, c ## _onhotkey_t >::type c ## _onhotkey_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_HOTKEY, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)((hWnd), (int)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L);
		}
	};
	
#define	ATOM_DEF_ONPAINT( c )	typedef void( c::* c ## _onpaint_t)( HWND ); typedef boost::mpl::pair< boost::mpl::int_< WM_PAINT >::type, c ## _onpaint_t >::type c ## _onpaint_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_PAINT, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)(hWnd), 0L);
		}
	};

#define	ATOM_DEF_ONCOMMAND( c )	typedef void( c::* c ## _oncommand_t )( int id, HWND hwndCtl, UINT codeNotify ); typedef boost::mpl::pair< boost::mpl::int_< WM_COMMAND >::type, c ## _oncommand_t >::type c ## _oncommand_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_COMMAND, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)((int)(LOWORD(wParam)), (HWND)(lParam), (UINT)HIWORD(wParam)), 0L);
		}
	};

#define	ATOM_DEF_ONNOTIFY( c )	typedef void( c::* c ## _onnotify_t )( int id, LPNMHDR lpnm ); typedef boost::mpl::pair< boost::mpl::int_< WM_NOTIFY >::type, c ## _onnotify_t >::type c ## _onnotify_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_NOTIFY, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)( (int)(wParam), (NMHDR*)(lParam)), 0L);
		}
	};

#define	ATOM_DEF_ONCLOSE( c )	typedef void( c::* c ## _onclose_t )( HWND ); typedef boost::mpl::pair< boost::mpl::int_< WM_CLOSE >::type, c ## _onclose_t >::type c ## _onclose_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_CLOSE, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)(hWnd), 0L);
		}
	};

	template < typename T, typename U >
	struct handle_msg< WM_DESTROY, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)(hWnd), 0L);
		}
	};

#define	ATOM_DEF_ONWMUSER( c, p, n ) typedef void( c::* c ## _on ## p ## _t )( HWND, WPARAM, LPARAM ); typedef boost::mpl::pair< boost::mpl::int_< WM_USER + n >::type, c ## _on ## p ## _t >::type c ## _on ## p ## _pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_USER + 1, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)(hWnd, wParam, lParam), 0L);
		}
	};

#define	ATOM_DEF_ONSYSCOMMAND( c )	typedef void( c::* c ## _onsyscommand_t )( HWND, UINT, int, int ); typedef boost::mpl::pair< boost::mpl::int_< WM_SYSCOMMAND >::type, c ## _onsyscommand_t >::type c ## _onsyscommand_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_SYSCOMMAND, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)((hWnd), (UINT)(wParam), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L);
		}
	};

#define	ATOM_DEF_ONSIZE( c )	typedef void( c::* c ## _onsize_t )( HWND, UINT, int, int ); typedef boost::mpl::pair< boost::mpl::int_< WM_SIZE >::type, c ## _onsize_t >::type c ## _onsize_pair_t;
	template < typename T, typename U >
	struct handle_msg< WM_SIZE, T, U > {
		static LRESULT call( T&t, U u, HWND hWnd, WPARAM wParam, LPARAM lParam ) {
			return ((t.*u)((hWnd), (UINT)(wParam), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L);
		}
	};

	//------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------
	template < typename B, typename T >
	class wwindow :	public boost::noncopyable, public wctrl
	{
		typedef TCHAR
			char_t;
		typedef std::basic_string< char_t >
			string_t;
		typedef std::basic_stringstream< char_t >
			stringstream_t;

	public:
		///
		enum input_t {
			mouse,
			keyboard
		};
		///
		template < typename P >
		wwindow( B& b, P const& p ) :
					wctrl()
				,	base( b )
				,	class_atom( 0 )
				,	wcex()
				,	cs()
				,	subcl()
				,	auto_destroy( false )
				,	msg( p )

		  {
			  memset( &wcex, 0, sizeof( wcex ) );
			  memset( &cs, 0, sizeof( cs ) );
		  }
		  ///
		  ~wwindow() {
			  deinit(); }
		  ///
		  wwindow const& activate() const {
			SetForegroundWindow( this->wnd ); return (*this); }
		  ///
		  wwindow const& invalidate() const {
			InvalidateRect( this->wnd, NULL, TRUE ); return (*this); }
		  ///
		  wwindow const& setStyles( DWORD const style, DWORD const style_ex ) const {
			  SetWindowLong( this->getHWND(), GWL_STYLE, style );
			  SetWindowLong( this->getHWND(), GWL_EXSTYLE, style_ex );
			  SetWindowPos( this->getHWND(), 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED );
			  return (*this); }
		  ///
		  wwindow const& setAlpha( BYTE const alpha ) const {
			  SetLayeredWindowAttributes( this->getHWND(), RGB( 0, 0, 0 ), alpha, LWA_ALPHA );
			  return (*this); }
		  ///
		  wwindow const& getAlpha(BYTE& alpha) const {
			  DWORD flags = LWA_ALPHA;
			  GetLayeredWindowAttributes(this->getHWND(), 0, &alpha, &flags);
			  return (*this);
		  }
		  ///
		  wwindow const& setTitle(string_t const& title) const {
			  SetWindowText(this->getHWND(), title.c_str());
			  return (*this);
		  }
		  ///
		  wwindow const& sysMenuInsert( UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, string_t const & lpNewItem ) const {
			  InsertMenu( GetSystemMenu( this->getHWND(), FALSE ), uPosition, uFlags, uIDNewItem, lpNewItem.c_str() );
			  return (*this); }
		  ///
		  wwindow const& sysMenuCheckRadioItem( UINT idFirst, UINT idLast, UINT idCheck, bool const byPosition ) const {
			  CheckMenuRadioItem( GetSystemMenu( this->getHWND(), FALSE ), idFirst, idLast, idCheck, ( byPosition )?(MF_BYPOSITION):(MF_BYCOMMAND) );
			  return (*this); }
		  ///
		  void	getClientRect( RECT& r ) const {
				GetClientRect( this->getHWND(), &r ); }
		  ///
		  wwindow const& inputCapture( input_t const t ) const {
			  switch ( t ) {
			  case mouse:
				  SetCapture( this->getHWND() );
				  break;
			  case keyboard:
				  SetFocus( this->getHWND() );
				  break;
			  }
			  return (*this);
		  };
		  ///
		  bool inputIsCaptured( input_t const t ) const {
			  switch ( t ) {
			  case mouse:
				  return ( GetCapture() == this->getHWND() );
			  case keyboard:
				  return ( GetFocus() == this->getHWND() );
			  }
			  return false;
		  }
		  ///
		  wwindow const& inputRelease( input_t const t ) const {
			  switch ( t ) {
			  case mouse:
				  ReleaseCapture();
				  break;
			  case keyboard:
				  SetFocus( NULL );
				  break;
			  }
			  return (*this);
		  }
		  ///
		  bool init( boost::function< bool ( WNDCLASSEX&, CREATESTRUCT& )> configure, bool const ad ) {
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
				  if ( configure( this->wcex, this->cs ) ) {
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
					  if ( this->wnd != 0 ) {
						  if ( this->wcex.lpfnWndProc != proc ) {
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
		  bool init( HWND w, bool const ad ) {
			  deinit();
			  {
				  if ( w != 0 ) {
					  wwindow* win = NULL;
					  if ( get_window_object( w, win ) == 0 ) {
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
		  static void run( boost::function< bool() > tick, int ) {
			  MSG msg = { 0 };
			  bool cont = true;
			  do {
				  while ( cont && PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ) {
					  if ( ( cont = ( msg.message != WM_QUIT ) ) == true ) {
						  TranslateMessage( &msg );
						  DispatchMessage( &msg );
					  }
				  }
			  } while( cont && tick() );
		  }
		  ///
		  static void run() {
			  struct _ {
				  static bool __( HWND, MSG* ) {
					  return false;
				  }
			  };
			  run( boost::bind( _::__, _1, _2 ) );
		  }
		  ///
		  static void run( boost::function< bool( HWND, MSG* ) > pred ) {
			  MSG msg = { 0 };
			  BOOL bRet;
			  while( ( bRet = GetMessage( &msg, 0, 0, 0 ) ) != 0 ) {
				  if ( bRet == -1 ) {
				  } else {
					  if ( !pred( msg.hwnd, &msg ) ){
						  TranslateMessage( &msg ); 
						  DispatchMessage( &msg ); 
					  }
				  }
			  }
		  }
		  static void exit( int const code ) {
			  PostQuitMessage( code );
		  }

		  ///
		  static void calc_rect( RECT& rect, DWORD const style, DWORD const ex_style, bool const menu, bool const center ) {
			  calc_rect( rect, style, ex_style, menu, center, center );
		  }
		  ///
		  static void calc_rect( RECT& rect, DWORD const style, DWORD const ex_style, bool const menu, bool const vcenter, bool const hcenter ) {
			  AdjustWindowRectEx( &rect, style, (menu)?(TRUE):(FALSE), ex_style );
			  if ( vcenter ) {
				  OffsetRect( 
				  &rect, 
				  0,
				  ( GetSystemMetrics( SM_CYFULLSCREEN ) - ( rect.bottom - rect.top ) ) /2 );
			  } else {
				  OffsetRect( &rect, 0, -rect.top );
			  }
			  if ( hcenter ) {
				  OffsetRect( 
				  &rect, 
				  ( GetSystemMetrics( SM_CXFULLSCREEN ) - ( rect.right - rect.left ) ) / 2,
				  0 );
			  } else {
				  OffsetRect( &rect, -rect.left, 0 );
			  }
		  }

	protected:

		template < typename >
		struct message;

		template <>
		struct message< Loki::NullType > {
			explicit message< Loki::NullType >( Loki::NullType const & p )
			{}

			static LRESULT process( B& b, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
				return DefWindowProc( hWnd, uMsg, wParam, lParam );
			}
		};

		template < UINT I, typename T, typename U >
		struct message< Loki::Typelist< boost::mpl::pair< boost::mpl::int_< I >, T >, U > > : public message< U > {
			T
				value;
			template< class P >
			explicit message< Loki::Typelist< boost::mpl::pair< boost::mpl::int_< I >, T >, U > >( std::pair< T, P > const& p ) : message< U >( p.second ), value( p.first )
			{}

			LRESULT process( B& b, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
				if ( uMsg == I ) {
					return handle_msg< I, B, T >::call( b, this->value, hWnd, wParam, lParam );
				}
				return message< U >::process( b, hWnd, uMsg, wParam, lParam );
			}
		};

		///
		void deinit() {
			if ( this->wnd != 0 ) {
				RemoveProp( this->wnd, ATOM_UTIL_WWINDOW_PROP );
				this->subcl.unsub();
				if ( this->auto_destroy ) {
					DestroyWindow( this->wnd );
				}
				if ( this->class_atom != 0 ) {
					UnregisterClass( reinterpret_cast< LPCTSTR >( this->class_atom ), this->wcex.hInstance );
				}
				this->class_atom	= 0;
				this->wnd			= 0;
				this->auto_destroy= false;
			}
		}
		///
		static bool process_dlg_msgs( HWND hDlg, MSG& msg ) {
			return ( hDlg && IsDialogMessage( hDlg, &msg ) ); }
		///
		static bool get_window_object( HWND w, wwindow*& win ) {
			return ( ( win = reinterpret_cast< wwindow* >( GetProp( w, ATOM_UTIL_WWINDOW_PROP ) ) ) != NULL ); }
		///
		static LRESULT CALLBACK proc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
			wwindow* win = NULL;
			if ( get_window_object( hWnd, win ) ) {
				return ( win->msg.process( win->base, hWnd, uMsg, wParam, lParam ) );
				//return ( win->subcl.call( hWnd, uMsg, wParam, lParam ) );
			}
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
		}

	private:
		///
		B&
			base;
		///
		ATOM
			class_atom;
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
		///
		message< T >
			msg;
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
			switch( uMsg ) {
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
			case WM_SETTINGCHANGE: {
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
