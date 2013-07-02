/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
| HISTORY:     2013.03.23 - Created                                           |
|-----------------------------------------------------------------------------|
| TODO:		                                                                  |
\-----------------------------------------------------------------------------/
*/
#pragma once

#ifndef ATOM_UTIL_WACCEL_HPP
#define ATOM_UTIL_WACCEL_HPP
#if defined(_WIN32)
//
#include <windows.h>
//
#include <vector>
//
namespace atom {

	class accel {
	public:
		//
		accel() : accels(), accel_table( NULL ) {
		}
		//
		~accel() {
			free_table();
		}
		//
		void add_accel( WORD cmd, bool alt, bool ctrl, bool shift, bool virtkey, WORD key ) {
			this->add_accel( cmd, ((alt)?(FALT):(0)) | ((ctrl)?(FCONTROL):(0)) | ((shift)?(FSHIFT):(0)) | ((virtkey)?(FVIRTKEY):(0)), key );
		}
		//
		void add_accel( WORD cmd, BYTE virt, WORD key ) {
			ACCEL a;
			LPACCEL pa = &a;
			for ( size_t i = 0; i < this->accels.size(); ++i ) {
				if ( this->accels[i].cmd == cmd ) {
					pa = &( this->accels[i] );
					break;
				}
			}
			//
			pa->fVirt	=	virt;
			pa->key		=	key;
			pa->cmd		=	cmd;
			//
			if ( pa == &a ) {
				this->accels.push_back( a );
			}
		}
		//
		void build() {
			this->free_table();
			this->accel_table = CreateAcceleratorTable( &(this->accels[0] ), this->accels.size() );
		}
		//
		bool translate( HWND hWnd, LPMSG lpMsg ) {
			if ( this->accel_table != NULL ) {
				return ( TranslateAccelerator( hWnd, this->accel_table, lpMsg ) != 0 );
			}
			return false;
		}
	protected:
		//
		void free_table() {
			if ( this->accel_table != NULL ) {
				DestroyAcceleratorTable( this->accel_table );
			}
			this->accel_table = NULL;
		}
	private:
		//
		typedef std::vector< ACCEL >
			accels_t;
		//
		accels_t
			accels;
		//
		HACCEL
			accel_table;
	};
}

#endif

#endif
