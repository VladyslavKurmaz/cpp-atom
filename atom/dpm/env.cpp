#include "./pch.hpp"
#include <boost/algorithm/string.hpp>
#include "./logger.hpp"
#include "./comp.hpp"
#include "./env.hpp"

env::env( logger_ptr l, appl_ptr a, string_t const & n, env_paths const & ps ) :
		name( n )
	,	paths( ps )
	,	config()
{
	atom::mount<env2logger>( this, l );
	atom::mount<env2appl>( this, a );
	//
	// open conf file
	try {
		boost::property_tree::read_json( this->get_paths().get_conf_file().string(), this->config );
	} catch (std::exception& ){
	}
}

env::~env() {
}

void
env::clear() {
	atom::clear_slot< comp_ptr, env2comps >( this->shared_from_this() );
	atom::clear_slot< env_ptr, env2envs >( this->shared_from_this() );
	base_node_t::clear();
}

void
env::scan() try {
	//
	// reset child envs and components

	//
	// open catalog file
	boost::filesystem::path catalog( this->get_paths().get_dpm_file( boost::filesystem::path( this->config.get<string_t>( dpm_conf_prop_catalog_file ) ) ) );
	if ( boost::filesystem::exists( catalog ) ) {
		boost::property_tree::ptree catalog_pt;
		boost::property_tree::read_json( catalog.string(), catalog_pt );
		//
		BOOST_FOREACH( const boost::property_tree::ptree::value_type& child, catalog_pt.get_child( catalog_conf_component )) {
			comp::create( this->get_logger(), this->get_appl(), this->shared_from_this(), child.second )->update();
		}
	} else {
	}
	//
	// scan child environments
	for ( boost::filesystem::directory_iterator end, dir( this->get_paths().get_home() ); dir != end; ++dir ) {
		boost::filesystem::path id = boost::filesystem::path( this->name ) / (*dir).path().leaf();
		std::string sid = id.string();
		std::replace( sid.begin(), sid.end(), bslash, slash );
		//
		env_ptr e = env::create( this->get_logger(), this->get_appl(), this->shared_from_this(), sid, *dir );
		if ( e ) {
			e->scan();
		}
	}
}
catch( std::exception& e ) {
	*(this->get_logger()) << e.what() << std::endl;
}

void
env::sync( bool const r ) {
}

void
env::print( logger_ptr l, env_ptr ce, string_t const& offs, bool const v ) {
	*l << (( ce.get() == this )?("*"):(" ")) << offs << "[" << this->name << " @ " << this->get_paths().get_home() << "]";
	if ( v ) {
		*l << " {";
		print_c( l, offs + string_t( "    " ) );
		*l << " }";
	}
	*l << std::endl;
	//
	string_t s = offs + string_t( " " );
	this->get_slot<env2envs>().for_each( boost::bind( &env::print, _1, l, ce, boost::cref( s ), v ) );
}

void
env::print_c( logger_ptr l, string_t const& offs ){
	this->get_slot<env2comps>().for_each( boost::bind( &comp::print, _1, l, boost::cref( offs ) ) );
}


void
env::find( string_t const& n, env_ptr& ce ) {
	if ( this->name == n ) {
		ce = this->shared_from_this();
	} else {
		return ( this->get_slot<env2envs>().for_each( boost::bind( &env::find, _1, boost::cref( n ), boost::ref( ce ) ) ) );
	}
}

comp_ptr
env::find_comp( string_t const& id ) {
	comp_ptr c;
	struct _ { static void __( comp_ptr component, string_t const& id, comp_ptr& r ) {
		if ( component->get_id() == id ) { r = component; } }; };
	//
	this->get_slot<env2comps>().for_each( boost::bind( _::__, _1, boost::cref( id ), boost::ref( c ) ) );
	//
	if ( !c ) {
		env_ptr p = this->get_parent();
		if ( p ) {
			return ( p->find_comp( id ) );
		} else {
			throw std::runtime_error( ( string_t( "[err] Unknown component:" ) + id ).c_str() );
		}
	}
	return ( c );
}

void
env::execute( string_t const& sids, string_t const& scmds, const bool r ) {
	std::vector< string_t > cmds;
	boost::split( cmds, scmds, boost::is_any_of( ";" ) );
	// build components' list
	comp_deq_t cs;
	comp::parse_depends( sids, this->shared_from_this(), cs, r );
	// execute 
	BOOST_FOREACH( string_t const& cmd, cmds ) {
		BOOST_FOREACH( comp_ptr c, cs ) {
			c->execute( cmd );
		}
	}
}
