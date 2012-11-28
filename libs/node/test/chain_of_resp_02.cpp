/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        libs/chain_of_resp/test/chain_of_rest_02.cpp                   |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2010.08.26                                                     |
|              2012.03.16 move to boost::test                                 |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/
#include <memory.h>
//
#include <boost/test/included/unit_test.hpp>
#include <boost/function.hpp>
//
#include <z3d/chain_of_resp.hpp>

boost::unit_test::test_suite * init_unit_test_suite(int,char * * const)
{
	return 0;
}

namespace {

	// read prototype, result-readed, param: read buf & buf size
	typedef boost::function< size_t( void*, size_t const ) >
		layer_read_t;
	// write prototype, result-writed, param: write buf & buf size
	typedef boost::function< size_t( void const*, size_t const ) >
		layer_write_t;
	//
	template < class T >
	class basic_layer :
		protected z3d::chain_of_resp< LOKI_TYPELIST_2( layer_read_t, layer_write_t ) >,
		public boost::enable_shared_from_this< T >,
		public boost::noncopyable
	{
		///
		typedef z3d::chain_of_resp< LOKI_TYPELIST_2( layer_read_t, layer_write_t ) >
			base_t;
	protected:
		///
		basic_layer()
		{}
		///
		virtual size_t on_read( void* buf, size_t const buf_size )
		{ return base_t::call< layer_read_t, size_t >( buf, buf_size ); }
		///
		virtual size_t on_write( void const* buf, size_t const buf_size )
		{ return base_t::call< layer_write_t, size_t >( buf, buf_size ); }
	public:
		///
		typedef boost::shared_ptr< T >
			layer_ptr_t;
		///
		size_t read( void* buf, size_t const buf_size )
		{ return on_read( buf, buf_size ); }
		///
		size_t write( void const* buf, size_t const buf_size )
		{ return on_write( buf, buf_size ); }
		///
		template < class P >
		void attach( boost::shared_ptr< P > l )
		{
			base_t::bind<layer_read_t>( boost::bind( &P::on_read, l, _1, _2 ) );
			base_t::bind<layer_write_t>( boost::bind( &P::on_write, l, _1, _2 ) );
		}
		///
		template < class P >
		static boost::shared_ptr< P > create()
		{ return boost::shared_ptr< P >( new P() ) ; }
	};
	//
	class layer1 : public basic_layer< layer1 >
	{
	protected:
		///
		layer1()
		{};
		///
		virtual size_t on_write( void const* buf, size_t const buf_size )
		{
			unsigned char const* in = reinterpret_cast<unsigned char const *>(buf);
			unsigned char* b = reinterpret_cast<unsigned char*>( alloca( buf_size ) );
			for( size_t i = 0; i < buf_size; ++i )
				b[i] = (( in[i] )?( in[i] | 0x20 ):( in[i] )) ;
			return basic_layer< layer1 >::on_write( b, buf_size );
		}
		///
		template < class > friend class ::basic_layer;
	};
	//
	class layer2 : public basic_layer< layer2 >
	{
	protected:
		///
		layer2()
		{};
		///
		virtual size_t on_read( void* buf, size_t const buf_size )
		{
			for( size_t i = 0; i < buf_size; ++i )
				(reinterpret_cast<unsigned char*>(buf))[i] = 'A';
			return buf_size;
		}
		///
		virtual size_t on_write( void const* buf, size_t const buf_size )
		{
			std::cout << reinterpret_cast<char const*>( buf ) << std::endl;
			return buf_size;
		}
		///
		template < class > friend class ::basic_layer;
	};

}

BOOST_AUTO_TEST_CASE( chainof_resp_01 )
{
	std::cout << std::endl << "z3d::utests::chain_of_resp_ut02::test()" << std::endl;
	//
	layer1::layer_ptr_t l1 = layer1::create< layer1 >();
	layer2::layer_ptr_t l2 = layer2::create< layer2 >();
	l1->attach( l2 );
	l2.reset();
	char out[] = "AbCdEfG";
	char in[16] = { 0 };
	std::cout << out << std::endl;
	l1->write( out, strlen( out ) + 1 );
	l1->read( in, 15 );
	std::cout << in << std::endl;
}
