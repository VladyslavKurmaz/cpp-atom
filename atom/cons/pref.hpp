#pragma once
#include "./classes.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > pref2logger;

class pref :
	public atom::node< LOKI_TYPELIST_1( pref2logger ) >,
	public boost::enable_shared_from_this< pref > {
	typedef atom::node< LOKI_TYPELIST_1( pref2logger ) >
		base_t;
public:
	enum pref_group_t {
		pgAutostart,
		pgHotkeys,
		pgFonts,
		pgWindow,
		pgUI
	};
	typedef boost::function< void() >
		callback_t;
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
	void
	register_process_callback( pref_group_t const g, callback_t c );
	///
	bool
	parse( string_t const& s );
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

private:
	///
	typedef std::multimap< atom::po::id_t, pref_group_t >
		pref_groups_t;
	typedef std::map< pref_group_t, std::pair< bool, callback_t > >
		process_callbacks_t;
	///
	pref( logger_ptr l );
	///
	atom::po
		po;
	///
	pref_groups_t
		pref_groups;
	///
	process_callbacks_t
		process_callbacks;
};