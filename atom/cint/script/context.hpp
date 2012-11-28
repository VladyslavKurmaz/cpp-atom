/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        z3d/script/context.hpp                                         |
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

#ifndef Z3D_SCRIPT_CONTEXT_HPP
#define Z3D_SCRIPT_CONTEXT_HPP

#include <z3d/node.hpp>
#include <z3d/ngmap.hpp>

namespace z3d { namespace script {
	//---------------------------------------------------------------------
	//
	//---------------------------------------------------------------------
	template< class T, template < class > class PTR >
	class context : public T::context_base_t,
					public z3d::node< ::Loki::Typelist< z3d::nstorage< typename T::language_t, PTR, z3d::narray1 >, ::Loki::NullType> >,
					public boost::noncopyable
	{
	protected:
		typedef typename T::log_t
			log_t;
		typedef typename boost::remove_reference< typename T::mutex_t >::type
			mutex_t;
		typedef mutex_t&
			mutex_ref_t;
		typedef typename T::context_id_t
			context_id_t;
		typedef typename T::dict_t
			dict_t;
		///
		typedef boost::lock_guard< mutex_t >
			lock_t;
		///
		log_t&
			log;
		///
		context_id_t
			id;
		///
		mutex_ref_t
			mutex_ref;
		///
		dict_t&
			dict;
		///
		context( log_t& l, context_id_t const cid, mutex_ref_t mr, dict_t& d ) :
				log( l )
			,	id( cid )
			,	mutex_ref( mr )
			,	dict( d )
			{}
	public:
		///
		typedef context_id_t
			id_t;
		///
		typedef std::less< id_t >
			id_cmp_t;
		///
		typedef PTR< typename T::context_t >
			context_ptr_t;
		///
		id_t get_id() const
			{ return ( this->id ); }
	};
} }
#endif//Z3D_SCRIPT_CONTEXT_HPP
