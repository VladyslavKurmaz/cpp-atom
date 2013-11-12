#include "./pch.hpp"
#include <boost/algorithm/string.hpp>
#include "./logger.hpp"
#include "./comp.hpp"
#include "./env.hpp"

env::env( logger_ptr l, appl_ptr a, string_t const & n, boost::filesystem::path const & h ) :
		name( n )
	,	paths( h )
{
	atom::mount<env2logger>( this, l );
	atom::mount<env2appl>( this, a );
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
	// open env json config file
	boost::filesystem::path cfg( this->get_paths().get_config_file() );
	if ( boost::filesystem::exists( cfg ) ) {
		boost::property_tree::ptree cfg_pt;
		boost::property_tree::read_json( cfg.string(), cfg_pt );
		//
		BOOST_FOREACH( const boost::property_tree::ptree::value_type& child, cfg_pt.get_child("components")) {
			comp::create( this->get_logger(), this->get_appl(), this->shared_from_this(), child.second )->update();
		}
	} else {
		stringstream_t ss;
		ss << "configuration file doesn't exist: " << cfg << std::endl;
		throw std::runtime_error( ss.str().c_str() );
	}
	//
	// scan child environments
	for ( boost::filesystem::directory_iterator end, dir( this->get_paths().get_env() ); dir != end; ++dir ) {
		env_paths p( *dir );
		if ( boost::filesystem::exists( p.get_config_file() ) ) {
			boost::filesystem::path id = boost::filesystem::path( this->name ) / (*dir).path().leaf();
			std::string sid = id.string();
			std::replace( sid.begin(), sid.end(), bslash, slash );
			env::create( this->get_logger(), this->get_appl(), this->shared_from_this(), sid, p.get_home() )->scan();
		}
	}
}
catch( std::exception& ) {
}

void
env::print( logger_ptr l, env_ptr ce, string_t const& offs, bool const v ) {
	*l << (( ce.get() == this )?("* "):("  ")) << offs << "[" << this->name << "] " << this->get_paths().get_home();
	if ( v ) {
		*l << " {" << std::endl;
		print_c( l, offs + string_t( "   " ) );
		*l << offs << "  }";
	}
	*l << std::endl;
	//
	string_t s = offs + string_t( "  " );
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

void
env::execute( string_t const& id, string_t const& cmd ) {
	std::vector< string_t > ids;
	boost::split( ids, id, boost::is_any_of( ";" ) );
	std::vector< string_t > cmds;
	boost::split( cmds, cmd, boost::is_any_of( ";" ) );
	//
	BOOST_FOREACH( string_t const& i, ids ) {
		comp_ptr component;
		struct _ { static void __( comp_ptr component, string_t const& id, comp_ptr& r ) {
			if ( component->get_id() == id ) { r = component; } }; };
		//
		this->get_slot<env2comps>().for_each( boost::bind( _::__, _1, boost::cref( i ), boost::ref( component ) ) );
		if ( component ) {
			BOOST_FOREACH( string_t const& c, cmds ) {
				component->execute( c );
			}
		} else {
			throw std::runtime_error( "[err] Unknown component" );
		}
	}
}
