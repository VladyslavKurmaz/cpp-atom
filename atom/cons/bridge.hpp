#pragma once
#include <atom/util/tty.hpp>
#include "./classes.hpp"


class bridge : public boost::noncopyable {

public:
	///
	bridge() :
			server( true )
		,	mutex_name( gen_guid() )
		,	pipe_name( gen_guid() )
		,	shmem_name( gen_guid() )
		,	mutex()
		,	pipe()
		,	read_thread()
#ifdef STANDALONE
		,	emul_thread()
#endif

	{}
	///
	bridge( string_t const& mn, string_t const& pn, string_t const& sn ) :
			server( false )
		,	mutex_name( mn )
		,	pipe_name( pn )
		,	shmem_name( sn )
		,	mutex()
		,	pipe()
		,	read_thread()
#ifdef STANDALONE
		,	emul_thread()
#endif
	{}
	///
	~bridge(){
		SetConsoleCtrlHandler( handler, FALSE );
	}
	///
	static BOOL WINAPI
	handler( DWORD dwCtrlType );
	///
	bridge& init();
	///
	bridge& bind();
	///
	bridge& run();
	///
	void write_to_pipe( bridge_msg::type const id, void const* param );
	///
	void
	close();

protected:
	///
	static string_t gen_guid() {
		GUID guid;
		CoCreateGuid( &guid );
		stringstream_t gss;
		gss << guid;
		return ( gss.str() );
	}
	///
	void read_from_pipe();
	///
#ifdef STANDALONE
	void emul_cons();
#endif

private:
	///
	bool
		server;
	string_t
		mutex_name;
	string_t
		pipe_name;
	string_t
		shmem_name;
	///
	atom::mutex
		mutex;
	atom::pipe
		pipe;
	boost::thread
		read_thread;
#ifdef STANDALONE
	boost::thread
		emul_thread;
#endif

};