#pragma once

class filter :
	public atom::node< LOKI_TYPELIST_1( filter2logger ) >,
	public boost::enable_shared_from_this< filter > {

	typedef atom::node< LOKI_TYPELIST_1( filter2logger ) >
		base_node_t;

public:
	///
	static filter_ptr create( logger_ptr l ) {
		filter_ptr result = filter_ptr( new filter( l ) );
		return result;
	}
	///
	~filter();
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
	filter( logger_ptr l );
};
