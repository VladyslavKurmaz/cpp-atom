#pragma once
#include "./classes.hpp"

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
	struct frame_coord {
		fraction left;
		fraction top;
		unsigned int width;
		unsigned int height;
		frame_coord() : left(), top(), width(), height(){}
		frame_coord( unsigned int ln, unsigned int ld, unsigned int tn, unsigned int td, unsigned int w, unsigned int h ) : left( ln, ld ), top( tn, td ), width( w ), height( h ){}
	};
	///
	static frame_ptr create( logger_ptr l, pref_ptr p, frame_coord const & fc ) {
		frame_ptr f = frame_ptr( new frame( l, p, fc ) );
		f->prev = f;
		f->next = f;
		return ( f );
	}
	///
	frame_coord const &
	get_coord() const { return ( this->coord ); }
	///
	frame_ptr
	get_prev() const { return ( this->prev ); }
	///
	TCHAR const*
	get_buffer() const { return ( this->buffer.c_str() ); }
	///
	frame_ptr
	get_next() const { return ( this->next ); }
	///
	~frame();
	///
	frame_ptr
	split( bool const pref_h );
	///
	void
	run( uni_string const& cmd );
	///
	void
	onchar( TCHAR ch );
	///
	void
	clear();
	///
	void
	append( void const* b, size_t const b_sz );

protected:
	//
	logger&
	get_logger() {
		return ( *( get_value( boost::mpl::identity< frame2logger >() ).item() ) ); }
private:
	//
	uni_string
		buffer;
	//
	frame_coord
		coord;
	///
	frame_ptr
		next;
	///
	frame_ptr
		prev;
	//
	process_ptr
		process;
	///
	frame( logger_ptr l, pref_ptr p, frame_coord const & fc );
};