#pragma once
#include "./log.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > pref2logger;

class pref :
	public atom::node< LOKI_TYPELIST_1( pref2logger ) >,
	public boost::enable_shared_from_this< pref > {
public:
	typedef atom::node< LOKI_TYPELIST_1( pref2logger ) >
		base_t;
	///
	typedef boost::shared_ptr< pref >
		shared_ptr;
	///
	static shared_ptr create( logger::shared_ptr l ) {
		return shared_ptr( new pref( l ) );
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
	pref( logger::shared_ptr l );
	///
	atom::po
		po;
};