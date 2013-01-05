/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.06.09 - Created                                           |
|              2012.01.05 - has been moved to github/atom/util                |
|-----------------------------------------------------------------------------|
| TODO:		                                                                  |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_UTIL_PO_HPP
#define ATOM_UTIL_PO_HPP

#include <map>
#include <iostream>
#include <fstream>

#pragma warning( push )
#pragma warning( disable : 4512 )
#pragma warning( disable : 4503 )
#	include <boost/program_options.hpp>
#pragma warning( pop )
#include <boost/noncopyable.hpp>

namespace atom {
	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	class po
	{
	public:
		typedef std::string
			string_t;
		typedef unsigned int
			id_t;
		typedef boost::program_options::options_description
			options_description_t;
		typedef boost::program_options::variables_map
			variables_map_t;
	private:
		typedef po
			this_type_t;
		typedef std::map< string_t, id_t >
			names_string_id_map_t;
		typedef std::map< id_t, string_t >
			names_id_string_map_t;
		typedef std::map< id_t, boost::shared_ptr< options_description_t > >
			options_descriptions_map_t;
		///
		options_descriptions_map_t
			descs;
		///
		variables_map_t
			vm;
		//
		names_string_id_map_t
			si_names;
		names_id_string_map_t
			is_names;

	protected:
		///
		void reg_name( id_t const id, string_t const& name )
		{
			si_names[ name ] = id;
			is_names[ id ] = name;
		}
		///
		id_t get_id( string_t const& name )
		{
			return ( si_names[ name ] );
		}
		///
		string_t const& get_name( id_t const id )
		{
			return ( is_names[ id ] );
		}
		///
		void notify( bool const ntf )
		{
			if ( ntf )
			{
				boost::program_options::notify( vm );  
			}
		}

	public:
		///
		po() : 
				descs()
			  ,	vm()
			  ,	si_names()
			  ,	is_names()
		  {
		  }
		  ///
		  options_description_t& add_desc( id_t const id, string_t const& name )
		  {
			  reg_name( id, name );
			  return ( *( ( descs[ id ] = boost::shared_ptr< options_description_t >( new options_description_t( name ) ) ).get() ) );
		  }
		  ///
		  options_description_t& get_desc( id_t const id )
		  {
			  return ( *( descs[ id ] ) );
		  }
		  ///
		  options_description_t& get_desc( string_t const& name )
		  {
			  return ( get_desc( get_id( name ) ) );
		  }
		  ///
		  template< class T >
		  this_type_t& add_option( id_t const option_id, string_t const& option_name, T option_value, string_t const& option_desc, options_description_t& desc )
		  {
			  reg_name( option_id, option_name );
			  desc.add_options()( option_name.c_str(), option_value, option_desc.c_str() );
			  return ( *this );
		  }
		  ///
		  this_type_t& add_option( id_t const option_id, string_t const& option_name, string_t const& option_desc, options_description_t& desc )
		  {
			  reg_name( option_id, option_name );
			  desc.add_options()( option_name.c_str(), option_desc.c_str() );
			  return ( *this );
		  }
		  ///
		  void parse_config( string_t const& file_name, options_description_t& desc, bool const ntf )
		  {
			  std::ifstream ifs( file_name.c_str() );
			  store( parse_config_file( ifs, desc ), vm );
			  notify( ntf );
		  }
#ifdef _WINDOWS
		  ///
		  void parse_cmd_line( std::string const& cmd_line, options_description_t& desc, bool const ntf )
		  {
			  std::vector< std::string > args = boost::program_options::split_winmain( cmd_line );
			  boost::program_options::store( boost::program_options::command_line_parser( args ).options( desc ).run(), vm );
			  notify( ntf );
		  }
		  ///
		  void parse_cmd_line( std::wstring const& cmd_line, options_description_t& desc, bool const ntf )
		  {
			  std::vector< std::wstring > args = boost::program_options::split_winmain( cmd_line );
			  boost::program_options::store( boost::program_options::wcommand_line_parser( args ).options( desc ).run(), vm );
			  notify( ntf );
		  }
#endif
		  ///
		  void parse_arg( int argc, char const * const argv[], options_description_t& desc, bool const ntf )
		  {
			  boost::program_options::store( boost::program_options::parse_command_line( argc, argv, desc ), vm );
			  notify( ntf );
		  }
		  ///
		  void parse_arg( int argc, wchar_t * argv[], options_description_t& desc, bool const ntf )
		  {
			  boost::program_options::store( boost::program_options::parse_command_line( argc, argv, desc ), vm );
			  notify( ntf );
		  }
		  ///
		  template< class T >
		  T as( id_t const option_id )
		  {
			  std::string n = get_name( option_id );
			  return ( vm[ n ].as<T>() );
		  }
		  ///
		  template< class T, class U >
		  T cast( id_t const option_id )
		  {
			  std::string n = get_name( option_id );
			  U u = vm[ n ].as<U>();
			  return ( boost::lexical_cast<T>( u ) );
		  }
		  ///
		  size_t count( id_t const option_id )
		  {
			  std::string n = get_name( option_id );
			  return ( vm.count( n ) );
		  }
	};
}

#endif //ATOM_UTIL_PO_HPP
