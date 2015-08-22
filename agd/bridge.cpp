#include "./pch.hpp"
#include "./bridge.hpp"


bridge::bridge(COORD const& sz) :
consoleSize(sz)
#ifdef STANDALONE
,	sharedmemName( TEST_SHAREDMEM_NAME )
,	pipeName( TEST_PIPE_NAME )
#else
, sharedmemName(gen_uuid())
, pipeName(gen_uuid())
#endif
, proc()
, sharedmem()
, sharedmemRegion()
, sharedmemBuffer(0)
, pipe() {
}

bridge::bridge(COORD const& sz, atom::string_t const& shname, atom::string_t const& pname) :
consoleSize(sz)
, sharedmemName(shname)
, pipeName(pname)
, proc()
, sharedmem()
, sharedmemRegion()
, sharedmemBuffer(0)
, pipe() {
}

bridge::~bridge() {
}

void bridge::join() {
	this->pipe.close();
	this->proc.join();
}

void bridge::console() {
	//
	if (sharedmemConfigure(true)) {
		if (this->pipe.create(this->pipeName)) {
#ifndef STANDALONE
			// start child console process
			TCHAR path[MAX_PATH] = { 0 };
			TCHAR drive[MAX_PATH] = { 0 };
			TCHAR dir[MAX_PATH] = { 0 };
			TCHAR filename[MAX_PATH] = { 0 };
			TCHAR ext[MAX_PATH] = { 0 };
			GetModuleFileName(NULL, path, MAX_PATH);
			_tsplitpath_s(path, drive, dir, filename, ext);
			_tmakepath_s(path, drive, dir, _T("proxy"), ext);
			atom::stringstream_t ss;
			ss << _T("\"") << path << _T("\" --pipe-name ") << this->pipeName << _T(" --sharedmem-name ") << this->sharedmemName;
			this->proc.run(ss.str(), this->consoleSize.X, this->consoleSize.Y, true, false);
#endif
			this->pipe.connect();
		}
	}
}

void vk2cons(WORD const vk) {
	//SHORT vk1 = VkKeyScanEx( 'a', GetKeyboardLayout( GetCurrentThreadId() ) );
	//SHORT vk2 = VkKeyScanEx( 'A', GetKeyboardLayout( GetCurrentThreadId() ) );
	//
	INPUT_RECORD ir[2] = { 0 };
	//
	ir[0].EventType = KEY_EVENT;
	ir[0].Event.KeyEvent.bKeyDown = TRUE;
	ir[0].Event.KeyEvent.wRepeatCount = 1;
	ir[0].Event.KeyEvent.wVirtualKeyCode = vk;
	ir[0].Event.KeyEvent.wVirtualScanCode = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
	BYTE kbrd[256] = { 0 };
	WORD c = 0;
	//GetKeyboardState( kbrd );
	ToAscii(
		ir[0].Event.KeyEvent.wVirtualKeyCode,
		ir[0].Event.KeyEvent.wVirtualScanCode,
		kbrd,
		&c,
		0
		);
	ir[0].Event.KeyEvent.uChar.UnicodeChar = c;
	//ir[0].Event.KeyEvent.uChar.AsciiChar	=	MapVirtualKey( vk, MAPVK_VK_TO_CHAR );
	ir[0].Event.KeyEvent.dwControlKeyState =
		((kbrd[VK_CAPITAL] & 0x01) ? (CAPSLOCK_ON) : (0)) |
		//ENHANCED_KEY
		((kbrd[VK_LMENU] & 0x80) ? (LEFT_ALT_PRESSED) : (0)) |
		((kbrd[VK_LCONTROL] & 0x80) ? (LEFT_CTRL_PRESSED) : (0)) |
		((kbrd[VK_NUMLOCK] & 0x01) ? (NUMLOCK_ON) : (0)) |
		((kbrd[VK_RMENU] & 0x80) ? (RIGHT_ALT_PRESSED) : (0)) |
		((kbrd[VK_RCONTROL] & 0x80) ? (RIGHT_CTRL_PRESSED) : (0)) |
		((kbrd[VK_SCROLL] & 0x01) ? (SCROLLLOCK_ON) : (0)) |
		((kbrd[VK_SHIFT] & 0x80) ? (SHIFT_PRESSED) : (0));

	//
	ir[1] = ir[0];
	ir[1].Event.KeyEvent.bKeyDown = FALSE;

	//	HKL WINAPI GetKeyboardLayout(
	//  _In_  DWORD idThread
	//);

	//
	DWORD wr = 0;
	WriteConsoleInput(GetStdHandle(STD_INPUT_HANDLE), ir, sizeof(ir) / sizeof(ir[0]), &wr);
}
void exit2cons() {
	vk2cons(VK_RETURN);
	vk2cons('E');
	vk2cons('X');
	vk2cons('I');
	vk2cons('T');
	vk2cons(VK_RETURN);
}

void bridge::proxy() {
	this->proc.run( atom::string_t(_T("cmd.exe")), 0, 0, false, true);
	// read loop from pipe
	if (sharedmemConfigure(false)) {
		if (this->pipe.open(this->pipeName)) {
			bool cont = true;
			bridgeMsg m;
			INPUT_RECORD ir;
			DWORD wr = 0;
			while (cont) {
				memset(&m, 0, sizeof(m));
				if (cont = this->pipe.read(&m, sizeof(m))) {
					switch (m.id) {
					case bridgeMsg::bmSize:
						break;
					case bridgeMsg::bmKbrd:
					{
						ir.EventType = KEY_EVENT;
						ir.Event.KeyEvent = m.key;
						WriteConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &ir, 1, &wr);
						//
						//memset( this->sharedmemBuffer, 0, this->getSharedmemSize() );

						//
						COORD rsize = { 0 };
						SMALL_RECT rview = { 0 };

						getConsoleSize(rsize, rview);
						//
						COORD	sz = this->consoleSize;
						//
						SHORT cx = std::min(sz.X, rsize.X) - 1;
						SHORT cy = std::min(sz.Y, rsize.Y);
						SMALL_RECT src = { 0, 0, cx, 0 };
						COORD	lt = { 0 };
						//
						for (SHORT i = 0; i < cy; i++) {
							if (ReadConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), this->sharedmemBuffer, sz, lt, &src)) {
							}
							else {
								std::cout << "error: " << GetLastError() << std::endl;
							}
							src.Top++;
							src.Bottom++;
							lt.Y++;
						}
						break;
					}
					case bridgeMsg::bmCtrlBreak:
						//GenerateConsoleCtrlEvent( CTRL_BREAK_EVENT, 0 );
						break;
					case bridgeMsg::bmCtrlC:
						//GenerateConsoleCtrlEvent( CTRL_C_EVENT, 0 );
						break;
					case bridgeMsg::bmExit:
						exit2cons();
						break;
					}
				}
			}
		}
	}
}

void  bridge::getLines(lines_t& lines){
	bool add = false;
	for (SHORT i = this->consoleSize.Y - 1; i >= 0; i--) {
		atom::char_t s[255] = { 0 };
		for (SHORT j = 0; j < this->consoleSize.X; j++) {
			s[j] = this->sharedmemBuffer[i * this->consoleSize.X + j].Char
#ifdef UNICODE
				.UnicodeChar
#else
				.AsciiChar
#endif
				;
			if (!add && s[j] && (s[j] != ' ')) {
				add = true;
			}
		}
		if (add) {
			lines.push_front(s);
		}
	}
}

void bridge::write(KEY_EVENT_RECORD const& k) {
	bridgeMsg m;
	m.id = bridgeMsg::bmKbrd;
	m.key = k;
	this->pipe.write(&m, sizeof(m));
}

void bridge::writeExit(){
	bridgeMsg m;
	m.id = bridgeMsg::bmExit;
	this->pipe.write(&m, sizeof(m));
}

bool bridge::sharedmemConfigure(bool const create){
	size_t const size = this->getSharedmemSize();
	if (create) {
		this->sharedmem = sharedmem_t(
			new boost::interprocess::windows_shared_memory(
			boost::interprocess::create_only,
			atom::s2s<std::string>(this->sharedmemName).c_str(),
			boost::interprocess::read_write,
			size)
			);
	}
	else {
		this->sharedmem = sharedmem_t(
			new boost::interprocess::windows_shared_memory(
			boost::interprocess::open_only,
			atom::s2s<std::string>(this->sharedmemName).c_str(),
			boost::interprocess::read_write)
			);
	}
	this->sharedmemRegion = sharedmem_region_t(
		new boost::interprocess::mapped_region(
		*(this->sharedmem.get()),
		boost::interprocess::read_write)
		);
	//
	this->sharedmemBuffer = static_cast<CHAR_INFO*>(this->sharedmemRegion->get_address());
	if (create && this->sharedmemBuffer) {
		memset(this->sharedmemBuffer, 0, size);
	}
	return true;
}
