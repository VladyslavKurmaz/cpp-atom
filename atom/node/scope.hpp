/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.07.07 - extract functionality from node                   |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_NODE_SCOPE_HPP
#define ATOM_NODE_SCOPE_HPP

#include <atom/node/slot.hpp>
//
namespace atom {
	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	template < class L, size_t I = 0 >
	class scope : public slot< L, I >
	{
	protected:
		/// prototype
		template < class, class, size_t, size_t >
		struct assigner;
		/// init specialization
		template< class N, class T, class U, size_t J, size_t S >
		struct assigner< N, std::pair< T, U >, J, S > : public assigner< N, U, J + 1, S - 1 >
		{
			assigner< N, std::pair< T, U >, J, S >( N& n, std::pair< T, U > const& p ) : assigner< N, U, J + 1, S - 1 >( n, p.second )
				{ n.set_slot< J >( p.first ); }
		};
		/// skip init specialization
		template< class N, class U, size_t J, size_t S >
		struct assigner< N, std::pair< Loki::NullType, U >, J, S > : public assigner< N, U, J + 1, S - 1 >
		{
			assigner< N, std::pair< Loki::NullType, U >, J, S >( N& n, std::pair< Loki::NullType, U > const& p ) : assigner< N, U, J + 1, S - 1 >( n, p.second )
				{}
		};
		/// break specialization
		template< class N, size_t J >
		struct assigner< N, Loki::NullType, J, 0 >
		{
			assigner< N, Loki::NullType, J, 0 >( N&, Loki::NullType const& )
				{}
		};
		/// helper
		template < class N, class IL >
		void assign( N& n, IL const& il )
		{
			assigner< N, IL, I, Loki::TL::Length<L>::value > a( n, il );
		}

	public:
		/// default cons
		scope() : slot< L, I >()
			{}
		/// init cons
		template< class IL >
		explicit scope( IL const& il ) : slot< L, I >( il )
			{}
		/// indexed access (get)
		template < size_t J >
		typename Loki::TL::TypeAt< L, J >::Result const& get_slot()
			{ return ( this->get_value( boost::mpl::int_< J >() ) ); }
		/// typed access (get)
		template < class T >
		T const& get_slot()
			{ return ( this->get_value( boost::mpl::identity< T >() ) ); }
		/// indexed access (set)
		template < size_t J >
		void set_slot( typename Loki::TL::TypeAt< L, J >::Result const& t )
			{ this->set_value( boost::mpl::int_< J >(), t ); }
		/// typed access (set)
		template < class T >
		void set_slot( T const& t )
			{ this->set_value( boost::mpl::identity< T >(), t ); }
		///
		template < class IL >
		void assign( IL const& il )
			{ assign( *this, il ); }
	};

}
#endif//Z3D_NODE_HPP
