#pragma once

class comp :
	public atom::node< LOKI_TYPELIST_3( comp2logger, comp2appl, comp2env ) >,
	public boost::enable_shared_from_this< comp > {

	typedef atom::node< LOKI_TYPELIST_3( comp2logger, comp2appl, comp2env ) >
		base_node_t;

public:
	///
	static comp_ptr create( logger_ptr l, appl_ptr a, env_ptr e, string_t const & id, boost::filesystem::path const & h ) {
		comp_ptr result = comp_ptr( new comp( l, a, id, h ) );
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
	void
	print( logger_ptr l, string_t const& offs );

protected:
	//
	logger_ptr get_logger() {
		return ( get_slot<comp2logger>().item() );
	}

private:
	string_t
		id;
	boost::filesystem::path
		home;
	///
	comp( logger_ptr l, appl_ptr a, string_t const & id, boost::filesystem::path const & h );
};