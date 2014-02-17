#include "stdafx.h"
#include "ProcessManager.hpp"



using namespace Quicksand;

CProcessManager::CProcessManager( void )
{

}

CProcessManager::~CProcessManager( void )
{
	ClearAllProcesses();
}

unsigned int CProcessManager::UpdateProcesses( unsigned long deltaMs )
{
	unsigned short int successCount = 0;
	unsigned short int failCount = 0;

	ProcessList::iterator it = m_ProcessList.begin();
	while (it != m_ProcessList.end())
	{
		//Get the value from the iterator
		StrongProcessPtr ptr = (*it);

		//use this and increment the old one just in case we need to remove it from the list
		ProcessList::iterator thisIt = it;
		++it;

		//if the process is uninitialized, then initialize it
		if (ptr->GetState() == CProcess::UNINITIALIZED)
			ptr->VOnInit();


		//the the process is running, update it
		if (ptr->GetState() == CProcess::RUNNING)
			ptr->VOnUpdated( deltaMs );

		
		//if the process is dead, then remove it and call the appropriate exit function
		if (ptr->IsDead())
		{
			switch (ptr->GetState())
			{
			case CProcess::SUCCEEDED:
			{
										ptr->VOnSuccess();
										StrongProcessPtr pChild = ptr->RemoveChild();
										if (pChild)
											AttachProcess( pChild );
										else
											++successCount;//this only counts if the chain has succeeded
										break;
			}
			case CProcess::ABORTED:
				ptr->VOnAbort();

				++failCount;

				break;
			case CProcess::FAILED:
				ptr->VOnFail();

				++failCount;

				break;
			}

			//the object is dead, so remove it from this list
			m_ProcessList.erase( thisIt );
		}
	}

	return ((successCount << 16) | !failCount);
}


WeakProcessPtr CProcessManager::AttachProcess( StrongProcessPtr pProcess )
{
	m_ProcessList.push_back( pProcess );
	return WeakProcessPtr( pProcess );
}


void CProcessManager::AbortAllProcesses( bool immediate )
{
	ProcessList::iterator it = m_ProcessList.begin();
	while (it != m_ProcessList.end())
	{
		//in case we need to delete 'it'
		ProcessList::iterator thisIt = it;
		++it;

		StrongProcessPtr pCurrent = (*thisIt);

		if (immediate)
		{
			(*it)->SetState( CProcess::ABORTED );
			if (pCurrent->IsAlive())
			{
				(*it)->VOnAbort();
				m_ProcessList.erase( thisIt );
			}
		}
	}
}

void CProcessManager::ClearAllProcesses( void )
{
	//NOTE: Runs NO exit code
	m_ProcessList.clear();
}