#include "./pch.hpp"
#include "./logger.hpp"
#include "./comp.hpp"
#include "./env.hpp"


env::env( logger_ptr l, appl_ptr a, string_t const & n, string_t const & h ) :
		name( n )
	,	home( h )
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
	boost::filesystem::path cfg( get_config_file( this->home ) );
	if ( boost::filesystem::exists( cfg ) ) {
		boost::property_tree::ptree cfg_pt;
		boost::property_tree::read_json( cfg.string(), cfg_pt );
		//
		BOOST_FOREACH( const boost::property_tree::ptree::value_type& child, cfg_pt.get_child("components")) {
			boost::filesystem::path ncp = get_env_folder( this->home );
			string_t id = child.second.get<string_t>("id");
			ncp /= boost::filesystem::path( id );
			comp_ptr nc = comp::create( this->get_logger(), this->get_appl(), this->shared_from_this(), id, ncp );
			//??????? load info from json to component
		}
	} else {
		stringstream_t ss;
		ss << "configuration file doesn't exist: " << cfg << std::endl;
		throw std::runtime_error( ss.str().c_str() );
	}
	//
	// scan components
	// scan child environments
	// for ( boost::filesystem::recursive_directory_iterator end, dir( this->home ); dir != end; ++dir ) {
	for ( boost::filesystem::directory_iterator end, dir( this->home ); dir != end; ++dir ) {
		//*get_logger() << *dir << std::endl;                                    
	}
}
catch( std::exception& ) {
}

void
env::print( logger_ptr l, env_ptr ce, string_t const& offs, bool const pc ) {
	*l << (( ce.get() == this )?("* "):("  ")) << offs << "[" << this->name << "] " << this->home << "" << std::endl;
	//
	struct _ {
		static bool __( env_ptr e, logger_ptr l, env_ptr ce, string_t const& offs, bool const pc ) {
			e->print( l, ce, offs, pc );
			return false;
		};
	};
	string_t s = offs + string_t( " " );
	this->get_slot<env2envs>().for_each( boost::bind( _::__, _1, l, ce, boost::cref( s ), pc ) );
	if ( pc ) {
		print_c( l, s + string_t( " " ) );
	}
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

boost::filesystem::path
env::get_dpm_folder( boost::filesystem::path const& h ) {
	return ( boost::filesystem::path( h ).operator/=( boost::filesystem::path( folder_dpm ) ) );
}

boost::filesystem::path
env::get_dl_folder( boost::filesystem::path const& h ) {
	return ( boost::filesystem::path( h ).operator/=( boost::filesystem::path( folder_dl ) ) );
}

boost::filesystem::path
env::get_env_folder( boost::filesystem::path const& h ) {
	return ( boost::filesystem::path( h ).operator/=( boost::filesystem::path( folder_env ) ) );
}

boost::filesystem::path
env::get_config_file( boost::filesystem::path const& h ) {
	return ( boost::filesystem::path( get_dpm_folder( h ) ).operator/=( boost::filesystem::path( file_config ) ) );
}
