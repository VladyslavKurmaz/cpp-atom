/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        z3d/script/language.hpp                                        |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.05.21 - Created                                           |
|              2010.08.21 - basic structure                                   |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/

#ifndef Z3D_SCRIPT_LANGUAGE_HPP
#define Z3D_SCRIPT_LANGUAGE_HPP

#include <z3d/nmap.hpp>
#include <z3d/node.hpp>
#include <z3d/script/context.hpp>

namespace z3d { namespace script {
	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	template< class T, template < class > class PTR >
	class language : public T::language_base_t,
					 public z3d::node< ::Loki::Typelist< z3d::nstorage< typename T::context_t, PTR, z3d::nmap >, ::Loki::NullType> >
	{
	protected:
		///
		typedef typename T::log_t
			log_t;
		typedef typename T::mutex_t
			mutex_t;
		typedef boost::lock_guard< mutex_t >
			lock_t;
		typedef typename T::dict_t
			dict_t;
		///
		log_t&
			log;
		///
		mutex_t
			mutex;
		///
		dict_t
			dict;
		///
		language( log_t& l ) : log( l ), mutex(), dict()
			{}
	public:
		///
		typedef PTR< typename T::language_t >
			language_ptr_t;
		///
		void add_dict_entry( typename dict_t::handle_t h )
			{ this->dict.add_entry( h ); }
	};
} }
#endif//Z3D_SCRIPT_LANGUAGE_HPP
