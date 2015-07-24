#include "./pch.hpp"
#include "./classes.hpp"
#include "./cmds.hpp"
#include "./log.hpp"
#include "./pref.hpp"
#include "./panel.hpp"
#include "./window.hpp"
#include "./ad.hpp"
#include <boost/algorithm/string.hpp>

ad::ad(boost::property_tree::ptree const& c, logger_ptr l, pref_ptr p, window_ptr w) :
		mode( c, l, p, w )
	,	ocrOutputFile( _T( "out" ) )
	,	ocrOutputFileWithExt( ocrOutputFile + _T( ".txt" ) )
	,	translationOutputFileW( _T( "tr.json" ) )
	,	translationOutputFile( "tr.json" )
	,	ctrl()
	,	adPanel()
{
}

ad::~ad() {
}

void ad::activate( bool const state ) {
	mode::activate(state);
	if ( !adPanel ) {
		adPanel = panel::create( this->getLogger(), this->getPref() );
		adPanel->init( this->getWindow()->getHWND() );
	}
	this->adPanel->show(state);
	this->adPanel->setADState(state);
	//if (state){
	//	this->adPanel->activate();
	//}
}

void ad::show( bool const state ) {
	mode::activate(state);
	this->adPanel->setADVisible(state);
	bool const s = state || this->adPanel->isLocked();
	this->adPanel->show(s);
	//if (s){
	//	this->adPanel->activate();
	//}
}

bool ad::command( int const id ) {
	return false;
}

void ad::key( KEY_EVENT_RECORD const& k ) {
}

void ad::mouselbdown( bool dblclick, int x, int y, unsigned int state ) {
	window_ptr w = this->getWindow();
	if ( !dblclick ) {
		ctrl.start( x, y );
		w->inputCapture( window::keyboard );
		w->inputCapture( window::mouse );
		w->invalidate();
	}
}

void getFileContents( atom::string_t const& n, std::string& s ) {
	std::ifstream t( n );

	t.seekg(0, std::ios::end);   
	s.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	s.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
}

std::string url_encode( std::string const& value ) {
    std::ostringstream escaped;
    escaped.fill('0');
	escaped << std::hex;

    for ( std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        std::string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        //if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
        //    escaped << c;
        //    continue;
        //}

        // Any other characters are percent-encoded
		escaped << '%' << std::setw(2) << int((unsigned char) c);
    }

    return escaped.str();
}

void ad::mouselbup(int x, int y, unsigned int state) {
	window_ptr w = this->getWindow();
	RECT r;
	if (w->inputIsCaptured(window::mouse)) {
		ctrl.getRect(r);
		//bitmapSave( "desktop.bmp", this->paintPadam.dcb.dc, this->paintPadam.dcb.bitmap );
		HDC dc = GetDC(NULL);
		size_t const cx = RECT_WIDTH(r);
		size_t const cy = RECT_HEIGHT(r);
		if ((cx > 8) && (cy > 8)) {
			dcb_t dcb;
			dcb.updateDC(cx, cy);
			BitBlt(dcb.dc,
				0,
				0,
				RECT_WIDTH(r),
				RECT_HEIGHT(r),
				dc,
				r.left,
				r.top,
				SRCCOPY);
			bitmapSave(_T("desktop.bmp"), dcb.dc, dcb.bitmap);
			ReleaseDC(NULL, dc);
			//
			atom::string_t src(_T("?"));
			atom::string_t dest(_T("?"));
			atom::proc proc;
			//
			if (proc.run(this->getOCRUrl(), false)) {
				proc.join();
				//
				try {
					std::string s;
					getFileContents(this->ocrOutputFileWithExt, s);
					if (s.length()){
						std::string encoded = url_encode(s);
						boost::replace_all(encoded, " ", "%20");
						//src = atom::s2s<atom::string_t>( s );
						//atom::string_t wencoded = atom::s2s<atom::string_t>( encoded );
						src = boost::locale::conv::to_utf<wchar_t>(s, "UTF-8");
						atom::string_t wencoded = boost::locale::conv::to_utf<wchar_t>(encoded, "UTF-8");
						//
						if (proc.run(this->getTranslateUrl(wencoded), false)) {
							proc.join();
							//
							boost::property_tree::ptree r;
							boost::property_tree::read_json(this->translationOutputFile, r);
							//
							BOOST_FOREACH(boost::property_tree::ptree::value_type const & c, r.get_child("data.translations")) {
								dest = boost::locale::conv::to_utf<wchar_t>(c.second.get<std::string>("translatedText"), "UTF-8");
								break;
							}
						}
					}
				}
				catch (std::exception&) {
				}
			}
			this->adPanel->addRecord(src, dest);
		}
	}
	w->inputRelease(window::mouse);
	w->invalidate();
}

void ad::mousemove(int x, int y, unsigned int state){
	window_ptr w = this->getWindow();
	if (w->inputIsCaptured(window::mouse)) {
		RECT r;
		GetClientRect(w->getHWND(), &r);
		ctrl.update(x, y, (GetKeyState(VK_SPACE) & 0x80) > 0, r);
		w->invalidate();
	}
}

void ad::paint(paint_param_t& paintParam, RECT const& rect) {
	window_ptr w = this->getWindow();
	mode::paint(paintParam, rect);
	if (w->inputIsCaptured(window::mouse)) {
		RECT rt;
		ctrl.getRect(rt);
		FrameRect(paintParam.dcb.dc, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH));
	}
}

void ad::clear() {
	base_t::clear();
}

atom::string_t ad::getOCRUrl() {
	pref::langspair_t const lngs = getPref()->langGetPair();
	atom::stringstream_t ss;
	ss << _T( "tesseract desktop.bmp -l " ) << lngs.first.get<0>() << _T( " " ) << this->ocrOutputFile;
	return ss.str();
}

atom::string_t ad::getTranslateUrl( atom::string_t const& encoded ) {
	pref::langspair_t const lngs = getPref()->langGetPair();
	atom::stringstream_t ss;
	ss << _T( "curl -k -o " ) << this->translationOutputFileW << _T(" \"" ) << _T( "https://www.googleapis.com/language/translate/v2?key=AIzaSyDawRGDyX-pevX3A22AODuXCPANs_JJm14&source=") << lngs.first.get<1>() << _T("&target=") << lngs.second.get<1>() << _T("&q=" ) << encoded << _T( "\"" );
	return ss.str();
}

