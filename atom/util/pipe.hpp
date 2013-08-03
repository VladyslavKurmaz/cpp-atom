/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2013.03.27 - Created                                           |
|-----------------------------------------------------------------------------|
|                                                                             |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_UTIL_PIPE_HPP
#define ATOM_UTIL_PIPE_HPP
#if defined(_WIN32)

#include <windows.h>
#include <sstream>

namespace atom {
	//----------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------
	class pipe : boost::noncopyable {
	public:
		///
		pipe() : 
				p( INVALID_HANDLE_VALUE )
			,	name()
			,	server( false ){
		}
		///
		~pipe() {
			close();
		}
		///
		std::string get_name() const {
			return ( this->name ); }
		///
		bool create( std::string const & name ) {
			if ( !this->is_valid() ) {
				server = true;
				this->name = name;
				std::stringstream ss;
				ss << "\\\\.\\pipe\\" << this->name;
				this->p = CreateNamedPipe( 
					ss.str().c_str(),           // pipe name 
					PIPE_ACCESS_DUPLEX,			// read/write access 
					PIPE_TYPE_MESSAGE |			// message type pipe 
					PIPE_READMODE_MESSAGE |		// message-read mode 
					PIPE_WAIT,					// blocking mode 
					PIPE_UNLIMITED_INSTANCES,	// max. instances  
					512,						// output buffer size 
					512,						// input buffer size 
					0,							// client time-out 
					NULL );						// default security attribute 
			}
			return ( this->is_valid() );
		}
		///
		bool connect() {
			if ( this->is_valid() ) {
				return ( ConnectNamedPipe( this->p, NULL) ? true : ( GetLastError() == ERROR_PIPE_CONNECTED ) );
			}
			return false;
		}
		///
		bool open( std::string const & name ) {
			if ( !this->is_valid() ) {
				this->name = name;
				std::stringstream ss;
				ss << "\\\\.\\pipe\\" << this->name;
				this->p = CreateFile( 
					ss.str().c_str(),   // pipe name 
					GENERIC_READ |  // read and write access 
					GENERIC_WRITE, 
					0,              // no sharing 
					NULL,           // default security attributes
					OPEN_EXISTING,  // opens existing pipe 
					0,              // default attributes 
					NULL);          // no template file
				if ( this->is_valid() ) {
					DWORD dwMode = PIPE_READMODE_MESSAGE; 
					if ( SetNamedPipeHandleState( 
						this->p,    // pipe handle 
						&dwMode,  // new pipe mode 
						NULL,     // don't set maximum bytes 
						NULL) ) {    // don't set maximum time 
							return true;
					}
				}
			}
			return false;
		}
		///
		bool write( void const* d, size_t const sz ) {
			bool result = false;
			if ( this->is_valid() ) {
				DWORD wr = 0;
				result = TRUE == WriteFile( this->p, d, sz, &wr, NULL );
				result = result && ( sz == wr );
			}
			return ( result );
		}
		///
		bool read( void* b, size_t const sz ) {
			bool result = false;
			if ( this->is_valid() ) {
				DWORD rd = 0;
				result = ( TRUE == ReadFile( 
					this->p,    // pipe handle 
					b,    // buffer to receive reply 
					sz,  // size of buffer 
					&rd,  // number of bytes read 
					NULL) );    // not overlapped 
			}
			return ( result );
		}
		///
		void close() {
			if ( this->is_valid() ) {
				if ( server ) {
					DisconnectNamedPipe( this->p );
				}
				CloseHandle( this->p );
				this->p = INVALID_HANDLE_VALUE;
			}
		}
	protected:
		///
		bool is_valid() const {
			return ( this->p != INVALID_HANDLE_VALUE ); }

	private:
		///
		HANDLE
			p;
		///
		std::string
			name;
		///
		bool
			server;
	};
}

#endif
#endif //ATOM_UTIL_PIPE_HPP
