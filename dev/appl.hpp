#pragma once

class appl :
	public atom::node< LOKI_TYPELIST_2( appl2logger, appl2entity ) >,
	public boost::enable_shared_from_this< appl > {

	typedef atom::node< LOKI_TYPELIST_2( appl2logger, appl2entity ) >
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
	entity_ptr
	get_root() {
		return ( get_slot<appl2entity>().item() ); }
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
	string_t
		msbuild;
	///
	entity_ptr
		cursor;
};