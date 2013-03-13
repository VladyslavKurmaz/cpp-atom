#pragma once
#include "./classes.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > pref2logger;

class pref :
	public atom::node< LOKI_TYPELIST_1( pref2logger ) >,
	public boost::enable_shared_from_this< pref > {
public:
	typedef atom::node< LOKI_TYPELIST_1( pref2logger ) >
		base_t;
	///
	static pref_ptr create( logger_ptr l ) {
		return pref_ptr( new pref( l ) );
	}
	///
	~pref();
	///
	bool
	init( int argc, char const * const argv[] );
	///
	template <typename T >
	T get( atom::po::id_t const id ) {
		return ( this->po.as<T>( id ) );
	}

protected:
	//
	logger& get_logger() {
		return ( *( get_value( boost::mpl::identity< pref2logger >() ).item() ) );
	}
	//

private:
	///
	pref( logger_ptr l );
	///
	atom::po
		po;
};