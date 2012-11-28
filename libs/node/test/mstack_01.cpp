/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        libs/mstack/test/mstack_01.cpp                                 |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.06.24                                                     |
|              2012.03.16 move to boost::test                                 |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{                                                                     } |
\-----------------------------------------------------------------------------/
*/
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/any.hpp>
#include <boost/test/included/unit_test.hpp>
//
#include <z3d/mstack.hpp>

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

namespace {

	struct A
	{
		bool v;
		int func1( char c ){ std::cout << "A::func1(" << c << ")" << std::endl; return 0; }
		void func2( int i ){ std::cout << "A::func2(" << i << ")" << std::endl; }
		bool func3( ){ std::cout << "A::func3(" << v << ")" << std::endl; return v; }
	};

	typedef int( A::* func1_t )( char );
	typedef void( A::* func2_t )( int );
	typedef bool( A::* func3_t )();
	struct C
	{
		A a;
		func1_t value;
		//typedef a.*value z;
	};

#define CALLP( o, a, i, arg ) ( o.a.*( o.get_value( boost::mpl::int_<i>() ) ) )( arg )
#define CALL( o, a, i ) ( o.a.*( o.get_value( boost::mpl::int_<i>() ) ) )()

	struct B : public z3d::slot< LOKI_TYPELIST_3( func1_t, func2_t, func3_t ), 0 >
	{
		typedef z3d::slot< LOKI_TYPELIST_3( func1_t, func2_t, func3_t ), 0 >
			slot_base_t;
		A a1;
		A a2;
		template < class P >
		B( P const& p ) : slot_base_t( p )
		{
			a1.v = true;
			a2.v = false;
		}
		//
		void call()
		{
			//funt1_t f = a.*( get_value( boost::mpl::int_<0>() ) );
			( a1.*( get_value( boost::mpl::int_<0>() ) ) )( 'c' );
			( a1.*( get_value( boost::mpl::int_<1>() ) ) )( 5 );
			//boost::bind( a1.*( get_value( boost::mpl::int_<1>() ) ) );
			//boost::function< void( int ) > f = a1.*( get_value( boost::mpl::int_<1>() ) );
		}
		//
		//typedef  int(* fff )(char);
		//void call1()
		//{
		//	funt1_t f = get_value( boost::mpl::int_<0>() );
		//	int (*pf)( char );
		//	pf = ( a.*f );

		//}
	};
}

BOOST_AUTO_TEST_CASE( mstack_01 )
{

	B b( INITLIST_3( &A::func1, &A::func2, &A::func3 ) );
	b.call();
	//b.call1();
	CALLP( b, a1, 0, ('C') );
	CALLP( b, a1, 1, ( 23 ) );
	CALL( b, a1, 2 );
	CALL( b, a2, 2 );
	//
	C c;
	c.value = &A::func1;
	( c.a.*c.value )( 'q' );
	//boost::bind( c.a.*c.value, &(c.a) );
	//func1_t ff = ( c.a.*c.value );

	//c.z
	//std::cout << typeid( (c.a).*(c.value) ).name() <<std::endl;

	//int (*pf)( char );
	//pf = ( (c.a).*(c.value) );
	//boost::any any( (c.a).*(c.value) );
	//
	//
	A a;
	func1_t f = &A::func1;
	(a.*f)( 't' );
	//boost::bind( (a.*f), a, _1 );
	//

	//
	z3d::mstack< int, bool > ms;
	struct _
	{
		static void push( int const& s1, int const& s2 )
		{
			std::cout << "push " << s1 << " " << s2 << std::endl;
		}
		static void pop( int const& s1, int const& s2, bool& r )
		{
			r = true;
			std::cout << "pop " << s1 << " " << s2 << " " << r << std::endl;
		}
	};
	ms.push( 1, boost::bind( &_::push, _1, _2 ), boost::bind( &_::pop, _1, _2, _3 ) );
	ms.push( 2, boost::bind( &_::push, _1, _2 ), boost::bind( &_::pop, _1, _2, _3 ) );
	ms.pop( true );
	ms.push( 3, boost::bind( &_::push, _1, _2 ), boost::bind( &_::pop, _1, _2, _3 ) );
	ms.pop( false );
	ms.pop( false );
}

