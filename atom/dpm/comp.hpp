#pragma once

class comp :
	public atom::node< LOKI_TYPELIST_3( comp2logger, comp2appl, comp2env ) >,
	public boost::enable_shared_from_this< comp > {

	typedef atom::node< LOKI_TYPELIST_3( comp2logger, comp2appl, comp2env ) >
		base_node_t;

public:
	///
	static comp_ptr create( logger_ptr l, appl_ptr a, env_ptr e, boost::property_tree::ptree const& pt ) {
		comp_ptr result = comp_ptr( new comp( l, a, pt ) );
		atom::mount<env2comps>( e, result );
		atom::mount<comp2env>( result, e );
		return result;
	}
	///
	~comp();
	///
	void
	clear();
	///
	string_t
	get_id() const {
		return ( this->props.get<string_t>("id") );
	}
	///
	void
	update();
	///
	void
	print( logger_ptr l, string_t const& offs );
	///
	void
	execute( string_t const& cmd );

protected:
	//
	logger_ptr get_logger() {
		return ( get_slot<comp2logger>().item() );
	}

private:
	boost::property_tree::ptree
		props;
	///
	comp( logger_ptr l, appl_ptr a, boost::property_tree::ptree const& pt );
};