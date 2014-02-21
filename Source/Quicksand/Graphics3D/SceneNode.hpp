#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include "Material.hpp"
#include "Shader.hpp"

namespace Quicksand
{

	//forward declarations
	class SceneNode;
	class Scene;
	class RayCast;
	class MovementController;
	class IResourceExtraData;
	class ActorComponent;
	class BaseRenderComponent;


	//the different types of alpha blending
	enum AlphaType
	{
		AlphaOpaque,
		AlphaTexture,
		AlphaMaterial,
		AlphaVertex
	};

	typedef unsigned int ActorID;

	class SceneNodeProperties
	{
		friend class SceneNode;

	protected:
		ActorID			m_ActorID;
		std::string		m_Name;
		mat4			m_ToWorld, m_FromWorld;
		float			m_Radius;
		RenderPass		m_RenderPass;
		Material		m_Material;
		AlphaType		m_AlphaType;

		void SetAlpha( const float alpha );

	public:

		SceneNodeProperties( void );
		const ActorId& ActorID( void ) const;
		mat4 const& ToWorld( void ) const;
		mat4 const FromWorld( void ) const;
		void Transform( mat4* toWorld, mat4 *fromWorld ) const;

		const char* Name( void ) const;

		bool HasAlpha( void ) const;
		float Alpha( void ) const;
		AlphaType AlphaType( void ) const;

		RenderPass RenderPass( void ) const;
		float Radius( void ) const;

		MaterialGetMaterial( void ) const;
	};

	// a useful typedef for making sceneNode hierarchy
	typedef std::vector<shared_ptr<ISceneNode> > SceneNodeList;

	//the scene node that represents an object in the world hierarchy
	class SceneNode
	{
		friend class Scene;

	protected:
		SceneNodeList		m_Children;
		SceneNode		   *m_Parent;
		SceneNodeProperties m_Props;
		//WeakBaseRenderComponentPtr m_RenderComponent;

	public:
		SceneNode( ActorID actorId, /*WeakBaseRenderComponentPtr renderComponent*/, RenderPass renderPass, const mat4 *to, const mat4 *from = NULL );
		virtual ~SceneNode( void );

		virtual const SceneNodeProperties* const VGet( void ) const;

		virtual void VSetTransform( const mat4 *toWorld, const mat4 *fromWorld = NULL );

		virtual long VOnRestore( Scene *pScene );
		virtual long VOnUpdate( Scene *pScene, DWORD const elapsedMs );
		

		virtual long VPreRender( Scene *pScene );
		virtual bool VIsVisible( Scene *pScene );
		virtual long VRender( Scene *pScene );
		virtual long VPostRender( Scene pScene );

		virtual bool VAddChild( shared_ptr<ISceneNode> child );
		virtual bool VRemoveChild( ActorID id );
		virtual long VPick( Scene* pScene, RayCast *pRayCast );

		void SetAlpha( const alpha );
		float GetAlpha( void ) const;

		vec3 GetPosition( void ) const;
		void SetPosition( const vec3& pos );

		const vec3 GetWorldPosition( void ) const;

		vec3 GetDirection( void ) const;

		void SetRadius( const float radius );
		void SetMaterial( const Material& mat );

	};



}


#endif