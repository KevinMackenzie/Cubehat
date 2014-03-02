#include "stdafx.h"
#include "Utils3D.hpp"
#include "Scene.hpp"

namespace Quicksand
{

	void CGLScene::RenderAlphaPass(void)
	{

	}

	CGLScene::CGLScene(void)
	{

	}

	CGLScene::~CGLScene(void)
	{

	}


	unsigned long CGLScene::OnRender(void)
	{

	}

	unsigned long CGLScene::OnUpdate(void)
	{

	}


	shared_ptr<IGLSceneNode> CGLScene::FindActor(ActorId id)
	{

	}

	bool CGLScene::AddChild(ActorId id, shared_ptr<IGLSceneNode> child)
	{

	}


	bool CGLScene::RemoveChild(ActorId id)
	{

	}


	void CGLScene::SetCamera(shared_ptr<CGLCameraNode> camera)
	{

	}

	const shared_ptr<CGLCameraNode> CGLScene::GetCamera(void) const
	{

	}


	void CGLScene::PushAndSetMatrix(const mat4& toWorld)
	{

	}

	void CGLScene::PopMatrix(void)
	{

	}

	const mat4* CGLScene::GetTopMatrix(void)
	{

	}


	CGLLightManager* CGLScene::GetLightManager(void)
	{

	}


	void CGLScene::AddAlphaSceneNode(CGLAlphaSceneNode *asn)
	{

	}

}