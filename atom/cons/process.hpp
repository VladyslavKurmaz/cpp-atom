#pragma once

#include "./log.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > process2logger;

class process :
	public atom::node< LOKI_TYPELIST_1( process2logger ) >,
	public boost::enable_shared_from_this< process > {
public:
	///
	typedef boost::shared_ptr< process >
		shared_ptr;
	///
	static shared_ptr process::create( logger::shared_ptr l ) {
		return shared_ptr( new process( l ) );
	}
	///
	~process();
	///
	void
		run( std::basic_string<TCHAR> const& cmd );
	///
	void 
		write( std::string const& str );
	///
	void 
		write( char const ch );
	///
	TCHAR const* 
	get_buffer() {
		return ( this->buffer.c_str() );
	}
	///
	void 
	append( CHAR* s ) {
		this->buffer += s;
	}
	///
	void 
		close();
	//
	logger& get_logger() {
		return ( *( get_value( boost::mpl::identity< process2logger >() ).item() ) );
	}

protected:
private:
	std::basic_string<TCHAR>
		buffer;
	///
	process( logger::shared_ptr l );
};