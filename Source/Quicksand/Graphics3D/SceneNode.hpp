#ifndef QSE_GLSCENENODE_HPP
#define QSE_GLSCENENODE_HPP

#include "CMaterial.hpp"
#include "Shader.hpp"

namespace Quicksand
{

	//forward declarations
	class CGLSceneNode;
	class CGLScene;
	class CRayCast;
	class CMovementController;
	class IResourceExtraData;
	class CActorComponent;
	class CBaseRenderComponent;


	//the different types of alpha blending
	enum GLAlphaType
	{
		AlphaOpaque,
		AlphaTexture,
		AlphaMaterial,
		AlphaVertex
	};

	typedef unsigned int ActorID;

	class CGLSceneNodeProperties
	{
		friend class CGLSceneNode;

	protected:
		ActorID			m_ActorID;
		std::string		m_Name;
		mat4			m_ToWorld, m_FromWorld;
		float			m_Radius;
		RenderPass		m_RenderPass;
		CMaterial		m_Material;
		GLAlphaType		m_AlphaType;

		void SetAlpha( const float alpha );

	public:

		CGLSceneNodeProperties( void );
		const ActorId& ActorID( void ) const;
		mat4 const& ToWorld( void ) const;
		mat4 const FromWorld( void ) const;
		void Transform( mat4* toWorld, mat4 *fromWorld ) const;

		const char* Name( void ) const;

		bool HasAlpha( void ) const;
		float Alpha( void ) const;
		GLAlphaType AlphaType( void ) const;

		RenderPass RenderPass( void ) const;
		float Radius( void ) const;

		CMaterial GetMaterial(void) const;
	};

	// a useful typedef for making CGLSceneNode hierarchy
	typedef std::vector<shared_ptr<IGLSceneNode> > GLSceneNodeList;

	//the scene node that represents an object in the world hierarchy
	class CGLSceneNode : public IGLSceneNode
	{
		friend class CScene;

	protected:
		GLSceneNodeList		   m_Children;
		CGLSceneNode		  *m_pParent;
		CGLSceneNodeProperties m_Props;

	public:
		CGLSceneNode( ActorID actorId, std::string name, RenderPass renderPass, const Color& diffuseColor, const mat4 *to, const mat4 *from = NULL );
		virtual ~CGLSceneNode( void );

		virtual const CGLSceneNodeProperties* const VGet( void ) const;

		virtual void VSetTransform( const mat4 *toWorld, const mat4 *fromWorld = NULL );

		virtual long VOnRestore(CGLScene *pScene);
		virtual long VOnUpdate(CGLScene *pScene, DWORD const elapsedMs);
		

		virtual long VPreRender(CGLScene *pScene);
		virtual bool VIsVisible(CGLScene *pScene);
		virtual long VRender(CGLScene *pScene);
		virtual long VPostRender(CGLScene *pScene);

		virtual bool VAddChild( shared_ptr<IGLSceneNode> child );
		virtual bool VRemoveChild( ActorID id );
		virtual long VPick( CScene* pScene, CRayCast *pRayCast );

		void SetAlpha( const float Alpha );
		float GetAlpha( void ) const;

		vec3 GetPosition( void ) const;
		void SetPosition( const vec3& pos );

		const vec3 GetWorldPosition( void ) const;

		vec3 GetDirection( void ) const;

		void SetRadius( const float radius );
		void SetMaterial( const CMaterial& mat );

	};


	//a scene node that has transparencies

	class CGLAlphaSceneNode
	{
		shared_ptr<IGLSceneNode> m_pNode;
		mat4 m_Concat;
		float m_ScreenZ;

		// For the STL sort...
		bool const operator <(CGLAlphaSceneNode const &other) { return m_ScreenZ < other.m_ScreenZ; }
	};

	typedef std::list<CGLAlphaSceneNode *> GLAlphaSceneNodes;


}


#endif