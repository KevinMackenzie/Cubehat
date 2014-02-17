#include "stdafx.h"
#include "RealtimeProcess.hpp"

using namespace Quicksand;

RealtimeProcess::RealtimeProcess( int priority )
{
	m_ThreadID = 0;
	m_ThreadPriority = priority;
}




DWORD WINAPI RealtimeProcess::ThreadProc( LPVOID lpParam )
{
	RealtimeProcess *proc = static_cast<RealtimeProcess*>(lpParam);
	proc->VThreadProc();
	return TRUE;
}

void RealtimeProcess::VOnInit()
{
	CProcess::VOnInit();

	m_hThread = CreateThread(
		NULL,			//default security attributes
		0,				//default stack size
		ThreadProc,		//thread process
		this,			//thread parameter is a pointer to this process
		0,				//default creation flags
		&m_ThreadID );	//receive thread identifier

	//make sure the thread was created
	if (m_hThread == NULL)
	{
		QSE_ERROR( "Could not create thread!" );
		Fail();
		return;
	}

	//now set the thread priority to the desired one
	SetThreadPriority( m_hThread, m_ThreadPriority );
}