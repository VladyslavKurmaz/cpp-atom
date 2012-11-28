/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2009.09.16 - Created                                           |
|              2010.04.16 - lowcase, constructor vars initializzation         |
|-----------------------------------------------------------------------------|
| TODO:		                                                                  |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_NODE_LOCK_HPP
#define ATOM_NODE_LOCK_HPP

namespace atom {
	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------
	struct no_lock
	{
		//
		void	lock(){};
		//
		void	unlock(){};
	};

	//-----------------------------------------------------------------------------
	//
	//-----------------------------------------------------------------------------
	template < class CL, class OL >
	class co_lock
	{
		typedef CL
			class_lock_t;
		typedef OL
			object_lock_t;
	private:
		///
		class_lock_t
			class_lock;
		///
		object_lock_t
			object_lock;
		///
		void cl_lock()
		{ class_lock.lock(); }
		///
		void cl_unlock()
		{ class_lock.unlock(); }

	public:
		///
		co_lock() :
				class_lock()
			,	object_lock()
			{ cl_lock(); }
		///
		co_lock( class_lock_t _class_lock ) :
				class_lock( _class_lock )
			,	object_lock()
			{ cl_lock(); }
		///
		co_lock( object_lock_t _object_lock, int ) :
				class_lock()
			,	object_lock( _object_lock )
			{ cl_lock(); }
		///
		co_lock( class_lock_t _class_lock, object_lock_t _object_lock ) :
				class_lock( _class_lock )
			,	object_lock( _object_lock )
			{ cl_lock(); }
		///
		~co_lock()
			{ cl_unlock(); }
		///
		void lock()
			{ object_lock.lock(); }
		///
		void unlock()
			{ object_lock.unlock(); }
	};

}
#endif//ATOM_NODE_LOCK_HPP
