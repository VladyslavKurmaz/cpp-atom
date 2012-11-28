/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.06.24                                                     |
|              2012.03.16 move to boost::test                                 |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#include <iostream>
//
#include <boost/test/included/unit_test.hpp>
//
#include <atom/node/node.hpp>
#include "./../../helpers.hpp"

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

/*
-------          -------          -------
|     | oo     1 |     | oo    oo |     |
|  A  |----------|  B  |----------|  C  |
|     |          |     |          |     |
-------          -------          -------
*/
class A;
class B;
class C;

typedef atom::nstorage< A, boost::shared_ptr, atom::narray1 > b_item_a;
typedef atom::nstorage< C, boost::shared_ptr, atom::nvector > b_item_c;

class B :	public atom::R< B >,
	public atom::node< LOKI_TYPELIST_2( b_item_a, b_item_c ) >
{
public:
	typedef boost::shared_ptr< B >
		smart_ptr;
	B( std::string const& n ) : atom::R< B >( n )
	{}
};

typedef atom::nstorage< B, boost::shared_ptr, atom::nset > a_item_b;

class A :	public atom::R< A >,
	public atom::node< LOKI_TYPELIST_1( a_item_b ) >
{
public:
	typedef boost::shared_ptr< A >
		smart_ptr;
	A( std::string const& n ) : atom::R< A >( n )
	{}
};

typedef atom::nstorage< B, boost::shared_ptr, atom::nlist > c_item_b;

class C :	public atom::R< C >,
	public atom::node< LOKI_TYPELIST_1( c_item_b ) >
{
public:
	typedef boost::shared_ptr< C >
		smart_ptr;
	C( std::string const& n ) : atom::R< C >( n )
	{}
};


enum
{
	A2B = 0,
	B2A = 0,
	B2C = 1,
	C2B = 0
};

BOOST_AUTO_TEST_CASE( node_01 )
{
	std::cout << std::endl << "atom::node" << std::endl;

	A::smart_ptr a1( new A( "a1" ) );
	B::smart_ptr b1( new B( "b1" ) );
	C::smart_ptr c1( new C( "c1" ) );
	C::smart_ptr c2( new C( "c2" ) );
	//
	std::cout << a1 << " " << b1 << " " << c1  << " " << c2 << std::endl;
	//
	atom::mount< A2B, B2A >( a1, b1 );
	std::cout << a1 << " " << b1 << " " << c1  << " " << c2 << std::endl;
	//
	atom::mount< B2C, C2B >( b1, c1 );
	std::cout << a1 << " " << b1 << " " << c1  << " " << c2 << std::endl;
	//
	atom::mount< B2C, C2B >( b1, c2 );
	std::cout << a1 << " " << b1 << " " << c1  << " " << c2 << std::endl;
#if 1
	b1->clear();
#else
	a1->clear();
	c1->clear();
	c2->clear();
#endif
	std::cout << a1 << " " << b1 << " " << c1  << " " << c2 << std::endl;
}
