/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2013.07.27 - Created                                           |
|-----------------------------------------------------------------------------|
| TODO:		                                                                    |
\-----------------------------------------------------------------------------/
*/
#pragma once

#ifndef ATOM_UTIL_TTY_HPP
#define ATOM_UTIL_TTY_HPP

namespace atom {

	template < typename T >
	class tty {
		typedef tty< T >
			this_t;
	public:
		typedef boost::function< bool( this_t&, std::string const& ) >
			pred_t;
		typedef std::basic_istream< T, std::char_traits< T > >
			istream_t;
		typedef std::basic_ostream< T, std::char_traits< T > >
			ostream_t;
		typedef std::basic_string< T >
			string_t;
		typedef std::basic_stringstream< T >
			stringstream_t;

	private:
		///
		ostream_t&
			os;
		istream_t&
			is;
		///
		bool
			loop;
		///
		string_t
			banner;
		string_t
			prompt;
		///
		typedef std::map< std::string, pred_t >
			descs_t;
		descs_t
			descs;

	protected:
		///
		void show_banner() {
			this->os<< std::endl << this->banner << std::endl;
			this->os << std::endl;
		}
		///
		void show_prompt() {
			this->os << this->prompt;
		}

	public:
		///
		tty( istream_t& i, ostream_t& o ) :
				is( i )
			,	os( o )
			,	loop( true )
			,	banner()
			,	prompt()
			,	descs() {
		  }
		  ///
		  bool process( string_t const& cmd, string_t const& param ) {
			  try {
				  descs_t::const_iterator it = this->descs.find( cmd );
				  if ( it != this->descs.end() ) {
					  return ( (*it ).second( *this, param ) );
				  } else {
					  throw std::exception( "Invalid command" );
				  }
			  }
			  catch ( std::exception& s ) {
				  this->os << s.what() << std::endl << std::endl;
			  }
			  return false;
		  }
		  ///
		  void exit() {
			  this->loop = false;
		  }
		  ///
		  void run() {
			  this->show_banner();
			  while( this->loop ) {
				  this->show_prompt();
				  string_t s;
				  std::getline( this->is, s );
				  //
				  stringstream_t ss;
				  ss << s;
				  string_t cmd;
				  ss >> cmd;
				  //
				  string_t const param = &( s.c_str()[ cmd.length() ] );
				  this->process( cmd, param );
				  this->os << std::endl;
			  }
		  }
	};


#if 0

//
// interactive loop
static atom::po::id_t const help_desk			=	1;
//
static atom::po::id_t const run_desk			=	help_desk + 1;
static atom::po::id_t const run_desk_command	=	run_desk + 1;
static atom::po::id_t const run_desk_domain		=	run_desk_command + 1;
static atom::po::id_t const run_desk_user		=	run_desk_domain + 1;
static atom::po::id_t const run_desk_password	=	run_desk_user + 1;
//
static atom::po::id_t const config_desk			=	run_desk_password + 1;
//
static atom::po::id_t const exit_desk			=	config_desk + 1;
//
class tty {
	///
	std::ostream&
		os;
	///
	std::istream&
		is;
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
		this->os<< std::endl << "cons v0.1.0 [/github/cpp-atom/cons]" << std::endl;
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
	bool on_config( atom::po::options_description_t&, std::string& ) {
		return true;
	}
	bool on_exit( atom::po::options_description_t&, std::string& ) {
		return false;
	}

public:
	///
	tty( std::ostream& o, std::istream& i ) :
			os( o )
		,	is( i )
		,	loop( true )
		,	po()
		,	descs() {
			  //
			  atom::po::options_description_t& hd = this->add_desc( help_desk, "help", boost::bind( &tty::on_help, this, _1, _2 ) );
			  //
			  atom::po::options_description_t& rd = this->add_desc( run_desk, "run", boost::bind( &tty::on_run, this, _1, _2 )  );
			  po.
				  add_option( run_desk_command,		"command,c",	boost::program_options::value<std::string>(), "command to run", rd ).
				  add_option( run_desk_domain,		"domain,d",		boost::program_options::value<std::string>(), "domain", rd ).
				  add_option( run_desk_user,		"user,u",		boost::program_options::value<std::string>(), "user", rd ).
				  add_option( run_desk_password,	"password,p",	boost::program_options::value<std::string>(), "password", rd );
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
			  std::getline( this->is, s );
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
};

#endif

}

#endif //ATOM_UTIL_TTY_HPP
