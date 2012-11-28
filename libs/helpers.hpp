/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| FILE:        libs/helpers.hpp                                               |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2010.06.24                                                     |
|              2012.11.28 - lib has been moved to github, new namespace: atom |
|-----------------------------------------------------------------------------|
| TODO:		                                                                    |
\-----------------------------------------------------------------------------/
*/

#include <boost/smart_ptr.hpp>
#if 0
#pragma warning ( push )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4512 )
#	include <boost/thread.hpp>
#pragma warning ( pop )
#endif

namespace atom {

	template < class T >
	class R : public boost::enable_shared_from_this< T >
	{
		std::string
			name;
	public:
		R( std::string const& n ) : name( n )
		{
			std::cout << typeid( T ).name() << "() [" << name << "]" << std::endl;
		}
		~R()
		{
			std::cout << "~" << typeid( T ).name() << "() [" << name << "]" << std::endl;
		}
		template< class U > friend std::ostream& operator<<( std::ostream& os, boost::shared_ptr< U >& t );
	};

	template< class T >
	std::ostream& operator<<( std::ostream& os, boost::shared_ptr< T >& t )
	{
		os << t.get()->name << ":" << t.use_count();
		return os;
	}
}