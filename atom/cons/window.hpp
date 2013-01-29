#pragma once

#include <atom/util/wwindow.hpp>

#include "./pref.hpp"

class window : public atom::wwindow
{
public:
	///
	window( std::ostream& l, pref& p );
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
};