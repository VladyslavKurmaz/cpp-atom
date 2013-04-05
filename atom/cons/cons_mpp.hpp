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
	/// from server side
	bool init( HWND hWnd, unsigned int const width, unsigned int const height );
	/// from client side
	bool init( string_t const& pname, string_t const& shmname, HANDLE hin, HANDLE hout );
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