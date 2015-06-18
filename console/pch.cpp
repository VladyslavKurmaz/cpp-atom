#include "./pch.hpp"

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Comctl32.lib")

// https://connect.microsoft.com/PowerShell/feedback/details/572313/powershell-exe-can-hang-if-stdin-is-redirected
// http://en.wikipedia.org/wiki/ANSI_escape_code
// http://support.microsoft.com/default.aspx?scid=kb;EN-US;q190351&wa=wsignin1.0
//SetWinEventHook.
// http://stackoverflow.com/questions/2175665/createprocess-process-do-not-terminate-when-redirecting-std-out-in-err

#ifdef STANDALONE
const std::string TEST_PIPE_NAME		= "TEST_PIPE";
const std::string TEST_SHAREDMEM_NAME	= "TEST_SHAREDMEM";
#endif

const TCHAR CMDID_SPLIT_NAME[]		= _T( "split" );
const TCHAR CMDID_EXPAND_NAME[]		= _T( "expand" );
const TCHAR CMDID_ROTATE_NAME[]		= _T( "rotate" );
const TCHAR CMDID_NEXT_NAME[]		= _T( "next" );
const TCHAR CMDID_PREV_NAME[]		= _T( "prev" );

const conf_cmd_t conf_cmds[] = {
	{ CMDID_SPLIT_NAME,		CMDID_SPLIT,	_T("split current frame") },
	{ CMDID_EXPAND_NAME,	CMDID_EXPAND,	_T("maximize/restore frame") },
	{ CMDID_ROTATE_NAME,	CMDID_ROTATE,	_T("rotate frames") },
	{ CMDID_NEXT_NAME,		CMDID_NEXT,		_T("focus next frame") },
	{ CMDID_PREV_NAME,		CMDID_PREV,		_T("focus prev frame") },
};
const size_t conf_cmds_cnt = sizeof( conf_cmds ) / sizeof( conf_cmds[0] );

const atom::char_t DELIM1[] = _T( ";" );
const atom::char_t DELIM2[] = _T( ":" );

int atom::node_cnt = 0;

atom::string_t gen_uuid() {
	GUID guid;
	CoCreateGuid( &guid );

	OLECHAR* bstr;
	::StringFromCLSID( guid, &bstr );
	atom::string_t s = std::wstring( bstr );
	::CoTaskMemFree( bstr );
	return ( s );
}

bool getConsoleSize( COORD& size, SMALL_RECT& view ) {
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo = { 0 };
	if ( GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbiInfo ) ) {
		size = csbiInfo.dwSize;
		view = csbiInfo.srWindow;
		return true;
	}
	return false;
}

/*
void window::processAutostart() {
//Registry.SetValue(@"HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run",
//          "MyStartUp",
//          @"C:\StartUpApp.exe");
}

*/

bool bitmapSave( atom::string_t const& file, HDC hDC, HBITMAP hBmp ) {
	bool result = false;

	BITMAP bmp; 
	PBITMAPINFO pbmi; 
	WORD    cClrBits; 
	// Retrieve the bitmap color format, width, and height.  
	if ( GetObject( hBmp, sizeof(BITMAP), (LPSTR)&bmp) ) { 
		// Convert the color format to a count of bits.  
		cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
		if (cClrBits == 1) 
			cClrBits = 1; 
		else if (cClrBits <= 4) 
			cClrBits = 4; 
		else if (cClrBits <= 8) 
			cClrBits = 8; 
		else if (cClrBits <= 16) 
			cClrBits = 16; 
		else if (cClrBits <= 24) 
			cClrBits = 24; 
		else cClrBits = 32; 

		// Allocate memory for the BITMAPINFO structure. (This structure  
		// contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
		// data structures.)  
		size_t infoSize = sizeof(BITMAPINFOHEADER); 
		if ( cClrBits < 24 ) {
			infoSize += sizeof(RGBQUAD) * (1<< cClrBits);
		}
		pbmi = (PBITMAPINFO)alloca( infoSize ); 

		// Initialize the fields in the BITMAPINFO structure.  
		pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
		pbmi->bmiHeader.biWidth = bmp.bmWidth; 
		pbmi->bmiHeader.biHeight = bmp.bmHeight; 
		pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
		pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
		if (cClrBits < 24) 
			pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

		// If the bitmap is not compressed, set the BI_RGB flag.  
		pbmi->bmiHeader.biCompression = BI_RGB; 

		// Compute the number of bytes in the array of color  
		// indices and store the result in biSizeImage.  
		// The width must be DWORD aligned unless the bitmap is RLE 
		// compressed. 
		pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
			* pbmi->bmiHeader.biHeight; 
		// Set biClrImportant to 0, indicating that all of the  
		// device colors are important.  
		pbmi->bmiHeader.biClrImportant = 0;
		//
		//
		HANDLE hf;                 // file handle  
		BITMAPFILEHEADER hdr;       // bitmap file-header  
		PBITMAPINFOHEADER pbih;     // bitmap info-header  
		LPBYTE lpBits;              // memory pointer  
		DWORD dwTotal;              // total count of bytes  
		DWORD cb;                   // incremental count of bytes  
		BYTE *hp;                   // byte pointer  
		DWORD dwTmp; 

		pbih = (PBITMAPINFOHEADER) pbmi; 
		char* p = new char [pbih->biSizeImage];
		lpBits = (LPBYTE) p;
		//lpBits = (LPBYTE) alloca( pbih->biSizeImage );

		if ( lpBits ) { 
			// Retrieve the color table (RGBQUAD array) and the bits  
			// (array of palette indices) from the DIB.  
			if ( GetDIBits(hDC, hBmp, 0, (WORD) pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS)) {

				// Create the .BMP file.  
				hf = CreateFile( file.c_str(), 
					GENERIC_READ | GENERIC_WRITE, 
					(DWORD) 0, 
					NULL, 
					CREATE_ALWAYS, 
					FILE_ATTRIBUTE_NORMAL, 
					(HANDLE) NULL); 
				if ( hf != INVALID_HANDLE_VALUE ) { 
					hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
					// Compute the size of the entire file.  
					hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage); 
					hdr.bfReserved1 = 0; 
					hdr.bfReserved2 = 0; 

					// Compute the offset to the array of color indices.  
					hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof (RGBQUAD); 

					// Copy the BITMAPFILEHEADER into the .BMP file.  
					if (WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp,  NULL)) {
						// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.  
						if (WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof (RGBQUAD), (LPDWORD) &dwTmp, ( NULL))) {
							// Copy the array of color indices into the .BMP file.  
							dwTotal = cb = pbih->biSizeImage; 
							hp = lpBits; 
							if (WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL)) {
								// Close the .BMP file.  
								if (CloseHandle(hf) ) { 
									result = true;
								}
							}
						}
					}
				}
			}
		}
		delete [] p;
	}
	return result;
}

#if 0
BOOL WINAPI bridge::handler( DWORD dwCtrlType ) {
	switch ( dwCtrlType ){
	case CTRL_C_EVENT:{
		return TRUE;
					  }
	case CTRL_BREAK_EVENT:{
		return TRUE;
						  }
	case CTRL_CLOSE_EVENT:{
		return TRUE;
						  }
	case CTRL_LOGOFF_EVENT:{
		return TRUE;
						   }
	case CTRL_SHUTDOWN_EVENT:{
		return TRUE;
							 }
	}
	return FALSE;
}


#include <strsafe.h>
void ErrorExit(LPTSTR lpszFunction) 
{ 
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
	StringCchPrintf((LPTSTR)lpDisplayBuf, 
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"), 
		lpszFunction, dw, lpMsgBuf); 
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}
#endif