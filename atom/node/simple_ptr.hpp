/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2009.09.30 - Created                                           |
|              2010.03.23 - Move to new namepace, devide for diff files       |
|              2012.03.26 gcc(cygwin) build                                   |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:        add shared_variant_ptr from z3d::util::msxml                   |
|              find new name for simple_ptr                                   |
\-----------------------------------------------------------------------------/
*/
#ifndef ATOM_NODE_SIMPLE_PTR_HPP
#define ATOM_NODE_SIMPLE_PTR_HPP

#include <boost/smart_ptr.hpp>

namespace atom {
	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	template< class T >
	class simple_ptr
	{
	private:
		///
		T*
			px;
		///
		typedef atom::simple_ptr<T>
			this_type;

	protected:
	public:
		typedef T element_type;
		typedef T value_type;
		typedef T * pointer;

		///
		simple_ptr() : px( 0 ) {}
		///
		template < class Y >
		explicit simple_ptr( Y* p ) : px( p ) {}
		///
		template<class Y>
		simple_ptr( simple_ptr<Y> const & r ) : px( r.px ) {}
		///
		template<class Y>
		simple_ptr( boost::shared_ptr<Y> const & r ) : px( r.get() ) {}
		///
		simple_ptr & operator=( simple_ptr const & r )
		{
			this_type(r).swap(*this);
			return *this;
		}
		///
		template<class Y>
		simple_ptr & operator=( simple_ptr< Y > const & r)
		{
			this_type(r).swap(*this);
			return *this;
		}
		///
		void reset()
		{
			this_type().swap(*this);
		}
		T& operator*() const
		{
			BOOST_ASSERT(px != 0);
			return *px;
		}

		T* operator->() const
		{
			BOOST_ASSERT(px != 0);
			return px;
		}

		T* get() const
		{
			return px;
		}
		void swap( simple_ptr<T> & other )
		{
			std::swap( px, other.px );
		}
	};

	template<class T, class U> inline bool operator==( simple_ptr<T> const & a, simple_ptr<U> const & b )
	{
		return a.get() == b.get();
	}

	template<class T, class U> inline bool operator!=( simple_ptr<T> const & a, simple_ptr<U> const & b )
	{
		return a.get() != b.get();
	}

	template<class T> inline bool operator!=( simple_ptr<T> const & a, simple_ptr<T> const & b )
	{
		return a.get() != b.get();
	}

	template< class T > inline T * get_pointer( simple_ptr<T> const & p )
	{
		return p.get();
	}

}

#endif//ATOM_NODE_SIMPLE_PTR_HPP
