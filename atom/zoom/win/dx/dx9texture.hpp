/*
/-----------------------------------------------------------------------------\
| Copyright © 2008-2013 by Vladyslav Kurmaz.                                  |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladislav Kurmaz                                               |
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_ZOOM_DX9TEXTURE_HPP
#define ATOM_ZOOM_DX9TEXTURE_HPP
#pragma once

namespace atom { namespace zoom {

	class dx9texture :
		public texture,
		protected dx9holder {
	public:
		///
		static dx9texture_ptr create( logger_ptr l, stream_ptr s, dx9_ptr d ) {
			return dx9texture_ptr( new dx9texture( l, s, d ) );
		}
		///
		virtual ~dx9texture();
		///
		virtual bool
		build( string_t const& s );
		///
		virtual void
		clear();

	protected:
		///
		dx9texture( logger_ptr l, stream_ptr s, dx9_ptr d );

	private:
		///
		IDirect3DTexture9*
			tex;
	};

} }

#endif //ATOM_ZOOM_DX9TEXTURE_HPP
