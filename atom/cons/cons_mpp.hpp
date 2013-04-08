#pragma once

class cons_mpp : public boost::noncopyable {
	///
	typedef std::basic_string< TCHAR >
		string_t;

public:
	///
	cons_mpp();
	///
	~cons_mpp();
	///
	string_t
	get_pipe_name() {
		return ( this->pipe_name ); }
	string_t
	get_shmem_name() {
		return ( this->shmem_name ); }
	/// from server side
	bool init( HWND hWnd, unsigned int const width, unsigned int const height );
	/// from client side
	bool init( string_t const& pname, string_t const& shmname, HANDLE hin, HANDLE hout, unsigned int const width, unsigned int const height );
	///
	bool bind();
	///
	void onkey( KEY_EVENT_RECORD const& key );
	///
	void ctrl_break();
	///
	void ctrl_c();
	///
	void onexit();



protected:
	///
	string_t
	gen_guid();
	///
	bool
	build_shmem( bool const create, unsigned int const width, unsigned int const height );
	///
	bool
	handle_kbrd( command const& c );
	///
	bool
	handle_ctrl_break( command const& c );
	///
	bool
	handle_ctrl_c( command const& c );
	///
	bool
	handle_exit( command const& c );


private:
	///
	string_t
		pipe_name;
	///
	atom::pipe
		pipe;
	///
	string_t
		shmem_name;
	///
	boost::shared_ptr< boost::interprocess::windows_shared_memory >
		shmem;
	///
	boost::shared_ptr< boost::interprocess::mapped_region >
		shmem_region;
	///
#pragma warning ( push )
#pragma warning ( disable : 4200 )
	struct shared_block_t {
		CONSOLE_SCREEN_BUFFER_INFOEX
			csbex;
		CHAR_INFO
			csb[0];
	} *shared_block;
#pragma warning ( pop )
	///
	HWND
		wnd;
	///
	HANDLE
		input;
	///
	HANDLE
		output;
};