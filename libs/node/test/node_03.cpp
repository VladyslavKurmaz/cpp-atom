/*
/-----------------------------------------------------------------------------\
| Copyright � 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.07.29                                                     |
|              2012.03.15 - move to boost::test                               |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#include <boost/test/included/unit_test.hpp>
//
#include <atom/node/node.hpp>

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

template < class T >
class refc
{
	///
	unsigned int
		rc;
	///
	std::string
		name;

protected:
	///
	~refc()
	{
		std::cout << "~" << typeid( T ).name() << "() [" << name << "]" << std::endl;
	}

public:
	///
	refc( std::string const& n ) : rc( 0 ), name( n )
	{
		std::cout << typeid( T ).name() << "() [" << name << "]" << std::endl;
	}
	///
	void add_ref()
	{ ++rc; }
	///
	void release()
	{ if ( !--rc ) delete this; }
	///
	unsigned int use_count()
	{ return ( rc ); }
	///
	template< class U > friend std::ostream& operator<<( std::ostream& os, boost::intrusive_ptr< U >& r );
};

template< class T >
std::ostream& operator<<( std::ostream& os, boost::intrusive_ptr< T >& r )
{
	os << r->name << ":" << r->use_count();
	return os;
}

template < class T >
void intrusive_ptr_add_ref( refc< T > * p )
{
	p->add_ref();
}

template < class T >
void intrusive_ptr_release( refc< T > * p )
{
	p->release();
}

//
class A;
class B;
//
typedef atom::nstorage< A, boost::intrusive_ptr, atom::narray1 > b_item_a;
//
class A : public refc< A >
{
public:
	typedef boost::intrusive_ptr< A >
		smart_ptr;
	A( std::string const& n ) : refc< A >( n )
	{}
};

BOOST_AUTO_TEST_CASE( node_03 )
{
	std::cout << std::endl << "atom::utests::node_ut03::test()" << std::endl;
	//
	A::smart_ptr a1( new A( "a1" ) );
	std::cout << a1 << std::endl;
}
