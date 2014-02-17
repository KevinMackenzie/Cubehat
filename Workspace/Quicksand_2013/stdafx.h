#ifndef STDAFX_H
#define STDAFX_H

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Windows Header Files:

#define NOMINMAX
#include <Windows.h>
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
#include <memory>
#include <tchar.h>

#include <mmsystem.h>

#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <map>


//link the static version of GLEW
#define GLEW_STATIC

//this lets us expose the native types for GLFW example, get the HWND of a GLFWwindow
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32

//these defines have to be at the top for some of the include files to use

#if defined(_DEBUG)
#	define QSE_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define QSE_NEW new
#endif


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

//OpenGL Header files
#include "../../Source/Quicksand/3rdParty/glew-1.10.0/include/GL/glew.h"
#include "../../Source/Quicksand/3rdParty/glfw-3.0.4/include/GLFW/glfw3.h"
#include "../../Source/Quicksand/3rdParty/glfw-3.0.4/include/GLFW/glfw3native.h"


//glm include files
#include "../../Source/Quicksand/3rdParty/glm-0.9.5.2/glm/glm.hpp"
#include "../../Source/Quicksand/3rdParty/glm-0.9.5.2/glm/gtc/quaternion.hpp"
#include "../../Source/Quicksand/3rdParty/glm-0.9.5.2/glm/gtx/quaternion.hpp"
#include "../../Source/Quicksand/3rdParty/glm-0.9.5.2/glm/gtc/matrix_transform.hpp"


namespace Quicksand
{
	//this needs to be before several of the include files
	using std::shared_ptr;
	using std::weak_ptr;
	using std::static_pointer_cast;
	using std::dynamic_pointer_cast;
}

#include "../../Source/Quicksand/3rdParty/tinyxml-2.6.2/tinyxml.h"

// fast delegate stuff 
#include "../../Source/Quicksand/3rdParty/FastDelegate/FastDelegate.h"

// Quicksand #includes
#include "../../Source/Quicksand/Debugging/Logger.hpp"  // this should be the first of the QSE includes since it defines QSE_ASSERT()
#include "../../Source/Quicksand/Utilities/Types.hpp"
#include "../../Source/Quicksand/Utilities/Templates.hpp"
#include "../../Source/Quicksand/Utilities/String.hpp"
#include "../../Source/Quicksand/Utilities/Interfaces.hpp"
#include "../../Source/Quicksand/Game/QuicksandGame.hpp"

//#include "../../Source/Quicksand/Graphics3D/geometry.h"



namespace Quicksand
{

	//to be less of a pain
	using glm::vec3;
	using glm::vec4;
	using glm::vec2;
	using glm::mat4;
	using glm::mat3;
	using glm::quat;

	class QSE_noncopyable
	{
	private:
		QSE_noncopyable( const QSE_noncopyable& x );
		QSE_noncopyable& operator=(const QSE_noncopyable& x);
	public:
		QSE_noncopyable() {}; // Default constructor  
	};



	using fastdelegate::MakeDelegate;

#pragma warning( disable : 4996 ) // 'function' declared deprecated - gets rid of all those 2005 warnings....




	typedef vec4 Color;

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

	extern vec3 g_Up;
	extern vec3 g_Right;
	extern vec3 g_Forward;

	extern vec4 g_Up4;
	extern vec4 g_Right4;
	extern vec4 g_Forward4;



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

}






extern INT WINAPI QuicksandProc( HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow );

#endif