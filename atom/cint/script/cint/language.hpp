/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        z3d/script/cint/language.hpp                                   |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.05.21 - Created                                           |
|              2010.08.21 - basic structure                                   |
|              2012.03.23 - gcc fix                                           |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/

#ifndef Z3D_SCRIPT_CINT_LANGUAGE_HPP
#define Z3D_SCRIPT_CINT_LANGUAGE_HPP

#include <boost/shared_ptr.hpp>
//
#include <z3d/script/language.hpp>
#include <z3d/script/cint/context.hpp>

#ifdef Z3D_SCRIPT_CINT_INTERPRETER
extern "C"
{
	extern void G__cpp_setupUser();
}

static void CINTErrorRouter()
{
	Sleep( 0 );
}

static void MyErrCallback( const char* str )
{
	std::cout << str;
}
#endif // Z3D_SCRIPT_CINT_INTERPRETER



namespace z3d { namespace script { namespace cint {
	//-----------------------------------------------------------------
	//
	//-----------------------------------------------------------------
	class language : public z3d::script::language< cint_traits, boost::shared_ptr >
	{
		///
		typedef z3d::script::language< cint_traits, boost::shared_ptr >
			base_t;
		///
		typedef cint_traits::context_t
			context_t;
		///
		cint_traits::context_id_t
			context_id;
		///
		bool
			initialized;

	protected:
		///
		language( base_t::log_t& l ) :
				base_t( l )
			,	context_id()
			,	initialized( false )
			{}

	public:
		typedef base_t::language_ptr_t
			language_ptr_t;
		///
		~language()
			{ deinit(); }
		///
		static base_t::language_ptr_t create( base_t::log_t& l )
			{ return ( base_t::language_ptr_t( new language( l ) ) ); }
		///
		bool init( std::string const& init_cmd, std::string const& cint_sys_dir )
		{
			base_t::lock_t lock( base_t::mutex );
			if ( !initialized )
			{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
				G__set_p2fsetup( G__cpp_setupUser );
				G__setothermain(2);
				//
				if( G__init_cint( init_cmd.c_str() ) >= 0 )
				{
					G__SetCINTSYSDIR( const_cast< char* >( cint_sys_dir.c_str() ) );
					G__set_aterror( CINTErrorRouter );
					G__set_errmsgcallback( MyErrCallback );
					//
					//G__AllocConsole();
					G__setautoconsole( 1 );
					//
					initialized = true;
				}
#else
				initialized = true;
#endif
			}
			return ( initialized );
		}
		///
		bool add_ipath( std::string const& ipath )
		{
			base_t::lock_t lock( base_t::mutex );
			if ( initialized )
			{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
				G__add_ipath( ipath.c_str() );
				return true;
#endif
			}
			return false;
		}
		///
		bool delete_ipath( std::string const& ipath )
		{
			base_t::lock_t lock( base_t::mutex );
			if ( initialized )
			{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
				G__delete_ipath( ipath.c_str() );
				return true;
#endif
			}
			return false;
		}
		///
		bool deinit()
		{
			base_t::lock_t lock( base_t::mutex );
			if ( initialized )
			{
#ifdef Z3D_SCRIPT_CINT_INTERPRETER
				G__FreeConsole();
				G__scratch_all();
				G__free_p2fsetup();
#endif
				initialized = false;
				return true;
			}
			return false;
		}
		///
		context_t::context_ptr_t make_context()
		{ 
			base_t::lock_t lock( base_t::mutex );
			context_t::context_ptr_t cont( new context_t( base_t::log, ++context_id, base_t::mutex, base_t::dict ) );
			//?? shared_ptr vs intrusive_ptr
			z3d::mount< 0, 0 >( language_ptr_t( this->shared_from_this() ), cont );
			return ( cont );
		}
	};

} } }

#endif//Z3D_SCRIPT_CINT_LANGUAGE_HPP
