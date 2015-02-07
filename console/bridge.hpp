#pragma once
#include <atom/util/tty.hpp>
#include "./classes.hpp"



class bridge : public boost::noncopyable {
	typedef boost::shared_ptr< boost::interprocess::windows_shared_memory >
		sharedmem_t;
	typedef boost::shared_ptr< boost::interprocess::mapped_region >
		sharedmem_region_t;

public:
	typedef std::deque< std::string >
		lines_t;
	//
	explicit bridge( COORD const& sz );
	//
	explicit bridge( COORD const& sz, std::string const& shname, std::string const& pname );
	//
	~bridge();
	//
	void join();
	//
	void console();
	//
	void proxy();
	//
	void getLines( lines_t& lines );
	//
	void write( KEY_EVENT_RECORD const& k );
	//
	void writeExit();


protected:
	//
	bool sharedmemConfigure( bool const create );
	//
	size_t getSharedmemSize() const {
		return ( sizeof( CHAR_INFO ) * this->consoleSize.X * this->consoleSize.Y );
	}

private:
	//
	COORD
		consoleSize;
	std::string
		sharedmemName;
	std::string
		pipeName;
	//
	atom::proc<TCHAR>
		proc;
	//
	sharedmem_t
		sharedmem;
	sharedmem_region_t
		sharedmemRegion;
	CHAR_INFO*
		sharedmemBuffer;
	//
	atom::pipe
		pipe;
};

#if 0

class bridge : public boost::noncopyable {

public:
	typedef boost::function< void() >
		on_exit_t;
	typedef boost::function< void( TCHAR const* ) >
		on_parse_t;
	///
	bridge() :
			server( true )
		,	on_exit()
		,	on_parse()
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
	void run( on_exit_t oe, on_parse_t op );
	///
	void run( on_exit_t oe, on_parse_t op, std::string const& mn, std::string const& wpn, std::string const& rpn );
	///
	void join();
	///
	void write( bridge_msg::type const id, void const* param );

protected:
	///
	static std::string gen_guid() {
		GUID guid;
		CoCreateGuid( &guid );
		std::stringstream gss;
		gss << guid;
		return ( gss.str() );
	}
	///
	void guard();
	///
	void read();
	///
	void tty();

private:
	bool
		server;
	on_exit_t
		on_exit;
	on_parse_t
		on_parse;
	//
	std::string
		mutex_name;
	std::string
		wpipe_name;
	std::string
		rpipe_name;
	//
	atom::mutex
		mutex;
	atom::pipe
		wpipe;
	atom::pipe
		rpipe;
	boost::thread
		guard_thread;

};
#endif