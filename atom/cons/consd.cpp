#include <windows.h>
#include <iostream>
#include <string>
#include <assert.h>

#include <atom/util/dbg.hpp>
#include <atom/util/po.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

#include "./cmds.hpp"
#include <atom/util/pipe.hpp>

//SetWinEventHook.
/*
TODO:
- share keyboard state between main application andchild console
- ctrl+break, ctr+c, correct exit after pressing prev combinations
- close from all processes in chain
- languages other than english
- run param +caption, +prefix
*/

void write_vk2cons( WORD const vk ) {
	//SHORT vk1 = VkKeyScanEx( 'a', GetKeyboardLayout( GetCurrentThreadId() ) );
	//SHORT vk2 = VkKeyScanEx( 'A', GetKeyboardLayout( GetCurrentThreadId() ) );
	//
	INPUT_RECORD ir[2] = { 0 };
	//
	ir[0].EventType = KEY_EVENT;
	ir[0].Event.KeyEvent.bKeyDown			=	TRUE;
	ir[0].Event.KeyEvent.wRepeatCount		=	1;
	ir[0].Event.KeyEvent.wVirtualKeyCode	=	vk;
	ir[0].Event.KeyEvent.wVirtualScanCode	=	MapVirtualKey( vk, MAPVK_VK_TO_VSC );
	BYTE kbrd[256] = { 0 };
	WORD c = 0;
	//GetKeyboardState( kbrd );
	ToAscii(
		ir[0].Event.KeyEvent.wVirtualKeyCode,
		ir[0].Event.KeyEvent.wVirtualScanCode,
		kbrd,
		&c,
		0
		);
	ir[0].Event.KeyEvent.uChar.UnicodeChar	=	c;
	//ir[0].Event.KeyEvent.uChar.AsciiChar	=	MapVirtualKey( vk, MAPVK_VK_TO_CHAR );
	ir[0].Event.KeyEvent.dwControlKeyState	=
		( ( kbrd[ VK_CAPITAL ] & 0x01 ) ? ( CAPSLOCK_ON ) : ( 0 ) ) |
		//ENHANCED_KEY
		( ( kbrd[ VK_LMENU ] & 0x80 ) ? ( LEFT_ALT_PRESSED ) : ( 0 ) ) |
		( ( kbrd[ VK_LCONTROL ] & 0x80 ) ? ( LEFT_CTRL_PRESSED ) : ( 0 ) ) |
		( ( kbrd[ VK_NUMLOCK ] & 0x01 ) ? ( NUMLOCK_ON ) : ( 0 ) ) |
		( ( kbrd[ VK_RMENU ] & 0x80 ) ? ( RIGHT_ALT_PRESSED ) : ( 0 ) ) |
		( ( kbrd[ VK_RCONTROL ] & 0x80 ) ? ( RIGHT_CTRL_PRESSED ) : ( 0 ) ) |
		( ( kbrd[ VK_SCROLL ] & 0x01 ) ? ( SCROLLLOCK_ON ) : ( 0 ) ) |
		( ( kbrd[ VK_SHIFT ] & 0x80 ) ? ( SHIFT_PRESSED ) : ( 0 ) ) ;

	//
	ir[1] = ir[0];
	ir[1].Event.KeyEvent.bKeyDown			=	FALSE;

	//	HKL WINAPI GetKeyboardLayout(
	//  _In_  DWORD idThread
	//);

	//
	DWORD wr = 0;
	WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), ir, sizeof( ir ) / sizeof( ir[0] ), &wr );
}

void wait_please() {
	std::string s;
	std::getline( std::cin, s );
}

BOOL WINAPI HandlerRoutine ( DWORD dwCtrlType ) {
	switch ( dwCtrlType ){
	case CTRL_C_EVENT:
		{
			return TRUE;
		}
	case CTRL_BREAK_EVENT:
		{
			return TRUE;
		}
	case CTRL_CLOSE_EVENT:
		{
			return TRUE;
		}
	case CTRL_LOGOFF_EVENT:
		{
			return TRUE;
		}
	case CTRL_SHUTDOWN_EVENT:
		{
			return TRUE;
		}
	}
	return FALSE;
}

DWORD WINAPI PipeGuard( LPVOID lpParameter ) {
	atom::pipe* pipe = reinterpret_cast<atom::pipe*>( lpParameter );
	bool cont = true;
	while ( cont ) {
		command c;
		if ( pipe->read( &c, sizeof( c )  ) ) {
			switch( c.type ) {
			case command::cmdSize:
				break;
			case command::cmdKbrd:
				{
					INPUT_RECORD ir;
					ir.EventType = KEY_EVENT;
					ir.Event.KeyEvent = c.key;
					DWORD wr = 0;
					WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), &ir, sizeof( ir ), &wr );
					break;
				}
			case command::cmdCtrlBreak:
				{
					GenerateConsoleCtrlEvent( CTRL_BREAK_EVENT, 0 );
					break;
				}
			case command::cmdCtrlC:
				{
					GenerateConsoleCtrlEvent( CTRL_C_EVENT, 0 );
					break;
				}
			case command::cmdExit:
				write_vk2cons( VK_RETURN );
				write_vk2cons( 'E' );
				write_vk2cons( 'X' );
				write_vk2cons( 'I' );
				write_vk2cons( 'T' );
				write_vk2cons( VK_RETURN );
				cont = false;
				break;
			}
		} else {
			cont = false;
		}
	}
	//close all child processes
	return 0;
}

int main( int argc, char *argv[] )
{
	SetConsoleCtrlHandler( HandlerRoutine, TRUE );
	//std::cout << "consd stared" << std::endl;
	//for ( int i = 0; i < argc; ++i ) {
	//	std::cout << "[" << i << "]:" << argv[i] << std::endl;
	//}
	ATOM_DBG_MARK_BEGIN( p1, -1 ); {
		atom::po po;
		atom::po::options_description_t& desc = po.add_desc( 0, "program options" );
		std::string pipe_name;
		//
		po.add_option( 1, "pipe", boost::program_options::value<std::string>( &pipe_name ), "pipe name", desc );
		try {
			po.parse_arg( argc, argv, desc, true );
			//
			if ( !po.count( 1 ) )
				throw std::exception( "[ERROR] Pipe's name wasn't defined" );
			//
		} catch( std::exception& excpt ) {
			std::stringstream ss;
			desc.print( ss );
			std::cout << excpt.what() << std::endl;
			std::cout << ss.str() << std::endl;
			wait_please();
			return -1;
		}
		//
		// interactive loop
		static atom::po::id_t const help_desk			=	1;
		//
		static atom::po::id_t const run_desk			=	help_desk + 1;
		static atom::po::id_t const run_desk_command	=	run_desk + 1;
		//
		static atom::po::id_t const runas_desk			=	run_desk_command + 1;
		static atom::po::id_t const runas_desk_command	=	runas_desk + 1;
		//
		static atom::po::id_t const runas_desk_domain	=	runas_desk_command + 1;
		static atom::po::id_t const runas_desk_user		=	runas_desk_domain + 1;
		static atom::po::id_t const runas_desk_password	=	runas_desk_user + 1;
		//
		static atom::po::id_t const config_desk			=	runas_desk_password + 1;
		//
		static atom::po::id_t const exit_desk			=	config_desk + 1;
		//
		class tty {
			///
			std::ostream&
				os;
			///
			bool
				loop;
			//
			atom::po
				po;
			//
			typedef std::map< std::string, std::pair< atom::po::id_t, boost::function< bool( atom::po::options_description_t&, std::string& ) > > >
				descs_t;
			descs_t
				descs;

		protected:
			///
			atom::po::options_description_t&
				add_desc( atom::po::id_t const id, std::string const& name, descs_t::value_type::second_type::second_type p ) {
				this->descs[ name ] = std::make_pair( id, p );
				return ( this->po.add_desc( id, name ) );
			}
			///
			void show_banner() {
				this->os << "cons v0.1.0 [github::cpp-atom]" << std::endl;
				this->os << std::endl;
				BOOST_FOREACH( descs_t::value_type &i, this->descs ) {
					std::stringstream ss;
					atom::po::options_description_t& d = this->po.get_desc( i.second.first );
					d.print( ss );
					this->os << ss.str();
				}
				this->os << std::endl;
			}
			///
			void show_prompt() {
				this->os << "-> : ";
			}
			///
			bool on_help( atom::po::options_description_t&, std::string& ) {
				this->show_banner();
				return true;
			}
			///
			bool on_run( atom::po::options_description_t& d, std::string& m ) {
				if ( this->po.count( run_desk_command ) ) {
					PROCESS_INFORMATION pi;
					STARTUPINFO si;
					//
					ZeroMemory( &si, sizeof( si ) );
					si.cb				= sizeof(STARTUPINFO);
					//si.dwFlags			= STARTF_USESHOWWINDOW;
					//si.wShowWindow		= SW_SHOW;
					//
					TCHAR p_name[MAX_PATH] = { 0 };
					strcpy_s( p_name, this->po.as<std::string>( run_desk_command ).c_str() );
					if ( CreateProcess( NULL, p_name, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi ) ) {
						WaitForSingleObject( pi.hProcess, INFINITE );
						os << std::endl;
					} else {
					}
				} else {
					m = "Invalid arguments";
				}
				return true;
			}
			///
			bool on_runas( atom::po::options_description_t&, std::string& ) {
				return true;
			}
			///
			bool on_config( atom::po::options_description_t&, std::string& ) {
				return true;
			}
			bool on_exit( atom::po::options_description_t&, std::string& ) {
				return false;
			}

		public:
			///
			tty( std::ostream& o ) :
					os( o )
				,	loop( true )
				,	po()
				,	descs() {
					//
					atom::po::options_description_t& hd = this->add_desc( help_desk, "help", boost::bind( &tty::on_help, this, _1, _2 ) );
					//
					atom::po::options_description_t& rd = this->add_desc( run_desk, "run", boost::bind( &tty::on_run, this, _1, _2 )  );
					po.
						add_option( run_desk_command, "c", boost::program_options::value<std::string>(), "command to run", rd );
					//
					atom::po::options_description_t& rad = this->add_desc( runas_desk, "runas", boost::bind( &tty::on_runas, this, _1, _2 )  );
					po.
						add_option( runas_desk_command, "c", boost::program_options::value<std::string>(), "command run", rad ).
						add_option( runas_desk_domain, "d", boost::program_options::value<std::string>(), "domain", rad ).
						add_option( runas_desk_user, "u", boost::program_options::value<std::string>(), "user", rad ).
						add_option( runas_desk_password, "p", boost::program_options::value<std::string>(), "pasword", rad );
					//
					//
					atom::po::options_description_t& cd = this->add_desc( config_desk, "config", boost::bind( &tty::on_config, this, _1, _2 )  );
					//
					atom::po::options_description_t& ed = this->add_desc( exit_desk, "exit", boost::bind( &tty::on_exit, this, _1, _2 )  );
			  }
			  ///
			  void run() {
				  this->show_banner();
				  while( this->loop ) {
					  this->show_prompt();
					  std::string s;
					  std::getline( std::cin, s );
					  //
					  std::stringstream ss;
					  ss << s;
					  std::string cmd;
					  ss >> cmd;
					  //
					  descs_t::iterator it = this->descs.find( cmd );
					  if ( it != this->descs.end() ) {
						  std::string param = &( s.c_str()[ cmd.length() ] );
						  atom::po::options_description_t d = this->po.get_desc( (*it).second.first );
						  try {
							  std::string msg;
							  this->po.parse_cmd_line( param, d, true );
							  this->loop = (*it).second.second( d, msg );
							  if ( msg.length() ) {
								  this->os << msg << std::endl;
							  }
						  } catch( std::exception& excpt ) {
							  this->os << excpt.what() << std::endl;
						  }
					  } else {
						  this->os << "invalid command: " << s << std::endl;
					  }
					  this->os << std::endl;
				  }
			  }
		} tty1(  std::cout  );
		atom::pipe pipe;
		if ( pipe.open( pipe_name ) ) {
			//
			HANDLE ht = CreateThread( NULL, 0, PipeGuard, reinterpret_cast<LPVOID>( &pipe ), 0, NULL );
			tty1.run();
			WaitForSingleObject( ht, INFINITE );
		} else {
			std::cout << "[ERROR] Pipe creation error" << std::endl;
		}
	}
	ATOM_DBG_MARK_END( p1, p2, p1p2diff, true );
	return 0;

}


		//CONSOLE_SCREEN_BUFFER_INFOEX csbiex = { 0 };
		//csbiex.cbSize = sizeof( csbiex );

		//GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex );
		//csbiex.dwSize.X = 100;
		//csbiex.dwSize.Y = 40;
		//csbiex.dwMaximumWindowSize.X = 100;
		//csbiex.dwMaximumWindowSize.Y = 40;
		//csbiex.srWindow.Right = 97;
		//csbiex.srWindow.Bottom = 33;


		//if ( SetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex ) ) {
		//	GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex );
		//	SMALL_RECT crt;
		//	crt.Left = 0;
		//	crt.Top = 0;
		//	crt.Right	= 99;
		//	crt.Bottom	= 39;
		//	SetConsoleWindowInfo( GetStdHandle( STD_OUTPUT_HANDLE ), FALSE, &crt );
		//	GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex );
		//}

		//CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };
		//GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );
		//csbi.dwSize.X = 100;
		//csbi.dwSize.Y = 40;
		//SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), csbi.dwSize );
		//GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );

		//SMALL_RECT crt;
		//crt.Left	= csbi.srWindow.Left;
		//crt.Top		= csbi.srWindow.Top;
		//crt.Right	= csbi.dwSize.X - 1;
		//crt.Bottom	= csbi.dwSize.Y - 1;
		//SetConsoleWindowInfo( GetStdHandle( STD_OUTPUT_HANDLE ), FALSE, &crt );
		//
		//std::string s;
		//std::getline( std::cin, s);
		//
		//PROCESS_INFORMATION pi;
		//STARTUPINFO si;
		////
		//ZeroMemory( &si, sizeof( si ) );
		//si.cb				= sizeof(STARTUPINFO);
		//si.dwFlags			= STARTF_USESHOWWINDOW;
		//si.wShowWindow		= SW_SHOW;
		////
		//TCHAR command[MAX_PATH] = 
		//	//"C:\\Program Files\\Far2\\Far.exe"
		//	//"c:\\work\\env\\cygwin\\cygwin.bat"
		//	//"powershell.exe"
		//	"cmd.exe"
		//	;
		//if ( CreateProcess( NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi ) ) {
		//	//write2cons( "apa" );
		//	//write2cons( "ping 127.0.0.1" );
		//	//write2cons( "\n\r" );
		//	//write_vk2cons( 'D' );
		//	//write_vk2cons( 'I' );
		//	//write_vk2cons( 'R' );
		//	//write_vk2cons( VK_RETURN );
		//	WaitForSingleObject( pi.hProcess, INFINITE );
		//}


void write2cons( std::string const& s ) {
	size_t const bsz = sizeof( INPUT_RECORD ) * ( 2 * s.length() );
	INPUT_RECORD* ir = NULL;
	INPUT_RECORD* iri = ir = reinterpret_cast<INPUT_RECORD*>( _alloca( bsz ) );
	memset( ir, 0, bsz );
	BOOST_FOREACH( char const c, s ) {
		iri->EventType = KEY_EVENT;
		iri->Event.KeyEvent.bKeyDown			=	TRUE;
		iri->Event.KeyEvent.wRepeatCount		=	1;
		iri->Event.KeyEvent.wVirtualKeyCode		=	c;
		iri->Event.KeyEvent.wVirtualScanCode	=	c;
		//iri->Event.KeyEvent.uChar.UnicodeChar	=	L'a';
		iri->Event.KeyEvent.uChar.AsciiChar		=	c;
		iri->Event.KeyEvent.dwControlKeyState	=	0;
		iri++;
		iri->EventType = KEY_EVENT;
		iri->Event.KeyEvent.bKeyDown			=	FALSE;
		iri->Event.KeyEvent.wRepeatCount		=	1;
		iri->Event.KeyEvent.wVirtualKeyCode		=	c;
		iri->Event.KeyEvent.wVirtualScanCode	=	c;
		//iri->Event.KeyEvent.uChar.UnicodeChar	=	L'a';
		iri->Event.KeyEvent.uChar.AsciiChar		=	c;
		iri->Event.KeyEvent.dwControlKeyState	=	0;
		iri++;
	}
	//
	DWORD wr = 0;
	WriteConsoleInput( GetStdHandle( STD_INPUT_HANDLE ), ir, 2 * s.length(), &wr );
}


//CONSOLE_SCREEN_BUFFER_INFOEX csbiex = { 0 };
//csbiex.cbSize = sizeof( csbiex );
//GetConsoleScreenBufferInfoEx( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiex );
//CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };
//GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );
//csbi.dwSize.X = 100;
//csbi.dwSize.Y = 1000;
//SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), csbi.dwSize );
////
//UINT in_cp = GetConsoleCP();
//UINT out_cp = GetConsoleOutputCP();
//std::string s;
//do {
//	std::cout << "c:\\work : git ";
//	std::getline( std::cin, s);
//	std::cout << s << std::endl;
//} while( s.size() );
//

