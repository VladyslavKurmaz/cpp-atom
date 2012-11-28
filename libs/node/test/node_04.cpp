/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.07.30                                                     |
|              2012.03.15 - move to boost::test                               |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#include <boost/test/included/unit_test.hpp>
#include <boost/thread/mutex.hpp>
//
#include <atom/node/node.hpp>

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

/*
1 2 3 4 5 6 
-------------
1| | | | | | |
-------------
2| | | | | | |
-------------
3| | | | | | |
-------------
4| | | | | | |
-------------
5| | | | | | |
-------------
6| | | | | | |
-------------
*/
namespace {
	class graph;
	class gnode;

	typedef unsigned int
		id_t;
	//----------------------------------------------------------------------------
	template < class T >
	class gnode_id_map : protected std::map< id_t, T >
	{
		///
		typedef std::map< id_t, T >
			base_type;
		///
		typedef T
			value_type;

	public:
		///
		template < class N >
		void insert( value_type v, N const& )
		{
			base_type::insert( std::make_pair( v->get_id(), v ) );
		}
		///
		template < class N >
		void erase( value_type v, N const& )
		{
			base_type::erase( v->get_id() );
		}
		///
		template < class N >
		void clear( N const& )
		{
			base_type::clear();
		}
		///
		value_type find( id_t const id ) const
		{
			typename base_type::const_iterator it = base_type::find( id );
			if ( it != base_type::end() )
			{
				return ( (*it).second );
			}
			return ( T() );
		}
	};
	//----------------------------------------------------------------------------
	typedef std::pair< id_t, id_t >
		key_t;
	template < class T >
	class gnode_pair : public std::pair< boost::shared_ptr< T >, boost::shared_ptr< T > >
	{};

	template< class T >
	class gnode_rel_map : protected std::map< key_t, T >
	{
		///
		typedef std::map< key_t, T >
			base_type;
		///
		typedef T
			value_type;

	public:
		///
		template < class N >
		void insert( value_type v, N const& )
		{
			//base_type::insert( std::make_pair( v->get_id(), v ) )
		}
		///
		template < class N >
		void erase( value_type v, N const& )
		{
			//base_type::erase( v->get_id() );
		}
		///
		template < class N >
		void clear( N const& )
		{
			base_type::clear();
		}
	};

	//----------------------------------------------------------------------------
	typedef atom::nstorage< graph, boost::shared_ptr, atom::narray1 >
		n2g_t;
	typedef atom::nstorage< gnode, atom::simple_ptr, gnode_id_map >
		n2n_t;
	typedef atom::nstorage< gnode, boost::shared_ptr, gnode_id_map >
		g2n_t;
	typedef atom::nstorage< gnode, gnode_pair, gnode_rel_map >
		g2r_t;
	//----------------------------------------------------------------------------
	enum
	{
		N2G = 0,
		N2N = 1,
		G2N = 0,
		G2R = 1
	};
	//----------------------------------------------------------------------------

	class gnode :	public atom::node< LOKI_TYPELIST_2( n2g_t, n2n_t ) >,
		public boost::enable_shared_from_this< gnode >,
		public boost::noncopyable
	{
		///
		id_t
			gid;
	protected:
		///
		gnode( id_t const id ) : gid( id )
		{ std::cout << "gnode() " << gid << std::endl; }

	public:
		typedef boost::shared_ptr< gnode >
			smart_ptr;
		///
		id_t get_id() const
		{ return ( this->gid ); }
		///
		~gnode()
		{ std::cout << "~gnode() " << gid << std::endl; }
		friend class graph;
	};

	class graph :	public atom::node< LOKI_TYPELIST_2( g2n_t, g2r_t ) >,
		public boost::enable_shared_from_this< graph >,
		public boost::noncopyable
	{
		///
		typedef boost::mutex
			mutex_t;
		///
		typedef boost::lock_guard< mutex_t >
			lock_t;
		///
		mutex_t
			mutex;
		///
		id_t
			gnode_id;
		///
	protected:
		///
		graph() : mutex(), gnode_id( 0 )
		{ std::cout << "graph()" << std::endl; }

	public:
		typedef boost::shared_ptr< graph >
			smart_ptr;
		///
		~graph()
		{ std::cout << "~graph()" << std::endl; }
		//
		gnode::smart_ptr create_node()
		{
			lock_t lock( mutex );
			//
			gnode::smart_ptr gn( gnode::smart_ptr( new gnode( ++gnode_id ) ) );
			atom::mount< N2G, G2N >( gn, shared_from_this() );
			return ( gn );
		}
		//
		gnode::smart_ptr get_node( id_t const id )
		{
			lock_t lock( mutex );
			return ( get_slot< g2n_t >().find( id ) );
		}
		///
		static smart_ptr create()
		{
			return ( smart_ptr( new graph ) );
		}
	};
}
BOOST_AUTO_TEST_CASE( node_04 )
{
	std::cout << std::endl << "atom::utests::node_ut04::test()" << std::endl;
	//
	gnode::smart_ptr gn;
	{
		graph::smart_ptr g = graph::create();
		g->create_node();
		g->create_node();
		gn = g->create_node();
		g->create_node();
		g->clear();
	}
}
