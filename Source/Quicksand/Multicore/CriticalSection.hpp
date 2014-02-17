#ifndef CRITICALSECTION_HPP
#define CRITICALSECTION_HPP


namespace Quicksand
{
	//some classes for good threadsafe multiprogramming

	class CriticalSection : public QSE_noncopyable
	{
	public:
		CriticalSection() { InitializeCriticalSection( &m_cs ); }
		~CriticalSection(){ DeleteCriticalSection( &m_cs ); }
		void Lock()		  { EnterCriticalSection( &m_cs ); }
		void Unlock()	  { LeaveCriticalSection( &m_cs ); }
	protected:
		mutable CRITICAL_SECTION m_cs;
	};



	class ScopedCriticalSection : public QSE_noncopyable
	{
	public:
		ScopedCriticalSection( CriticalSection& csResource )
			: m_csResource( csResource )
			{m_csResource.Lock();}

		~ScopedCriticalSection(){ m_csResource.Unlock(); }

	private:
		CriticalSection &m_csResource;
	};



}

#endif