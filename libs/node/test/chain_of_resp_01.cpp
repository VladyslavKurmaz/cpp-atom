/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.08.26                                                     |
|              2012.03.16 move to boost::test                                 |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/
#include <boost/test/included/unit_test.hpp>
#include <boost/function.hpp>
//
#include <atom/node/chain_of_resp.hpp>

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

namespace {

	// simple function
	void f1( int const& p )
	{
		std::cout << "void f1(" << p << ")" << std::endl;
	}
	typedef void(*f1_t)( int const& );

	// class member
	class A
	{
	public:
		void f( int const v, int const a )
		{
			std::cout << "void A::f(" << v << ", " << a << ")->" << v + a << std::endl;
		}
	};
	typedef boost::function< void( int const, int const ) >
		Af_t;

}
BOOST_AUTO_TEST_CASE( chain_of_resp_01 )
{
	std::cout << std::endl << "atom::utests::chain_of_resp_ut01::test()" << std::endl;
	//
	A a1;
	A a2;
	//
	DEFLIST_3( f1_t, ::Loki::NullType, Af_t ) 
		il1( INITLIST_3( f1, ::Loki::NullType(), boost::bind( &A::f, &a1, _1, _2 ) ) );

	atom::chain_of_resp< LOKI_TYPELIST_3( f1_t, Af_t, Af_t ) > 
		chain1( il1 );
	//
	chain1.call<f1_t>()( 5 );
	//chain1.unbind<0>();
	chain1.call<0>()( 6 );
	chain1.call<f1_t, void>( 7 );
	//
	chain1.bind<1>( boost::bind( &A::f, &a2, _1, _2 ) );
	chain1.call<1>()( -1, 1 );
	//
	chain1.call<2>()( 1, 0 );
	chain1.call<2, void>( 1, 0 );
	//
}
