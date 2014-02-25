#include "stdafx.h"
#include "SceneNode.hpp"
#include "Scene.hpp"
#include "../Math/Math.hpp"

namespace Quicksand
{


	void GLSceneNodeProperties::SetAlpha(const float alpha)
	{
		m_Material.SetAlpha(alpha);
	}

	GLSceneNodeProperties::GLSceneNodeProperties(void)
	{
		m_ActorID = INVALID_ACTOR_ID;
		m_Radius = 0;
		m_RenderPass = RenderPass_0;
		m_AlphaType = AlphaOpaque;
	}

	const ActorId& GLSceneNodeProperties::ActorID(void) const
	{
		return m_ActorID;
	}

	mat4 const& GLSceneNodeProperties::ToWorld(void) const
	{
		return m_ToWorld;
	}

	mat4 const GLSceneNodeProperties::FromWorld(void) const
	{
		return m_FromWorld;
	}

	void GLSceneNodeProperties::Transform(mat4* toWorld, mat4 *fromWorld) const
	{
		if (toWorld != NULL)
			*toWorld = m_ToWorld;
		if (fromWorld != NULL)
			*fromWorld = m_FromWorld;
	}


	const char* GLSceneNodeProperties::Name(void) const
	{
		return m_Name.c_str();
	}


	bool GLSceneNodeProperties::HasAlpha(void) const
	{
		return m_Material.HasAlpha();
	}

	float GLSceneNodeProperties::Alpha(void) const
	{
		return m_Material.GetAlpha();
	}

	GLAlphaType GLSceneNodeProperties::AlphaType(void) const
	{
		return m_AlphaType;
	}


	RenderPass GLSceneNodeProperties::RenderPass(void) const
	{
		return m_RenderPass;
	}

	float GLSceneNodeProperties::Radius(void) const
	{
		return m_Radius;
	}


	Material GLSceneNodeProperties::GetMaterial(void) const
	{
		return m_Material;
	}

	//////////////////////////////////
	//GLSceneNode
	//
	///////////////////////////////////

	GLSceneNode::GLSceneNode(ActorID actorId, std::string name, RenderPass renderPass, const Color& diffuseColor, const mat4 *to, const mat4 *from)
	{
		m_pParent = NULL;

		m_Props.m_ActorID = actorId;
		m_Props.m_RenderPass = renderPass;
		m_Props.m_Name = name;
		m_Props.m_AlphaType = AlphaOpaque;

		VSetTransform(to, from);
		SetRadius(0);
		m_Props.m_Material.SetDiffuse(diffuseColor);

	}

	GLSceneNode::~GLSceneNode(void)
	{

	}


	const GLSceneNodeProperties* const GLSceneNode::VGet(void) const
	{
		return &m_Props;
	}


	void GLSceneNode::VSetTransform(const mat4 *toWorld, const mat4 *fromWorld )
	{
		m_Props.m_ToWorld = *toWorld;
		m_Props.m_FromWorld = *fromWorld;
	}


	long GLSceneNode::VOnRestore(GLScene *pScene)
	{

	}

	long GLSceneNode::VOnUpdate(GLScene *pScene, DWORD const elapsedMs)
	{

	}



	long GLSceneNode::VPreRender(GLScene *pScene)
	{

	}

	bool GLSceneNode::VIsVisible(GLScene *pScene)
	{

	}

	long GLSceneNode::VRender(GLScene *pScene)
	{

	}

	long GLSceneNode::VPostRender(GLScene *pScene)
	{

	}


	bool GLSceneNode::VAddChild(shared_ptr<IGLSceneNode> child)
	{
		m_Children.push_back(child);

		shared_ptr<GLSceneNode> kid = static_pointer_cast<GLSceneNode>(child);

		kid->m_pParent = this;					// [mrmike] Post-press fix - the parent was never set!

		// The radius of the sphere should be fixed right here
		vec3 kidPos = GetTransMatPosition(kid->VGet()->ToWorld());

		float newRadius = glm::length(kidPos) + kid->VGet()->Radius();

		if (newRadius > m_Props.m_Radius)
			m_Props.m_Radius = newRadius;

		return true;
	}

	bool GLSceneNode::VRemoveChild(ActorID id)
	{
		for (GLSceneNodeList::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
		{
			const GLSceneNodeProperties* pProps = (*i)->VGet();
			if (pProps->ActorID() != INVALID_ACTOR_ID && id == pProps->ActorID())
			{
				i = m_Children.erase(i);	//this can be expensive for vectors
				return true;
			}
		}
		return false;
	}

	long GLSceneNode::VPick(Scene* pScene, RayCast *pRayCast)
	{
		for (GLSceneNodeList::const_iterator i = m_Children.begin(); i != m_Children.end(); ++i)
		{
			HRESULT hr = (*i)->VPick(pScene, pRayCast);

			if (hr == E_FAIL)
				return E_FAIL;
		}

		return S_OK;
	}


	void GLSceneNode::SetAlpha(const float alpha)
	{
		m_Props.m_Material.SetAlpha(alpha);
		for (GLSceneNodeList::const_iterator i = m_Children.begin(); i != m_Children.end(); ++i)
		{
			shared_ptr<GLSceneNode> sceneNode = static_pointer_cast<GLSceneNode>(*i);
			sceneNode->SetAlpha(alpha);
		}
	}

	float GLSceneNode::GetAlpha(void) const
	{
		return m_Props.m_Material.GetAlpha();
	}


	vec3 GLSceneNode::GetPosition(void) const
	{
		return GetTransMatPosition(m_Props.ToWorld());
	}

	void GLSceneNode::SetPosition(const vec3& pos)
	{
		m_Props.m_ToWorld[3][0] = pos.x;
		m_Props.m_ToWorld[3][1] = pos.y;
		m_Props.m_ToWorld[3][2] = pos.z;
	}


	const vec3 GLSceneNode::GetWorldPosition(void) const
	{
		return GetTransMatPosition(m_Props.m_ToWorld);
	}


	vec3 GLSceneNode::GetDirection(void) const
	{
		return GetTransMatDirection(m_Props.ToWorld);
	}


	void GLSceneNode::SetRadius(const float radius)
	{
		m_Props.m_Radius = radius;
	}

	void GLSceneNode::SetMaterial(const Material& mat)
	{
		m_Props.m_Material = mat;
	}


}