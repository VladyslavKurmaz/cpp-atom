/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.06.24                                                     |
|              2012.03.15 - move to boost::test                               |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#include <boost/test/included/unit_test.hpp>
//
#include <atom/node/node.hpp>
#include "./../../helpers.hpp"

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

class A;
class B;

typedef atom::nstorage< B, boost::shared_ptr, atom::nvector > external_t;
typedef external_t& a_item_b;
typedef atom::nstorage< A, boost::shared_ptr, atom::nlist > b_item_a;

class A :	public atom::R< A >,
	public atom::node< LOKI_TYPELIST_1( a_item_b ) >
{
	typedef atom::node< LOKI_TYPELIST_1( a_item_b ) >
		node_base_t;

public:
	typedef boost::shared_ptr< A >
		smart_ptr;
	template < class P >
	A( std::string n, P const& p ) : atom::R< A >( n ), node_base_t( p )
	{}
};

class B :	public atom::R< B >,
	public atom::node< LOKI_TYPELIST_1( b_item_a ) >
{
public:
	typedef boost::shared_ptr< B >
		smart_ptr;
	B( std::string n ) : atom::R< B >( n )
	{}
};

enum
{
	A2B = 0,
	B2A = 0
};

BOOST_AUTO_TEST_CASE( node_02 )
{
	std::cout << std::endl << "atom::node" << std::endl;
	external_t storage;
	//
	A::smart_ptr a1( new A( "a1", INITLIST_1( boost::ref( storage ) ) ) );
	B::smart_ptr b1( new B( "b1" ) );
	//
	std::cout << a1 << " " << b1 << std::endl;
	atom::mount< A2B, B2A >( a1, b1 );
	std::cout << a1 << " " << b1 << std::endl;
}
