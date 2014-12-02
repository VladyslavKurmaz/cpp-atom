#pragma once
#include "./classes.hpp"

#if 0
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
	uni_string
	run( uni_string const& cmd );
	///
	process& 
	kbrd( KEY_EVENT_RECORD const& key );
	///
	process& 
	close();
	///
	process& 
	ctrl_break();
	///
	process& 
	terminate();
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
	///
	process( logger_ptr l, frame_ptr f );
};
#endif