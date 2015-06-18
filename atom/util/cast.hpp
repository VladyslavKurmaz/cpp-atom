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

#endif //ATOM_UTIL_CAST_HPP
