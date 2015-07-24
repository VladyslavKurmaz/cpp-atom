#pragma once
#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>

//
class logger;
typedef boost::shared_ptr< logger >
	logger_ptr;
#define LOGGER_ACCESSOR( t )	logger_ptr getLogger() { return ( ( get_value( boost::mpl::identity< t >() ).item() ) ); }
//
class pref;
typedef boost::shared_ptr< pref >
	pref_ptr;
#define PREF_ACCESSOR( t )	pref_ptr getPref() { return ( ( get_value( boost::mpl::identity< t >() ).item() ) ); }
//
class area;
typedef boost::shared_ptr< area >
	area_ptr;
//
class mode;
typedef boost::shared_ptr< mode >
	mode_ptr;
//
class frame;
typedef boost::shared_ptr< frame >
	frame_ptr;
//
class window;
typedef boost::shared_ptr< window >
	window_ptr;
#define WINDOW_ACCESSOR( t )	window_ptr getWindow() { return ( ( get_value( boost::mpl::identity< t >() ).item() ) ); }
//
class panel;
typedef boost::shared_ptr< panel >
	panel_ptr;
//
class appl;
typedef boost::shared_ptr< appl >
	appl_ptr;
//
//
struct bridgeMsg {
	enum type {
		bmNone			= 0,
		bmSize			= 1,
		bmKbrd			= 2,
		bmCtrlBreak		= 3,
		bmCtrlC			= 4,
		bmExit			= 5
	} id;
	union {
		SIZE				size;
		KEY_EVENT_RECORD	key;
	};
};
//
//
class fraction {
public:
	///
	fraction() : n(0), d(1) {
	}
	///
	fraction( int const nn, int const dd ) : n( nn ), d( dd ) {
	}
	///
	fraction( fraction const& f ) : n( f.n ), d( f.d ) {
	}
	///
	int get_n() const {
		return ( this->n );
	}
	///
	int get_d() const {
		return ( this->d );
	}
	///
	fraction const operator+( fraction const& r ) const {
		return fraction( this->n * r.d + r.n * this->d, this->d * r.d );
	}
private:
	int n;
	int d;
};
///
struct frame_coord {
	fraction left;
	fraction top;
	unsigned int width;
	unsigned int height;
	frame_coord() : left(), top(), width(), height(){}
	frame_coord( unsigned int ln, unsigned int ld, unsigned int tn, unsigned int td, unsigned int w, unsigned int h ) : left( ln, ld ), top( tn, td ), width( w ), height( h ){}
};

namespace atom {
	class mutex : public boost::noncopyable {
		typedef std::basic_string< TCHAR >
			string_t;
	public:
		///
		mutex() :
				m( NULL )
			,	timeout( INFINITE ) {
		}
		///
		~mutex() {
		}
		///
		bool
		create( string_t const& name, bool owner ) {
			return ( NULL != ( this->m = CreateMutex( NULL, owner, name.c_str() ) ) );
		}
		///
		void
		lock() const {
			switch ( WaitForSingleObject( this->m, this->timeout ) ) {
			case WAIT_ABANDONED:
				break;
			case WAIT_OBJECT_0:
				break;
			case WAIT_TIMEOUT:
				break;
			case WAIT_FAILED:
				break;
			}
		}
		///
		void
		unlock() const {
			ReleaseMutex( this->m );
		}
	protected:
	private:
		///
		HANDLE
			m;
		///
		DWORD
			timeout;
	};
}

struct paint_font_t {
	unsigned int
		height;
	atom::shared_gdiobj< HFONT >
		font;
	COLORREF
		color;
};

struct dcb_t {
	atom::shared_dc
		dc;
	atom::shared_gdiobj< HBITMAP >
		bitmap;
	void updateDC( size_t const cx, size_t const cy ) {
		HDC dc = GetDC( NULL );
		{
			this->dc		= CreateCompatibleDC( dc );
			this->bitmap	= CreateCompatibleBitmap( dc, cx, cy );
			SelectObject( this->dc, this->bitmap );
		}
		ReleaseDC( NULL, dc );
	}
};

struct paint_param_t {
	dcb_t
		dcb;
	RECT
		padding;
	atom::shared_gdiobj< HBRUSH >
		borderActive;
	atom::shared_gdiobj< HBRUSH >
		borderInactive;
	
	paint_font_t
		textFont;
	paint_font_t
		sysFont;
	//
	void updateDC( size_t const cx, size_t const cy ) {
		dcb.updateDC( cx, cy );
	}
};
