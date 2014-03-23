#include "./pch.hpp"
#include <boost/algorithm/string.hpp>
#include "./logger.hpp"
#include "./comp.hpp"
#include "./env.hpp"

env::env( logger_ptr l, appl_ptr a, string_t const & n, env_paths const & ps ) :
		name( n )
	,	paths( ps )
	,	config()
	,	actions()
{
	atom::mount<env2logger>( this, l );
	atom::mount<env2appl>( this, a );
	//
	// open conf file
	try {
		boost::property_tree::read_json( this->get_paths().get_conf_file().string(), this->config );
	} catch (std::exception& e ){
		*(this->get_logger()) << e.what() << std::endl;
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
	boost::filesystem::path catalog( this->get_paths().get_dpm_file( boost::filesystem::path( this->config.get<string_t>( CONST_DPM_CONF_CATALOG_FILE ) ) ) );
	if ( boost::filesystem::exists( catalog ) ) {
		boost::property_tree::ptree catalog_pt;
		boost::property_tree::read_json( catalog.string(), catalog_pt );
		//
		BOOST_FOREACH( const boost::property_tree::ptree::value_type& child, catalog_pt.get_child( CONST_CATALOG_CONF_COMPONENT )) {
			comp::create( this->get_logger(), this->get_appl(), this->shared_from_this(), child.second )->update();
		}
	} else {
		throw std::exception( "Environment catalog.conf file doesn't exist" );
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
env::action( string_t const& a, unsigned int const l, bool const r, bool const v ) try {
	actions_t::iterator it = this->actions.find( a );
	//
	if ( it != this->actions.end() ) {
		(*it).second( a, l );
		//
		if ( v ) {
			//this->get_slot<env2comps>().for_each( boost::bind( _::__, _1, boost::cref( id ), boost::ref( c ) ) );
		}
		//
		if ( r ) {
			this->get_slot<env2envs>().for_each( boost::bind( &env::action, _1, a, l + 1, r, v ) );
		}
	} else {
	}
}
catch( std::exception& e ) {
	*(this->get_logger()) << e.what() << std::endl;
}

void
env::init() {
	actions[ CONST_CMD_INFO ] = boost::bind( &env::info, this->shared_from_this(), _1, _2 );
	actions[ CONST_CMD_SYNC ] = boost::bind( &env::sync, this->shared_from_this(), _1, _2 );
	actions[ CONST_CMD_STATUS ] = boost::bind( &env::status, this->shared_from_this(), _1, _2 );
}

void
env::sync( string_t const& a, unsigned int const l ) {
	this->print_title( 0, true );
	//boost::property_tree::ptree::const_iterator it = this->config.find("pi");
	boost::filesystem::path dpm = this->get_paths().get_dpm();
	bool const checkout = !boost::filesystem::exists( dpm );
	//
	//string_t repo = this->config.get( CONST_DPM_CONF_REPO_GIT, string_t() );
	string_t repo = this->config.get<string_t>( CONST_DPM_CONF_REPO_GIT );
	string_t remote = this->config.get<string_t>( CONST_DPM_CONF_REPO_REMOTE );
	string_t branch = this->config.get<string_t>( CONST_DPM_CONF_REPO_BRANCH );
	string_t user_name = this->config.get<string_t>( CONST_DPM_CONF_REPO_USER_NAME );
	string_t user_email = this->config.get<string_t>( CONST_DPM_CONF_REPO_USER_EMAIL );
	if ( repo.length() ) {
		if ( checkout ) {
			if ( boost::filesystem::create_directory( dpm ) ) {
				atom::exec( string_t("git clone ") + repo + string_t(" ./"), dpm.string() );
				atom::exec( string_t("git checkout ") + branch, dpm.string() );
				atom::exec( string_t("git config --local user.name \"") + user_name + string_t("\""), dpm.string() );
				atom::exec( string_t("git config --local user.email \"") + user_email + string_t("\""), dpm.string() );
			} else {
				throw std::exception( ".dpm folder creation error" );
			}
		} else {
			atom::exec( string_t("git checkout ") + branch, dpm.string() );
			atom::exec( string_t("git pull ") + remote + string_t(" ") + branch, dpm.string() );
		}
	} else {
		string_t repo = this->config.get( CONST_DPM_CONF_REPO_SVN, string_t() );
		if ( repo.length() ) {
		} else {
			throw std::exception( "There is no repository info at dpm.conf file" );
		}
	}
}

	//if ( v ) {
	//	*(this->get_logger()) << " {";
	//	this->get_slot<env2comps>().for_each( boost::bind( &comp::info, _1, boost::cref( offs + string_t( "   " ) ) ) );
	//	*(this->get_logger()) << " }";
	//}
	////
	//if ( r ) {
	//	string_t s = offs + string_t( " " );
	//	this->get_slot<env2envs>().for_each( boost::bind( &env::info, _1, boost::cref( s ), r, v ) );
	//}

void
env::info( string_t const& a, unsigned int const l ) {
	this->print_title( l, true );
}

void
env::status( string_t const& a, unsigned int const l ) {
	this->print_title( 0, true );
	boost::filesystem::path dpm = this->get_paths().get_dpm();
	bool const checkout = !boost::filesystem::exists( dpm );
	//
	//string_t repo = this->config.get( CONST_DPM_CONF_REPO_GIT, string_t() );
	string_t repo = this->config.get<string_t>( CONST_DPM_CONF_REPO_GIT );
	if ( repo.length() ) {
		if ( checkout ) {
			throw std::exception( ".dpm folder doesn't exist" );
		} else {
			atom::exec( string_t("git status"), dpm.string() );
		}
	} else {
		string_t repo = this->config.get( CONST_DPM_CONF_REPO_SVN, string_t() );
		if ( repo.length() ) {
		} else {
			throw std::exception( "There is no repository info at dpm.conf file" );
		}
	}
}

void
env::print_title( unsigned int const l, bool el ) {
	assert( l < 512 );
	char_t offset[512] = {' '};
	offset[ l ] = '\0';
	*(this->get_logger()) << offset << "[" << this->name << "@" << this->get_paths().get_home() << "]";
	//
	if ( el ) {
		*(this->get_logger()) << std::endl;
	}
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
