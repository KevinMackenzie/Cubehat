#include "stdafx.h"
#include "EventManager.hpp"

namespace Quicksand
{
	//implimentation of the CQuicksandEventManager class

	CQuicksandEventManager::CQuicksandEventManager( bool bSetAsGlobal ) : IEventManager( "CQuicksandEventManager", bSetAsGlobal )
	{
		m_activeQueue = 0;
	}

	CQuicksandEventManager::~CQuicksandEventManager( void )
	{
		//nothing to do
	}


	bool CQuicksandEventManager::VAddListener( const EventListenerDelegate& eventDelegate, const EventType& type )
	{
		QSE_LOG( "Events", "Attempting to add delegate function for event type: " + ToStr( type, 16 ) );

		EventListenerList& eventListenerList = m_eventListeners[type];  // this will find or create the entry
		for (auto it = eventListenerList.begin( ); it != eventListenerList.end( ); ++it)
		{
			if (eventDelegate == (*it))
			{
				QSE_WARNING( "Attempting to double-register a delegate" );
				return false;
			}
		}

		eventListenerList.push_back( eventDelegate );
		QSE_LOG( "Events", "Successfully added delegate for event type: " + ToStr( type, 16 ) );

		return true;
	}

	bool CQuicksandEventManager::VRemoveListener( const EventListenerDelegate& eventDelegate, const EventType& type )
	{
		QSE_LOG( "Events", "Attempting to remove delegate function from event type: " + ToStr( type, 16 ) );
		bool success = false;

		auto findIt = m_eventListeners.find( type );
		if (findIt != m_eventListeners.end( ))
		{
			EventListenerList& listeners = findIt->second;
			for (auto it = listeners.begin( ); it != listeners.end( ); ++it)
			{
				if (eventDelegate == (*it))
				{
					listeners.erase( it );
					QSE_LOG( "Events", "Successfully removed delegate function from event type: " + ToStr( type, 16 ) );
					success = true;
					break;  // we don't need to continue because it should be impossible for the same delegate function to be registered for the same event more than once
				}
			}
		}

		return success;
	}

	bool CQuicksandEventManager::VTriggerEvent( const IEventDataPtr& pEvent ) const
	{
		QSE_LOG( "Events", "Attempting to trigger event " + std::string( pEvent->GetName( ) ) );
		bool processed = false;

		auto findIt = m_eventListeners.find( pEvent->VGetEventType( ) );
		if (findIt != m_eventListeners.end( ))
		{
			const EventListenerList& eventListenerList = findIt->second;
			for (EventListenerList::const_iterator it = eventListenerList.begin( ); it != eventListenerList.end( ); ++it)
			{
				EventListenerDelegate listener = (*it);
				QSE_LOG( "Events", "Sending Event " + std::string( pEvent->GetName( ) ) + " to delegate." );
				listener( pEvent );  // call the delegate
				processed = true;
			}
		}

		return processed;
	}

	bool CQuicksandEventManager::VAbortEvent( const EventType& inType, bool allOfType )
	{
		QSE_ASSERT( m_activeQueue >= 0 );
		QSE_ASSERT( m_activeQueue < EVENTMANAGER_NUM_QUEUES );

		bool success = false;
		EventListenerMap::iterator findIt = m_eventListeners.find( inType );

		if (findIt != m_eventListeners.end( ))
		{
			EventQueue& eventQueue = m_queues[m_activeQueue];
			auto it = eventQueue.begin( );
			while (it != eventQueue.end( ))
			{
				// Removing an item from the queue will invalidate the iterator, so have it point to the next member.  All
				// work inside this loop will be done using thisIt.
				auto thisIt = it;
				++it;

				if ((*thisIt)->VGetEventType( ) == inType)
				{
					eventQueue.erase( thisIt );
					success = true;
					if (!allOfType)
						break;
				}
			}
		}

		return success;
	}

	//process the event queue, and do all that fun stuff
	bool CQuicksandEventManager::VUpdate( const long maxMS )
	{
		unsigned long currMs = GetTickCount( );
		unsigned long maxMs = ((maxMS == IEventManager::kINFINITE) ? (IEventManager::kINFINITE) : (currMs + maxMS));

		//TODO: create the realtime event queue, but impliment the memsafe stuff first
		// This section added to handle events from other threads.
		/*IEventDataPtr pRealtimeEvent;
		while (m_realtimeEventQueue.try_pop( pRealtimeEvent ))
		{
			VQueueEvent( pRealtimeEvent );

			currMs = GetTickCount( );
			if (maxMS != IEventManager::kINFINITE)
			{
				if (currMs >= maxMs)
				{
					QSE_ERROR( "A realtime process is spamming the event manager!" );
				}
			}
		}*/

		// swap active queues and clear the new queue after the swap
		int queueToProcess = m_activeQueue;
		m_activeQueue = (m_activeQueue + 1) % EVENTMANAGER_NUM_QUEUES;
		m_queues[m_activeQueue].clear( );

		QSE_LOG( "EventLoop", "Processing Event Queue " + ToStr( queueToProcess ) + "; " + ToStr( (unsigned long)m_queues[queueToProcess].size( ) ) + " events to process" );

		// Process the queue
		while (!m_queues[queueToProcess].empty( ))
		{
			// pop the front of the queue
			IEventDataPtr pEvent = m_queues[queueToProcess].front( );
			m_queues[queueToProcess].pop_front( );
			QSE_LOG( "EventLoop", "\t\tProcessing Event " + std::string( pEvent->GetName( ) ) );

			const EventType& eventType = pEvent->VGetEventType( );

			// find all the delegate functions registered for this event
			auto findIt = m_eventListeners.find( eventType );
			if (findIt != m_eventListeners.end( ))
			{
				const EventListenerList& eventListeners = findIt->second;
				QSE_LOG( "EventLoop", "\t\tFound " + ToStr( (unsigned long)eventListeners.size( ) ) + " delegates" );

				// call each listener
				for (auto it = eventListeners.begin( ); it != eventListeners.end( ); ++it)
				{
					EventListenerDelegate listener = (*it);
					QSE_LOG( "EventLoop", "\t\tSending event " + std::string( pEvent->GetName( ) ) + " to delegate" );
					listener( pEvent );
				}
			}

			// check to see if time ran out
			currMs = GetTickCount( );
			if (maxMS != IEventManager::kINFINITE && currMs >= maxMs)
			{
				QSE_LOG( "EventLoop", "Aborting event processing; time ran out" );
				break;
			}
		}

		// If we couldn't process all of the events, push the remaining events to the new active queue.
		// Note: To preserve sequencing, go back-to-front, inserting them at the head of the active queue
		bool queueFlushed = (m_queues[queueToProcess].empty( ));
		if (!queueFlushed)
		{
			while (!m_queues[queueToProcess].empty( ))
			{
				IEventDataPtr pEvent = m_queues[queueToProcess].back( );
				m_queues[queueToProcess].pop_back( );
				m_queues[m_activeQueue].push_front( pEvent );
			}
		}

		return queueFlushed;
	}

	bool CQuicksandEventManager::VQueueEvent( const IEventDataPtr& pEvent )
	{
		QSE_ASSERT( m_activeQueue >= 0 );
		QSE_ASSERT( m_activeQueue < EVENTMANAGER_NUM_QUEUES );

		// make sure the event is valid
		if (!pEvent)
		{
			QSE_ERROR( "Invalid event in VQueueEvent()" );
			return false;
		}

		QSE_LOG( "Events", "Attempting to queue event: " + std::string( pEvent->GetName( ) ) );

		auto findIt = m_eventListeners.find( pEvent->VGetEventType( ) );
		if (findIt != m_eventListeners.end( ))
		{
			m_queues[m_activeQueue].push_back( pEvent );
			QSE_LOG( "Events", "Successfully queued event: " + std::string( pEvent->GetName( ) ) );
			return true;
		}
		else
		{
			QSE_LOG( "Events", "Skipping event since there are no delegates registered to receive it: " + std::string( pEvent->GetName( ) ) );
			return false;
		}
	}

	bool CQuicksandEventManager::VThreadSafeQueueEvent( const IEventDataPtr& pEvent )
	{
		//m_realtimeEventQueue.push( pEvent );
		return true;
	}


}