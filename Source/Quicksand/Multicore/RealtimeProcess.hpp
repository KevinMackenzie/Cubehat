#ifndef QSE_REALTIMEPROCESS_HPP
#define QSE_REALTIMEPROCESS_HPP

#include "../MainLoop/Process.hpp"

namespace Quicksand
{
	//a class Cthat allows realtime processes(process that do not need VOnUpdate() to run)

	class CRealtimeProcess : public CProcess
	{
	protected:
		HANDLE m_hThread;
		DWORD m_ThreadID;
		int m_ThreadPriority;
	public:
		//Other priority types are 
		//THREAD_PRIORITY_ABOVE_NORMAL
		//THREAD_PRIORITY_BELOW_NORMAL
		//THREAD_PRIORITY_HIGHEST
		//THREAD_PRIORITY_TIME_CRITICAL
		//THREAD_PRIORITY_LOWEST
		//THREAD_PRIORITY_IDLE

		CRealtimeProcess( int priority = THREAD_PRIORITY_NORMAL );

		virtual ~CRealtimeProcess(){ CloseHandle( m_hThread ); }
		static DWORD WINAPI ThreadProc( LPVOID lpParam );
	protected:
		virtual void VOnInit();
		virtual void VOnpdate( unsigned long deltaMs ){}
		virtual void VThreadProc( void ) = 0;
	};


}



#endif