#ifndef SCENE_HPP
#define SCENE_HPP

#include "SceneNode.hpp"


namespace Quicksand
{
	typedef std::map<ActorId, shared_ptr<IGLSceneNode> > SceneActorMap;


	//forward declarations
	class GLCameraNode;
	class GLSkyNode;
	class GLLightNode;
	class GLLightManager;
	class GLMatrixStack;

	class GLScene
	{

	protected:
		shared_ptr<GLSceneNode> m_Root;
		shared_ptr<GLCameraNode> m_Camera;
		shared_ptr<IGLRenderer> m_Renderer;

		GLMatrixStack *m_MatrixStack;
		AlphaSceneNodes m_AlphaSceneNodes;
		SceneActorMap m_ActorMap;

		GLLightManager *m_LightManager;

		void RenderAlphaPass(void);

	public:

		GLScene(void);
		virtual ~GLScene(void);

		unsigned long OnRender(void);
		unsigned long OnUpdate(void);

		shared_ptr<IGLSceneNode> FindActor(ActorId id);
		bool AddChild(ActorId id, shared_ptr<IGLSceneNode> child);

		bool RemoveChild(ActorId id);

		void SetCamera(shared_ptr<GLCameraNode> camera);
		const shared_ptr<GLCameraNode> GetCamera(void) const;

		void PushAndSetMatrix(const mat4& toWorld);
		void PopMatrix(void);
		const mat4* GetTopMatrix(void);

		GLLightManager* GetLightManager(void);

		void AddAlphaSceneNode(GLAlphaSceneNode *asn);

	};


}


#endif