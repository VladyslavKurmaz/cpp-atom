#pragma once

#include "./log.hpp"
#include "./pref.hpp"
#include "./process.hpp"

class fraction {
public:
	///
	fraction() : n(0), d(1) {
	}
	///
	fraction( int const nn, int const dd ) : n( nn ), d( dd ) {
	}
	///
	fraction( fraction const& f ) : n( f.n ), d( f.d ) {
	}
	///
	int get_n() const {
		return ( this->n );
	}
	///
	int get_d() const {
		return ( this->d );
	}
	///
	fraction const operator+( fraction const& r ) const {
		return fraction( this->n * r.d + r.n * this->d, this->d * r.d );
	}
private:
	int n;
	int d;
};

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > frame2logger;
typedef atom::nstorage< pref, boost::shared_ptr, atom::narray1 > frame2pref;


class frame :
	public atom::node< LOKI_TYPELIST_2( frame2logger, frame2pref ) >,
	public boost::enable_shared_from_this< frame > {
	typedef atom::node< LOKI_TYPELIST_2( frame2logger, frame2pref ) >
		base_node_t;
public:
	///
	typedef boost::shared_ptr< frame >
		shared_ptr;
	///
	struct frame_coord {
		fraction left;
		fraction top;
		unsigned int width;
		unsigned int height;
		frame_coord() : left(), top(), width(), height(){}
		frame_coord( unsigned int ln, unsigned int ld, unsigned int tn, unsigned int td, unsigned int w, unsigned int h ) : left( ln, ld ), top( tn, td ), width( w ), height( h ){}
	};
	///
	static shared_ptr create( logger::shared_ptr l, pref::shared_ptr p, frame_coord const & fc ) {
		shared_ptr f = shared_ptr( new frame( l, p, fc ) );
		f->prev = f;
		f->next = f;
		return ( f );
	}
	///
	frame_coord const & get_coord() const {
		return ( this->coord ); }
	///
	shared_ptr get_prev() const {
		return ( this->prev ); }
	///
	shared_ptr get_next() const {
		return ( this->next ); }
	///
	~frame();
	///
	shared_ptr split( bool const pref_h );
	///
	void run( std::basic_string<TCHAR> const& cmd );
	///
	void onchar( TCHAR ch );
	///
	void clear();

protected:
	//
	logger& get_logger() {
		return ( *( get_value( boost::mpl::identity< frame2logger >() ).item() ) );
	}
private:
	frame_coord
		coord;
	///
	shared_ptr
		next;
	///
	shared_ptr
		prev;
	//
	process::shared_ptr
		process;
	///
	frame( logger::shared_ptr l, pref::shared_ptr p, frame_coord const & fc );
};