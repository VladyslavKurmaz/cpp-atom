#pragma once

class entity :
	public atom::node< LOKI_TYPELIST_3( entity2logger, entity2entity, entity2entities ) >,
	public boost::enable_shared_from_this< filter > {

	typedef atom::node< LOKI_TYPELIST_3( entity2logger, entity2entity, entity2entities ) >
		base_node_t;

public:
	///
	~entity();
	///
	void
	clear();

protected:
	//
	logger_ptr get_logger() {
		return ( get_slot<filter2logger>().item() );
	}


private:
	///
	entity( logger_ptr l, entity_ptr p );
};
