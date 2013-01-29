#pragma once

#include "./pref.hpp"

class frame
{
public:
	///
	frame( std::ostream& l, pref& p );

protected:
private:
	///
	std::ostream&
		log;
	///
	pref&
		prefs;
};