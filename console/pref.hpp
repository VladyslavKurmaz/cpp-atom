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
		pgEmpty,
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
	parse( std::string const& s );
	///
	template <typename T >
	T get( atom::po::id_t const id ) {
		return ( this->po.as<T>( id ) );
	}

	void
	calculateDocks( placement& windowPlacement );

protected:
	//
	logger& getLogger() {
		return ( *( get_value( boost::mpl::identity< pref2logger >() ).item() ) );
	}
	//
	void id2gr( atom::po::id_t const id, pref_group_t const g ) {
		this->prefGroups.insert( std::pair<atom::po::id_t, pref_group_t >( id, g ) );
	}

private:
	///
	typedef std::map< atom::po::id_t, pref_group_t >
		prefGroupsMap;
	typedef std::map< pref_group_t, std::pair< bool, callback_t > >
		processCallbacksMap;
	///
	pref( logger_ptr l );
	///
	atom::po
		po;
	///
	prefGroupsMap
		prefGroups;
	///
	processCallbacksMap
		processCallbacks;
};