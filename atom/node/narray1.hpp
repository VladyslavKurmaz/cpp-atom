/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.07.07 - move class from node.hpp                          |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:                                                                       |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_NODE_NARRAY1_HPP
#define ATOM_NODE_NARRAY1_HPP

#include <boost/array.hpp>

namespace atom {
	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	template < class T >
	class narray1 : protected boost::array< T, 1 >
	{
		///
		typedef typename boost::array< T, 1 >::value_type
			value_type;

	public:
		///
		template < class N >
		void insert( value_type v, N const& )
		{
			this->elems[0] = v;
		}
		///
		template < class N >
		void erase( value_type v, N const& )
		{
			assert( this->elems[0] == v );
			this->elems[0] = value_type();
		}
		///
		template < class N >
		void clear( N const& )
		{
			this->elems[0] = value_type();
		}
		///
		value_type item() const
		{
			return ( this->elems[0] );
		}
	};
}
#endif//ATOM_NODE_NARRAY1_HPP
