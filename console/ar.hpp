#pragma once
#include "./mode.hpp"

class ar :
	public mode {
public:
	///
	virtual ~ar();
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
	ar( logger_ptr l, pref_ptr p, window_ptr w );
	//
	atom::rectCtrl ctrl;
	//
	panel_ptr
		arPanel;

	friend mode;
};
