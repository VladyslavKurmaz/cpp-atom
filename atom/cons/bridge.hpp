#pragma once
#include <atom/util/tty.hpp>
#include "./classes.hpp"


class bridge : public boost::noncopyable {

public:
	typedef boost::function< void() >
		on_exit_t;
	///
	bridge() :
			server( true )
		,	on_exit()
		,	mutex_name( gen_guid() )
		,	wpipe_name( gen_guid() )
		,	rpipe_name( gen_guid() )
		,	mutex()
		,	wpipe()
		,	rpipe()
		,	guard_thread() {
		SetConsoleCtrlHandler( handler, TRUE );
	}
	///
	~bridge(){
		SetConsoleCtrlHandler( handler, FALSE );
	}
	///
	static BOOL WINAPI
	handler( DWORD dwCtrlType );
	///
	void run( on_exit_t oe );
	///
	void run( on_exit_t oe, string_t const& mn, string_t const& wpn, string_t const& rpn );
	///
	void join();
	///
	void write( bridge_msg::type const id, void const* param );

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
	void guard();
	///
	void read();

private:
	bool
		server;
	on_exit_t
		on_exit;
	string_t
		mutex_name;
	string_t
		wpipe_name;
	string_t
		rpipe_name;
	///
	atom::mutex
		mutex;
	atom::pipe
		wpipe;
	atom::pipe
		rpipe;
	boost::thread
		guard_thread;

};