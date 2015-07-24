#pragma once
#include "./mode.hpp"

class ad :
	public mode {
public:
	///
	virtual ~ad();
	///
	virtual void activate( bool const state );
	///
	virtual void show( bool const state );
	///
	virtual bool command( int const id );
	//
	virtual void key( KEY_EVENT_RECORD const& k );
	//
	virtual void mouselbdown( bool dblclick, int x, int y, unsigned int state );
	//
	virtual void mouselbup( int x, int y, unsigned int state );
	//
	virtual void mousemove( int x, int y, unsigned int state );
	///
	virtual void paint( paint_param_t& paintParam, RECT const& rect );
	///
	virtual void clear();

protected:

private:
	///
	ad(boost::property_tree::ptree const& c, logger_ptr l, pref_ptr p, window_ptr w);
	//
	atom::string_t const ocrOutputFile;
	atom::string_t const ocrOutputFileWithExt;
	atom::string_t const translationOutputFileW;
	std::string const translationOutputFile;
	//
	atom::rectCtrl ctrl;
	//
	panel_ptr
		adPanel;
	//
	typedef std::pair< atom::string_t, atom::string_t >
		urls_t;
	//
	atom::string_t getOCRUrl();
	//
	atom::string_t getTranslateUrl( atom::string_t const& encoded );


	friend mode;
};
