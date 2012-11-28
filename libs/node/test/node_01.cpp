/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        libs/node/test/node_01.cpp                                     |
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

#include <iostream>
//
#include <boost/test/included/unit_test.hpp>
//
#include <z3d/node.hpp>
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

typedef z3d::nstorage< A, boost::shared_ptr, z3d::narray1 > b_item_a;
typedef z3d::nstorage< C, boost::shared_ptr, z3d::nvector > b_item_c;

class B :	public z3d::R< B >,
	public z3d::node< LOKI_TYPELIST_2( b_item_a, b_item_c ) >
{
public:
	typedef boost::shared_ptr< B >
		smart_ptr;
	B( std::string const& n ) : z3d::R< B >( n )
	{}
};

typedef z3d::nstorage< B, boost::shared_ptr, z3d::nset > a_item_b;

class A :	public z3d::R< A >,
	public z3d::node< LOKI_TYPELIST_1( a_item_b ) >
{
public:
	typedef boost::shared_ptr< A >
		smart_ptr;
	A( std::string const& n ) : z3d::R< A >( n )
	{}
};

typedef z3d::nstorage< B, boost::shared_ptr, z3d::nlist > c_item_b;

class C :	public z3d::R< C >,
	public z3d::node< LOKI_TYPELIST_1( c_item_b ) >
{
public:
	typedef boost::shared_ptr< C >
		smart_ptr;
	C( std::string const& n ) : z3d::R< C >( n )
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
	std::cout << std::endl << "z3d::node" << std::endl;

	A::smart_ptr a1( new A( "a1" ) );
	B::smart_ptr b1( new B( "b1" ) );
	C::smart_ptr c1( new C( "c1" ) );
	C::smart_ptr c2( new C( "c2" ) );
	//
	std::cout << a1 << " " << b1 << " " << c1  << " " << c2 << std::endl;
	//
	z3d::mount< A2B, B2A >( a1, b1 );
	std::cout << a1 << " " << b1 << " " << c1  << " " << c2 << std::endl;
	//
	z3d::mount< B2C, C2B >( b1, c1 );
	std::cout << a1 << " " << b1 << " " << c1  << " " << c2 << std::endl;
	//
	z3d::mount< B2C, C2B >( b1, c2 );
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
