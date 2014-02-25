#ifndef GLSceneNode_HPP
#define GLSceneNode_HPP

#include "Material.hpp"
#include "Shader.hpp"

namespace Quicksand
{

	//forward declarations
	class GLSceneNode;
	class GLScene;
	class RayCast;
	class MovementController;
	class IResourceExtraData;
	class ActorComponent;
	class BaseRenderComponent;


	//the different types of alpha blending
	enum GLAlphaType
	{
		AlphaOpaque,
		AlphaTexture,
		AlphaMaterial,
		AlphaVertex
	};

	typedef unsigned int ActorID;

	class GLSceneNodeProperties
	{
		friend class GLSceneNode;

	protected:
		ActorID			m_ActorID;
		std::string		m_Name;
		mat4			m_ToWorld, m_FromWorld;
		float			m_Radius;
		RenderPass		m_RenderPass;
		Material		m_Material;
		GLAlphaType		m_AlphaType;

		void SetAlpha( const float alpha );

	public:

		GLSceneNodeProperties( void );
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

		Material GetMaterial(void) const;
	};

	// a useful typedef for making GLSceneNode hierarchy
	typedef std::vector<shared_ptr<IGLSceneNode> > GLSceneNodeList;

	//the scene node that represents an object in the world hierarchy
	class GLSceneNode : public IGLSceneNode
	{
		friend class Scene;

	protected:
		GLSceneNodeList		  m_Children;
		GLSceneNode		     *m_pParent;
		GLSceneNodeProperties m_Props;

	public:
		GLSceneNode( ActorID actorId, std::string name, RenderPass renderPass, const Color& diffuseColor, const mat4 *to, const mat4 *from = NULL );
		virtual ~GLSceneNode( void );

		virtual const GLSceneNodeProperties* const VGet( void ) const;

		virtual void VSetTransform( const mat4 *toWorld, const mat4 *fromWorld = NULL );

		virtual long VOnRestore(GLScene *pScene);
		virtual long VOnUpdate(GLScene *pScene, DWORD const elapsedMs);
		

		virtual long VPreRender(GLScene *pScene);
		virtual bool VIsVisible(GLScene *pScene);
		virtual long VRender(GLScene *pScene);
		virtual long VPostRender(GLScene *pScene);

		virtual bool VAddChild( shared_ptr<IGLSceneNode> child );
		virtual bool VRemoveChild( ActorID id );
		virtual long VPick( Scene* pScene, RayCast *pRayCast );

		void SetAlpha( const float Alpha );
		float GetAlpha( void ) const;

		vec3 GetPosition( void ) const;
		void SetPosition( const vec3& pos );

		const vec3 GetWorldPosition( void ) const;

		vec3 GetDirection( void ) const;

		void SetRadius( const float radius );
		void SetMaterial( const Material& mat );

	};


	//a scene node that has transparencies

	class GLAlphaSceneNode
	{
		shared_ptr<IGLSceneNode> m_pNode;
		mat4 m_Concat;
		float m_ScreenZ;

		// For the STL sort...
		bool const operator <(GLAlphaSceneNode const &other) { return m_ScreenZ < other.m_ScreenZ; }
	};

	typedef std::list<GLAlphaSceneNode *> GLAlphaSceneNodes;


}


#endif