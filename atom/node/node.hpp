/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2009.08.28                                                     |
|              2009.11.05 - Add container, mount/unmount strategies           |
|              2010.02.20 - Refactoring class                                 |
|              2010.02.26 - Implement & testing                               |
|              2010.06.09 - Full refactoring & disign changing ( dimention    |
|                           abstraction added, recursive template hierachly,  |
|                           references, initializing list )                   |
|              2010.06.16 - added mount/unmount/clear advanced parameter,     |
|                           dimention enumeration                             |
|              2010.06.20 - move interface functions to containers,           |
|                           storage just define container subtype:            |
|                           class, start pointer, container                   |
|              2010.06.22 - replace z3d::dimention class by z3d::slot         |
|              2010.07.07 - extract narray1 & scope functionality             |
|              2012.03.15 - gcc fix for t-> template attach< TT >( u )        |
|              2012.03.23 - gcc fix for boost::mpl::_int                      |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:	        + выбрать способ включения блокировки через наследование или  |
|              как член класса [ блокировка убрана как свойство ]             |
|               - use boost::mpl::vector, boost::tuple                        |
|               - move from gateway init template & get/set wrappers to node  |
|               - add attach/detach functions by type                         |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_NODE_NODE_HPP
#define ATOM_NODE_NODE_HPP

//
#include <boost/smart_ptr.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/noncopyable.hpp>
#include <boost/ref.hpp>
//
#include <loki/Typelist.h>
//
#include <atom/node/simple_ptr.hpp>
#include <atom/node/nset.hpp>
#include <atom/node/nvector.hpp>
#include <atom/node/nlist.hpp>
#include <atom/node/narray1.hpp>
#include <atom/node/slot.hpp>
#include <atom/node/scope.hpp>
//
namespace atom {
	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	template < class T, template < class > class R, template < class > class C >
	class nstorage : public C < R < T > > { };

	template < class R, template < class > class C >
	class nstorage2 : public C < R > { };

	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	extern int node_cnt;

	template < class L, size_t I = 0 >
	class node : public scope< L, I >
	{
	protected:
		//---------------------------------------------------------------------
		// relations mount\unmount methods
		//---------------------------------------------------------------------
		///
		template < size_t J, class T >
		void attach( T & t )
			{ get_value( boost::mpl::int_< J >() ).insert( t, *this ); }
		///
		template < typename TT, class T >
		void attach( T & t )
			{ get_value( boost::mpl::identity< TT >() ).insert( t, *this ); }
		///
		template < size_t J, class T >
		void detach( T & t )
			{ get_value( boost::mpl::int_< J >/*::type*/() ).erase( t, *this ); }
		///
		template < typename TT, class T >
		void detach( T & t )
			{ get_value( boost::mpl::identity< TT >() ).erase( t, *this ); }

		//---------------------------------------------------------------------
		// helper class for clearing all relations
		//---------------------------------------------------------------------
		template< class N, size_t i >
		class cleaner;

		template< class N >
		class cleaner< N, 0 >
		{
		public:
			cleaner( N& ){}
		};

		template < class N, size_t i >
		class cleaner : public cleaner< N, i - 1 >
		{
		public:
			cleaner( N& n ) : cleaner< N, i - 1 >( n )
				{ n.get_value( boost::mpl::int_< I + i - 1 >() ).clear( n ); }
		};

	public:
		///
		node() : scope< L, I >()
			{ ++node_cnt; }
		///
		template< class IL >
		explicit node( IL const& il ) : scope< L, I >( il )
			{ ++node_cnt; }
		///
		~node() {
			--node_cnt; }
		///
		void clear()
			{ cleaner< node, Loki::TL::Length<L>::value > clean( *this ); }
		///
		template < size_t TI, class T, class U > friend void mount( T t, U u );
		template < typename TT, class T, class U > friend void mount( T t, U u );
		template < size_t TI, size_t UI, class T, class U > friend void mount( T t, U u );
		template < typename TT, typename UT, class T, class U > friend void mount( T t, U u );
		template < size_t TI, class T, class U > friend void unmount( T t, U u );
		template < typename TT, class T, class U > friend void unmount( T t, U u );
		template < size_t TI, size_t UI, class T, class U > friend void unmount( T t, U u );
		template < typename TT, typename UT, class T, class U > friend void unmount( T t, U u );
	};
	/// create unary relation (indexed)
	template < size_t TI, class T, class U >
	void mount( T t, U u )
		{ t-> template attach< TI >( u ); }
	/// create unary relation (typed)
	template < typename TT, class T, class U >
	void mount( T t, U u )
		{ t-> template attach< TT >( u ); }
	/// create binary relation (indexed)
	template < size_t TI, size_t UI, class T, class U >
	void mount( T t, U u )
		{ mount< TI >( t, u ); u-> template attach< UI >( t ); }
	/// create binary relation (typed)
	template < typename TT, typename UT, class T, class U >
	void mount( T t, U u )
		{ mount< TT >( t, u ); u-> template attach< UT >( t ); }
	/// destroy unary relation (indexed)
	template < size_t TI, class T, class U >
	void unmount( T t, U u )
		{ t-> template detach< TI >( u ); }
	/// destroy unary relation (typed)
	template < typename TT, class T, class U >
	void unmount( T t, U u )
		{ t-> template detach< TT >( u ); }
	/// destroy binary relation (indexed)
	template < size_t TI, size_t UI, class T, class U >
	void unmount( T t, U u )
		{ unmount< TI >( t, u ); u-> template detach< UI >( t ); }
	/// destroy binary relation (indexed)
	template < typename TT, typename UT, class T, class U >
	void unmount( T t, U u )
		{ unmount< TT >( t, u ); u-> template detach< UT >( t ); }
	/// cleare node via smart pointer
	template < class T >
	void clear_node( T const & p ) {
		if ( p ) { p->clear(); } }
	/// recursive cleare node via smart pointer
	template < class T, class S, class N >
	void clear_slot( N & n ) {
		if ( n ) { 
			n->get_slot<S>().for_each2( boost::bind( &clear_node<T>, _1 ) );
		}
	}
}
#endif//ATOM_NODE_NODE_HPP
