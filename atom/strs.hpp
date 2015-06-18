/*
/-----------------------------------------------------------------------------\
| Copyright © 2015 by Vladyslav Kurmaz.                                       |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2013.05.10                                                     |
\-----------------------------------------------------------------------------/
*/
#pragma once

#ifndef ATOM_STRS_HPP
#define ATOM_STRS_HPP

namespace atom {

#ifdef UNICODE
	typedef wchar_t char_t;
	//#define _T( x )  x
#else
	typedef char char_t;
	//#define _T( x )  L ## x
#endif



	typedef std::basic_string< char_t > string_t;
	typedef std::basic_stringstream< char_t > stringstream_t;
	typedef std::basic_ostream< char_t > ostream_t;
	typedef std::basic_ofstream< char_t > ofstream_t;


	//----------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------
	template< typename R, typename T > 
	inline R s2s( T const& );

	template<> 
	inline std::wstring s2s( std::string const& s )
	{
		return boost::lexical_cast< std::wstring >( s );
	}

	template<> 
	inline std::string s2s( std::wstring const& s )
	{
		return boost::lexical_cast< std::string >( s );
	}


#if defined(WIN32)

	inline ostream_t& operator << ( ostream_t& os, GUID const& guid ) {
		OLECHAR* bstr;
		::StringFromCLSID( guid, &bstr );
#ifdef UNICODE
		os << std::wstring( bstr );
#else
		os << atom::s2s< std::string >( std::wstring( bstr ) );
#endif
		::CoTaskMemFree( bstr );
		return os;
	}

#endif

}

#endif//ATOM_STRS_HPP

