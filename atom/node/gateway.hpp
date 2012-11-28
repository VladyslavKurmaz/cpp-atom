/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.06.17                                                     |
|              2012.06.10 - added boost pp                                    |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:		   - автоматическа€ генераци€ функци€ и заполнение на них           |
|              указателей-слотов                                              |
|              - ???удалить этот класс, как повтор€ющий ф-ть z3d::slot        |
|              - move all functionallity to z3d::node                         |
|              - add real gateway as methods call table                       |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_NODE_GATEWAY_HPP
#define ATOM_NODE_GATEWAY_HPP

#include <atom/node/scope.hpp>

#ifndef ATOM_NODE_GATEWAY_MAX_PARAMS
#	define ATOM_NODE_GATEWAY_MAX_PARAMS 1
#endif

namespace atom {
	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------
	template < class A, class L, size_t I = 0 >
	class gateway : protected atom::scope< L, I >
	{
		A
			accessor;
	protected:
	public:
		/// def const
		gateway() : atom::scope< L, I >(), accessor()
			{}
		/// init cons
		template < class T >
		gateway( T a, int ) : atom::scope< L, I >(), accessor( a )
			{}
		/// init cons
		template< class IL >
		explicit gateway( IL const& il ) : atom::scope< L, I >( il ), accessor()
			{}
		/// init cons
		template< class T, class IL >
		gateway( T a, IL const& il ) : atom::scope< L, I >( il ), accessor( a )
			{}
		///
		template < class T >
		void set_accessor( T a )
			{ this->accessor = a; }
		///
		/// typed access
#define Z3D_GATEWAY_TYPED_CALL( z, n, _ ) \
template < typename T, typename R BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM_PARAMS( n, typename T ) > \
R call( BOOST_PP_ENUM_BINARY_PARAMS( n, T, p ) ) \
	{ return ( ( ( accessor() ).*( this-> template get_slot< T >() ) )( BOOST_PP_ENUM_PARAMS( n, p ) ) ); }
BOOST_PP_REPEAT( BOOST_PP_INC( ATOM_NODE_GATEWAY_MAX_PARAMS ), Z3D_GATEWAY_TYPED_CALL, _)
#undef Z3D_GATEWAY_TYPED_CALL
		///
		/// indexed access
#define Z3D_GATEWAY_INDEXED_CALL( z, n, _ ) \
template < size_t J, typename R BOOST_PP_COMMA_IF( n ) BOOST_PP_ENUM_PARAMS( n, typename T ) > \
R call( BOOST_PP_ENUM_BINARY_PARAMS( n, T, p ) ) \
	{ return ( ( ( accessor() ).*( this-> template get_slot< J >() ) )( BOOST_PP_ENUM_PARAMS( n, p ) ) ); }
BOOST_PP_REPEAT( BOOST_PP_INC( ATOM_NODE_GATEWAY_MAX_PARAMS ), Z3D_GATEWAY_INDEXED_CALL, _)
#undef Z3D_GATEWAY_INDEXED_CALL
	};
}
#endif//ATOM_NODE_GATEWAY_HPP
