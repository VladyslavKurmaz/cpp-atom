#pragma once
#include "./mode.hpp"

class shell :
	public mode {
public:
	///
	virtual ~shell();
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
	shell( logger_ptr l, pref_ptr p );
	//
	area_ptr
		headArea;
	//
	frame_ptr
		currentFrame;
	//
	bool
		expandMode;
	//
	COORD
		consoleSize;
	friend mode;
};
