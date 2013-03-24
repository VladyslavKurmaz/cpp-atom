/*
/-----------------------------------------------------------------------------\
| Copyright � 2008-2013 by Vladyslav Kurmaz.                                  |
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
#include <boost/foreach.hpp>

namespace atom {
	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	class po {
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
		void
			reg_name( id_t const id, string_t const& name ) {
				si_names[ name ] = id;
				is_names[ id ] = name;
		}
		///
		id_t
			get_id( string_t const& name ) {
				return ( si_names[ name ] );
		}
		///
		string_t const&
			get_name( id_t const id ) {
				return ( is_names[ id ] );
		}
		///
		void
			notify( bool const ntf ) {
				if ( ntf ) {
					boost::program_options::notify( vm );  
				}
		}

	public:
		///
		po() : 
		  descs()
			  ,	vm()
			  ,	si_names()
			  ,	is_names() {
		  }
		  ///
		  options_description_t& 
		  add_desc( id_t const id, string_t const& name ) {
			  reg_name( id, name );
			  return ( *( ( descs[ id ] = boost::shared_ptr< options_description_t >( new options_description_t( name ) ) ).get() ) );
		  }
		  ///
		  options_description_t&
		  get_desc( id_t const id ) {
			  return ( *( descs[ id ] ) ); }
		  ///
		  options_description_t&
		  get_desc( string_t const& name ) {
			  return ( get_desc( get_id( name ) ) ); }
		  ///
		  template< class T >
		  this_type_t&
		  add_option( id_t const option_id, string_t const& option_name, T option_value, string_t const& option_desc, options_description_t& desc ) {
			  reg_name( option_id, option_name );
			  desc.add_options()( option_name.c_str(), option_value, option_desc.c_str() );
			  return ( *this );
		  }
		  ///
		  this_type_t&
		  add_option( id_t const option_id, string_t const& option_name, string_t const& option_desc, options_description_t& desc ) {
			  reg_name( option_id, option_name );
			  desc.add_options()( option_name.c_str(), option_desc.c_str() );
			  return ( *this );
		  }
		  ///
		  void
		  parse_config( string_t const& file_name, options_description_t& desc, bool const ntf ) {
			  std::ifstream ifs( file_name.c_str() );
			  store( parse_config_file( ifs, desc ), vm );
			  notify( ntf );
		  }
#ifdef _WINDOWS
		  ///
		  void
		  parse_cmd_line( std::string const& cmd_line, options_description_t& desc, bool const ntf ) {
			  std::vector< std::string > args = boost::program_options::split_winmain( cmd_line );
			  boost::program_options::store( boost::program_options::command_line_parser( args ).options( desc ).run(), vm );
			  notify( ntf );
		  }
		  ///
		  void
		  parse_cmd_line( std::wstring const& cmd_line, options_description_t& desc, bool const ntf ) {
			  std::vector< std::wstring > args = boost::program_options::split_winmain( cmd_line );
			  boost::program_options::store( boost::program_options::wcommand_line_parser( args ).options( desc ).run(), vm );
			  notify( ntf );
		  }
#endif
		  ///
		  void
		  parse_arg( int argc, char const * const argv[], options_description_t& desc, bool const ntf ) {
			  boost::program_options::store( boost::program_options::parse_command_line( argc, argv, desc ), vm );
			  notify( ntf );
		  }
		  ///
		  void parse_arg( int argc , wchar_t * argv[], options_description_t& desc, bool const ntf )
		  {
			  boost::program_options::store( boost::program_options::parse_command_line( argc, argv, desc ), vm );
			  notify( ntf );
		  }
		  ///
		  template< class T >
		  T
		  as( id_t const option_id ) {
			  std::string n = get_name( option_id );
			  return ( vm[ n ].as<T>() );
		  }
		  ///
		  template< class T, class U >
		  T
		  cast( id_t const option_id ) {
			  std::string n = get_name( option_id );
			  U u = vm[ n ].as<U>();
			  return ( boost::lexical_cast<T>( u ) );
		  }
		  ///
		  size_t
		  count( id_t const option_id ) {
			  std::string n = get_name( option_id );
			  return ( vm.count( n ) );
		  }
	};
	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	template < typename C, typename V >
	struct parse_tag {
		C const*	name;
		V			value;
	};

	template < typename C, typename V >
	struct parse_result {
		size_t		total_found;
		V			result;
		std::vector< std::basic_string<C > >
			unparsed;
		parse_result() : total_found( 0 ), result(), unparsed() {}
	};

	template < typename C, typename V >
	parse_result< C, V >
	parse_tags( std::basic_string<C> const& input, parse_tag< C, V > const table[], size_t const table_size, std::vector< std::basic_string<C> > const& strs ) {
		parse_result< C, V > result;
		//
		result.total_found = strs.size(); 
		BOOST_FOREACH( std::basic_string<C> const& s, strs ) {
			bool found = false;
			for( size_t i = 0; i < table_size; ++i ) {
				if ( s == table[i].name ) {
					result.result |= table[i].value;
					found = true;
					break;
				}
			}
			if ( !found ) {
				result.unparsed.push_back( s );
			}
		}
		return result;
	}

	template < typename C, typename V >
	parse_result< C, V >
	parse_tags( std::basic_string<C> const& input, parse_tag< C, V > const table[], size_t const table_size, std::basic_string<C> const& splitters ) {
		parse_result< C, V > result;
		//
		std::vector< std::basic_string<C> > strs;
		boost::split( strs, input, boost::is_any_of( splitters ) );
		return parse_tags< C, V >( input, table, table_size, strs );
	}
	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	template < typename C >
	class attributes {
		typedef C
			char_t;
		typedef std::basic_string< C >
			string_t;
	public:
		///
		attributes( string_t const& s, string_t const& d1, string_t const& d2 ) : attribs() {
			this->parse( s, d1, d2 );
		}
		///
		template < typename T >
		T as( string_t const& id ) {
			attr_map_t::const_iterator it;
			if ( ( it = this->attribs.find( id ) ) != this->attribs.end() ) {
				return ( boost::lexical_cast< T >( (*it).second ) );
			}
			return ( T() );
		}
	private:
		///
		typedef std::map< string_t, string_t >
			attr_map_t;
		///
		attr_map_t
			attribs;
		///
		void
		parse( string_t const& s, string_t const& d1, string_t const& d2 ) {
			std::vector< std::basic_string<C> > pairs;
			std::vector< std::basic_string<C> > vals;
			boost::split( pairs, s, boost::is_any_of( d1 ) );
			BOOST_FOREACH( std::basic_string<C> const& p, pairs ) {
				boost::split( vals, p, boost::is_any_of( d2 ) );
				if ( vals.size() == 2 ) {
					this->attribs[ vals[0] ] = vals[1];
				}
			}
		}
	};
}

#endif //ATOM_UTIL_PO_HPP
