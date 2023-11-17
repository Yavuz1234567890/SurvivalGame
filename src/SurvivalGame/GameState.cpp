#include "GameState.h"
#include "Math/anMath.h"
#include "Core/anKeyCodes.h"

#define CLAMP(x, y, z) x <= y ? y : x >= z ? z : x
static anFloat2 Clamp2f(const anFloat2& x, const anFloat2& y, const anFloat2& z)
{
	return anFloat2(CLAMP(x.X, y.X, z.X), CLAMP(x.Y, y.Y, z.Y));
}

GameState::GameState(anApplication* app)
	: anState(app)
{
}

GameState::~GameState()
{
}

void GameState::Initialize()
{
	mMapTexture = anLoadTexture("assets/map.png");
	
	const int iMapWidth = (int)mMapTexture->GetWidth();
	const int iMapHeight = (int)mMapTexture->GetHeight();

	mMapSize = { (float)iMapWidth, (float)iMapHeight };

	const int wWidth = (int)mApplication->GetWindow()->GetStartWidth();
	const int wHeight = (int)mApplication->GetWindow()->GetStartHeight();

	mfWidth = (float)wWidth;
	mfHeight = (float)wHeight;

	const float playerMovementRectWidth = mfWidth * 0.75f;
	const float playerMovementRectHeight = mfHeight * 0.6f;

	mPlayerMovementRectMax = { playerMovementRectWidth * 0.5f, playerMovementRectHeight * 0.5f };
	mPlayerMovementRectMin = { playerMovementRectWidth * -0.5f, playerMovementRectHeight * -0.5f };
}

void GameState::Update(float dt)
{
	anFloat2 direction = { anCos(anDegreesToRadians(mPlayerRot)), anSin(anDegreesToRadians(mPlayerRot)) };
	if (mKeyW)
		mPlayerAxis = { direction.X, -direction.Y };

	if (mKeyS)
		mPlayerAxis = { -direction.X, direction.Y };

	if (mKeyA)
		mPlayerAxis = { -direction.Y, -direction.X };

	if (mKeyD)
		mPlayerAxis = { direction.Y, direction.X };

	mPlayerPos += mPlayerAxis;
	mPlayerPos = Clamp2f(mPlayerPos, mPlayerMovementRectMin, mPlayerMovementRectMax);
	if (mPlayerPos.X == mPlayerMovementRectMin.X || mPlayerPos.X == mPlayerMovementRectMax.X)
		mCameraPos.X += mPlayerAxis.X;

	if (mPlayerPos.Y == mPlayerMovementRectMin.Y || mPlayerPos.Y == mPlayerMovementRectMax.Y)
		mCameraPos.Y += mPlayerAxis.Y;

	mCameraPos = Clamp2f(mCameraPos, anFloat2((mfWidth - mMapSize.X) * 0.5f, (mfHeight - mMapSize.Y) * 0.5f), anFloat2((mMapSize.X - mfWidth) * 0.5f, (mMapSize.Y - mfHeight) * 0.5f));

	mPlayerRot = -anRadiansToDegrees(anAtan2(mMousePos.Y - mPlayerPos.Y, mMousePos.X - mPlayerPos.X));
}

void GameState::OnEvent(const anEvent& event)
{
	if (event.Type == anEvent::MouseMove)
	{
		mMousePos.X = event.MousePosition.X - mfWidth * 0.5f;
		mMousePos.Y = event.MousePosition.Y - mfHeight * 0.5f;
	}

	if (event.Type == anEvent::KeyDown)
	{
		if (event.KeyCode == anKeyW)
			mKeyW = true;

		if (event.KeyCode == anKeyS)
			mKeyS = true;

		if (event.KeyCode == anKeyA)
			mKeyA = true;

		if (event.KeyCode == anKeyD)
			mKeyD = true;
	}

	if (event.Type == anEvent::KeyUp)
	{
		if (event.KeyCode == anKeyW)
			mKeyW = false;

		if (event.KeyCode == anKeyS)
			mKeyS = false;

		if (event.KeyCode == anKeyA)
			mKeyA = false;

		if (event.KeyCode == anKeyD)
			mKeyD = false;

		if (!mKeyW && !mKeyS && !mKeyA && !mKeyD)
			mPlayerAxis = {};
	}
}

void GameState::Render(anRenderer& renderer)
{
	renderer.DrawTexture(mMapTexture, mMapPos - mCameraPos, mMapSize, { 255, 255, 255 });
	renderer.DrawTexture(anTexture::GetWhiteTexture(), mPlayerPos, mPlayerSize, mPlayerRot, { 255, 255, 255 });
}
