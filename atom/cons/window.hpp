#pragma once

#include <atom/util/wwindow.hpp>
#include "./log.hpp"
#include "./pref.hpp"
#include "./frame.hpp"
#include "./window.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > window2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > window2pref;
typedef atom::nstorage< frame, boost::shared_ptr, atom::nlist > window2frame;

class window : public atom::wwindow, public atom::node< LOKI_TYPELIST_3( window2logger, window2pref, window2frame ) >
{
public:
	///
	typedef boost::shared_ptr< window >
		shared_ptr;
	///
	static shared_ptr create( logger::shared_ptr l, pref::shared_ptr p ) {
		return shared_ptr( new window( l, p ) );
	}
	///
	~window();
	///
	bool
		init();
	///
	void
		run();

protected:
	//
	logger& get_logger() {
		return ( *( get_value( boost::mpl::identity< window2logger >() ).item() ) );
	}
private:
	///
	window( logger::shared_ptr l, pref::shared_ptr p );
};