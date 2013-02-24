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
	struct frame_coord {
		unsigned int left_n;
		unsigned int left_d;
		unsigned int top_n;
		unsigned int top_d;
		unsigned int width;
		unsigned int height;
		frame_coord() : left_n(), left_d(), top_n(), top_d(), width(), height(){}
		frame_coord( unsigned int ln, unsigned int ld, unsigned int tn, unsigned int td, unsigned int w, unsigned int h ) : left_n( ln ), left_d( ld ), top_n( tn ), top_d( td ), width( w ), height( h ){}
	};
	///
	static shared_ptr create( logger::shared_ptr l, pref::shared_ptr p, frame_coord const & fc ) {
		return shared_ptr( new frame( l, p, fc ) );
	}
	///
	frame_coord const & get_coord() const {
		return ( this->coord ); }
	///
	~frame();

protected:
	//
	logger& get_logger() {
		return ( *( get_value( boost::mpl::identity< frame2logger >() ).item() ) );
	}
private:
	frame_coord
		coord;
	///
	frame( logger::shared_ptr l, pref::shared_ptr p, frame_coord const & fc );
};