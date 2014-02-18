#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "EventManagerInterfaces.hpp"

//the number of event queues to use
#define EVENTMANAGER_NUM_QUEUES 2

//the implimentation of the IEventManager interface
namespace Quicksand
{

	class QuicksandEventManager : public IEventManager
	{

		typedef std::list<EventListenerDelegate> EventListenerList;
		typedef std::map<EventType, EventListenerList> EventListenerMap;
		typedef std::list<IEventDataPtr> EventQueue;

		EventListenerMap m_eventListeners;
		EventQueue m_queues[EVENTMANAGER_NUM_QUEUES];
		int m_activeQueue;  // index of actively processing queue; events enque to the opposing queue

		//ThreadSafeEventQueue m_realtimeEventQueue;

	public:
		QuicksandEventManager( bool bSetAsGlobal = true );
		~QuicksandEventManager( void );

		//registers a delegate fnction that will be called when the event type is triggered
		//the return value is the success of the registration
		virtual bool VAddListener( const EventListenerDelegate& eventDelegate, const EventType& type );

		// Fire off event NOW.  This bypasses the queue entirely and immediately calls all delegate functions registered 
		// for the event.
		virtual bool VTriggerEvent( const IEventDataPtr& pEvent ) const;

		//removes the listner with the type of eventType and the delegate eventDelegate
		//returns whether the event removal was succesful(maybe the event pair doesn't exist)
		virtual bool VRemoveListener( const EventListenerDelegate& eventDelegate, const EventType& type);

		//this fires off the event with the data pEvent
		virtual bool VQueueEvent( const IEventDataPtr& pEvent );
		virtual bool VThreadSafeQueueEvent( const IEventDataPtr& pEvent );//similar to above, but is thread-safe

		//finds the next availible event of type type, and if allOfType is true, it wipes
		//all events of that type
		//returns whether the event was found and removed.  
		//the difference between this and removelistener, is this removes it only from the queue
		virtual bool VAbortEvent( const EventType& type, bool allOfType = false );

		//allow processing for any queued messaging.  The provided time is used if we have
		// a limited amount of time to process the queue
		//default is infinite
		virtual bool VUpdate( const long maxMS = kINFINITE );
	};

}


#endif