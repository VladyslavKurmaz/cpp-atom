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
	///
	virtual void paint( paint_param_t& paintParam, RECT const& rect );
	///
	virtual void clear();

protected:

private:
	///
	ar( logger_ptr l, pref_ptr p );
	friend mode;
};
