#ifndef QSE_PROCESSMANAGER_HPP
#define QSE_PROCESSMANAGER_HPP

#include "Process.hpp"

namespace Quicksand
{

class CProcessManager
{
	typedef std::list<StrongProcessPtr> ProcessList;

	ProcessList m_ProcessList;//store a list of the queued processes

public:
	//construction
	CProcessManager( void );
	~CProcessManager( void );

	// interface
	unsigned int UpdateProcesses( unsigned long deltaMs );//goes through list of processes and updates them
	WeakProcessPtr AttachProcess( StrongProcessPtr pProcess );//attaches another process to be called at next update
	void AbortAllProcesses( bool immediate );//abort all of the current processes.  If immediate == true, then kill them now
											 //if not, then kill them during the call to UpdateProcesses

	//accessors
	unsigned int GetProcessCount( void ) const { return m_ProcessList.size(); }

private:
	void ClearAllProcesses( void );//should only be called by the destructer 

};

}


#endif