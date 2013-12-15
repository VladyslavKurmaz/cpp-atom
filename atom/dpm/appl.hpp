#pragma once

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
	///
	std::string
		def_env;
	///
	bool
		shell_mode;
	///
	string_t
		home;
	///
	string_t
		msbuild;
	///
	env_ptr
		cenv;
};