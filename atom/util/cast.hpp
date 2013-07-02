/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2011.03.08 - Created                                           |
|              2013.02.19 - has been moved to github/atom/util                |
|-----------------------------------------------------------------------------|
| TODO:        - make GUID2string cast code more proff                        |
|                                                                             |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_UTIL_CAST_HPP
#define ATOM_UTIL_CAST_HPP

#include <iostream>
#include <iomanip>
#include <string> 
#include <locale> 
#include <boost/lexical_cast.hpp> 

namespace boost {
	//----------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------
	template<>
	inline std::wstring lexical_cast<std::wstring, std::string>(const std::string& arg)
	{
		std::wstring result;
		std::locale loc( std::locale::classic() );
		const std::ctype< char > &ct = std::use_facet<std::ctype< char > >(loc);
		for(unsigned int i= 0; i < arg.size(); ++i)
		{
			result += ct.narrow( arg[i],'?' );
		}
		return result;
	}

	template<>
	inline std::string lexical_cast<std::string, std::wstring>(const std::wstring& arg)
	{
		std::string result;
		std::locale loc( std::locale::classic() );
		const std::ctype< wchar_t > &ct = std::use_facet<std::ctype< wchar_t > >(loc);
		for(unsigned int i= 0; i < arg.size(); ++i)
		{
			result += ct.narrow( arg[i],'?' );
		}
		return result;
	}



	//----------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------
#if 0
	template<>
	inline std::wstring lexical_cast<std::wstring, z3d::platform::traits::guid_t>( z3d::platform::traits::guid_t const& /*arg*/ )
	{
		std::wstringstream result;
		return ( result.str() );
	}

	template<>
	inline std::string lexical_cast<std::string, z3d::platform::traits::guid_t>( z3d::platform::traits::guid_t const& arg )
	{
		std::stringstream result;
		result << std::hex << std::setfill( '0' ) << std::uppercase 
			<< '{'
			<< std::setw( 8 ) << arg.Data1 << '-'
			<< std::setw( 4 ) << arg.Data2 << '-'
			<< std::setw( 4 ) << arg.Data3 << '-';
		//
		for( unsigned int i = 0; i < sizeof( arg.Data4 ) / sizeof( arg.Data4[0] ); ++i )
		{
			result << std::setw( 2 );
			result << (unsigned int)arg.Data4[i];
			if ( i == 1 )
				result << '-';
		}
		result << '}';
		return ( result.str() );
	}
#endif
}

namespace atom {
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
}

#if defined(_WIN32)
namespace std {
	inline std::ostream& operator << ( std::ostream& os, GUID const& guid ) {
		OLECHAR* bstr;
		::StringFromCLSID( guid, &bstr );
		os << atom::s2s< std::string >( std::wstring( bstr ) );
		::CoTaskMemFree( bstr );
		return os;
	}
}

   //GUID guid;

   // unsigned long p0;
   // int p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;

   // int err = sscanf_s(s.c_str(), "%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
   //     &p0, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10);

   // guid.Data1 = p0;
   // guid.Data2 = p1;
   // guid.Data3 = p2;
   // guid.Data4[0] = p3;
   // guid.Data4[1] = p4;
   // guid.Data4[2] = p5;
   // guid.Data4[3] = p6;
   // guid.Data4[4] = p7;
   // guid.Data4[5] = p8;
   // guid.Data4[6] = p9;
   // guid.Data4[7] = p10;

#endif

#endif //ATOM_UTIL_CAST_HPP
