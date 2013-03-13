#pragma once
#include "./classes.hpp"

typedef atom::nstorage< logger, boost::shared_ptr, atom::narray1 > process2logger;
typedef atom::nstorage< frame, boost::shared_ptr, atom::narray1 > process2frame;

class process :
	public atom::node< LOKI_TYPELIST_2( process2logger, process2frame ) >,
	public boost::enable_shared_from_this< process > {
		typedef atom::node< LOKI_TYPELIST_2( process2logger, process2frame ) >
			base_node_t;
public:
	///
	static process_ptr process::create( logger_ptr l, frame_ptr f ) {
		return process_ptr( new process( l, f ) );
	}
	///
	~process();
	///
	void
	run( uni_string const& cmd );
	///
	void 
	write( std::string const& str );
	///
	void 
	write( char const ch );
	///
	void 
	write( void const * b, DWORD const b_sz );
	///
	TCHAR const* 
	get_buffer() {
		return ( this->buffer.c_str() );
	}
	///
	bool is_running() {
		return ( this->run_thread ); }
	///
	process& 
	close();
	///
	process& 
	clear();

protected:
	//
	logger&
	get_logger() {
		return ( *( get_value( boost::mpl::identity< process2logger >() ).item() ) ); }
	//
	static DWORD WINAPI
	read_from_pipe( LPVOID lpvThreadParam );

private:
	//
	HANDLE
		std_in;
	//
	HANDLE
		child_process;
	//
	HANDLE
		output_read;
	//
	HANDLE
		input_write;
	//
	HANDLE
		thread;
	//
	DWORD
		threadid;
	//
	bool
		run_thread;
	//
	uni_string
		buffer;
	///
	DWORD
		bytes_wrote;
	///
	DWORD
		bytes_read;
	///
	process( logger_ptr l, frame_ptr f );
};