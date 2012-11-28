/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        libs/gateway/test/gateway_02.cpp                               |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.08.27                                                     |
|              2012.03.16 move to boost::test                                 |
|-----------------------------------------------------------------------------|
| TODO:		                                                                  |
|-----------------------------------------------------------------------------|
| TAGS{                                                                     } |
\-----------------------------------------------------------------------------/
*/
#include <iostream>
#include <stack>
//
#include <boost/test/included/unit_test.hpp>
#include <boost/noncopyable.hpp>
//
#include <z3d/gateway.hpp>


boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

namespace {

	class base
	{
	public:
		virtual void update( int const i ) = 0;
	};
	///
	class A : public base
	{
	public:
		virtual void update( int const i )
		{ std::cout << "A::update(" << i << ");" << std::endl; }
	};
	///
	class B : public base
	{
	public:
		virtual void update( int const i )
		{ std::cout << "B::update(" << i << ");" << std::endl; }
	};
	///
	typedef void ( base::* base_update_t )( int const );
	///
	class manager : public z3d::gateway< manager&, LOKI_TYPELIST_1( base_update_t ) >,
		protected std::stack< boost::reference_wrapper< base > >,
		public boost::noncopyable
	{
	public:
		///
		manager() : z3d::gateway< manager&, LOKI_TYPELIST_1( base_update_t ) >( boost::ref( *this ), INITLIST_1( &base::update ) )
		{}
		///
		base& operator()()
		{ return stack< boost::reference_wrapper< base > >::top(); }
		///
		void push( base& p )
		{ stack< boost::reference_wrapper< base > >::push( boost::ref( p ) ); }
		///
		void pop()
		{ stack< boost::reference_wrapper< base > >::pop(); }
	};
}

BOOST_AUTO_TEST_CASE( gateway_01 )
{
	std::cout << std::endl << "z3d::utests::gateway_ut02::test()" << std::endl;
	//
	A a1;
	A a2;
	B b1;
	manager man;
	man.push( b1 );
	man.push( a1 );
	man.call< base_update_t, void >( 0 );
	man.pop();
	man.call< base_update_t, void >( 1 );
	man.push( a2 );
	man.call< base_update_t, void >( 2 );
}

