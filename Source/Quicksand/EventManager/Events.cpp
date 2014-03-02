#include "stdafx.h"
#include "EventManager.hpp"
#include "Events.hpp"

//TODO: impliment events
namespace Quicksand
{

	// To define a new event - you need a 32-bit GUID. 
	// In Visual Studio, go to Tools->Create GUID and grab the first bit.

	const EventType CEvtData_Environment_Loaded::sk_EventType( 0xa3814acd );
	const EventType CEvtData_Remote_Environment_Loaded::sk_EventType( 0x8E2AD6E6 );
	const EventType CEvtData_New_Actor::sk_EventType( 0xe86c7c31 );
	const EventType CEvtData_Move_Actor::sk_EventType( 0xeeaa0a40 );
	const EventType CEvtData_Destroy_Actor::sk_EventType( 0x77dd2b3a );
	const EventType CEvtData_New_Render_Component::sk_EventType( 0xaf4aff75 );
	const EventType CEvtData_Modified_Render_Component::sk_EventType( 0x80fe9766 );
	const EventType CEvtData_Request_Start_Game::sk_EventType( 0x11f2b19d );
	const EventType CEvtData_Remote_Client::sk_EventType( 0x301693d5 );
	const EventType CEvtData_Network_Player_Actor_Assignment::sk_EventType( 0xa7c92f11 );
	const EventType CEvtData_Update_Tick::sk_EventType( 0xf0f5d183 );
	const EventType CEvtData_Decompress_Request::sk_EventType( 0xc128a129 );
	const EventType CEvtData_Decompression_Progress::sk_EventType( 0x68de1f28 );
	const EventType CEvtData_Request_New_Actor::sk_EventType( 0x40378c64 );
	//const EventType EvtData_Request_Destroy_Actor::sk_EventType( 0xf5395770 );
	//const EventType EvtData_PlaySound::sk_EventType( 0x3d8118ee );

	/*bool EvtData_PlaySound::VBuildEventFromScript( void )
	{
		if (m_eventData.IsString( ))
		{
			m_soundResource = m_eventData.GetString( );
			return true;
		}

		return false;
	}*/

	/*void RegisterEngineScriptEvents( void )
	{
		REGISTER_SCRIPT_EVENT( EvtData_Request_Destroy_Actor, EvtData_Request_Destroy_Actor::sk_EventType );
		REGISTER_SCRIPT_EVENT( EvtData_PhysCollision, EvtData_PhysCollision::sk_EventType );
		REGISTER_SCRIPT_EVENT( EvtData_PlaySound, EvtData_PlaySound::sk_EventType );
	}*/


}