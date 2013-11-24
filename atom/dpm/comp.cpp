#include "./pch.hpp"
#include "./logger.hpp"
#include "./comp.hpp"
#include "./env.hpp"

comp::comp( logger_ptr l, appl_ptr a, boost::property_tree::ptree const& pt ) :
		props( pt )
{
	atom::mount<comp2logger>( this, l );
	atom::mount<comp2appl>( this, a );
}

comp::~comp() {
}

void
comp::clear() {
	base_node_t::clear();
}

void
comp::update() {
	// load additional json config from project folder
}

void
comp::print( logger_ptr l, string_t const& offs ) {
	*l << offs << this->get_id() << std::endl;
}

void
comp::execute( string_t const& cmd ) {
	//
	*(this->get_logger()) << this->get_id() << " execute" << std::endl;
	BOOST_FOREACH( const boost::property_tree::ptree::value_type& child, this->props.get_child("stages")) {
		//*(this->get_logger()) << this->get_id() << " run:" << child.second.get<string_t>("id") << std::endl;
	}


	//
	// run msbuild
	//stringstream_t ss;
	//ss	<< "msbuild.exe";
		//<< " /p:component=\"" << pos1 << "\""
		//<< " /p:stage=\"" << pos2 << "\""
		//<< " /p:osystem=\"" << this->po.as< string_t >( po_osystem ) << "\""
		//<< " /p:instruction_set=\"" << this->po.as< string_t >( po_instruction_set ) << "\""
		//<< " /p:address_model=\"" << this->po.as< string_t >( po_address_model ) << "\""
		//<< " /p:configuration=\"" << this->po.as< string_t >( po_configuration ) << "\""
		//<< " /p:toolset=\"" << this->po.as< string_t >( po_toolset ) << "\""
		//<< " /p:recursive=" << (( this->po.count( po_recursive ) )?( "true" ):( "false" ))
		//<< std::endl;
	//
	//atom::exec( ss.str(), this->get_env()->get_paths().get_dpm().string() );
}

void comp::parse_depends( string_t const& sids, env_ptr e, comp_deq_t& cs, const bool r ) {
	parse_hierarchy( pt_comp_depends, sids, e, cs, r );
}

void comp::parse_inherits( string_t const& sids, env_ptr e, comp_deq_t& cs, const bool r ) {
	parse_hierarchy( pt_comp_inherits, sids, e, cs, r );
}

void comp::parse_hierarchy( string_t const& key, string_t const& sids, env_ptr e, comp_deq_t& cs, const bool r ) {
	std::vector< string_t > ids;
	boost::split( ids, sids, boost::is_any_of( ";" ) );
	//
	std::vector< comp_ptr > ncs;
	BOOST_FOREACH( string_t const& id, ids ) {
		if ( !id.empty() ) {
			comp_ptr nc = e->find_comp( id );
			comp_deq_t::iterator it = cs.begin();
			comp_deq_t::iterator eit = cs.end();
			for(; it != eit; ++it ) {
				if ( id == (*it)->get_id() ) {
					cs.erase( it );
					break;
				}
			}
			//
			cs.push_front( nc );
			ncs.push_back( nc );
		}
	}
	//
	if ( r ) {
		BOOST_FOREACH( comp_ptr c, ncs ) {
			parse_hierarchy( key, c->props.get<string_t>( key ), c->get_env(), cs, r );
		}
	}
}
