The WifiPadServer software is released under the GPL3 license.
See LICENSE.txt for details.

This source code distribution includes 3rd party software code for convenience of building the software. The 3rd party code is located in the libs/ directory and includes portions of the boost, libpng, zlib, wtl code. Please read their respective README and license files for more information.

NOTICE, a modified version of ATL from the Microsoft Platform SDK 2003 r2 is required to build the WifiPad Server on Microsoft Visual Studio Express. Due to unclear licensing, ATL is not bundled with the source code. To get ATL, please download the Microsoft Platform SDK 2003 r2:

http://www.microsoft.com/downloads/details.aspx?familyid=0BAF2B35-C656-4969-ACE8-E4C0C0716ADB&displaylang=en

Copy the headers in the ATL directory of the installed SDK to libs/atl/ and make the modifications as suggested by this website:

http://www.codeproject.com/KB/wtl/WTLExpress.aspx

However, instead of doing:

-- snip --
#define AllocStdCallThunk() HeapAlloc(GetProcessHeap(), 
                              0, sizeof(_stdcallthunk))
#define FreeStdCallThunk(p) HeapFree(GetProcessHeap(), 0, p)
-- snip --

Do this instead:

-- snip --
static inline void* __stdcall AllocStdCallThunk()
{
LPVOID mem = VirtualAlloc(0, sizeof(_stdcallthunk), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
if (!mem) return 0;
return mem;
}

static inline void __stdcall FreeStdCallThunk(void* ptr)
{
if (!ptr) return;
VirtualFree(ptr, 0, MEM_RELEASE);
}
-- snip --

A patch is included in the atl directory which can be applied to the ATL header code. I thank whomever that forum user who provided that code.

Enjoy!
