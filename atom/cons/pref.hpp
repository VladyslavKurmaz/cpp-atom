#pragma once

#include <atom/util/po.hpp>

class pref
{
public:
	///
	pref( std::ostream& l );
	///
	~pref();
	///
	bool
		init( int argc, char const * const argv[] );

protected:
	//
	static const
		atom::po::id_t	po_help					=	0;

private:
	///
	std::ostream&
		log;
	///
	atom::po
		po;
};