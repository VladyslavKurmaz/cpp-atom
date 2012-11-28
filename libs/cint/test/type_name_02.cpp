/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        libs/type_name/test/type_name_02.cpp                           |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2011.08.08                                                     |
|              2012.03.16 move to boost::test                                 |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{ DEV                                                                 } |
\-----------------------------------------------------------------------------/
*/

#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <utility>
//
#include<boost/test/included/unit_test.hpp>
//
#include <z3d/type_name.hpp>

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}


struct A {};
struct B : public A {};
template < typename, typename, typename >
class my_template{};
//
Z3D_TYPE_NAME_DEF_TERM_USER( A );
Z3D_TYPE_NAME_DEF_TERM_USER( B );
Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER1( boost::shared_ptr );
Z3D_TYPE_NAME_DEF_TEMPLATE_TERM_USER3( my_template );

template < typename T >
void print_type( char const* name )
{
	static int i = 0;
	std::cout
		<< "(" << ++i << ")\t" << name << std::endl
		<< "\t\ttypeid : " << typeid( T ).name() << std::endl
		<< "\t\ttype_  : " << z3d::type_name::type_< T >::name() << std::endl << std::endl;
}

BOOST_AUTO_TEST_CASE( type_name_02 )
{
	print_type< long long& >( "long long&" );
	print_type< char[16] >( "char[16]" );
	print_type< long const* volatile >( "long const* volatile" );
	print_type< B const * const >( "B const * const" );
	print_type< unsigned char(*)( void*(*)(int const&) ) >( "unsigned char(*)( void*(*)(int const&)" );
	print_type< my_template< int, std::vector< float >, char > >( "my_template< int, std::vector< float >, char >" );
	print_type< boost::shared_ptr< A > >( "boost::shared_ptr< A >" );
	print_type< std::vector< int > >( "std::vector< int >" );
	print_type< std::set< int > >( "std::set< int >" );
	print_type< std::map< int, int > >( "std::map< int, int >" );
	print_type< int A::* >( "int A::*" );
	print_type< bool(A::*)( B const&, float ) >( "bool(A::*)( B const&, float )" );
}