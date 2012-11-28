/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.07.12                                                     |
|              2012.03.16 move to boost::test                                 |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/
#include <iostream>
//
#include <boost/test/included/unit_test.hpp>
#include <boost/bind.hpp>
//
#include <atom/node/gateway.hpp>

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

BOOST_AUTO_TEST_CASE( gateway_02 )
{
	struct _
	{
		static std::string& get()
		{
			static std::string str;
			str += "a";
			return str;
		}
	};
	//
	typedef size_t( std::string::* string_fn1_t )() const;
	typedef char const*( std::string::* string_fn2_t )() const;
	typedef std::string&( std::string::* string_fn3_t )( size_t, char );
	typedef std::string&( *string_acc_t )();
	//
	atom::gateway< string_acc_t, LOKI_TYPELIST_2( string_fn1_t, string_fn2_t ) >
		gateway( &_::get, INITLIST_2( &std::string::size, &std::string::c_str ) );
	//
	std::cout << std::endl << "atom::utests::gateway_ut01::test()" << std::endl;
	//
	std::cout << "first call: " << gateway.call< string_fn1_t, size_t >() << std::endl;
	std::cout << "second call: " << gateway.call< 0, size_t >() << std::endl;
	std::cout << "third call: " << gateway.call< string_fn2_t, char const* >() << std::endl;
}

