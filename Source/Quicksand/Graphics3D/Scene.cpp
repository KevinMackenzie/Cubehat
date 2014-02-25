#include "stdafx.h"
#include "Utils3D.hpp"
#include "Scene.hpp"

namespace Quicksand
{

	void GLScene::RenderAlphaPass(void)
	{

	}

	GLScene::GLScene(void)
	{

	}

	GLScene::~GLScene(void)
	{

	}


	unsigned long GLScene::OnRender(void)
	{

	}

	unsigned long GLScene::OnUpdate(void)
	{

	}


	shared_ptr<IGLSceneNode> GLScene::FindActor(ActorId id)
	{

	}

	bool GLScene::AddChild(ActorId id, shared_ptr<IGLSceneNode> child)
	{

	}


	bool GLScene::RemoveChild(ActorId id)
	{

	}


	void GLScene::SetCamera(shared_ptr<GLCameraNode> camera)
	{

	}

	const shared_ptr<GLCameraNode> GLScene::GetCamera(void) const
	{

	}


	void GLScene::PushAndSetMatrix(const mat4& toWorld)
	{

	}

	void GLScene::PopMatrix(void)
	{

	}

	const mat4* GLScene::GetTopMatrix(void)
	{

	}


	GLLightManager* GLScene::GetLightManager(void)
	{

	}


	void GLScene::AddAlphaSceneNode(GLAlphaSceneNode *asn)
	{

	}

}