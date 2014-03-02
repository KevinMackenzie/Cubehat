#include "stdafx.h"
#include "SceneNode.hpp"
#include "Scene.hpp"
#include "../Math/Math.hpp"

namespace Quicksand
{


	void CGLSceneNodeProperties::SetAlpha(const float alpha)
	{
		m_Material.SetAlpha(alpha);
	}

	CGLSceneNodeProperties::CGLSceneNodeProperties(void)
	{
		m_ActorID = INVALID_ACTOR_ID;
		m_Radius = 0;
		m_RenderPass = RenderPass_0;
		m_AlphaType = AlphaOpaque;
	}

	const ActorId& CGLSceneNodeProperties::ActorID(void) const
	{
		return m_ActorID;
	}

	mat4 const& CGLSceneNodeProperties::ToWorld(void) const
	{
		return m_ToWorld;
	}

	mat4 const CGLSceneNodeProperties::FromWorld(void) const
	{
		return m_FromWorld;
	}

	void CGLSceneNodeProperties::Transform(mat4* toWorld, mat4 *fromWorld) const
	{
		if (toWorld != NULL)
			*toWorld = m_ToWorld;
		if (fromWorld != NULL)
			*fromWorld = m_FromWorld;
	}


	const char* CGLSceneNodeProperties::Name(void) const
	{
		return m_Name.c_str();
	}


	bool CGLSceneNodeProperties::HasAlpha(void) const
	{
		return m_Material.HasAlpha();
	}

	float CGLSceneNodeProperties::Alpha(void) const
	{
		return m_Material.GetAlpha();
	}

	GLAlphaType CGLSceneNodeProperties::AlphaType(void) const
	{
		return m_AlphaType;
	}


	RenderPass CGLSceneNodeProperties::RenderPass(void) const
	{
		return m_RenderPass;
	}

	float CGLSceneNodeProperties::Radius(void) const
	{
		return m_Radius;
	}


	CMaterial CGLSceneNodeProperties::GetMaterial(void) const
	{
		return m_Material;
	}

	//////////////////////////////////
	//CGLSceneNode
	//
	///////////////////////////////////

	CGLSceneNode::CGLSceneNode(ActorID actorId, std::string name, RenderPass renderPass, const Color& diffuseColor, const mat4 *to, const mat4 *from)
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

	CGLSceneNode::~CGLSceneNode(void)
	{

	}


	const CGLSceneNodeProperties* const CGLSceneNode::VGet(void) const
	{
		return &m_Props;
	}


	void CGLSceneNode::VSetTransform(const mat4 *toWorld, const mat4 *fromWorld )
	{
		m_Props.m_ToWorld = *toWorld;
		m_Props.m_FromWorld = *fromWorld;
	}


	long CGLSceneNode::VOnRestore(CGLScene *pScene)
	{

	}

	long CGLSceneNode::VOnUpdate(CGLScene *pScene, DWORD const elapsedMs)
	{

	}



	long CGLSceneNode::VPreRender(CGLScene *pScene)
	{

	}

	bool CGLSceneNode::VIsVisible(CGLScene *pScene)
	{

	}

	long CGLSceneNode::VRender(CGLScene *pScene)
	{

	}

	long CGLSceneNode::VPostRender(CGLScene *pScene)
	{

	}


	bool CGLSceneNode::VAddChild(shared_ptr<IGLSceneNode> child)
	{
		m_Children.push_back(child);

		shared_ptr<CGLSceneNode> kid = static_pointer_cast<CGLSceneNode>(child);

		kid->m_pParent = this;					// [mrmike] Post-press fix - the parent was never set!

		// The radius of the sphere should be fixed right here
		vec3 kidPos = GetTransMatPosition(kid->VGet()->ToWorld());

		float newRadius = glm::length(kidPos) + kid->VGet()->Radius();

		if (newRadius > m_Props.m_Radius)
			m_Props.m_Radius = newRadius;

		return true;
	}

	bool CGLSceneNode::VRemoveChild(ActorID id)
	{
		for (GLSceneNodeList::iterator i = m_Children.begin(); i != m_Children.end(); ++i)
		{
			const CGLSceneNodeProperties* pProps = (*i)->VGet();
			if (pProps->ActorID() != INVALID_ACTOR_ID && id == pProps->ActorID())
			{
				i = m_Children.erase(i);	//this can be expensive for vectors
				return true;
			}
		}
		return false;
	}

	long CGLSceneNode::VPick(CScene* pScene, CRayCast *pRayCast)
	{
		for (GLSceneNodeList::const_iterator i = m_Children.begin(); i != m_Children.end(); ++i)
		{
			HRESULT hr = (*i)->VPick(pScene, pRayCast);

			if (hr == E_FAIL)
				return E_FAIL;
		}

		return S_OK;
	}


	void CGLSceneNode::SetAlpha(const float alpha)
	{
		m_Props.m_Material.SetAlpha(alpha);
		for (GLSceneNodeList::const_iterator i = m_Children.begin(); i != m_Children.end(); ++i)
		{
			shared_ptr<CGLSceneNode> sceneNode = static_pointer_cast<CGLSceneNode>(*i);
			sceneNode->SetAlpha(alpha);
		}
	}

	float CGLSceneNode::GetAlpha(void) const
	{
		return m_Props.m_Material.GetAlpha();
	}


	vec3 CGLSceneNode::GetPosition(void) const
	{
		return GetTransMatPosition(m_Props.ToWorld());
	}

	void CGLSceneNode::SetPosition(const vec3& pos)
	{
		m_Props.m_ToWorld[3][0] = pos.x;
		m_Props.m_ToWorld[3][1] = pos.y;
		m_Props.m_ToWorld[3][2] = pos.z;
	}


	const vec3 CGLSceneNode::GetWorldPosition(void) const
	{
		return GetTransMatPosition(m_Props.m_ToWorld);
	}


	vec3 CGLSceneNode::GetDirection(void) const
	{
		return GetTransMatDirection(m_Props.ToWorld);
	}


	void CGLSceneNode::SetRadius(const float radius)
	{
		m_Props.m_Radius = radius;
	}

	void CGLSceneNode::SetMaterial(const CMaterial& mat)
	{
		m_Props.m_Material = mat;
	}


}