#pragma once
#include "./classes.hpp"


class cons_mpp : public boost::noncopyable {
	///
	typedef std::basic_string< TCHAR >
		string_t;
	typedef std::basic_stringstream< TCHAR >
		stringstream_t;
	typedef boost::shared_ptr< boost::interprocess::windows_shared_memory >
		shmem_t;
	typedef boost::shared_ptr< boost::interprocess::mapped_region >
		shmem_region_t;

public:
	///
	cons_mpp();
	///
	~cons_mpp();
	///
	string_t
	get_cmdline() {
		stringstream_t ss;
		ss
			<< " --pipe=" << this->pipe_name
			<< " --mutex=" << this->mutex_name
			<< " --header=" << this->header_name
			;
		return ( ss.str() ); }
	///
	string_t
	get_pipe_name() {
		return ( this->pipe_name ); }
	///
	string_t
	get_mutex_name() {
		return ( this->mutex_name ); }
	///
	string_t
	get_header_name() {
		return ( this->header_name ); }
	/// from server side
	bool server_init( HWND hWnd, unsigned int const width, unsigned int const height );
	///
	bool server_bind();
	///
	void cons_mpp::close();
	/// from client side
	void client_run( string_t const& pname, string_t const& mname, string_t const& hname );
	///
	void
	process( command::type const id, void const* param );
	///
	void draw( HDC dc, RECT const& rt, LONG const cw, LONG const ch );

protected:
	///
	string_t
	gen_guid();
	///
	template < typename T >
	bool
	build_shmem( string_t const& name, size_t const size, shmem_t & shmem, shmem_region_t & shmem_region, T*& p ) {
		//
		bool const create = ( size > 0 );
		if ( create ) {
			shmem = shmem_t( 
				new boost::interprocess::windows_shared_memory( 
						boost::interprocess::create_only,
						name.c_str(),
						boost::interprocess::read_write,
						size )
					);
		} else {
			shmem = shmem_t( 
						new boost::interprocess::windows_shared_memory(
						boost::interprocess::open_only,
						name.c_str(),
						boost::interprocess::read_write )
					);
		}
		shmem_region = shmem_region_t(
							new boost::interprocess::mapped_region(
								*(shmem.get()),
								boost::interprocess::read_write )
						);
		//
		p = static_cast< T* >( shmem_region->get_address() );
		if ( create && p ) {
			memset( p, 0, size );
		}
		return true;
	}
	///
	void
	copy_csb();

private:
	///
	string_t
		pipe_name;
	atom::pipe
		pipe;
	///
	string_t
		mutex_name;
	atom::mutex
		mutex;
	///
	string_t
		header_name;
	shmem_t
		header;
	shmem_region_t
		header_region;
	struct header_t {
		CONSOLE_SCREEN_BUFFER_INFOEX
			csbiex;
		bool
			invalid;
		TCHAR
			buffer_name[64];
	} *header_ptr;
	///
	shmem_t
		buffer;
	shmem_region_t
		buffer_region;
	CHAR_INFO
		*buffer_ptr;
	///
	HWND
		wnd;
	///
	HANDLE
		child;
	///
	PROCESS_INFORMATION
		pi;
	///
	STARTUPINFO
		si;
};