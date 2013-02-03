#pragma once
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

#include "./log.hpp"
#include "./pref.hpp"
#include "./window.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > appl2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > appl2pref;

class appl : public atom::node< LOKI_TYPELIST_2( appl2logger, appl2pref ) >
{
public:
	///
	typedef boost::shared_ptr< appl >
		shared_ptr;
	///
	static shared_ptr create( logger::shared_ptr l, pref::shared_ptr p ) {
		return shared_ptr( new appl( l, p ) );
	}
	///
	~appl();
	///
	bool
		init();
	///
	void
		run();

protected:
	//
	logger& get_logger() {
		return ( *( get_value( boost::mpl::identity< pref2logger >() ).item() ) );
	}

private:
	///
	appl( logger::shared_ptr l, pref::shared_ptr p );
	///
	//window 
	//	wnd;
};