#ifndef QSE_INTERFACES_HPP
#define QSE_INTERFACES_HPP

//all of the interface clases used in this engine


namespace Quicksand
{

	class CActor;
	class CActorComponent;

	typedef unsigned int ActorId;
	typedef unsigned int ComponentId;

	const ActorId INVALID_ACTOR_ID = 0;
	const ComponentId INVALID_COMPONENT_ID = 0;

	typedef shared_ptr<CActor> StrongActorPtr;
	typedef weak_ptr<CActor> WeakActorPtr;
	typedef shared_ptr<CActorComponent> StrongActorComponentPtr;
	typedef weak_ptr<CActorComponent> WeakActorComponentPtr;

	template<class CT>
	struct SortBy_SharedPtr_Content
	{
		bool operator()( const shared_ptr<T> &lhs, const shared_ptr<T> &rhs ) const
		{
			return *lhs < *rhs;
		}
	};

	//
	// class IScreenElement							- Chapter 10, page 285
	//
	class IScreenElement
	{
	public:
		virtual HRESULT VOnRestore( ) = 0;
		virtual HRESULT VOnLostDevice( ) = 0;
		virtual HRESULT VOnRender( double fTime, float fElapsedTime ) = 0;
		virtual void VOnUpdate( int deltaMilliseconds ) = 0;

		virtual int VGetZOrder( ) const = 0;
		virtual void VSetZOrder( int const zOrder ) = 0;
		virtual bool VIsVisible( ) const = 0;
		virtual void VSetVisible( bool visible ) = 0;

		virtual LRESULT CALLBACK VOnMsgProc( AppMsg msg ) = 0;

		virtual ~IScreenElement( ) { };
		virtual bool const operator <(IScreenElement const &other) { return VGetZOrder( ) < other.VGetZOrder( ); }
	};


	class IGamePhysics;

	class IGameLogic
	{
	public:
		virtual WeakActorPtr VGetActor( const ActorId id ) = 0;
		virtual StrongActorPtr VCreateActor( const std::string &actorResource, TiXmlElement *overrides, const mat4 *initialTransform = NULL, const ActorId serversActorId = INVALID_ACTOR_ID ) = 0;
		virtual void VDestroyActor( const ActorId actorId ) = 0;
		virtual bool VLoadGame( const char* levelResource ) = 0;
		virtual void VSetProxy( ) = 0;
		virtual void VOnUpdate( float time, float elapsedTime ) = 0;
		virtual void VChangeState( enum BaseGameState newState ) = 0;
		virtual void VMoveActor( const ActorId id, mat4 const &mat ) = 0;
		virtual shared_ptr<IGamePhysics> VGetGamePhysics( void ) = 0;
	};

	enum GameViewType
	{
		GameView_Human,
		GameView_Remote,
		GameView_AI,
		GameView_Recorder,
		GameView_Other
	};

	typedef unsigned int GameViewId;
	extern const GameViewId gc_InvalidGameViewId;

	class IGameView
	{
	public:
		virtual HRESULT VOnRestore( ) = 0;
		virtual void VOnRender( double fTime, float fElapsedTime ) = 0;
		virtual HRESULT VOnLostDevice( ) = 0;
		virtual GameViewType VGetType( ) = 0;
		virtual GameViewId VGetId( ) const = 0;
		virtual void VOnAttach( GameViewId vid, ActorId aid ) = 0;

		virtual LRESULT CALLBACK VOnMsgProc( AppMsg msg ) = 0;
		virtual void VOnUpdate( unsigned long deltaMs ) = 0;

		virtual ~IGameView( ) { };
	};



	typedef std::list<shared_ptr<IScreenElement> > ScreenElementList;
	typedef std::list<shared_ptr<IGameView> > GameViewList;


	////////////////////////////////////////////////////
	//
	// IKeyboardHandler Description		- Chapter 9, page 242
	// ICPointerHandler Description		- Chapter 9, page 242
	// IJoystickHandler Description		- Chapter 9, page 242
	// IGamepadHandler Description		- Chapter 9, page 242
	//
	// These are the public APIs for any object that implements reactions
	// to events sent by hardware user interface devices.
	//
	// Note: IJoystickHandler and IGamepadHandler are not currently
	//       implemented anywhere in the codebase. They are here
	//       as examples, and could require modification to actually work!
	//
	////////////////////////////////////////////////////

	class IKeyboardHandler
	{
	public:
		virtual bool VOnKeyDown( const BYTE c ) = 0;
		virtual bool VOnKeyUp( const BYTE c ) = 0;
	};

	class ICPointerHandler
	{
	public:
		virtual bool VOnCPointerMove( const CPoint &pos, const int radius ) = 0;
		virtual bool VOnCPointerButtonDown( const CPoint &pos, const int radius, const std::string &buttonName ) = 0;
		virtual bool VOnCPointerButtonUp( const CPoint &pos, const int radius, const std::string &buttonName ) = 0;
	};

	class IJoystickHandler
	{
		virtual bool VOnButtonDown( const std::string &buttonName, int const pressure ) = 0;
		virtual bool VOnButtonUp( const std::string &buttonName ) = 0;
		virtual bool VOnJoystick( float const x, float const y ) = 0;
	};

	class IGamepadHandler
	{
		virtual bool VOnTrigger( const std::string &triggerName, float const pressure ) = 0;
		virtual bool VOnButtonDown( const std::string &buttonName, int const pressure ) = 0;
		virtual bool VOnButtonUp( const std::string &buttonName ) = 0;
		virtual bool VOnDirectionalPad( const std::string &direction ) = 0;
		virtual bool VOnThumbstick( const std::string &stickName, float const x, float const y ) = 0;
	};

	////////////////////////////////////////////////////
	//
	// class IResourceFile						- Chapter 8, page 222
	// The core of a resource cache system
	// 
	////////////////////////////////////////////////////

	class CResource;
	class IResourceFile;
	class CResHandle;

	//
	// class IResourceLoader					- Chapter 8, page 224
	//
	class IResourceLoader
	{
	public:
		virtual std::string VGetPattern( ) = 0;
		virtual bool VUseRawFile( ) = 0;
		virtual bool VDiscardRawBufferAfterLoad( ) = 0;
		virtual bool VAddNullZero( ) { return false; }
		virtual unsigned int VGetLoadedResourceSize( char *rawBuffer, unsigned int rawSize ) = 0;
		virtual bool VLoadResource( char *rawBuffer, unsigned int rawSize, shared_ptr<CResHandle> handle ) = 0;
	};

	class IResourceFile
	{
	public:
		virtual bool VOpen( ) = 0;
		virtual int VGetRawResourceSize( const CResource &r ) = 0;
		virtual int VGetRawResource( const CResource &r, char *buffer ) = 0;
		virtual int VGetNumResources( ) const = 0;
		virtual std::string VGetResourceName( int num ) const = 0;
		virtual bool VIsUsingDevelopmentDirectories( void ) const = 0;
		virtual ~IResourceFile( ) { }
	};




	/////////////////////////////////////////////////////////////////////////////
	// enum RenderPass							- Chapter 16, page 529
	//
	//   3D scenes are drawn in passes - this enum defines the render passes
	//   supported by the 3D scene graph created by class CScene.
	/////////////////////////////////////////////////////////////////////////////

	enum RenderPass
	{
		RenderPass_0,
		RenderPass_Static = RenderPass_0,
		RenderPass_Actor,
		RenderPass_Sky,
		RenderPass_NotRendered,
		RenderPass_Last
	};




	class CScene;
	class CGLSceneNodeProperties;
	class CRayCast;
	class CLightNode;

	typedef std::list<shared_ptr<CLightNode> > Lights;


	class IRenderState
	{
	public:
		virtual std::string VToString( ) = 0;
	};

	//
	// class IRenderer				- not described in the 4th edition
	//
	// class IRenderer abstracts the renderer implementation so the engine
	//   can use D3D9 or D3D11.
	//
	class IRenderer
	{
	public:
		virtual void VSetBackgroundColor( BYTE bgA, BYTE bgR, BYTE bgG, BYTE bgB ) = 0;
		virtual HRESULT VOnRestore( ) = 0;
		virtual void VShutdown( ) = 0;
		virtual bool VPreRender( ) = 0;
		virtual bool VPostRender( ) = 0;
		virtual void VCalcLighting( Lights *lights, int maximumLights ) = 0;
		virtual void VSetWorldTransform( const mat4 *m ) = 0;
		virtual void VSetViewTransform( const mat4 *m ) = 0;
		virtual void VSetProjectionTransform( const mat4 *m ) = 0;
		virtual shared_ptr<IRenderState> VPrepareAlphaPass( ) = 0;
		virtual shared_ptr<IRenderState> VPrepareSkyBoxPass( ) = 0;
		virtual void VDrawLine( const vec3& from, const vec3& to, const Color& color ) = 0;
	};

	/////////////////////////////////////////////////////////////////////////////
	// class IGLSceneNode					- Chapter 16, page 524
	//
	//   This is the public interface for nodes in a 3D scene graph.
	/////////////////////////////////////////////////////////////////////////////

	class IGLSceneNode
	{
	public:
		virtual const CGLSceneNodeProperties * const VGet( ) const = 0;

		virtual void VSetTransform( const mat4 *toWorld, const mat4 *fromWorld = NULL ) = 0;

		virtual HRESULT VOnUpdate( CScene *pScene, DWORD const elapsedMs ) = 0;
		virtual HRESULT VOnRestore( CScene *pScene ) = 0;

		virtual HRESULT VPreRender( CScene *pScene ) = 0;
		virtual bool VIsVisible( CScene *pScene ) const = 0;
		virtual HRESULT VRender( CScene *pScene ) = 0;
		virtual HRESULT VRenderChildren( CScene *pScene ) = 0;
		virtual HRESULT VPostRender( CScene *pScene ) = 0;

		virtual bool VAddChild( shared_ptr<IGLSceneNode> kid ) = 0;
		virtual bool VRemoveChild( ActorId id ) = 0;
		virtual HRESULT VOnLostDevice( CScene *pScene ) = 0;
		virtual HRESULT VPick( CScene *pScene, CRayCast *pRayCast ) = 0;


		virtual ~IGLSceneNode( ) { };
	};



	/////////////////////////////////////////////////////////////////////////////
	// class IGamePhysics							- Chapter 17, page 589
	//
	//   The interface defintion for a generic physics API.
	/////////////////////////////////////////////////////////////////////////////

	class IGamePhysics
	{
	public:

		// Initialiazation and Maintenance of the Physics World
		virtual bool VInitialize( ) = 0;
		virtual void VSyncVisibleScene( ) = 0;
		virtual void VOnUpdate( float deltaSeconds ) = 0;

		// Initialization of Physics Objects
		virtual void VAddSphere( float radius, WeakActorPtr actor, /*const Mat4x4& initialTransform, */const std::string& densityStr, const std::string& physicsMaterial ) = 0;
		virtual void VAddBox( const vec3& dimensions, WeakActorPtr gameActor, /*const Mat4x4& initialTransform, */ const std::string& densityStr, const std::string& physicsMaterial ) = 0;
		virtual void VAddCPointCloud( vec3 *verts, int numCPoints, WeakActorPtr gameActor, /*const Mat4x4& initialTransform, */ const std::string& densityStr, const std::string& physicsMaterial ) = 0;
		virtual void VRemoveActor( ActorId id ) = 0;

		// Debugging
		virtual void VRenderDiagnostics( ) = 0;

		// Physics world modifiers
		virtual void VCreateTrigger( WeakActorPtr pGameActor, const vec3 &pos, const float dim ) = 0;
		virtual void VApplyForce( const vec3 &dir, float newtons, ActorId aid ) = 0;
		virtual void VApplyTorque( const vec3 &dir, float newtons, ActorId aid ) = 0;
		virtual bool VKinematicMove( const mat4 &mat, ActorId aid ) = 0;

		// Physics actor states
		virtual void VRotateY( ActorId actorId, float angleRadians, float time ) = 0;
		virtual float VGetOrientationY( ActorId actorId ) = 0;
		virtual void VStopActor( ActorId actorId ) = 0;
		virtual vec3 VGetVelocity( ActorId actorId ) = 0;
		virtual void VSetVelocity( ActorId actorId, const vec3& vel ) = 0;
		virtual vec3 VGetAngularVelocity( ActorId actorId ) = 0;
		virtual void VSetAngularVelocity( ActorId actorId, const vec3& vel ) = 0;
		virtual void VTranslate( ActorId actorId, const vec3& vec ) = 0;

		virtual void VSetTransform( const ActorId id, const mat4& mat ) = 0;
		virtual mat4 VGetTransform( const ActorId id ) = 0;

		virtual ~IGamePhysics( ) { };
	};


	/////////////////////////////////////////////////////////////////////////////
	// IScriptManager Description
	//
	//   This is the public interface for the scripting system.
	/////////////////////////////////////////////////////////////////////////////
	class IScriptManager
	{
	public:
		virtual ~IScriptManager( void ) {}
		virtual bool VInit( void ) = 0;
		virtual void VExecuteFile( const char* resource ) = 0;
		virtual void VExecuteString( const char* str ) = 0;
	};



}


#endif INTERFACES_HPP