/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        libs/scope/test/scope_01.cpp                                   |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.07.12                                                     |
|              2012.03.16 move to boost::test                                 |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{                                                                     } |
\-----------------------------------------------------------------------------/
*/

#include <iostream>
//
#include <boost/test/included/unit_test.hpp>
//
#include <z3d/scope.hpp>

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

namespace {
	typedef int (*func1_t)( float, char );
	typedef void (*func2_t)( int );
	typedef void (*func3_t)( void );
	typedef void (*func4_t)( char );

	int f1( float a1, char a2 )
	{
		std::cout << "int f1( " << a1 << ", " << a2 << " )" << std::endl;
		return 0;
	}

	void f2( int a1 )
	{
		std::cout << "int f2( " << a1 << " )" << std::endl;
	}

	void f3( void )
	{
		std::cout << "int f3()" << std::endl;
	}

	void f4( char a1 )
	{
		std::cout << "int f4( " << a1 << " )" << std::endl;
	}

	void test()
	{
	}
}

BOOST_AUTO_TEST_CASE( scope_01 )
{
	std::cout << std::endl << "z3d::utests::scope_ut01::test()" << std::endl;
	//
	DEFLIST_4( Loki::NullType, Loki::NullType, func3_t, func4_t ) 
		il1( INITLIST_4( Loki::NullType(), Loki::NullType(), f3, f4 ) );
	//
	DEFLIST_4( func1_t, func2_t, Loki::NullType, Loki::NullType ) 
		il2( INITLIST_4( f1, f2, Loki::NullType(), Loki::NullType() ) );
	//
	z3d::scope< LOKI_TYPELIST_4( func1_t, func2_t, func3_t, func4_t ) > 
		g( il1 );
	//
	g.assign( il2 );
	//
	g.get_slot< func1_t >()( 1.f, 'a' );
	g.get_slot< 1 >()( 10 );
	g.get_slot< 2 >()();
	g.get_slot< func4_t >()( 'b' );
}
