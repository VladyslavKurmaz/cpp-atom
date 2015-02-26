#include "./pch.hpp"
#include "./classes.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./panel.hpp"
#include "./ar.hpp"
#include "./window.hpp"
#include <boost/algorithm/string.hpp>

ar::ar( logger_ptr l, pref_ptr p, window_ptr w ):
		mode( l, p, w )
	,	ctrl()
	,	arPanel() {
}

ar::~ar() {
}

void ar::activate( bool const state ) {
	if ( !arPanel ) {
		arPanel = panel::create( this->getLogger(), this->getPref() );
		arPanel->init( this->getWindow()->getHWND() );
		//LONG clst = GetClassLong( hStatic, GCL_STYLE );
		//SetClassLong( hStatic, GCL_STYLE, clst | CS_NOCLOSE );

	}
}

void ar::show( bool const state ) {
	this->arPanel->show( state );
}

bool ar::command( int const id ) {
	return false;
}

void ar::key( KEY_EVENT_RECORD const& k ) {
}

void ar::mouselbdown( bool dblclick, int x, int y, unsigned int state ) {
	window_ptr w = this->getWindow();
	if ( !dblclick ) {
		ctrl.start( x, y );
		w->inputCapture( window::keyboard );
		w->inputCapture( window::mouse );
		w->invalidate();
	}
}

void getFileContents( std::string const& n, std::string& s ) {
	std::ifstream t( n );

	t.seekg(0, std::ios::end);   
	s.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	s.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
}

void ar::mouselbup( int x, int y, unsigned int state ) {
	window_ptr w = this->getWindow();
	RECT r;
	if ( w->inputIsCaptured( window::mouse ) ) {
		ctrl.getRect( r );
		//bitmapSave( "desktop.bmp", this->paintParam.dcb.dc, this->paintParam.dcb.bitmap );
		HDC dc = GetDC( NULL );
		size_t const cx = RECT_WIDTH( r );
		size_t const cy = RECT_HEIGHT( r );
		if ( ( cx > 8 ) && ( cy > 8 ) ) {
			dcb_t dcb;
			dcb.updateDC( cx, cy );
			BitBlt( dcb.dc,
				0,
				0,
				RECT_WIDTH( r ),
				RECT_HEIGHT( r ),
				dc,
				r.left,
				r.top,
				SRCCOPY );
			bitmapSave( _T( "desktop.bmp" ), dcb.dc, dcb.bitmap );
			ReleaseDC( NULL, dc );
			//
			atom::string_t src( _T( "?" ) );
			atom::string_t dest( _T( "?" ) );
			atom::proc proc;
			if ( proc.run( _T( "tesseract desktop.bmp out" ), false ) ) {
				proc.join();
				//
				std::string s;
				getFileContents( "out.txt", s );
				std::string encoded = s;
				boost::replace_all( encoded, " ", "%20");
				src = atom::s2s<atom::string_t>( s );
				atom::string_t wencoded = atom::s2s<atom::string_t>( encoded );

				//
				atom::stringstream_t ss;
				ss << _T( "curl -k -o tr.json \"" ) << _T( "https://www.googleapis.com/language/translate/v2?key=AIzaSyDawRGDyX-pevX3A22AODuXCPANs_JJm14&source=en&target=ru&q=" ) << wencoded << _T( "\"" );
				if ( proc.run( ss.str(), true ) ) {
					proc.join();
					//
					boost::property_tree::ptree r;
					boost::property_tree::read_json( "tr.json", r );
					//
					BOOST_FOREACH( boost::property_tree::ptree::value_type const & c, r.get_child( "data.translations" )) {
						dest = boost::locale::conv::to_utf<wchar_t>( c.second.get<std::string>("translatedText"), "UTF-8" );
						break;
					}
//MultiByteToWideChar(
//  _In_       UINT CodePage,
//  _In_       DWORD dwFlags,
//  _In_       LPCSTR lpMultiByteStr,
//  _In_       int cbMultiByte,
//  _Out_opt_  LPWSTR lpWideCharStr,
//  _In_       int cchWideChar
//);

				}
			}
			this->arPanel->addRecord( src, dest );
		}
	}
	w->inputRelease( window::mouse );
}

void ar::mousemove( int x, int y, unsigned int state ){
	window_ptr w = this->getWindow();
	if ( w->inputIsCaptured( window::mouse ) ) {
		RECT r;
		GetClientRect( w->getHWND(), &r );
		ctrl.update( x, y, ( GetKeyState( VK_SPACE ) & 0x80 ) > 0, r );
		w->invalidate();
	}
}

void ar::paint( paint_param_t& paintParam, RECT const& rect ) {
	window_ptr w = this->getWindow();
	if ( w->inputIsCaptured(window::mouse ) ) {
		RECT rt;
		ctrl.getRect( rt );
		FrameRect( paintParam.dcb.dc, &rt, (HBRUSH)GetStockObject( WHITE_BRUSH ) );
	}
}

void ar::clear() {
	base_t::clear();
}


