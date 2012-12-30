/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        z3d/type_name.hpp                                              |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.08.24 - Created                                           |
|              2010.09.20 - Change namespace, base specs                      |
|              2011.08.12 - Added templates recognition (Boost.Preprocessor)  |
|              2011.08.17 - build under cygwin (gcc-4.3.4)                    |
|              2012.06.10 - add more boost pp                                 |
|-----------------------------------------------------------------------------|
| TODO:      + remove strncpy_s                                               |
|            + use std::copy instead custom copy for                          |
|            - make term_::name static function                               |
|            - add const modifier for functions                               |
|            - add PP for templates/std conainers                             |
|            - use stringbuf instead stringstream                             |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/

#ifndef Z3D_TYPE_NAME_HPP
#define Z3D_TYPE_NAME_HPP

#include <string>
#include <functional>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <sstream>
//
#include <boost/lexical_cast.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/preprocessor/repeat_from_to.hpp>
#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/facilities/identity.hpp>
//
#include <loki/Typelist.h>
//
#include <atom/node/tldefs.hpp>

namespace atom { namespace cint {

	#ifndef Z3D_TYPE_NAME_PARAMS
	#	define Z3D_TYPE_NAME_PARAMS 15
	#endif

	template < typename > struct term_;

	//-----------------------------------------------------------------------------
	// base terms
	//-----------------------------------------------------------------------------
	#define Z3D_TYPE_NAME_TYPE(i) Z3D_TYPE_NAME_TYPE##i

	#define Z3D_TYPE_NAME_TYPE0 void
	#define Z3D_TYPE_NAME_TYPE1 bool
	#define Z3D_TYPE_NAME_TYPE2 char
	#define Z3D_TYPE_NAME_TYPE3 signed char
	#define Z3D_TYPE_NAME_TYPE4 unsigned char
	#define Z3D_TYPE_NAME_TYPE5 wchar_t
	#define Z3D_TYPE_NAME_TYPE6 short int
	//signed short int
	#define Z3D_TYPE_NAME_TYPE7 unsigned short int
	#define Z3D_TYPE_NAME_TYPE8 int
	//signed int
	#define Z3D_TYPE_NAME_TYPE9 unsigned int
	#define Z3D_TYPE_NAME_TYPE10 long int
	//signed long int
	#define Z3D_TYPE_NAME_TYPE11 unsigned long int
	#define Z3D_TYPE_NAME_TYPE12 long long int
	//signed long long int
	#define Z3D_TYPE_NAME_TYPE13 unsigned long long int
	#define Z3D_TYPE_NAME_TYPE14 float
	#define Z3D_TYPE_NAME_TYPE15 double
	#define Z3D_TYPE_NAME_TYPE16 long double

	#define Z3D_TYPE_NAME_TYPE_CNT 17

	#define Z3D_TYPE_NAME_DEF_BASE_TYPES( z, i, _ )                                                                 \
		template <>                                                                                                   \
		struct term_< Z3D_TYPE_NAME_TYPE(i) >                                                                         \
		{                                                                                                             \
			enum { length = sizeof( BOOST_PP_STRINGIZE(Z3D_TYPE_NAME_TYPE(i)) ) - 1 };                                  \
			std::ostream& name( std::ostream& os ) const { return( os << BOOST_PP_STRINGIZE(Z3D_TYPE_NAME_TYPE(i)) ); } \
		};
	BOOST_PP_REPEAT( Z3D_TYPE_NAME_TYPE_CNT, Z3D_TYPE_NAME_DEF_BASE_TYPES, _)

	#define Z3D_TYPE_NAME_DEF_TERM( t ) \
		template <>\
		struct term_< t >\
		{\
			enum { length = sizeof( #t ) - 1 };\
			std::ostream& name( std::ostream& os ) const { return( os << #t ); }\
		};



	//-----------------------------------------------------------------------------
	// mods
	//-----------------------------------------------------------------------------
#define Z3D_TYPE_NAME_DEF_MOD( m ) \
	template < typename T >\
	struct term_< T m >\
	{\
		typedef term_< T > type_t;\
		type_t type;\
		enum { length = type_t::length + sizeof( #m ) - 1 + 1 };\
		std::ostream& name( std::ostream& os ) const { return ( type.name( os ) << " "#m ); }\
	};

	Z3D_TYPE_NAME_DEF_MOD( const );
	Z3D_TYPE_NAME_DEF_MOD( volatile );
	Z3D_TYPE_NAME_DEF_MOD( const volatile );
	Z3D_TYPE_NAME_DEF_MOD( & );
	Z3D_TYPE_NAME_DEF_MOD( * );
	Z3D_TYPE_NAME_DEF_MOD( [] );


	template < size_t A >
	struct arr_size
	{
		enum { size = arr_size< A / 10 >::size + 1 };
	};

	template <>
	struct arr_size< 0 >
	{
		enum { size = 0 };
	};

	template < typename T, size_t S >
	struct term_< T[S] >
	{
		typedef term_< T > type_t;
		type_t type;
		enum { length = type_t::length + arr_size< S >::size + 2 };
		std::ostream& name( std::ostream& os ) const { return( type.name( os ) << "[" << S << "]" ); }\
	};

	//-----------------------------------------------------------------------------
	// func param
	//-----------------------------------------------------------------------------
	//
	template < typename T, size_t L >
	struct func_param_
	{	enum { length = L };
		std::ostream& name( std::ostream& os ) const
			{ return ( os ); } };
	//
	template < typename T, size_t L >
	struct func_param_< LOKI_TYPELIST_1( T ), L >
	{	enum { length = term_< T >::length + L };
		std::ostream& name( std::ostream& os ) const
			{ return ( term_< T >().name( os ) ); } };
	//
	template < typename T, typename U, size_t L >
	struct func_param_< Loki::Typelist< T, U >, L > : public func_param_< U, L + term_< T >::length + 1 >
	{	std::ostream& name( std::ostream& os ) const
			{ return ( func_param_< U, L + term_< T >::length + 1 >::name( term_< T >().name( os ) << "," ) ); } };
	//-----------------------------------------------------------------------------
	// free funcs
	//-----------------------------------------------------------------------------
	#ifndef Z3D_TYPE_NAME_FREE_FUNC_PARAMS
	#	define Z3D_TYPE_NAME_FREE_FUNC_PARAMS Z3D_TYPE_NAME_PARAMS
	#endif

	#define Z3D_TYPE_NAME_FREE_FUNC_ARGS(n) ( BOOST_PP_ENUM_PARAMS(n, T) )

	#define Z3D_TYPE_NAME_DEF_FREE_FUNC(z, n, text) \
		template < typename R BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM_PARAMS( n, typename T ) >\
		struct term_< R(*)( BOOST_PP_ENUM_PARAMS( n, T ) ) >\
		{\
			typedef term_< R > result_t;\
			typedef func_param_< BOOST_PP_IDENTITY( LOKI_TYPELIST_##n Z3D_TYPE_NAME_FREE_FUNC_ARGS(n))(), 0 > params_t;\
			result_t result;\
			params_t params;\
			enum { length = result_t::length + params_t::length + 5 };\
			std::ostream& name( std::ostream& os ) const { return ( params.name( result.name( os ) << "(*)(" ) << ")" ); }\
		};

	BOOST_PP_REPEAT( BOOST_PP_INC( Z3D_TYPE_NAME_FREE_FUNC_PARAMS ), Z3D_TYPE_NAME_DEF_FREE_FUNC, _)

	#undef Z3D_TYPE_NAME_DEF_FREE_FUNC
	//-----------------------------------------------------------------------------
	// pointer to member
	//-----------------------------------------------------------------------------
	template < typename T, class C >
	struct term_< T C::* >
	{
		typedef term_< T > type_t;
		typedef term_< C > cls_t;
		type_t type;
		cls_t cls;
		enum { length = type_t::length + cls_t::length + 4 };
		std::ostream& name( std::ostream& os ) const
			{ return ( cls.name( type.name( os ) << " " ) << "::*" ); }
	};

	//-----------------------------------------------------------------------------
	// pointer to member function
	//-----------------------------------------------------------------------------
	#ifndef Z3D_TYPE_NAME_MEMBER_FUNC_PARAMS
	#	define Z3D_TYPE_NAME_MEMBER_FUNC_PARAMS Z3D_TYPE_NAME_PARAMS
	#endif

	#define Z3D_TYPE_NAME_MEMBER_FUNC_ARGS(n) ( BOOST_PP_ENUM_PARAMS(n, T) )

	#define Z3D_TYPE_NAME_DEF_MEMBER_FUNC(z, n, text) \
		template < typename R, typename C BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM_PARAMS( n, typename T ) > \
		struct term_< R(C::*)( BOOST_PP_ENUM_PARAMS( n, T ) ) > \
		{ \
			typedef term_< R > result_t;\
			typedef term_< C > cls_t;\
			typedef func_param_< BOOST_PP_IDENTITY( LOKI_TYPELIST_##n Z3D_TYPE_NAME_MEMBER_FUNC_ARGS(n))(), 0 > params_t; \
			result_t result;\
			cls_t cls;\
			params_t params;\
			enum { length = result_t::length + cls_t::length + params_t::length + 7 };\
			std::ostream& name( std::ostream& os ) const\
				{ return ( params.name( cls.name( result.name( os ) << "(" ) << "::*)(" ) << ")" ); }\
		};

	BOOST_PP_REPEAT( BOOST_PP_INC( Z3D_TYPE_NAME_MEMBER_FUNC_PARAMS ), Z3D_TYPE_NAME_DEF_MEMBER_FUNC, _)
	
	#undef Z3D_TYPE_NAME_DEF_MEMBER_FUNC
	//-----------------------------------------------------------------------------
	// templates terms
	//-----------------------------------------------------------------------------
	#define Z3D_TYPE_NAME_DEF_TEMPLATE_TERM( z, n, t ) \
		template < BOOST_PP_ENUM_PARAMS(n, typename P) >\
		struct term_< boost::mpl::identity< t< BOOST_PP_ENUM_PARAMS(n, P) > > >\
		{\
			enum { length = sizeof( #t ) - 1 };\
			std::ostream& name( std::ostream& os ) const { return( os << #t ); }\
		};

	//-----------------------------------------------------------------------------
	// templates
	//-----------------------------------------------------------------------------
	#ifndef Z3D_TYPE_NAME_TEMPLATE_PARAMS
	#	define Z3D_TYPE_NAME_TEMPLATE_PARAMS Z3D_TYPE_NAME_PARAMS
	#endif

	#define Z3D_TYPE_NAME_DEF_TEMPLATE_ARGS(n) ( BOOST_PP_ENUM_PARAMS(n, P) )

	#define Z3D_TYPE_NAME_DEF_TEMPLATE(z, n, text)                                                 \
		template < BOOST_PP_ENUM_PARAMS(n, typename P)                                               \
  		       , template < BOOST_PP_ENUM_PARAMS(n,typename BOOST_PP_INTERCEPT ) > class C >       \
		struct term_< C< BOOST_PP_ENUM_PARAMS(n, P) > >                                              \
		{                                                                                            \
			typedef term_< boost::mpl::identity< C< BOOST_PP_ENUM_PARAMS(n,P) > > > cls_t;             \
			typedef func_param_< BOOST_PP_IDENTITY( LOKI_TYPELIST_##n Z3D_TYPE_NAME_DEF_TEMPLATE_ARGS(n))(), 0 > params_t;   \
			cls_t cls;                                                                                 \
			params_t params;                                                                           \
			enum { length = cls_t::length + params_t::length + 2 };                                    \
			std::ostream& name( std::ostream& os ) const                                               \
				{ return( params.name( cls.name( os ) << "<" ) << ">" ); }                               \
		};

	BOOST_PP_REPEAT_FROM_TO( 1, Z3D_TYPE_NAME_TEMPLATE_PARAMS, Z3D_TYPE_NAME_DEF_TEMPLATE, _ )

	//-----------------------------------------------------------------------------
	// get type string
	//-----------------------------------------------------------------------------
	template < typename T >
	struct type_
	{
		enum { length = term_<T>::length + 1 };
		static char const* name()
		{
			if ( !tn[0])
			{
				//std::stringbuf sb;
				//sb.pubsetbuf( tn, length );
				//term_<T>().name( sb );
			
				std::stringstream ss;
				term_<T>().name( ss );
				std::string s(ss.str());
				std::copy( s.begin(), s.end(), tn );
			}
			return ( tn );
		}
	private:
		static char tn[ length ];
	};
	template< typename T > char type_< T >::tn[ type_< T >::length ] = { 0 };

	//-----------------------------------------------------------------------------
	// get formated pair type_and_param "(int)1", "(char*)0x00A087FC", etc.
	//-----------------------------------------------------------------------------
	template < typename P > struct format_
		{ template < class OS > static OS& param( OS& os, P p ) { os << p; return os; } };

	template < > struct format_< char >
		{ template < class OS > static OS& param( OS& os, char p ) { os << "'" << p << "'"; return os; } };

	template < typename P > struct format_< P* >
	{ template < class OS > static OS& param( OS& os, P* p ) { os << "0x" << reinterpret_cast< void const * >( p ); return os; } };

	template < typename P >
	std::string param_( P p )
	{
		std::stringstream ss;
		format_<P>::param( ss << "(" << type_< P >::name() << ")", p );	
		return ( ss.str() );
	};

} }

#define Z3D_TYPE_NAME_DEF_TERM_USER( t )	namespace atom { namespace cint { Z3D_TYPE_NAME_DEF_TERM( t ) } }

// add auto generation
#define Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER1( t )	namespace atom { namespace cint { Z3D_TYPE_NAME_DEF_TEMPLATE_TERM( 1, 1, t ) } }
#define Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER2( t )	namespace atom { namespace cint { Z3D_TYPE_NAME_DEF_TEMPLATE_TERM( 1, 2, t ) } }
#define Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER3( t )	namespace atom { namespace cint { Z3D_TYPE_NAME_DEF_TEMPLATE_TERM( 1, 3, t ) } }
#define Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER4( t )	namespace atom { namespace cint { Z3D_TYPE_NAME_DEF_TEMPLATE_TERM( 1, 4, t ) } }
#define Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER5( t )	namespace atom { namespace cint { Z3D_TYPE_NAME_DEF_TEMPLATE_TERM( 1, 5, t ) } }
#define Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER6( t )	namespace atom { namespace cint { Z3D_TYPE_NAME_DEF_TEMPLATE_TERM( 1, 6, t ) } }
#define Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER7( t )	namespace atom { namespace cint { Z3D_TYPE_NAME_DEF_TEMPLATE_TERM( 1, 7, t ) } }
#define Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER8( t )	namespace atom { namespace cint { Z3D_TYPE_NAME_DEF_TEMPLATE_TERM( 1, 8, t ) } }
#define Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER9( t )	namespace atom { namespace cint { Z3D_TYPE_NAME_DEF_TEMPLATE_TERM( 1, 9, t ) } }
#define Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER10( t )	namespace atom { namespace cint { Z3D_TYPE_NAME_DEF_TEMPLATE_TERM( 1, 10, t ) } }


Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER1( std::less );
Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER2( std::pair );
Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER1( std::vector );
Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER1( std::allocator );
Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER3( std::set );
Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER4( std::map );

#endif //Z3D_TYPE_NAME_HPP
