#pragma once
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

#include "./log.hpp"
#include "./pref.hpp"
#include "./window.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > appl2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > appl2pref;
typedef atom::nstorage< window, boost::shared_ptr, atom::narray1 > appl2window;

class appl :
	public atom::node< LOKI_TYPELIST_3( appl2logger, appl2pref, appl2window ) >,
	public boost::enable_shared_from_this< appl > {
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
	appl&
		run();
	///
	void
		clear();

protected:
	//
	logger& get_logger() {
		return ( *( get_value( boost::mpl::identity< appl2logger >() ).item() ) );
	}
	//
	window& get_window() {
		return ( *( get_value( boost::mpl::identity< appl2window >() ).item() ) );
	}

private:
	///
	appl( logger::shared_ptr l, pref::shared_ptr p );
};