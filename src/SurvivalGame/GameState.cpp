#include "GameState.h"
#include "Math/anMath.h"

GameState::GameState(anApplication* app)
	: anState(app)
{
}

GameState::~GameState()
{
}

void GameState::Initialize()
{
	const int wWidth = (int)mApplication->GetWindow()->GetStartWidth();
	const int wHeight = (int)mApplication->GetWindow()->GetStartHeight();

	mfWidth = (float)wWidth;
	mfHeight = (float)wHeight;
}

void GameState::Update(float dt)
{
	mPlayerRot = -anRadiansToDegrees(anAtan2(mMousePos.Y - mPlayerPos.Y, mMousePos.X - mPlayerPos.X));
}

void GameState::OnEvent(const anEvent& event)
{
	if (event.Type == anEvent::MouseMove)
	{
		mMousePos.X = event.MousePosition.X - mfWidth * 0.5f;
		mMousePos.Y = event.MousePosition.Y - mfHeight * 0.5f;
	}
}

void GameState::Render(anRenderer& renderer)
{
	renderer.DrawTexture(anTexture::GetWhiteTexture(), mPlayerPos, mPlayerSize, mPlayerRot, { 255, 255, 255 });
}
