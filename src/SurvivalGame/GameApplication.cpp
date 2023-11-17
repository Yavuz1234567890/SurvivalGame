#include "Core/anApplication.h"
#include "Core/anEntryPoint.h"
#include "Renderer/anRenderer.h"
#include "Device/anGPUCommands.h"
#include "World/anWorld.h"
#include "Core/anMessage.h"
#include "Core/anKeyCodes.h"
#include "State/anStateManager.h"
#include "Math/anMath.h"
#include "GameState.h"
#include "PreloadedAssets.h"

class GameApplication : public anApplication
{
public:
	GameApplication()
		: anApplication({ "Survival", 1200, 700, false })
	{
	}

	~GameApplication()
	{
	}

	void Initialize() override
	{
		PreloadedAssets::Load();

		mWindow->SetWindowIcon("icon.png");

		anInitializeRandomDevice();

		mWorld = new anWorld();

		mRenderer.Initialize();

		mfWidth = (float)mApplicationDesc.Width;
		mfHeight = (float)mApplicationDesc.Height;
	
		mProjection = anMatrix4::Ortho(mfWidth * -0.5f, mfWidth * 0.5f, mfHeight * 0.5f, mfHeight * -0.5f, -1.0f, 1.0f);
		mRenderer.SetMatrix(mProjection);

		mWorld->Initialize();

		SetCurrentState<GameState>();
	}

	void Update(float dt) override
	{
		mWorld->Update(dt);
		
		anClear();
		anEnableBlend();

		mRenderer.Start();

		anApplication::Render(mRenderer);
		mWorld->Render(mRenderer);

		mRenderer.End();
	}
	
	void OnEvent(const anEvent& event) override
	{
	}

private:
	anRenderer mRenderer;

	anMatrix4 mProjection;

	anScene* mScene = nullptr;
	anWorld* mWorld = nullptr;

	float mfWidth = 0.0f;
	float mfHeight = 0.0f;
};

int anStartApplication(char** args, int argc)
{
	GameApplication* app = new GameApplication();
	app->Start();

	return 0;
}
