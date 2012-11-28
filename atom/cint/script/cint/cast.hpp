/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        z3d/script/cint/cast.hpp                                       |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.08.24 - Created                                           |
|              2012.03.23 - gcc fix for nested class partial specialisaton    |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/

#ifndef Z3D_SCRIPT_CINT_RESULT_HPP
#define Z3D_SCRIPT_CINT_RESULT_HPP

namespace z3d { namespace script { namespace cint {
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	template < typename T, bool >
	class result
	{
		T cast( G__value const& v )
		{
			return ( reinterpret_cast< T >( G__int( v ) ) );
		}
	};

	template < typename T >
	class result< T, true >
	{
		T cint_cast( G__value const& v )
		{
			return ( reinterpret_cast< T >( G__double( v ) ) );
		}
	};

#if 0
	template < bool >
	struct result
	{
		template < typename T >
		static T convert( G__value const& v )
			{ return ( ( T )( G__int( v ) ) ); }
		template <>
		static void convert< void >( G__value const& )
			{ return; }
	};
	template <>
	struct result< true >
	{
		template < typename T >
		static T convert( G__value const& v )
			{ return ( ( T )( G__double( v ) ) ); }
	};
#endif
} } }
#endif //Z3D_SCRIPT_CINT_RESULT_HPP
