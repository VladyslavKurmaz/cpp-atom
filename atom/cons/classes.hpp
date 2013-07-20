#pragma once
#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>

//
class logger;
typedef boost::shared_ptr< logger >
	logger_ptr;
//
class pref;
typedef boost::shared_ptr< pref >
	pref_ptr;
//
class frame;
typedef boost::shared_ptr< frame >
	frame_ptr;
//
class window;
typedef boost::shared_ptr< window >
	window_ptr;
//
class appl;
typedef boost::shared_ptr< appl >
	appl_ptr;
//
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
///
struct frame_coord {
	fraction left;
	fraction top;
	unsigned int width;
	unsigned int height;
	frame_coord() : left(), top(), width(), height(){}
	frame_coord( unsigned int ln, unsigned int ld, unsigned int tn, unsigned int td, unsigned int w, unsigned int h ) : left( ln, ld ), top( tn, td ), width( w ), height( h ){}
};
