/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.04.14 - Created                                           |
|              2012.03.26 gcc(cygwin) build                                   |
|              2013.01.29 - has been moved to github/atom/util                |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|                                                                             |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_UTIL_LOG_HPP
#define ATOM_UTIL_LOG_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#pragma warning ( push )
#pragma warning ( disable : 4244 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning ( pop )

namespace atom {
	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	inline std::string date_time_to_string()
	{
		return "["+ boost::posix_time::to_simple_string( boost::posix_time::second_clock::local_time() ) + "]";
	}
	//------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------
	template < class T, class K >
	class basic_log
	{
	public:
		typedef T ostream_t;
		typedef K ofstream_t;
	private:
	protected:
		///
		typedef basic_log< T, K >
			log_t;
		///
		typedef std::vector< boost::reference_wrapper< ostream_t > >
			streams_t;
		///
		streams_t
			streams;
		///
		ofstream_t
			log_file;

	public:
		///
		basic_log() :
		  streams()
			  ,	log_file()
		  {
		  }
		  ///
		  log_t& add_stream( ostream_t& _stream )
		  { this->streams.push_back( boost::ref< ostream_t >( _stream ) ); return ( *this ); }
		  ///
		  log_t& add_std_cout()
		  {
			  this->add_stream(
#ifdef UNICODE
				  std::wcout
#else
				  std::cout
#endif
				  );
			  return ( *this );
		  }
		  ///
		  template< typename C >
		  log_t& add_file( std::basic_string< C > const& _file_name )
		  {
			  if ( !( this->log_file.is_open() ) )
			  {
				  this->log_file.open( _file_name.c_str(), std::ios::app );
				  if ( this->log_file.is_open() )
				  {
					  this->add_stream( this->log_file );
				  }
			  }
			  return ( *this );
		  }
		  ///
			template < typename T >
			void write( T const* _str, size_t const _str_len ) {
				for ( streams_t::iterator it = this->streams.begin(); it != this->streams.end(); ++it ) {
					(*it).get().write( _str, _str_len );
				}
			}
			///
			template< typename C >
			void write(std::basic_string< C > const & _str ) {
				write( _str.c_str(), _str.length() );
			}
			///
			template< typename T, typename K, typename P >
				friend basic_log<T, K>& operator<<(basic_log<T, K>& l, P const& t);
			template< typename T, typename K >
				friend basic_log<T, K>& operator<<(basic_log<T, K>& l, typename basic_log<T, K>::ostream_t &(*pfn)( typename basic_log<T, K>::ostream_t&));
			template< typename T, typename K >
				friend basic_log<T, K>& operator<<(basic_log<T, K>& l, std::ios_base&(*pfn)(std::ios_base&));
	};
	///
	template< typename T, typename K, typename P >
	inline basic_log<T, K>& operator<<(basic_log<T, K>& l, P const& t) {
		struct _{
			void __(boost::reference_wrapper< basic_log<T, K>::ostream_t >& s, P const& t) {
				s.get() << t;
			}
		} ___;
		//
		for_each(
			l.streams.begin(),
			l.streams.end(),
			boost::bind(&_::__, &___, _1, boost::cref(t)));
		return l;
	}
	///
	template< typename T, typename K >
	inline basic_log<T, K>& operator<<(basic_log<T, K>& l, typename basic_log<T, K>::ostream_t &(*pfn)( typename basic_log<T, K>::ostream_t&))
	{
		for ( log::streams_t::iterator it = l.streams.begin(); it != l.streams.end(); ++it )
		{
			pfn( (*it) );
		}
		return l; 
	}
	///
	template< typename T, typename K >
	inline basic_log<T, K>& operator<<(basic_log<T, K>& l, std::ios_base&(*pfn)(std::ios_base&))
	{ 
		for ( log::streams_t::iterator it = l.streams.begin(); it != l.streams.end(); ++it )
		{
			pfn( (*it) );
		}
		return l; 
	}

#ifdef UNICODE
	typedef basic_log< std::wostream, std::wofstream > log;
#else
	typedef basic_log< std::ostream, std::ofstream > log;
#endif

}

#endif//ATOM_UTIL_LOG_HPP
