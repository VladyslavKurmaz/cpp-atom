#pragma once
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>


#include "./pref.hpp"
#include "./window.hpp"

class appl
{
public:
	///
	appl( std::ostream& l, pref& p );
	///
	~appl();
	///
	bool
		init();
	///
	void
		run();

protected:

private:
	///
	std::ostream&
		log;
	///
	pref&
		prefs;
	///
	window 
		wnd;
};