#ifndef STDAFX_H
#define STDAFX_H

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Windows Header Files:

#define NOMINMAX
#include <windows.h>
#include <windowsx.h>

#include <crtdbg.h>

#define _VS2005_
#if _MSC_VER < 1400
#undef _VS2005_
#endif

// [rez] I am saddened that Visual Studio 2010 doesn't support the new C++0x final keyword.  It does support 
// the override keyword as well as a special Microsoft-specific sealed keyword.  The override keyword works 
// exactly as the C++0x spec says it does and the sealed keyword acts like the C++0x final keyword.  I toyed 
// around with whether or not I wanted to include this so I will leave the decision up to you.  If you want 
// to use the new C++0x style of override & final, just uncomment this line.  This works on Visual Studio 2008 
// and up for sure.  I'm not sure about earlier versions.
//#define final sealed


// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <mmsystem.h>

#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>



//OpenGL Header files
#include "../../Source/Quicksand/3rdParty/glew-1.10.0/include/GL/glew.h"
#include "../../Source/Quicksand/3rdParty/glfw-3.0.4/include/GLFW/glfw3.h"


//glm include files
#include "../../Source/Quicksand/3rdParty/glm-0.9.5.2/glm/glm.hpp"
#include "../../Source/Quicksand/3rdParty/glm-0.9.5.2/glm/gtc/quaternion.hpp"
#include "../../Source/Quicksand/3rdParty/glm-0.9.5.2/glm/gtx/quaternion.hpp"
#include "../../Source/Quicksand/3rdParty/glm-0.9.5.2/glm/gtc/matrix_transform.hpp"

namespace Quicksand
{
	using std::shared_ptr;
	using std::weak_ptr;
	using std::static_pointer_cast;
	using std::dynamic_pointer_cast;

	class QSE_noncopyable
	{
	private:
		QSE_noncopyable( const QSE_noncopyable& x );
		QSE_noncopyable& operator=(const QSE_noncopyable& x);
	public:
		QSE_noncopyable( ) {}; // Default constructor  
	};


	// Game Code Complete - Chapter 12, page 446-447
#if defined(_DEBUG)
#	define QSE_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define QSE_NEW new
#endif


//#include <tinyxml.h> TODO:

	// fast delegate stuff TODO:
	//#include "../3rdParty/FastDelegate/FastDelegate.h"
	//using fastdelegate::MakeDelegate;

#pragma warning( disable : 4996 ) // 'function' declared deprecated - gets rid of all those 2005 warnings....


	// GameCode #includes
#include "..\Debugging\Logger.h"  // this should be the first of the QSE includes since it defines QSE_ASSERT()
#include "..\Utilities\types.h"
#include "..\Utilities\templates.h"
#include "..\Graphics3D\geometry.h"


	extern Color g_White;
	extern Color g_Black;
	extern Color g_Cyan;
	extern Color g_Red;
	extern Color g_Green;
	extern Color g_Blue;
	extern Color g_Yellow;
	extern Color g_Gray40;
	extern Color g_Gray25;
	extern Color g_Gray65;
	extern Color g_Transparent;

	extern Vec3 g_Up;
	extern Vec3 g_Right;
	extern Vec3 g_Forward;

	extern Vec4 g_Up4;
	extern Vec4 g_Right4;
	extern Vec4 g_Forward4;



	//  AppMsg				- Chapter 9, page 248

	struct AppMsg
	{
		HWND m_hWnd;
		UINT m_uMsg;
		WPARAM m_wParam;
		LPARAM m_lParam;
	};



	// Useful #defines

	extern const float fOPAQUE;
	extern const int iOPAQUE;
	extern const float fTRANSPARENT;
	extern const int iTRANSPARENT;

	extern const int MEGABYTE;
	extern const float SIXTY_HERTZ;

	extern const int SCREEN_WIDTH;
	extern const int SCREEN_HEIGHT;

#if !defined(SAFE_DELETE)
#define SAFE_DELETE(x) if(x) delete x; x=NULL;
#endif

#if !defined(SAFE_DELETE_ARRAY)
#define SAFE_DELETE_ARRAY(x) if (x) delete [] x; x=NULL; 
#endif

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE(x) if(x) x->Release(); x=NULL;
#endif

#ifdef UNICODE
#define _tcssprintf wsprintf
#define tcsplitpath _wsplitpath
#else
#define _tcssprintf sprintf
#define tcsplitpath _splitpath
#endif

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "




};

extern INT WINAPI QuicksandProc( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow );

#endif