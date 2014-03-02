#ifndef QSE_SCENE_HPP
#define QSE_SCENE_HPP

#include "SceneNode.hpp"


namespace Quicksand
{
	typedef std::map<ActorId, shared_ptr<IGLSceneNode> > SceneActorMap;


	//forward declarations
	class CGLCameraNode;
	class CGLSkyNode;
	class CGLLightNode;
	class CGLLightManager;
	class CGLMatrixStack;

	class CGLScene
	{

	protected:
		shared_ptr<CGLSceneNode> m_Root;
		shared_ptr<CGLCameraNode> m_Camera;
		shared_ptr<IGLRenderer> m_Renderer;

		CGLMatrixStack *m_MatrixStack;
		AlphaSceneNodes m_AlphaSceneNodes;
		SceneActorMap m_ActorMap;

		CGLLightManager *m_LightManager;

		void RenderAlphaPass(void);

	public:

		CGLScene(void);
		virtual ~CGLScene(void);

		unsigned long OnRender(void);
		unsigned long OnUpdate(void);

		shared_ptr<IGLSceneNode> FindActor(ActorId id);
		bool AddChild(ActorId id, shared_ptr<IGLSceneNode> child);

		bool RemoveChild(ActorId id);

		void SetCamera(shared_ptr<CGLCameraNode> camera);
		const shared_ptr<CGLCameraNode> GetCamera(void) const;

		void PushAndSetMatrix(const mat4& toWorld);
		void PopMatrix(void);
		const mat4* GetTopMatrix(void);

		CGLLightManager* GetLightManager(void);

		void AddAlphaSceneNode(CGLAlphaSceneNode *asn);

	};


}


#endif