#ifndef QSE_TEMPLATES_HPP
#define QSE_TEMPLATES_HPP

//---------------------------------------------------------------------------------------------------------------------
// CSingleton template manages setting/resettting global variables.
//---------------------------------------------------------------------------------------------------------------------

namespace Quicksand
{

	template <class CT>
	class CSingleton
	{
		T m_OldValue;
		T* m_pGlobalValue;

	public:
		CSingleton( T newValue, T* globalValue )
		{
			m_pGlobalValue = globalValue;
			m_OldValue = *globalValue;
			*m_pGlobalValue = newValue;
		}

		virtual ~CSingleton() { *m_pGlobalValue = m_OldValue; }
	};


	//---------------------------------------------------------------------------------------------------------------------
	// [rez] Tr1 / Boost smart pointers make me sad.  In order to dereference a weak_ptr, you have to cast it to a 
	// shared_ptr first.  You still have to check to see if the pointer is dead dead by calling expired() on the weak_ptr, 
	// so why not just allow the weak_ptr to be dereferenceable?  It doesn't buy anything to force this extra step because 
	// you can still cast a dead weak_ptr to a shared_ptr and crash.  Nice.  Anyway, this function takes some of that 
	// headache away.
	//---------------------------------------------------------------------------------------------------------------------
	template <class CType>
	shared_ptr<Type> MakeStrongPtr( weak_ptr<Type> pWeakPtr )
	{
		if (!pWeakPtr.expired())
			return shared_ptr<Type>( pWeakPtr );
		else
			return shared_ptr<Type>();
	}


	//////////////////////////////////////////////////////////////////////////////
	// COptional.h
	//
	// An isolation point for optionality, provides a way to define
	// objects having to provide a special "null" state.
	//
	// In short:
	//
	// struct COptional<T>
	// {
	//     bool m_bValid;
	//
	//	   T	m_data;
	// };
	//
	//

#include <new>

	class COptional_empty { };

	template <unsigned long size>
	class COptional_base
	{
	public:
		// Default - invalid.

		COptional_base() : m_bValid( false ) { }

		COptional_base & operator = (COptional_base const & t)
		{
			m_bValid = t.m_bValid;
			return *this;
		}

		//Copy constructor
		COptional_base( COptional_base const & other )
			: m_bValid( other.m_bValid )  { }

		//utility functions
		bool const valid() const		{ return m_bValid; }
		bool const invalid() const		{ return !m_bValid; }

	protected:
		bool m_bValid;
		char m_data[size];  // storage space for T
	};

	template <class CT>
	class COptional : public COptional_base<sizeof(T)>
	{
	public:
		// Default - invalid.

		COptional()	 {    }
		COptional( T const & t )  { construct( t ); m_bValid = (true); }
		COptional( COptional_empty const & ) {	}

		COptional & operator = (T const & t)
		{
			if (m_bValid)
			{
				*GetT() = t;
			}
			else
			{
				construct( t );
				m_bValid = true;	// order important for exception safety.
			}

			return *this;
		}

		//Copy constructor
		COptional( COptional const & other )
		{
			if (other.m_bValid)
			{
				construct( *other );
				m_bValid = true;	// order important for exception safety.
			}
		}

		COptional & operator = (COptional const & other)
		{
			QSE_ASSERT( !(this == &other) );	// don't copy over self!
			if (m_bValid)
			{						// first, have to destroy our original.
				m_bValid = false;	// for exception safety if destroy() throws.
				// (big trouble if destroy() throws, though)
				destroy();
			}

			if (other.m_bValid)
			{
				construct( *other );
				m_bValid = true;	// order vital.

			}
			return *this;
		}


		bool const operator == (COptional const & other) const
		{
			if ((!valid()) && (!other.valid())) { return true; }
			if (valid() ^ other.valid()) { return false; }
			return ((** this) == (*other));
		}

		bool const operator < (COptional const & other) const
		{
			// equally invalid - not smaller.
			if ((!valid()) && (!other.valid()))   { return false; }

			// I'm not valid, other must be, smaller.
			if (!valid())	{ return true; }

			// I'm valid, other is not valid, I'm larger
			if (!other.valid()) { return false; }

			return ((** this) < (*other));
		}

		~COptional() { if (m_bValid) destroy(); }

		// Accessors.

		T const & operator * () const			{ QSE_ASSERT( m_bValid ); return *GetT(); }
		T & operator * ()						{ QSE_ASSERT( m_bValid ); return *GetT(); }
		T const * const operator -> () const	{ QSE_ASSERT( m_bValid ); return GetT(); }
		T		* const operator -> ()			{ QSE_ASSERT( m_bValid ); return GetT(); }

		//This clears the value of this COptional variable and makes it invalid once again.
		void clear()
		{
			if (m_bValid)
			{
				m_bValid = false;
				destroy();
			}
		}

		//utility functions
		bool const valid() const		{ return m_bValid; }
		bool const invalid() const		{ return !m_bValid; }

	private:


		T const * const GetT() const { return reinterpret_cast<T const * const>(m_data); }
		T * const GetT()			 { return reinterpret_cast<T * const>(m_data); }
		void construct( T const & t )  { new (GetT()) T( t ); }
		void destroy() { GetT()->~T(); }
	};


	template <class CBaseType, class CSubType>
	BaseType* GenericObjectCreationFunction( void ) { return new SubType; }

	template <class CBaseClass, class IdType>
	class CGenericObjectFactory
	{
		typedef BaseClass* (*ObjectCreationFunction)(void);
		std::map<IdType, ObjectCreationFunction> m_creationFunctions;

	public:
		template <class CSubClass>
		bool Register( IdType id )
		{
			auto findIt = m_creationFunctions.find( id );
			if (findIt == m_creationFunctions.end())
			{
				m_creationFunctions[id] = &GenericObjectCreationFunction<BaseClass, SubClass>;  // insert() is giving me compiler errors
				return true;
			}

			return false;
		}

		BaseClass* Create( IdType id )
		{
			auto findIt = m_creationFunctions.find( id );
			if (findIt != m_creationFunctions.end())
			{
				ObjectCreationFunction pFunc = findIt->second;
				return pFunc();
			}

			return NULL;
		}
	};

}

#endif