/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        libs/node/test/node_02.cpp                                     |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.06.24                                                     |
|              2012.03.15 - move to boost::test                               |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{                                                                     } |
\-----------------------------------------------------------------------------/
*/

#include <boost/test/included/unit_test.hpp>
//
#include <z3d/node.hpp>
#include "./../../helpers.hpp"

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

class A;
class B;

typedef z3d::nstorage< B, boost::shared_ptr, z3d::nvector > external_t;
typedef external_t& a_item_b;
typedef z3d::nstorage< A, boost::shared_ptr, z3d::nlist > b_item_a;

class A :	public z3d::R< A >,
	public z3d::node< LOKI_TYPELIST_1( a_item_b ) >
{
	typedef z3d::node< LOKI_TYPELIST_1( a_item_b ) >
		node_base_t;

public:
	typedef boost::shared_ptr< A >
		smart_ptr;
	template < class P >
	A( std::string n, P const& p ) : z3d::R< A >( n ), node_base_t( p )
	{}
};

class B :	public z3d::R< B >,
	public z3d::node< LOKI_TYPELIST_1( b_item_a ) >
{
public:
	typedef boost::shared_ptr< B >
		smart_ptr;
	B( std::string n ) : z3d::R< B >( n )
	{}
};

enum
{
	A2B = 0,
	B2A = 0
};

BOOST_AUTO_TEST_CASE( node_02 )
{
	std::cout << std::endl << "z3d::node" << std::endl;
	external_t storage;
	//
	A::smart_ptr a1( new A( "a1", INITLIST_1( boost::ref( storage ) ) ) );
	B::smart_ptr b1( new B( "b1" ) );
	//
	std::cout << a1 << " " << b1 << std::endl;
	z3d::mount< A2B, B2A >( a1, b1 );
	std::cout << a1 << " " << b1 << std::endl;
}
