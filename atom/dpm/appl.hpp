#pragma once

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > appl2logger;
typedef atom::nstorage< env, boost::shared_ptr, atom::narray1 > appl2env;

class appl :
	public atom::node< LOKI_TYPELIST_2( appl2logger, appl2env ) >,
	public boost::enable_shared_from_this< appl > {

	typedef atom::node< LOKI_TYPELIST_2( appl2logger, appl2env ) >
		base_node_t;

public:
	///
	static appl_ptr create( logger_ptr l ) {
		return appl_ptr( new appl( l ) );
	}
	///
	~appl();
	///
	bool
	init( int argc, char const * const argv[] );
	///
	void
	run( std::ostream& os, std::istream& is );
	///
	void
	clear();

protected:
	//
	logger_ptr
	get_logger() {
		return ( get_slot<appl2logger>().item() ); }
	//
	env_ptr
	get_env() {
		return ( get_slot<appl2env>().item() ); }
	///
	void
	scan();
	///
	bool
	process_command();
	///
	void
	print_error( atom::po::options_description_t const& desc, std::exception& exc );

private:
	///
	appl( logger_ptr l );
	///
	atom::po
		po;
};