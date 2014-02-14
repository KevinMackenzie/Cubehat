#ifndef PROCESS_HPP
#define PROCESS_HPP


namespace Quicksand
{



/////////////////////////////////////////////
//	class CProcess
//
//	a class representing a single process
//	to be exictued in the game loop.
//	all uses of this must be derived, then 
//	used in CProcessManager
//


//start with some smart pointer typedef's
class CProcess;
typedef shared_ptr<CProcess> StrongProcessPtr;
typedef weak_ptr<CProcess>   WeakProcessPtr;

class CProcess
{
	//we want the process manager to access this class
	friend class CProcessManager;

public:
	enum State
	{
		//Process types that are neither dead or alive
		UNINITIALIZED = 0,	//Created, but not running
		REMOVED,			//Removed from the process list, but not destroyed
		//This can happen when this process is parent to another process

		//Living Process types
		RUNNING,			//self explanitory(alive and running)
		PAUSED,				//initialized but paused

		//Dead Process types
		SUCCEEDED,			//completely Successfully (everything that succeeds must DIE)
		FAILED,				//failed to complete
		ABORTED				//Aborted, but may not have started (it was in the queue, but never ran)
	};


private:

	State m_State;//the state of the process if any
	StrongProcessPtr m_pChild;//the child process(that will be queued after this process completes)
	//This allows for chains of processes that might be useful in animations, or complex ai decisions


public:

	//construction
	CProcess( void );
	virtual ~CProcess( void );

protected:

	//interface functions that should be overrided by child classes
	virtual void VOnInit( void ){}
	virtual void VOnUpdated( unsigned long deltaMs ) = 0;//this is the only one required to overwrite
	virtual void VOnSuccess( void ){}
	virtual void VOnFail( void ){}
	virtual void VOnAbort( void ){}

public:
	//functions that will end this process
	inline void Succeed( void );
	inline void Fail( void );

	//pause
	inline void Pause( void );
	inline void UnPause( void );

	//accessors
	State GetState( void )	const { return m_State; }
	bool IsAlive( void ) 	const { return (m_State == RUNNING || m_State == PAUSED); }
	bool IsDead( void )		const { return (m_State == SUCCEEDED || m_State == FAILED || m_State == ABORTED); }

	//functions for attaching child processes
	inline void AttachChild( StrongProcessPtr pChild );
	StrongProcessPtr RemoveChild( void );	//Releases ownership of the child
	StrongProcessPtr PeekChild( void ) const	{ return m_pChild; }//does not release ownership of the child

private:
	void SetState( State newState ) { m_State = newState; }
};

inline void CProcess::Succeed( void )
{
	QSE_ASSERT( m_State == RUNNING || m_State == PAUSED );
	m_State = SUCCEEDED;
}

inline void CProcess::Fail( void )
{
	QSE_ASSERT( m_State == RUNNING || m_State == PAUSED );
	m_State = FAILED;
}

inline void CProcess::AttachChild( StrongProcessPtr pChild )
{
	if (m_pChild)
		m_pChild->AttachChild( pChild );
	else
		m_pChild = pChild;
}

inline void CProcess::Pause( void )
{
	if (m_State == RUNNING)
		m_State = PAUSED;
	else
		QSE_WARNING( "Attempting to pause a process that isn't running" );
}

inline void CProcess::UnPause( void )
{
	if (m_State == PAUSED)
		m_State = RUNNING;
	else
		QSE_WARNING( "Attempting to unpause a process that isn't paused" );
}


}

#endif