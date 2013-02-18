#pragma once

#include "./log.hpp"
#include "./pref.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > frame2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > frame2pref;


class frame : public atom::node< LOKI_TYPELIST_2( frame2logger, frame2pref ) > {
public:
	///
	typedef boost::shared_ptr< frame >
		shared_ptr;
	///
	static shared_ptr create( logger::shared_ptr l, pref::shared_ptr p ) {
		return shared_ptr( new frame( l, p ) );
	}
	///
	~frame();

protected:
	//
	logger& get_logger() {
		return ( *( get_value( boost::mpl::identity< frame2logger >() ).item() ) );
	}
private:
	///
	frame( logger::shared_ptr l, pref::shared_ptr p );
};