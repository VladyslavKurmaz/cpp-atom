/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2012 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladyslav.kurmaz@rozoom-group.com                                           |
|-----------------------------------------------------------------------------|
| FILE:        z3d/ptr.hpp                                                    |
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
| HISTORY:     2009.09.30 - Created                                           |
|              2010.03.23 - Move to new namepace, devide for diff files       |
|              2012.03.26 gcc(cygwin) build                                   |
|-----------------------------------------------------------------------------|
| TODO:        add shared_variant_ptr from z3d::util::msxml                   |
|              find new name for simple_ptr                                   |
|-----------------------------------------------------------------------------|
| TAGS{ SDK                                                                 } |
\-----------------------------------------------------------------------------/
*/
#ifndef Z3D_PTR_HPP
#define Z3D_PTR_HPP

#include <boost/smart_ptr.hpp>
#include <z3d/platform.hpp>

namespace z3d
{
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
		typedef z3d::simple_ptr<T>
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

	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	template< typename T >
	class basic_shared_mod_ptr : protected boost::shared_ptr< void >
	{
		typedef typename T::module_t
			module_t;
		///
		module_t
			module;
		///
		static void unload( module_t h )
			{ if ( h != 0 ) T::unload_lib( h ); }

	public:
		///
		basic_shared_mod_ptr() : shared_ptr(), module( 0 )
			{}
		///
		explicit basic_shared_mod_ptr( module_t h ) : shared_ptr(), module( h )
			{}
		///
		template< typename charT >
		explicit basic_shared_mod_ptr( std::basic_string< charT > const& name ) : 
			shared_ptr( T::load_lib( T::build_lib_name( name ) ), unload ), module( 0 )
			{}
		///
		module_t get()
		{	if ( this->module )
				return ( this->module );
			return ( reinterpret_cast< module_t >( boost::shared_ptr< void >::get() ) ); }
	};


#ifdef Z3D_WINDOWS
	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	template< class T >
	class shared_com_ptr : public boost::shared_ptr< T >
	{
	public:
		///
		shared_com_ptr() : boost::shared_ptr< T >() {}
		///
		shared_com_ptr( T* t ) : boost::shared_ptr< T >( t, ReleaseCOM ) {}
		///
		bool valid() const
		{ return ( boost::shared_ptr< T >::get() != NULL ); }
		///
		static void ReleaseCOM( IUnknown* d ) 
		{
			if ( d != 0 )
				d->Release();
		}
	};

	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	class shared_dc_ptr : public boost::shared_ptr< void >
	{
	public:
		///
		shared_dc_ptr() : shared_ptr() {}
		///
		shared_dc_ptr( HWND hWnd ) : shared_ptr( GetWindowDC( hWnd ), ReleaseHDC ) {}
		///
		shared_dc_ptr( HWND hWnd, int ) : shared_ptr( GetDC( hWnd ), ReleaseHDC ) {}
		///
		static void ReleaseHDC( HDC dc )
		{
			if ( dc != 0 ) ReleaseDC( WindowFromDC( dc ), dc );
		}
	};

	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	class shared_handle_ptr : public boost::shared_ptr< void >
	{
	public:
		///
		shared_handle_ptr() : shared_ptr() {}
		///
		shared_handle_ptr( HANDLE handle ) : shared_ptr( handle, ReleaseHandle ) {}
		///
		static void ReleaseHandle( HANDLE handle )
		{
			if ( handle != 0 ) CloseHandle( handle );
		}
	};



	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	class shared_gdiobj_ptr : public boost::shared_ptr< void >
	{
	public:
		///
		shared_gdiobj_ptr() : shared_ptr() {}
		///
		shared_gdiobj_ptr( HANDLE handle ) : shared_ptr( handle, ReleaseGDI ) {}
		///
		static void ReleaseGDI( HANDLE handle )
		{
			if ( handle != 0 ) DeleteObject( reinterpret_cast< HGDIOBJ >( handle ) );
		}
	};
	//-------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------
	class shared_sysstring_ptr : public boost::shared_ptr< void >
	{
	public:
		///
		shared_sysstring_ptr() : shared_ptr() {}
		///
		shared_sysstring_ptr( BSTR handle ) : shared_ptr( handle, ReleaseSysString ) {}
		///
		operator BSTR() { return reinterpret_cast<BSTR>( this->get() ); }
		static void ReleaseSysString( BSTR s )
		{
			if ( s != 0 ) SysFreeString( s );
		}
	};

	class shared_variant_ptr : public boost::shared_ptr< VARIANT >
	{
	public:
		///
		shared_variant_ptr() : shared_ptr() {}
		///
		shared_variant_ptr( VARIANT* v ) : shared_ptr( v, VariantClear ) { VariantInit( v ); }
	};
#endif
}

#endif//Z3D_PTR_HPP
