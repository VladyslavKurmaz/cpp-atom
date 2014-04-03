#pragma once

class context :
	public atom::node< LOKI_TYPELIST_1( context2logger ) >,
	public boost::enable_shared_from_this< context > {

	typedef atom::node< LOKI_TYPELIST_1( context2logger ) >
		base_node_t;

public:
	///
	static context_ptr create( logger_ptr l ) {
		context_ptr result = context_ptr( new context( l ) );
		return result;
	}
	///
	~context();
	///
	void
	clear();

protected:

private:
	///
	context( logger_ptr l );
};
