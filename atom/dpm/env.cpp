#include "./pch.hpp"
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
	atom::clear_r< env_ptr, env2envs >( this->shared_from_this() );
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
			boost::filesystem::path ncp = this->get_paths().get_env();
			string_t id = child.second.get<string_t>("id");
			ncp /= boost::filesystem::path( id );
			comp_ptr nc = comp::create( this->get_logger(), this->get_appl(), this->shared_from_this(), id, ncp );
			//??????? load info from json to component
			//?????/ scan component folder
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
			boost::filesystem::path id = (*dir).path();
			id = id.leaf();
			env::create( this->get_logger(), this->get_appl(), this->shared_from_this(), id.string(), p.get_home() )->scan();
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
	struct _ {
		static bool __( env_ptr e, logger_ptr l, env_ptr ce, string_t const& offs, bool const v ) {
			e->print( l, ce, offs, v );
			return false;
		};
	};
	string_t s = offs + string_t( "  " );
	this->get_slot<env2envs>().for_each( boost::bind( _::__, _1, l, ce, boost::cref( s ), v ) );
}

void
env::print_c( logger_ptr l, string_t const& offs ){
	struct _ {
		static bool __( comp_ptr c, logger_ptr l, string_t const& offs ) {
			c->print( l, offs );
			return false;
		};
	};
	this->get_slot<env2comps>().for_each( boost::bind( _::__, _1, l, boost::cref( offs ) ) );
}


bool
env::find( string_t const& n, env_ptr& ce ) {
	if ( this->name == n ) {
		ce = this->shared_from_this();
		return true;
	}
	struct _ {
		static bool __( env_ptr e, string_t const& n, env_ptr& ce ) {
			return ( e->find( n, ce ) );
		};
	};
	return ( this->get_slot<env2envs>().for_each( boost::bind( _::__, _1, boost::cref( n ), boost::ref( ce ) ) ) );
}

void
env::execute( string_t const& id, string_t const& cmd ) {
	comp_ptr c;
	//
	struct _ {
		static bool __( comp_ptr c, string_t const& id, comp_ptr& r ) {
			if ( c->get_id() == id ) {
				r = c;
				return true;
			}
			return ( false );
		};
	};
	this->get_slot<env2comps>().for_each( boost::bind( _::__, _1, boost::cref( id ), boost::ref( c ) ) );
	if ( c ) {
		c->execute( cmd );
	} else {
		throw std::runtime_error( "[err] Unknown component" );
	}
}
