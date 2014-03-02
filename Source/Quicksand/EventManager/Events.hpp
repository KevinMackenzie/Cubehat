#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "EventManager.hpp"
#include "../Game/QuicksandGame.hpp"
//#include "../LUAScripting/ScriptEvent.h"

//ATTENTION:
//all code in this file has been borrowed from the Game Code Complete Fourth Edition code accompanyment
//it falls under the GNU and lesser GNU liscence

namespace Quicksand
{

	// Auxiliary data decls ...
	//
	// data that is passed per-event in the userData parameter
	// 
	// ( for some, but not all, events )

	//-- new object notification


	//void RegisterEngineScriptEvents( void );


	//---------------------------------------------------------------------------------------------------------------------
	// CEvtData_New_Actor - This event is sent out when an actor is *actually* created.
	//---------------------------------------------------------------------------------------------------------------------
	class CEvtData_New_Actor : public CBaseEventData
	{
		ActorId m_actorId;
		GameViewId m_viewId;

	public:
		static const EventType sk_EventType;

		CEvtData_New_Actor( void )
		{
			m_actorId = INVALID_ACTOR_ID;
			m_viewId = gc_InvalidGameViewId;
		}

		explicit CEvtData_New_Actor( ActorId actorId, GameViewId viewId = gc_InvalidGameViewId )
			: m_actorId( actorId ),
			m_viewId( viewId )
		{
		}

		virtual void VDeserialize( std::istrstream& in )
		{
			in >> m_actorId;
			in >> m_viewId;
		}

		virtual const EventType& VGetEventType( void ) const
		{
			return sk_EventType;
		}

		virtual IEventDataPtr VCopy( void ) const
		{
			return IEventDataPtr( QSE_NEW CEvtData_New_Actor( m_actorId, m_viewId ) );
		}

		virtual void VSerialize( std::ostrstream& out ) const
		{
			out << m_actorId << " ";
			out << m_viewId << " ";
		}


		virtual const char* GetName( void ) const
		{
			return "CEvtData_New_Actor";
		}

		const ActorId GetActorId( void ) const
		{
			return m_actorId;
		}

		GameViewId GetViewId( void ) const
		{
			return m_viewId;
		}
	};


	//---------------------------------------------------------------------------------------------------------------------
	// EvtData_Destroy_Actor - sent when actors are destroyed	
	//---------------------------------------------------------------------------------------------------------------------
	class CEvtData_Destroy_Actor : public CBaseEventData
	{
		ActorId m_id;

	public:
		static const EventType sk_EventType;

		explicit CEvtData_Destroy_Actor( ActorId id = INVALID_ACTOR_ID )
			: m_id( id )
		{
			//
		}

		virtual const EventType& VGetEventType( void ) const
		{
			return sk_EventType;
		}

		virtual IEventDataPtr VCopy( void ) const
		{
			return IEventDataPtr( QSE_NEW CEvtData_Destroy_Actor( m_id ) );
		}

		virtual void VSerialize( std::ostrstream &out ) const
		{
			out << m_id;
		}

		virtual void VDeserialize( std::istrstream& in )
		{
			in >> m_id;
		}

		virtual const char* GetName( void ) const
		{
			return "EvtData_Destroy_Actor";
		}

		ActorId GetId( void ) const { return m_id; }
	};


	//---------------------------------------------------------------------------------------------------------------------
	// EvtData_Move_Actor - sent when actors are moved
	//---------------------------------------------------------------------------------------------------------------------
	class CEvtData_Move_Actor : public CBaseEventData
	{
		ActorId m_id;
		mat4 m_matrix;

	public:
		static const EventType sk_EventType;

		virtual const EventType& VGetEventType( void ) const
		{
			return sk_EventType;
		}

		CEvtData_Move_Actor( void )
		{
			m_id = INVALID_ACTOR_ID;
		}

		CEvtData_Move_Actor( ActorId id, const mat4& matrix )
			: m_id( id ), m_matrix( matrix )
		{
			//
		}

		virtual void VSerialize( std::ostrstream &out ) const
		{
			out << m_id << " ";
			for (int i = 0; i<4; ++i)
			{
				for (int j = 0; j<4; ++j)
				{
					out << m_matrix[i][j] << " ";
				}
			}
		}

		virtual void VDeserialize( std::istrstream& in )
		{
			in >> m_id;
			for (int i = 0; i<4; ++i)
			{
				for (int j = 0; j<4; ++j)
				{
					in >> m_matrix[i][j];
				}
			}
		}

		virtual IEventDataPtr VCopy( ) const
		{
			return IEventDataPtr( QSE_NEW CEvtData_Move_Actor( m_id, m_matrix ) );
		}

		virtual const char* GetName( void ) const
		{
			return "EvtData_Move_Actor";
		}

		ActorId GetId( void ) const
		{
			return m_id;
		}

		const mat4& GetMatrix( void ) const
		{
			return m_matrix;
		}
	};

	//TODO:impliment CGLSceneNode along with all of the other 3dGraphics Api Things
	class CGLSceneNode;

	//---------------------------------------------------------------------------------------------------------------------
	// EvtData_New_Render_Component - This event is sent out when an actor is *actually* created.
	//---------------------------------------------------------------------------------------------------------------------
	class CEvtData_New_Render_Component : public CBaseEventData
	{
		ActorId m_actorId;
		shared_ptr<CGLSceneNode> m_pGLSceneNode;

	public:
		static const EventType sk_EventType;

		CEvtData_New_Render_Component( void )
		{
			m_actorId = INVALID_ACTOR_ID;
		}

		explicit CEvtData_New_Render_Component( ActorId actorId, shared_ptr<CGLSceneNode> pGLSceneNode )
			: m_actorId( actorId ),
			m_pGLSceneNode( pGLSceneNode )
		{
		}

		virtual void VSerialize( std::ostrstream& out ) const
		{
			QSE_ERROR( GetName( ) + std::string( " should not be serialzied!" ) );
		}

		virtual void VDeserialize( std::istrstream& in )
		{
			QSE_ERROR( GetName( ) + std::string( " should not be serialzied!" ) );
		}

		virtual const EventType& VGetEventType( void ) const
		{
			return sk_EventType;
		}

		virtual IEventDataPtr VCopy( void ) const
		{
			return IEventDataPtr( QSE_NEW CEvtData_New_Render_Component( m_actorId, m_pGLSceneNode ) );
		}

		virtual const char* GetName( void ) const
		{
			return "EvtData_New_Render_Component";
		}

		const ActorId GetActorId( void ) const
		{
			return m_actorId;
		}

		shared_ptr<CGLSceneNode> GetGLSceneNode( void ) const
		{
			return m_pGLSceneNode;
		}
	};


	//---------------------------------------------------------------------------------------------------------------------
	// EvtData_Modified_Render_Component - This event is sent out when a render component is changed
	//   NOTE: This class Cis not described in the book!
	//---------------------------------------------------------------------------------------------------------------------
	class CEvtData_Modified_Render_Component : public CBaseEventData
	{
		ActorId m_id;

	public:
		static const EventType sk_EventType;

		virtual const EventType& VGetEventType( void ) const
		{
			return sk_EventType;
		}

		CEvtData_Modified_Render_Component( void )
		{
			m_id = INVALID_ACTOR_ID;
		}

		CEvtData_Modified_Render_Component( ActorId id )
			: m_id( id )
		{
		}

		virtual void VSerialize( std::ostrstream &out ) const
		{
			out << m_id;
		}

		virtual void VDeserialize( std::istrstream& in )
		{
			in >> m_id;
		}

		virtual IEventDataPtr VCopy( ) const
		{
			return IEventDataPtr( QSE_NEW CEvtData_Modified_Render_Component( m_id ) );
		}

		virtual const char* GetName( void ) const
		{
			return "EvtData_Modified_Render_Component";
		}

		ActorId GetActorId( void ) const
		{
			return m_id;
		}
	};



	//---------------------------------------------------------------------------------------------------------------------
	// EvtData_Environment_Loaded - this event is sent when a new game is started
	//---------------------------------------------------------------------------------------------------------------------
	class CEvtData_Environment_Loaded : public CBaseEventData
	{
	public:
		static const EventType sk_EventType;

		CEvtData_Environment_Loaded( void ) { }
		virtual const EventType& VGetEventType( void ) const	{ return sk_EventType; }
		virtual IEventDataPtr VCopy( void ) const
		{
			return IEventDataPtr( QSE_NEW CEvtData_Environment_Loaded( ) );
		}
		virtual const char* GetName( void ) const  { return "EvtData_Environment_Loaded"; }
	};


	//---------------------------------------------------------------------------------------------------------------------
	// EvtData_Environment_Loaded - this event is sent when a client has loaded its environment
	//   This is special because we only want this event to go from client to server, and stop there. The
	//   EvtData_Environment_Loaded is received by server and proxy logics alike. Thy to do this with just the above 
	//   event and you'll get into an endless loop of the EvtData_Environment_Loaded event making infinite round trips
	//   from client to server.
	//
	// FUTURE_WORK: It would be an interesting idea to add a "Private" type of event that is addressed only to a specific 
	//              listener. Of course, that might be a really dumb idea too - someone will have to try it!
	//---------------------------------------------------------------------------------------------------------------------
	class CEvtData_Remote_Environment_Loaded : public CBaseEventData
	{
	public:
		static const EventType sk_EventType;

		CEvtData_Remote_Environment_Loaded( void ) { }
		virtual const EventType& VGetEventType( void ) const	{ return sk_EventType; }
		virtual IEventDataPtr VCopy( void ) const
		{
			return IEventDataPtr( QSE_NEW CEvtData_Remote_Environment_Loaded( ) );
		}
		virtual const char* GetName( void ) const  { return "EvtData_Remote_Environment_Loaded"; }
	};


	//---------------------------------------------------------------------------------------------------------------------
	// EvtData_Request_Start_Game - this is sent by the authoritative game logic to all views so they will load a game level.
	//---------------------------------------------------------------------------------------------------------------------
	class CEvtData_Request_Start_Game : public CBaseEventData
	{

	public:
		static const EventType sk_EventType;

		CEvtData_Request_Start_Game( void ) { }

		virtual const EventType& VGetEventType( void ) const
		{
			return sk_EventType;
		}

		virtual IEventDataPtr VCopy( ) const
		{
			return IEventDataPtr( QSE_NEW CEvtData_Request_Start_Game( ) );
		}

		virtual const char* GetName( void ) const
		{
			return "EvtData_Request_Start_Game";
		}
	};


	/**** HOLY CRAP THIS ISN"T USED ANYMORE????
	//---------------------------------------------------------------------------------------------------------------------
	// EvtData_Game_State - sent whenever the game state is changed (look for "BGS_" to see the different states)
	//---------------------------------------------------------------------------------------------------------------------
	class CEvtData_Game_State : public BaseEventData
	{
	BaseGameState m_gameState;
	std::string m_parameter;

	public:
	static const EventType sk_EventType;

	EvtData_Game_State(void)
	{
	m_gameState = BGS_Invalid;
	}

	explicit EvtData_Game_State(const BaseGameState gameState, const std::string &parameter)
	: m_gameState(gameState), m_parameter(parameter)
	{
	}

	virtual const EventType & VGetEventType( void ) const
	{
	return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
	return IEventDataPtr( QSE_NEW EvtData_Game_State( m_gameState, m_parameter ) );
	}

	virtual void VSerialize(std::ostrstream &out) const
	{
	const int tempVal = static_cast< int >( m_gameState );
	out << tempVal << " ";
	out << m_parameter;
	}

	virtual void VDeserialize(std::istrstream &in)
	{
	int tempVal;
	in >> tempVal;
	m_gameState = static_cast<BaseGameState>( tempVal );
	in >> m_parameter;
	}

	virtual const char* GetName(void) const
	{
	return "EvtData_Game_State";
	}

	BaseGameState GetGameState(void) const
	{
	return m_gameState;
	}

	const std::string GetParameter(void) const
	{
	return m_parameter;
	}
	};
	**********************/


	//---------------------------------------------------------------------------------------------------------------------
	// EvtData_Remote_Client						- Chapter 19, page 687
	// 
	//   Sent whenever a new client attaches to a game logic acting as a server				
	//---------------------------------------------------------------------------------------------------------------------
	class CEvtData_Remote_Client : public CBaseEventData
	{
		int m_socketId;
		int m_ipAddress;

	public:
		static const EventType sk_EventType;

		CEvtData_Remote_Client( void )
		{
			m_socketId = 0;
			m_ipAddress = 0;
		}

		CEvtData_Remote_Client( const int socketid, const int ipaddress )
			: m_socketId( socketid ), m_ipAddress( ipaddress )
		{
		}

		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		virtual IEventDataPtr VCopy( ) const
		{
			return IEventDataPtr( QSE_NEW CEvtData_Remote_Client( m_socketId, m_ipAddress ) );
		}

		virtual const char* GetName( void ) const
		{
			return "EvtData_Remote_Client";
		}

		virtual void VSerialize( std::ostrstream &out ) const
		{
			out << m_socketId << " ";
			out << m_ipAddress;
		}

		virtual void VDeserialize( std::istrstream &in )
		{
			in >> m_socketId;
			in >> m_ipAddress;
		}

		int GetSocketId( void ) const
		{
			return m_socketId;
		}

		int GetIpAddress( void ) const
		{
			return m_ipAddress;
		}
	};


	//---------------------------------------------------------------------------------------------------------------------
	// EvtData_Update_Tick - sent by the game logic each game tick
	//---------------------------------------------------------------------------------------------------------------------
	class CEvtData_Update_Tick : public CBaseEventData
	{
		int m_DeltaMilliseconds;

	public:
		static const EventType sk_EventType;

		explicit CEvtData_Update_Tick( const int deltaMilliseconds )
			: m_DeltaMilliseconds( deltaMilliseconds )
		{
		}

		virtual const EventType& VGetEventType( void ) const
		{
			return sk_EventType;
		}

		virtual IEventDataPtr VCopy( ) const
		{
			return IEventDataPtr( QSE_NEW CEvtData_Update_Tick( m_DeltaMilliseconds ) );
		}

		virtual void VSerialize( std::ostrstream & out )
		{
			QSE_ERROR( "You should not be serializing update ticks!" );
		}

		virtual const char* GetName( void ) const
		{
			return "EvtData_Update_Tick";
		}
	};


	//---------------------------------------------------------------------------------------------------------------------
	// EvtData_Network_Player_Actor_Assignment - sent by the server to the clients when a network view is assigned a player number
	//---------------------------------------------------------------------------------------------------------------------
	class CEvtData_Network_Player_Actor_Assignment : public CBaseEventData
	{
		ActorId m_ActorId;
		int m_SocketId;

	public:
		static const EventType sk_EventType;

		CEvtData_Network_Player_Actor_Assignment( )
		{
			m_ActorId = INVALID_ACTOR_ID;
			m_SocketId = -1;
		}

		explicit CEvtData_Network_Player_Actor_Assignment( const ActorId actorId, const int socketId )
			: m_ActorId( actorId ), m_SocketId( socketId )

		{
		}

		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		virtual IEventDataPtr VCopy( ) const
		{
			return IEventDataPtr( QSE_NEW CEvtData_Network_Player_Actor_Assignment( m_ActorId, m_SocketId ) );
		}

		virtual const char* GetName( void ) const
		{
			return "EvtData_Network_Player_Actor_Assignment";
		}


		virtual void VSerialize( std::ostrstream &out ) const
		{
			out << m_ActorId << " ";
			out << m_SocketId;
		}

		virtual void VDeserialize( std::istrstream &in )
		{
			in >> m_ActorId;
			in >> m_SocketId;
		}

		ActorId GetActorId( void ) const
		{
			return m_ActorId;
		}

		ActorId GetSocketId( void ) const
		{
			return m_SocketId;
		}
	};


	//---------------------------------------------------------------------------------------------------------------------
	// EvtData_Decompress_Request - sent to a multithreaded game event listener to decompress something in the resource file
	//---------------------------------------------------------------------------------------------------------------------
	class CEvtData_Decompress_Request : public CBaseEventData
	{
		std::wstring m_zipFileName;
		std::string m_fileName;

	public:
		static const EventType sk_EventType;

		explicit CEvtData_Decompress_Request( std::wstring zipFileName, std::string filename )
			: m_zipFileName( zipFileName ),
			m_fileName( filename )
		{
		}

		virtual const EventType& VGetEventType( void ) const
		{
			return sk_EventType;
		}

		virtual IEventDataPtr VCopy( ) const
		{
			return IEventDataPtr( QSE_NEW CEvtData_Decompress_Request( m_zipFileName, m_fileName ) );
		}

		virtual void VSerialize( std::ostrstream & out )
		{
			QSE_ERROR( "You should not be serializing decompression requests!" );
		}

		const std::wstring& GetZipFilename( void ) const
		{
			return m_zipFileName;
		}

		const std::string& GetFilename( void ) const
		{
			return m_fileName;
		}
		virtual const char* GetName( void ) const
		{
			return "EvtData_Decompress_Request";
		}
	};


	//---------------------------------------------------------------------------------------------------------------------
	// EvtData_Decompression_Progress - sent by the decompression thread to report progress
	//---------------------------------------------------------------------------------------------------------------------
	class CEvtData_Decompression_Progress : public CBaseEventData
	{
		int m_progress;
		std::wstring m_zipFileName;
		std::string m_fileName;
		void *m_buffer;

	public:
		static const EventType sk_EventType;

		CEvtData_Decompression_Progress( int progress, std::wstring zipFileName, std::string filename, void *buffer )
			: m_progress( progress ),
			m_zipFileName( zipFileName ),
			m_fileName( filename ),
			m_buffer( buffer )
		{
		}

		virtual const EventType & VGetEventType( void ) const
		{
			return sk_EventType;
		}

		virtual IEventDataPtr VCopy( ) const
		{
			return IEventDataPtr( QSE_NEW CEvtData_Decompression_Progress( m_progress, m_zipFileName, m_fileName, m_buffer ) );
		}

		virtual void VSerialize( std::ostrstream & out )
		{
			QSE_ERROR( "You should not be serializing decompression progress events!" );
		}

		virtual const char* GetName( void ) const
		{
			return "EvtData_Decompression_Progress";
		}

	};


	//---------------------------------------------------------------------------------------------------------------------
	// class CEvtData_Request_New_Actor				
	// This event is sent by a server asking Client proxy logics to create new actors from their local resources.
	// It can be sent from script or via code.
	// This event is also sent from the server game logic to client logics AFTER it has created a new actor. The logics will allow follow suit to stay in sync.
	//---------------------------------------------------------------------------------------------------------------------
	class CEvtData_Request_New_Actor : public CBaseEventData
	{
		std::string m_actorResource;
		bool m_hasInitialTransform;
		mat4 m_initialTransform;
		ActorId m_serverActorId;
		GameViewId m_viewId;

	public:
		static const EventType sk_EventType;

		CEvtData_Request_New_Actor( )
		{
			m_actorResource = "";
			m_hasInitialTransform = false;
			m_initialTransform = mat4();
			m_serverActorId = -1;
			m_viewId = gc_InvalidGameViewId;
		}

		explicit CEvtData_Request_New_Actor( const std::string &actorResource, const mat4 *initialTransform = NULL, const ActorId serverActorId = INVALID_ACTOR_ID, const GameViewId viewId = gc_InvalidGameViewId )
		{
			m_actorResource = actorResource;
			if (initialTransform)
			{
				m_hasInitialTransform = true;
				m_initialTransform = *initialTransform;
			}
			else
				m_hasInitialTransform = false;

			m_serverActorId = serverActorId;
			m_viewId = viewId;
		}

		virtual const EventType& VGetEventType( void ) const
		{
			return sk_EventType;
		}

		virtual void VDeserialize( std::istrstream & in )
		{
			in >> m_actorResource;
			in >> m_hasInitialTransform;
			if (m_hasInitialTransform)
			{
				for (int i = 0; i<4; ++i)
				{
					for (int j = 0; j<4; ++j)
					{
						in >> m_initialTransform[i][j];
					}
				}
			}
			in >> m_serverActorId;
			in >> m_viewId;
		}

		virtual IEventDataPtr VCopy( ) const
		{
			return IEventDataPtr( QSE_NEW CEvtData_Request_New_Actor( m_actorResource, (m_hasInitialTransform) ? &m_initialTransform : NULL, m_serverActorId, m_viewId ) );
		}

		virtual void VSerialize( std::ostrstream & out ) const
		{
			out << m_actorResource << " ";
			out << m_hasInitialTransform << " ";
			if (m_hasInitialTransform)
			{
				for (int i = 0; i<4; ++i)
				{
					for (int j = 0; j<4; ++j)
					{
						out << m_initialTransform[i][j] << " ";
					}
				}
			}
			out << m_serverActorId << " ";
			out << m_viewId << " ";
		}

		virtual const char* GetName( void ) const { return "EvtData_Request_New_Actor"; }

		const std::string &GetActorResource( void ) const { return m_actorResource; }
		const mat4 *GetInitialTransform( void ) const { return (m_hasInitialTransform) ? &m_initialTransform : NULL; }
		const ActorId GetServerActorId( void ) const 	{ return m_serverActorId; }
		GameViewId GetViewId( void ) const { return m_viewId; }
	};

	//TODO: impliment script manager

	//---------------------------------------------------------------------------------------------------------------------
	// EvtData_Request_Destroy_Actor - sent by any system requesting that the game logic destroy an actor	
	//    FUTURE WORK - This event shouldn't really exist - subsystems should never ask the game logic to destroy something through an event, should they?
	//---------------------------------------------------------------------------------------------------------------------
	/*class CEvtData_Request_Destroy_Actor : public ScriptEvent
	{
		ActorId m_actorId;

	public:
		static const EventType sk_EventType;

		EvtData_Request_Destroy_Actor( )
		{
			m_actorId = INVALID_ACTOR_ID;
		}

		EvtData_Request_Destroy_Actor( ActorId actorId )
		{
			m_actorId = actorId;
		}

		virtual const EventType& VGetEventType( void ) const
		{
			return sk_EventType;
		}

		virtual void VDeserialize( std::istrstream & in )
		{
			in >> m_actorId;
		}

		virtual IEventDataPtr VCopy( ) const
		{
			return IEventDataPtr( QSE_NEW EvtData_Request_Destroy_Actor( m_actorId ) );
		}

		virtual void VSerialize( std::ostrstream & out ) const
		{
			out << m_actorId;
		}

		virtual const char* GetName( void ) const
		{
			return "EvtData_Request_Destroy_Actor";
		}

		ActorId GetActorId( void ) const
		{
			return m_actorId;
		}

		virtual bool VBuildEventFromScript( void )
		{
			if (m_eventData.IsInteger( ))
			{
				m_actorId = m_eventData.GetInteger( );
				return true;
			}
			return false;
		}

		EXPORT_FOR_SCRIPT_EVENT( EvtData_Request_Destroy_Actor );
	};*/

	//TODO: impliment scripting and audio

	//---------------------------------------------------------------------------------------------------------------------
	// EvtData_PlaySound - sent by any system wishing for a HumanView to play a sound
	//---------------------------------------------------------------------------------------------------------------------
	/*class CEvtData_PlaySound : public ScriptEvent
	{
		std::string m_soundResource;

	public:
		static const EventType sk_EventType;

		EvtData_PlaySound( void ) { }
		EvtData_PlaySound( const std::string& soundResource )
			: m_soundResource( soundResource )
		{
		}

		virtual const EventType& VGetEventType( void ) const
		{
			return sk_EventType;
		}

		virtual IEventDataPtr VCopy( ) const
		{
			return IEventDataPtr( QSE_NEW EvtData_PlaySound( m_soundResource ) );
		}

		virtual void VSerialize( std::ostrstream& out ) const
		{
			out << m_soundResource;
		}

		virtual void VDeserialize( std::istrstream& in )
		{
			in >> m_soundResource;
		}

		const std::string& GetResource( void ) const
		{
			return m_soundResource;
		}

		virtual const char* GetName( void ) const
		{
			return "EvtData_PlaySound";
		}

		virtual bool VBuildEventFromScript( void );

		EXPORT_FOR_SCRIPT_EVENT( EvtData_PlaySound );
	};*/



}



#endif