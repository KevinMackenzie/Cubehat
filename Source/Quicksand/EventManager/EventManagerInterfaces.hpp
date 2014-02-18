#ifndef EVENTMANAGERINTERFACES_HPP
#define EVENTMANAGERINTERFACES_HPP

//for strstream classes
#include <strstream>
#include "../3rdParty/FastDelegate/FastDelegate.h"
#include "../Multicore/CriticalSection.hpp"
#include "../Utilities/Templates.hpp"
//TODO:
//#include "../Debugging/MemoryWatcher.hpp"

namespace Quicksand
{
	class IEventData;

	typedef unsigned long EventType;
	typedef shared_ptr<IEventData> IEventDataPtr;
	typedef fastdelegate::FastDelegate1<IEventDataPtr> EventListenerDelegate;
	//typedef concurrent_queue<IEventDataPtr> ThreadSafeEventQueue;

	//define's for adding events
	extern GenericObjectFactory<IEventData, EventType> g_eventFactory;
	#define REGISTER_EVENT(eventClass) g_eventFactory.Register<eventClass>(eventClass::sk_EventType)
	#define CREATE_EVENT(eventType) g_eventFactory.Create(eventType)

	//a base data class for event data passed into events.
	//this data interface contains to data at all ( except of course the virtual method table)
	class IEventData
	{
	public:
		//OVERRIDES
		virtual ~IEventData( void ){}
		virtual const EventType& VGetEventType( void ) const = 0;
		virtual float GetTimeStamp( void ) const = 0;
		virtual void VSerialize( std::ostrstream& out ) const = 0;
		virtual void VDeserialize( std::istrstream& in ) = 0;
		virtual IEventDataPtr VCopy( void ) const = 0;
		virtual const char* GetName( void ) const = 0;

		//QSE_MEMORY_WATCHER_DECLARATION()

	};


	class BaseEventData : public IEventData
	{
		const float m_fTimeStamp;

	public:
		//make this explicit to avoid any further overloading of this constructor
		explicit BaseEventData( const float fTimeStamp = 0.0f ) : m_fTimeStamp( fTimeStamp ){}
		

		//return the type of event
		virtual const EventType& VGetEventType( void ) const = 0;

		float GetTimeStamp( void ) const { return m_fTimeStamp; }

		//for serializing across networks
		//TODO: make this something more secure than string streams, but
		// it it good for debugging
		virtual void VSerialize( std::ostrstream &out ) const {}
		virtual void VDeserialize( std::istrstream& in ) {}

	};


	//this is an object that maintains a list of registered events
	//and their liseners.  There can be any number of receivers or listeners for each event
	//Since we are using shared_ptr, once this manager is destroyed, all objects will
	//be destroyed automatically except for outstanding references.
	class IEventManager
	{
	public:

		enum eConstants { kINFINITE = 0xffffffff };

		explicit IEventManager( const char* pName, bool bSetAsGlobal );
		virtual ~IEventManager( void );

		//registers a delegate fnction that will be called when the event type is triggered
		//the return value is the success of the registration
		virtual bool VAddListener( const EventListenerDelegate& eventDelegate, const EventType& type ) = 0;

		//removes the listner with the type of eventType and the delegate eventDelegate
		//returns whether the event removal was succesful(maybe the event pair doesn't exist)
		virtual bool VRemoveListener( const EventListenerDelegate& eventDelegate, const EventType& type ) = 0;

		// Fire off event NOW.  This bypasses the queue entirely and immediately calls all delegate functions registered 
		// for the event.
		virtual bool VTriggerEvent( const IEventDataPtr& pEvent ) const = 0;

		//this fires off the event with the data pEvent
		virtual bool VQueueEvent(const IEventDataPtr& pEvent) = 0;
		virtual bool VThreadSafeQueueEvent(const IEventDataPtr& pEvent) = 0;//similar to above, but is thread-safe

		//finds the next availible event of type type, and if allOfType is true, it wipes
		//all events of that type
		//returns whether the event was found and removed.  
		//the difference between this and removelistener, is this removes it only from the queue
		virtual bool VAbortEvent( const EventType& type, bool allOfType = false ) = 0;

		//allow processing for any queued messaging.  The provided time is used if we have
		// a limited amount of time to process the queue
		//default is infinite
		virtual bool VUpdate( const long maxMS = kINFINITE ) = 0;


		// Getter for the main global event manager.  This is the event manager that is used by the majority of the 
		// engine, though you are free to define your own as long as you instantiate it with setAsGlobal set to false.
		// It is not valid to have more than one global event manager.
		static IEventManager* Get( void );
	};


}


#endif