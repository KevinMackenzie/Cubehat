#ifndef QUICKSANDGAME_HPP
#define QUICKSANDGAME_HPP

class CTiXmlDocument;

struct GLFWwindow;

namespace Quicksand
{


	struct GameOptions
	{
		// Level option
		std::string m_Level;

		// Rendering options
		std::string m_Renderer;
		bool m_runFullSpeed;
		CPoint m_ScreenSize;
		bool m_RunFullScreen;

		// Sound options
		float m_soundEffectsVolume;
		float m_musicVolume;

		// Multiplayer options
		int m_expectedPlayers;
		int m_listenPort;
		std::string m_gameHost;
		int m_numAIs;
		int m_maxAIs;
		int m_maxPlayers;

		// resource cache options
		bool m_useDevelopmentDirectories;

		// TiXmlElement - look at this to find other options added by the developer
		TiXmlDocument *m_pDoc;

		GameOptions( );
		~GameOptions( ) { SAFE_DELETE( m_pDoc ); }

		void Init( const char* xmlFilePath, LPWSTR lpCmdLine );
	};


	class CBaseGameLogic;
	class CQuicksandApp;


	extern CQuicksandApp* g_pApp;

	class CQuicksandApp
	{

		//DATA:
		bool m_bWindowedMode;					// true if the app is windowed, false if fullscreen
		bool m_bIsRunning;						// true if everything is initialized and the game is in the main loop
		bool m_bQuitRequested;					// true if the app should run the exit sequence
		bool m_bQuitting;						// true if the app is running the exit sequence
		CRect m_rcDesktop;			// current desktop size - not necessarilly the client window size
		CPoint m_ScreenSize;			// game screen size
		bool m_bIsEditorRunning;				// true if the game editor is running

		HINSTANCE   m_hInstance;//the application instance


		void RegisterEngineEvents( void );

		//the time it has been since the last frame update
		float m_fTimeElapsed;

		//a set of update methods
		void OnUpdateGame(float fTime, float fEllapsedTime);
		void OnUpdateFrame(float fTime, float fEllapsedTime);

	protected:
		std::map<std::wstring, std::wstring> m_TextResource;
		std::map<std::wstring, UINT> m_Hotkeys;

		int m_HasModalDialog;					// determines if a modal dialog is up
		int PumpUntilMessage( UINT msgEnd, WPARAM* pWParam, LPARAM* pLParam );
		int	EatSpecificMessages( UINT msgType, COptional<LPARAM> lParam, COptional<WPARAM> wParam );
		void FlashWhileMinimized( );

	public:
		//crucial main functions
		void MainLoop( void );
		void ShutDown( void );

		class CResCache *m_pResCache;
		TCHAR m_SaveGameDirectory[MAX_PATH];


		struct GameOptions m_Options;
		CBaseGameLogic* m_pGame;

		//event manager
		class CEventManager *m_pEventManager;


		//constructors
		CQuicksandApp( void );
		~CQuicksandApp( void );


		//must override these methods in subclass
		//virtual BaseGameLogic  *VCreateGameAndView( ) = 0;
		virtual bool			VLoadGame( void );
		virtual void VRegisterGameEvents( void );

		//this returns a weak pointer to the game instance
		static CQuicksandApp *GetApp( void ){ return g_pApp; }

		//these must be overriden and are simple to impliment
		virtual const char *VGetGameTitle( ) const = 0;
		virtual TCHAR *VGetGameAppDirectory( ) = 0;
		virtual HICON  VGetIcon( ) = 0;


		//dealing with string resources and hotkeys
		bool			LoadStrings( std::string language );
		std::wstring	GetString( std::wstring sID );
		int				GetHotKeyForString( std::wstring sID );
		UINT			MapCharToKeycode( const char pHotkey );


		// Main loop processing
		void AbortGame( )					{ m_bQuitting = true;			}
		bool IsRunning( )					{ return m_bIsRunning;			}
		void SetQuitting( bool quitting )	{ m_bQuitting = quitting;		}

		//BaseGameLogic* GetGameLogic( void ) const { return m_pGame; }

		bool InitInstance( HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenHeight );

		//windows message handling
		static LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void *pUserContext );
		LRESULT OnPowerBroadcast( int event );
		LRESULT OnSysCommand( WPARAM wParam, LPARAM lParam );
		LRESULT OnClose( );
		LRESULT OnAltEnter( );




		//BELOW THIS: openGL specific stuff

		GLFWwindow* m_pOpenGLWindow;

		//GLFW callback functions
		static void ErrorCallbackProc( int error, const char* errorMessage );

	};





	extern bool CheckStorage( const DWORDLONG diskSpaceNeeded );
	extern DWORD ReadCPUSpeed( );
	extern bool CheckMemory( const DWORDLONG physicalRAMNeeded, const DWORDLONG virtualRAMNeeded );
	extern bool IsOnlyInstance( LPCTSTR gameTitle );
	extern const TCHAR *GetSaveGameDirectory( HWND hWnd, const TCHAR *gameAppDirectory );


}



#endif