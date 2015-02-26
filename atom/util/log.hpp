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
	class log
	{
	private:
	protected:
		///
		typedef log
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
		log():
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
		  log_t& add_file( string_t const& _file_name )
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
		  void write( T const* _str, size_t const _str_len )
		  {
#if 0
			  struct s
			  { 
				  void f( boost::reference_wrapper<ostream_t>& sref, char const* _str, size_t const _str_len )
				  { sref.get().write( _str, _str_len ); }
			  } v;
			  //
			  std::for_each(
				  this->streams.begin(),
				  this->streams.end(),
				  boost::bind( &s::f, &v, _1, _str, _str_len ) );
#else
			  for ( streams_t::iterator it = this->streams.begin(); it != this->streams.end(); ++it )
			  {
				  (*it).get().write( _str, _str_len );
			  }
#endif
		  }
		  ///
		  void write( string_t const & _str )
		  {
			  write( _str.c_str(), _str.length() );
		  }
		  ///
		  template< typename T > friend log& operator<<( log& l, T const& t );
		  friend log& operator<<( log& l, ostream_t&(*pfn)(ostream_t&) );
		  friend log& operator<<( log& l, std::ios_base&(*pfn)(std::ios_base&) );
	};
	///

	template< typename T >
	inline log& operator<<( log& l, T const& t )
	{
		struct _
		{
			void __( boost::reference_wrapper< ostream_t >& s, T const& t )
			{ s.get() << t; }
		} ___;
		//
		for_each(
			l.streams.begin(),
			l.streams.end(),
			boost::bind( &_::__, &___, _1, boost::cref( t ) ) );
		return l;
	}
	///
	template<>
	inline log& operator<< <std::string>( log& l, std::string const& t )
	{
		l << s2s<string_t>( t );
		return l;
	}
	///
	inline log& operator<<( log& l, ostream_t&(*pfn)(ostream_t&) )
	{
#if 0
		struct _
		{
			void __( boost::reference_wrapper< std::ostream >& s, std::ostream&(*pfn)( std::ostream& ) )
			{ pfn( s ); }
		} ___;
		//
		for_each(
			l.streams.begin(),
			l.streams.end(),
			boost::bind( &_::__, &___, _1, pfn ) );
#else
		for ( log::streams_t::iterator it = l.streams.begin(); it != l.streams.end(); ++it )
		{
			pfn( (*it) );
		}
#endif
		return l; 
	}
	///
	inline log& operator<<( log& l, std::ios_base&(*pfn)(std::ios_base&) )
	{ 
#if 0
		struct _
		{
			void __( boost::reference_wrapper< std::ostream >& s, std::ios_base&(*pfn)( std::ios_base& ) )
			{ pfn( s ); }
		} ___;
		//
		for_each(
			l.streams.begin(),
			l.streams.end(),
			boost::bind( &_::__, &___, _1, pfn ) );
#else
		for ( log::streams_t::iterator it = l.streams.begin(); it != l.streams.end(); ++it )
		{
			pfn( (*it) );
		}
#endif
		return l; 
	}
}

#endif//ATOM_UTIL_LOG_HPP
