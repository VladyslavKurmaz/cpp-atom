/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        z3d/script/cint/context.hpp                                    |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.05.21 - Created                                           |
|              2010.08.21 - basic structure                                   |
|              2012.03.23 - gcc fix for friend class definition               |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/

#ifndef Z3D_SCRIPT_CINT_CONTEXT_HPP
#define Z3D_SCRIPT_CINT_CONTEXT_HPP

#include <boost/shared_ptr.hpp>
#include <boost/type_traits.hpp>
//
#include <z3d/packet.hpp>
#include <z3d/script/cint/language.hpp>
#include <z3d/script/context.hpp>

namespace z3d { namespace script { namespace cint {

	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	class context : public z3d::script::context< cint_traits, boost::shared_ptr >
	{
		///
		typedef z3d::script::context< cint_traits, boost::shared_ptr >
			base_t;
		///
		typedef char const*
			string_t;

	protected:
		///
		context( base_t::log_t& l, base_t::id_t const cid, base_t::mutex_ref_t mr, base_t::dict_t& d ) : base_t( l, cid, mr, d )
			{}
		///
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
		template < typename R >
		R call_script( string_t func, string_t param )
		{
			std::stringstream ss;
			ss << func << '(' << param << ')' << ';';
			//base_t::log << ss.str().c_str() << std::endl;
			return ( result< boost::is_float< R >::value >::convert< R >( G__calc( ss.str().c_str() ) ) );
		}
#endif

	public:
		typedef base_t::context_ptr_t
			context_ptr_t;
		///
		~context()
		{
			base_t::lock_t lock( base_t::mutex_ref );
			z3d::unmount< 0 >( get_slot<0>().item(), boost::shared_ptr< context >( this ) );
		}
		///
		bool load( std::string const& script )
		{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
			base_t::lock_t lock( base_t::mutex_ref );
			return( G__loadfile( script.c_str() ) == G__LOADFILE_SUCCESS );
#else
			return true;
#endif
		}
		///
		bool unload( std::string const& script )
		{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
			base_t::lock_t lock( base_t::mutex_ref );
			return( G__unloadfile( script.c_str() ) == G__UNLOADFILE_SUCCESS );
#else
			return true;
#endif
		}
		///
		template< typename R >
		R eval( string_t expr )
		{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
			return ( result< boost::is_float< R >::value >::convert< R >( G__exec_text( expr ) ) );
#else
			return ( R() );
#endif
		}
		///
		template< typename R >
		R call( string_t func )
		{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
			return ( call_script< R >( func, "" ) );
#else
			return ( dict.get<R(*)()>( func )() );
#endif
		}
		///
		template< typename R, typename P1 >
		R call( string_t func, P1 p1 )
		{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
			std::stringstream ss;
			ss
				<< z3d::type_name::param_( p1 )
				;
			return ( call_script< R >( func, ss.str().c_str() ) );
#else
			return ( dict.get<R(*)( P1 )>( func )( p1 ) );
#endif
		}
		///
		template< typename R, typename P1, typename P2 >
		R call( string_t func, P1 p1, P2 p2 )
		{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
			std::stringstream ss;
			ss
				<< z3d::type_name::param_( p1 ) << "," << z3d::type_name::param_( p2 )
				;
			return ( call_script< R >( func, ss.str().c_str() ) );
#else
			return ( dict.get<R(*)( P1, P2 )>( func )( p1, p2 ) );
#endif
		}
		///
		template< typename R, typename P1, typename P2, typename P3 >
		R call( string_t func, P1 p1, P2 p2, P3 p3 )
		{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
			std::stringstream ss;
			ss
				<< z3d::type_name::param_( p1 ) << "," << z3d::type_name::param_( p2 ) << "," << z3d::type_name::param_( p3 )
				;
			return ( call_script< R >( func, ss.str().c_str() ) );
#else
			return ( dict.get<R(*)( P1, P2, P3 )>( func )( p1, p2, p3 ) );
#endif
		}
		///
		template< typename R, typename P1, typename P2, typename P3, typename P4 >
		R call( string_t func, P1 p1, P2 p2, P3 p3, P4 p4 )
		{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
			std::stringstream ss;
			ss
				<< z3d::type_name::param_( p1 ) << "," << z3d::type_name::param_( p2 ) << "," << z3d::type_name::param_( p3 ) << ","
				<< z3d::type_name::param_( p4 )
				;
			return ( call_script< R >( func, ss.str().c_str() ) );
#else
			return ( dict.get<R(*)( P1, P2, P3, P4 )>( func )( p1, p2, p3, p4 ) );
#endif
		}
		///
		template< typename R, typename P1, typename P2, typename P3, typename P4, typename P5 >
		R call( string_t func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5 )
		{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
			std::stringstream ss;
			ss
				<< z3d::type_name::param_( p1 ) << "," << z3d::type_name::param_( p2 ) << "," << z3d::type_name::param_( p3 ) << ","
				<< z3d::type_name::param_( p4 ) << "," << z3d::type_name::param_( p5 )
				;
			return ( call_script< R >( func, ss.str().c_str() ) );
#else
			return ( dict.get<R(*)( P1, P2, P3, P4, P5 )>( func )( p1, p2, p3, p4, p5 ) );
#endif
		}
		///
		template< typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6 >
		R call( string_t func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6 )
		{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
			std::stringstream ss;
			ss
				<< z3d::type_name::param_( p1 ) << "," << z3d::type_name::param_( p2 ) << "," << z3d::type_name::param_( p3 ) << ","
				<< z3d::type_name::param_( p4 ) << "," << z3d::type_name::param_( p5 ) << "," << z3d::type_name::param_( p6 )
				;
			return ( call_script< R >( func, ss.str().c_str() ) );
#else
			return ( dict.get<R(*)( P1, P2, P3, P4, P5, P6 )>( func )( p1, p2, p3, p4, p5, p6 ) );
#endif
		}
		///
		template< typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7 >
		R call( string_t func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7 )
		{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
			std::stringstream ss;
			ss
				<< z3d::type_name::param_( p1 ) << "," << z3d::type_name::param_( p2 ) << "," << z3d::type_name::param_( p3 ) << ","
				<< z3d::type_name::param_( p4 ) << "," << z3d::type_name::param_( p5 ) << "," << z3d::type_name::param_( p6 ) << ","
				<< z3d::type_name::param_( p7 )
				;
			return ( call_script< R >( func, ss.str().c_str() ) );
#else
			return ( dict.get<R(*)( P1, P2, P3, P4, P5, P6, P7 )>( func )( p1, p2, p3, p4, p5, p6, p7 ) );
#endif
		}
		///
		template< typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8 >
		R call( string_t func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8 )
		{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
			std::stringstream ss;
			ss
				<< z3d::type_name::param_( p1 ) << "," << z3d::type_name::param_( p2 ) << "," << z3d::type_name::param_( p3 ) << ","
				<< z3d::type_name::param_( p4 ) << "," << z3d::type_name::param_( p5 ) << "," << z3d::type_name::param_( p6 ) << ","
				<< z3d::type_name::param_( p7 ) << "," << z3d::type_name::param_( p8 )
				;
			return ( call_script< R >( func, ss.str().c_str() ) );
#else
			return ( dict.get<R(*)( P1, P2, P3, P4, P5, P6, P7, P8 )>( func )( p1, p2, p3, p4, p5, p6, p7, p8 ) );
#endif
		}
		///
		template< typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9 >
		R call( string_t func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9 )
		{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
			std::stringstream ss;
			ss
				<< z3d::type_name::param_( p1 ) << "," << z3d::type_name::param_( p2 ) << "," << z3d::type_name::param_( p3 ) << ","
				<< z3d::type_name::param_( p4 ) << "," << z3d::type_name::param_( p5 ) << "," << z3d::type_name::param_( p6 ) << ","
				<< z3d::type_name::param_( p7 ) << "," << z3d::type_name::param_( p8 ) << "," << z3d::type_name::param_( p9 )
				;
			return ( call_script< R >( func, ss.str().c_str() ) );
#else
			return ( dict.get<R(*)( P1, P2, P3, P4, P5, P6, P7, P8, P9 )>( func )( p1, p2, p3, p4, p5, p6, p7, p8, p9 ) );
#endif
		}
		///
		template< typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10 >
		R call( string_t func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10 )
		{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
			std::stringstream ss;
			ss
				<< z3d::type_name::param_( p1 ) << "," << z3d::type_name::param_( p2 ) << "," << z3d::type_name::param_( p3 ) << ","
				<< z3d::type_name::param_( p4 ) << "," << z3d::type_name::param_( p5 ) << "," << z3d::type_name::param_( p6 ) << ","
				<< z3d::type_name::param_( p7 ) << "," << z3d::type_name::param_( p8 ) << "," << z3d::type_name::param_( p9 ) << ","
				<< z3d::type_name::param_( p10 )
				;
			return ( call_script< R >( func, ss.str().c_str() ) );
#else
			return ( dict.get<R(*)( P1, P2, P3, P4, P5, P6, P7, P8, P9, P10 )>( func )( p1, p2, p3, p4, p5, p6, p7, p8, p9, p10 ) );
#endif
		}

		friend class language;
	};
} } }

#endif//Z3D_SCRIPT_CONTEXT_HPP
