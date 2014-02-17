#include "stdafx.h"
#include "Process.hpp"

namespace Quicksand
{
	CProcess::CProcess( void )
	{
		m_State = UNINITIALIZED;
	}

	CProcess::~CProcess( void )
	{
		if (m_pChild)
		{
			m_pChild->VOnAbort();
		}
	}


	void CProcess::Succeed( void )
	{
		QSE_ASSERT( m_State == RUNNING || m_State == PAUSED );
		m_State = SUCCEEDED;
	}

	void CProcess::Fail( void )
	{
		QSE_ASSERT( m_State == RUNNING || m_State == PAUSED );
		m_State = FAILED;
	}

	void CProcess::AttachChild( StrongProcessPtr pChild )
	{
		if (m_pChild)
			m_pChild->AttachChild( pChild );
		else
			m_pChild = pChild;
	}

	void CProcess::Pause( void )
	{
		if (m_State == RUNNING)
			m_State = PAUSED;
		else
			QSE_WARNING( "Attempting to pause a process that isn't running" );
	}

	void CProcess::UnPause( void )
	{
		if (m_State == PAUSED)
			m_State = RUNNING;
		else
			QSE_WARNING( "Attempting to unpause a process that isn't paused" );
	}

	StrongProcessPtr CProcess::RemoveChild( void )
	{
		if (m_pChild)
		{
			StrongProcessPtr pChild = m_pChild;  // this keeps the child from getting destroyed when we clear it
			m_pChild.reset( );
			//pChild->SetParent(NULL);
			return pChild;
		}

		return StrongProcessPtr( );
	}
}