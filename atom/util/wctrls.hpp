/*
/-----------------------------------------------------------------------------\
| Copyright © 2015 by Vladyslav Kurmaz.                                       |
| All Rights Reserved                                                         |
| vladislav.kurmaz@gmail.com                                                  |
|-----------------------------------------------------------------------------|
| DESCRIPTION:                                                                |
| AUTHOR:      Vladyslav Kurmaz                                               |
|-----------------------------------------------------------------------------|
\-----------------------------------------------------------------------------/
*/

#ifndef ATOM_UTIL_WCTRLS_HPP
#define ATOM_UTIL_WCTRLS_HPP
#if defined(_WIN32)
//
#include <vector>
//
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
//
#include <vector>

#define RECT_WIDTH( r ) ( (r).right - (r).left )
#define RECT_HEIGHT( r ) ( (r).bottom - (r).top )

namespace atom {

	//---------------------------------------------------------------------------
	class wImageList {
	public:
		//
		wImageList() : imageList( NULL ) {}
		//
		~wImageList(){
			ImageList_Destroy( imageList );
		}
		//
/*
		wImageList& create( int const cx, int const cy, UINT const flags, int const cInitial, int const cGrow ) {
			this->imageList = ImageList_Create( cx, cy, flags, cInitial, cGrow );
			return (*this);
		}
		wImageList& create( atom::string_t const& file, int const cx, int const cGrow, COLORREF mask ) {
			//this->imageList = ImageList_Create( cx, cy, flags, cInitial, cGrow );
			this->imageList = ImageList_LoadImage( NULL, file.c_str(), cx, cGrow, mask, IMAGE_BITMAP, LR_LOADFROMFILE );
			return (*this);
		}
*/
		wImageList& create(HINSTANCE hi, LPCTSTR lpbmp, int cx, int cGrow, COLORREF crMask) {
			this->imageList = ImageList_LoadBitmap(hi, lpbmp, cx, cGrow, crMask);
			return (*this);
		}
		wImageList& append(HBITMAP hbmp, COLORREF crMask, int& index) {
			ImageList_SetBkColor(this->imageList, crMask);
			index = ImageList_AddMasked(this->imageList, hbmp, crMask);
			return (*this);
		}
		//
		HIMAGELIST getImageList() const {
			return ( this->imageList );
		}

 	protected:

	private:
		///
		HIMAGELIST imageList;
	};
	//---------------------------------------------------------------------------
	class wctrl {
	public:
		//
		wctrl() : wnd( NULL ) {	}
		//
		explicit wctrl( HWND h ) : wnd( h ) {}
		//
		~wctrl(){}
		//
		HWND getHWND()  const { return this->wnd; }
		//
		wctrl const& show() const { ShowWindow( this->wnd, SW_SHOW ); return (*this); }
		//
		wctrl const& hide() const { ShowWindow( this->wnd, SW_HIDE ); return (*this); }
		//
		wctrl const& show( bool const s ) const { if ( s ) { this->show(); } else { this->hide(); }; return (*this); }
		///
		bool isVisible() const { return ((IsWindowVisible(this->wnd))?(true):(false)); }
		///
		wctrl const& invalidate() const { InvalidateRect( this->wnd, NULL, TRUE ) ; return (*this); }

	protected:
		//
		HWND	wnd;

	private:
	};

	//---------------------------------------------------------------------------
	class wctrlListView : public wctrl {
	public:
		//
		wctrlListView() : wctrl() {}
		//
		~wctrlListView(){}
		//
		wctrlListView& create( DWORD const styleEx, DWORD const style, RECT const& rect, DWORD const id, wctrl const& parent ) {
			this->wnd = CreateWindowEx(
				styleEx,
				WC_LISTVIEW, 
				NULL,
				style,
				rect.left,
				rect.top,
				RECT_WIDTH( rect ),
				RECT_HEIGHT( rect ),
				parent.getHWND(),
				(HMENU)id,
				(HINSTANCE)GetModuleHandle( NULL ),
				NULL); 
			return (*this);
		}
		//
		wctrlListView const& updateColumn( bool const insert, int const iCol, int const format, int const cx, atom::string_t const& text, int const subItem ) const {
			LVCOLUMN lvc = { 0 };
			TCHAR txt[ 1024 ] = { 0 };
			wcsncpy( txt, text.c_str(), 1024 );
			//
			lvc.mask		=	
				((format)?(LVCF_FMT):(0)) |
				((cx>0)?(LVCF_WIDTH):(0)) |
				((wcslen(txt))?(LVCF_TEXT):(0)) |
				((subItem<0)?(0):(LVCF_SUBITEM));
			lvc.fmt			=	format;
			lvc.cx			=	cx;
			lvc.pszText		=	txt;
			//lvc.cchTextMax	=	;
			lvc.iSubItem	=	subItem;
			//lvc.iImage		=	;
			//lvc.iOrder		=	;
			//lvc.cxMin		=	;
			//lvc.cxDefault	=	;
			//lvc.cxIdeal		=	;
			if ( insert ) {
				ListView_InsertColumn( this->getHWND(), iCol, &lvc );
			} else {
				ListView_SetColumn( this->getHWND(), iCol, &lvc );
			}

			return (*this);
		}
		
		int getItemCount() const {
			return ListView_GetItemCount( this->getHWND() );
		}

		void deleteAllItems() const {
			ListView_DeleteAllItems( this->getHWND() );
		}

		wctrlListView const& insertItem( int const item, int const subItem, atom::string_t const& text ) const {
			LVITEM lvI = { 0 };
			TCHAR txt[ 1024 ] = { 0 };
			wcsncpy( txt, text.c_str(), 1024 );
			//
			lvI.mask		=	LVIF_TEXT;
			lvI.iItem		=	item;
			lvI.iSubItem	=	subItem;
			//lvI.state		=	;
			//lvI.stateMask	=	;
			lvI.pszText		=	txt;
			//lvI.cchTextMax	=	;
			//lvI.iImage		=	;
			//lvI.lParam		=	;
			//lvI.iIndent		=	;
			//lvI.iGroupId	=	;
			//lvI.cColumns	=	;
			//lvI.puColumns	=	;
			//lvI.piColFmt	=	;
			//lvI.iGroup		=	;
			ListView_InsertItem( this->getHWND(), &lvI );

			return (*this);
		}
		wctrlListView const& setItem( int const item, int const subItem, atom::string_t const& text ) const {
			LVITEM lvI = { 0 };
			TCHAR txt[ 1024 ] = { 0 };
			wcsncpy( txt, text.c_str(), 1024 );
			//
			lvI.mask		=	LVIF_TEXT;
			lvI.iItem		=	item;
			lvI.iSubItem	=	subItem;
			//lvI.state		=	;
			//lvI.stateMask	=	;
			lvI.pszText		=	txt;
			//lvI.cchTextMax	=	;
			//lvI.iImage		=	;
			//lvI.lParam		=	;
			//lvI.iIndent		=	;
			//lvI.iGroupId	=	;
			//lvI.cColumns	=	;
			//lvI.puColumns	=	;
			//lvI.piColFmt	=	;
			//lvI.iGroup		=	;
			ListView_SetItem( this->getHWND(), &lvI );

			return (*this);
		}

	protected:

	private:
	};

	//---------------------------------------------------------------------------
	class wctrlToolbar : public wctrl {
	public:
		//
		wctrlToolbar() : wctrl(), buttons() {}
		//
		~wctrlToolbar(){}
		//
		wctrlToolbar& create( DWORD const styleEx, DWORD const style, RECT const& rect, DWORD const id, wImageList const& imageList, wctrl const& parent ) {
			this->wnd = CreateWindowEx(
				styleEx,
				TOOLBARCLASSNAME, 
				NULL,
				style,
				rect.left,
				rect.top,
				RECT_WIDTH( rect ),
				RECT_HEIGHT( rect ),
				parent.getHWND(),
				(HMENU)id,
				(HINSTANCE)GetModuleHandle( NULL ),
				NULL);
			SendMessage( this->getHWND(), TB_SETPARENT, (WPARAM)parent.getHWND(), 0 );
			SendMessage( this->getHWND(), TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
			SendMessage( this->getHWND(), TB_SETEXTENDEDSTYLE, 0, TBSTYLE_EX_DRAWDDARROWS );
			SendMessage( this->getHWND(), TB_SETIMAGELIST, 0, (LPARAM)imageList.getImageList() );
			return (*this);
		}
		//
		wctrlToolbar& addButton( int const bitmap, int const cmd, BYTE const state, BYTE const style, DWORD_PTR data, atom::string_t const& text ) {
			TBBUTTON btn = { 0 };
			//
			btn.iBitmap		=	bitmap;
			btn.idCommand	=	cmd;
			btn.fsState		=	state;
			btn.fsStyle		=	style;
			btn.dwData		=	data;
			btn.iString		=	((text.length())?((INT_PTR)text.c_str()):(NULL));
			//
			this->buttons.push_back( btn );

			return (*this);
		}
		//
		wctrlToolbar& updateButtonImage( unsigned int const btnId, int const bitmapIndex ) {
			TBBUTTONINFO tbInfo = { 0 };
			tbInfo.cbSize  = sizeof(TBBUTTONINFO);
			tbInfo.dwMask  = TBIF_IMAGE;
			tbInfo.iImage = bitmapIndex;
			SendMessage( this->getHWND(), TB_SETBUTTONINFO, (WPARAM)btnId, (LPARAM)&tbInfo );
			return (*this);
		}
		//
		wctrlToolbar& build() {
			SendMessage( this->getHWND(), TB_ADDBUTTONS, this->buttons.size(), (LPARAM)this->buttons.data());
			return (*this);
		}
		//
		wctrlToolbar& setButtonSize( int const cx, int const cy ) {
			SendMessage( this->getHWND(), TB_SETBUTTONSIZE, 0, MAKELPARAM( cx, cy ) );
			return (*this);
		}
		//
		wctrlToolbar& setIndent(unsigned int const indent) {
			SendMessage(this->getHWND(), TB_SETINDENT, indent, 0);
			return (*this);
		}
		///
		bool isBottonChecked(int const command) const {
			return (SendMessage(this->getHWND(), TB_GETSTATE, command, 0) & TBSTATE_CHECKED);
		}
		
	protected:

	private:
		typedef std::vector< TBBUTTON >
			buttons_t;
		buttons_t buttons;
	};
	//---------------------------------------------------------------------------
	class wmenu {
	public:
		//
		wmenu() : menu( NULL ) {}
		//
		~wmenu(){
			DestroyMenu( menu );
		}
		//
		wmenu& create() {
			menu = CreatePopupMenu();
			return (*this);
		}
		//
		wmenu& appendItem( UINT flags, UINT_PTR id, atom::string_t const& text ) {
			AppendMenu( this->menu, flags, id, text.c_str() );
			return (*this);
		}
		//
		UINT popup( UINT flags, int x, int y, wctrl const& ctrl, RECT const& exclude ) {
			TPMPARAMS tpm;
			tpm.cbSize    = sizeof(TPMPARAMS);
			tpm.rcExclude = exclude;
			//
			return TrackPopupMenuEx(
				this->menu, 
				flags, 
				x, y,
				ctrl.getHWND(),
				&tpm );
		}

 	protected:

	private:
		///
		HMENU menu;
	};
}


#endif
#endif // ATOM_UTIL_WCTRLS_HPP
