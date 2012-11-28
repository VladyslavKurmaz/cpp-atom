/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        z3d/chain_of_resp.hpp                                          |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.07.07 - initial design moved fom gateway                  |
|              2010.08.26 - added up to 10 call parameters, unbind functions  |
|              2012.06.10 - added boost pp                                    |
|-----------------------------------------------------------------------------|
| TODO:		                                                                  |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/

#ifndef Z3D_CHAIN_OF_RESP_HPP
#define Z3D_CHAIN_OF_RESP_HPP

//
#include <z3d/node.hpp>

#ifndef Z3D_CHAIN_OF_RESP_MAX_PARAMS
#	define Z3D_CHAIN_OF_RESP_MAX_PARAMS 15
#endif

namespace z3d {
	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------
	template < class L, size_t I = 0 >
	class chain_of_resp : protected z3d::scope< L, I >
	{
	protected:
	public:
		/// def cons
		chain_of_resp() : z3d::scope< L, I >()
			{}
		/// init cons
		template < class IL >
		explicit chain_of_resp( IL const& il ) : z3d::scope< L, I >( il )
			{}
		///
		/// typed access
		template < class T >
		bool empty()
			{ return ( this-> template get_slot< T >() == 0 ); }
		/// function type accessor (typed)
		template < class T >
		T const& call()
			{ return ( this-> template get_slot< T >() ); }
		///
#define Z3D_CHAIN_OF_RESP_TYPED_CALL( z, n, _ ) \
template < class T, typename R BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM_PARAMS( n, typename T ) > \
R call( BOOST_PP_ENUM_BINARY_PARAMS( n, T, p ) ) \
	{ return ( this-> template call< T >()( BOOST_PP_ENUM_PARAMS( n, p ) ) ); }
BOOST_PP_REPEAT( BOOST_PP_INC( Z3D_CHAIN_OF_RESP_MAX_PARAMS ), Z3D_CHAIN_OF_RESP_TYPED_CALL, _)
#undef Z3D_CHAIN_OF_RESP_TYPED_CALL
		///
		/// indexed access
		template < size_t J >
		bool empty()
			{ return ( this-> template get_slot< J >() == 0 ); }
		/// function type accessor (indexed)
		template < size_t J >
		typename Loki::TL::TypeAt< L, J >::Result const& call()
			{ return ( this-> template get_slot< J >() ); }
#define Z3D_CHAIN_OF_RESP_INDEXED_CALL( z, n, _ ) \
template < size_t J, typename R BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM_PARAMS( n, typename T ) > \
R call( BOOST_PP_ENUM_BINARY_PARAMS( n, T, p ) ) \
	{ return ( this-> template call< J >()( BOOST_PP_ENUM_PARAMS( n, p ) ) ); }
BOOST_PP_REPEAT( BOOST_PP_INC( Z3D_CHAIN_OF_RESP_MAX_PARAMS ), Z3D_CHAIN_OF_RESP_INDEXED_CALL, _)
#undef Z3D_CHAIN_OF_RESP_INDEXED_CALL
		///
		/// typed bind
		template < class T >
		void bind( T const& t )
			{ this->set_slot( t ); }
		/// typed unbind
		template < class T >
		void unbind()
			{ this->set_slot( T() ); }
		/// indexed bind
		template < size_t J >
		void bind( typename Loki::TL::TypeAt< L, J >::Result const& t )
			{ this-> template set_slot< J >( t ); }
		/// indexed unbind
		template < size_t J >
		void unbind()
			{ this-> template set_slot< J >( typename Loki::TL::TypeAt< L, J >::Result() ); }
	};
}
#endif//Z3D_CHAIN_OF_RESP_HPP
