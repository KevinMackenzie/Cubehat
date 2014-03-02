#ifndef QSE_CRITICALSECTION_HPP
#define QSE_CRITICALSECTION_HPP


namespace Quicksand
{
	//some classes for good threadsafe multiprogramming

	class CCriticalSection : public CQSE_noncopyable
	{
	public:
		CCriticalSection() { InitializeCriticalSection( &m_cs ); }
		~CCriticalSection(){ DeleteCriticalSection( &m_cs ); }
		void Lock()		  { EnterCriticalSection( &m_cs ); }
		void Unlock()	  { LeaveCriticalSection( &m_cs ); }
	protected:
		mutable CRITICAL_SECTION m_cs;
	};



	class CScopedCriticalSection : public CQSE_noncopyable
	{
	public:
		CScopedCriticalSection( CCriticalSection& csResource )
			: m_csResource( csResource )
			{m_csResource.Lock();}

		~CScopedCriticalSection(){ m_csResource.Unlock(); }

	private:
		CCriticalSection &m_csResource;
	};



}

#endif