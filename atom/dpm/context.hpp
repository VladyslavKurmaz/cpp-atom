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
	///
	void
	add_env_var( string_t const& n, string_t const& v );
	///
	void dump() {
		logger_ptr l = this->get_logger();
		BOOST_FOREACH( env_var_pair_t const& p, this->env_vars ) {
			(*l) << "SET " << p.first << "=" << p.second << std::endl;
		}
	}

protected:
	//
	logger_ptr get_logger() {
		return ( get_slot<context2logger>().item() );
	}


private:
	typedef std::pair< string_t, string_t >
		env_var_pair_t;
	typedef std::vector< env_var_pair_t >
		env_vars_t;
	env_vars_t
		env_vars;
	///
	context( logger_ptr l );
};
