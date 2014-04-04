#include "./pch.hpp"
#include "./logger.hpp"
#include "./context.hpp"
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
comp::build_env_block( context_ptr cont ) {
	comp_deq_t ics;
	parse_inherits( this->get_id(), this->get_env(), ics, true );
	BOOST_FOREACH( comp_ptr c, ics ) {
		get_env()->find_comp( c->get_id() )->build_env_vars( this->get_id(), get_env()->get_paths(), cont );
	}
}

void
comp::action( string_t const& a, unsigned int const l, bool const r ) {
	print_offset( *(this->get_logger()), l + 2 ) << this->get_id() << std::endl;
}

void
comp::update() {
	// load additional json config from project folder
	// merge two property trees http://paste.tbee-clan.de/TX2Vm
	/*
?boost::property_tree::ptree
Dashboard::mergePropertyTrees( const boost::property_tree::ptree& rptFirst, const boost::property_tree::ptree& rptSecond )
{
	// Take over first property tree
	boost::property_tree::ptree ptMerged = rptFirst;

	// Keep track of keys and values (subtrees) in second property tree
	queue<string> qKeys;
	queue<boost::property_tree::ptree> qValues;
	qValues.push( rptSecond );

	// Iterate over second property tree
	while( !qValues.empty() )
	{
		// Setup keys and corresponding values
		boost::property_tree::ptree ptree = qValues.front();
		qValues.pop();
		string keychain = "";
		if( !qKeys.empty() )
		{
			keychain = qKeys.front();
			qKeys.pop();
		}

		// Iterate over keys level-wise
		BOOST_FOREACH( const boost::property_tree::ptree::value_type& child, ptree )
		{
			// Leaf
			if( child.second.size() == 0 )
			{
				// No "." for first level entries
				string s;
				if( keychain != "" )
					s = keychain + "." + child.first.data();
				else
					s = child.first.data();

				// Put into combined property tree
				ptMerged.put( s, child.second.data() );
			}
			// Subtree
			else
			{
				// Put keys (identifiers of subtrees) and all of its parents (where present)
				// aside for later iteration. Keys on first level have no parents
				if( keychain != "" )
					qKeys.push( keychain + "." + child.first.data() );
				else
					qKeys.push( child.first.data() );

				// Put values (the subtrees) aside, too
				qValues.push( child.second );
			}
		}  // -- End of BOOST_FOREACH
	}  // --- End of while

	return ptMerged;
}
	*/
}

void
comp::info( string_t const& offs ) {
	*(this->get_logger()) << std::endl << offs << this->get_id();
}

void
comp::execute( string_t const& c ) {
	context_ptr cont = context::create( this->get_logger() );
	//
	comp_deq_t dcs;
	parse_depends( this->get_id(), this->get_env(), dcs, true );
	BOOST_FOREACH( comp_ptr c, dcs ) {
		get_env()->find_comp( c->get_id() )->build_env_block( cont );
	}

#if 0
	//
	logger& l = *(this->get_logger());
	//
	print_title( l, this->get_id(), 0 ) << std::endl;
	boost::optional< boost::property_tree::ptree& > package = this->props.get_child_optional( CONST_PT_COMP_PACKAGE );
	if( package )
	{
		BOOST_FOREACH( const boost::property_tree::ptree::value_type& child, this->props.get_child( CONST_PT_COMP_PACKAGE )) {
			l << child.first.c_str() << " : " << child.second.data().c_str() << std::endl;
			//*(this->get_logger()) << this->get_id() << " run:" << child.second.get<string_t>("id") << std::endl;

			//BOOST_FOREACH(ptree::value_type &v,
			//            pt.get_child("debug.modules"))
			//        m_modules.insert(v.second.data());

		}
	}

	//
	comp_deq_t cs;
	parse_inherits( this->get_id(), this->get_env(), cs, true );
	//collect var array and script arrays
	//	construct and execute cmd file
	BOOST_FOREACH( comp_ptr c, cs ) {
		l << "* " << c->get_id() << std::endl;
	}
	l << std::endl;


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
#endif
}

void
comp::build_env_vars( string_t const& prefix, env_paths const& ep, context_ptr cont ){
	logger& l = *(this->get_logger());
	//
	boost::optional< boost::property_tree::ptree& > var = this->props.get_child_optional( CONST_PT_COMP_VAR );
	if( var ) {
		boost::filesystem::path const& home = ep.get_home();
		BOOST_FOREACH( const boost::property_tree::ptree::value_type& child, this->props.get_child( CONST_PT_COMP_VAR )) {
			boost::filesystem::path p = home;
			p /= boost::filesystem::path( child.second.data().c_str() );
			l << prefix << child.first.c_str() << "=" << p.string() << std::endl;
		}
	}
}

void
comp::parse_depends( string_t const& sids, env_ptr e, comp_deq_t& cs, const bool r ) {
	parse_hierarchy( CONST_PT_COMP_DEPENDS, sids, e, cs, r );
}

void
comp::parse_inherits( string_t const& sids, env_ptr e, comp_deq_t& cs, const bool r ) {
	parse_hierarchy( CONST_PT_COMP_INHERITS, sids, e, cs, r );
}

void
comp::parse_hierarchy( string_t const& key, string_t const& sids, env_ptr e, comp_deq_t& cs, const bool r ) {
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
			parse_hierarchy( key, c->props.get( key, string_t() ), c->get_env(), cs, r );
		}
	}
}

