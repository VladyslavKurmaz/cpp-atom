#pragma once
#include "./classes.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > appl2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > appl2pref;
typedef atom::nstorage< window, boost::shared_ptr, atom::narray1 > appl2window;

class appl :
	public atom::node< LOKI_TYPELIST_3( appl2logger, appl2pref, appl2window ) >,
	public boost::enable_shared_from_this< appl > {
public:
	///
	static appl_ptr create( logger_ptr l, pref_ptr p ) {
		return appl_ptr( new appl( l, p ) );
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
	logger& getLogger() {
		return ( *( get_value( boost::mpl::identity< appl2logger >() ).item() ) );
	}
	//
	window& getWindow() {
		return ( *( get_value( boost::mpl::identity< appl2window >() ).item() ) );
	}

private:
	///
	appl( logger_ptr l, pref_ptr p );
};