#include "GameState.h"
#include "Math/anMath.h"
#include "Core/anKeyCodes.h"
#include "PreloadedAssets.h"

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
	const int wWidth = (int)mApplication->GetWindow()->GetStartWidth();
	const int wHeight = (int)mApplication->GetWindow()->GetStartHeight();

	mfWidth = (float)wWidth;
	mfHeight = (float)wHeight;

	mIsLoading = true;
}

void GameState::Update(float dt)
{
	if (mIsLoading)
	{
		switch (mLoadingProccess)
		{
		// finish loading
		case 100:
			mIsLoading = false;
			break;
		// load images
		case 20:
			LoadImages();
			break;
		// initialize variables
		case 50:
			InitializeVariables();
			break;
		}

		++mLoadingProccess;
	}
	else
	{
		PlayerMovement(dt);
	}
}

void GameState::OnEvent(const anEvent& event)
{
	if (event.Type == anEvent::MouseMove)
	{
		mMousePos.X = event.MousePosition.X - mfWidth * 0.5f;
		mMousePos.Y = event.MousePosition.Y - mfHeight * 0.5f;
	}

	if (!mIsLoading)
	{
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

		if (event.Type == anEvent::MouseDown)
		{
			if (event.MouseButton == 1)
				mPlayerSpeed = mPlayerMaxSpeed;
		}

		if (event.Type == anEvent::MouseUp)
		{
			if (event.MouseButton == 1)
				mPlayerSpeed = mPlayerMinSpeed;
		}
	}
}

void GameState::Render(anRenderer& renderer)
{
	if (mIsLoading)
	{
		renderer.DrawTexture(anTexture::GetWhiteTexture(), { 0.0f, mLoadingProccessBarSize.Y * -0.5f }, mLoadingProccessBarSize, { 100, 100, 100 });
		renderer.DrawTexture(anTexture::GetWhiteTexture(), { mLoadingProccessBarSize.X * mLoadingProccess / 200 - mLoadingProccessBarSize.X * 0.5f, mLoadingProccessBarSize.Y * -0.5f }, { mLoadingProccessBarSize.X * mLoadingProccess / 100, 30.0f }, { 50, 180, 50 });
		renderer.DrawString(PreloadedAssets::GetRalewayFont(), { -2.0f * (float)PreloadedAssets::GetRalewayFontSize(), (float)PreloadedAssets::GetRalewayFontSize() + mLoadingProccessBarSize.Y * 0.5f }, "Loading", { 255, 255, 255 });
	}
	else
	{
		renderer.DrawTexture(mMapTexture, mMapPos - mCameraPos, mMapSize, { 255, 255, 255 });
		renderer.DrawTextureSub(mPlayerTexture, mPlayerPos, mPlayerSize, { (mPlayerHappinessStatus * 2 + mPlayerEyesClosed) * mPlayerSize.X, 0.0f }, mPlayerSize, mPlayerRot, { 255, 255, 255 });
	}
}

void GameState::PlayerMovement(float dt)
{
	anFloat2 direction = { anCos(anDegreesToRadians(mPlayerRot)), anSin(anDegreesToRadians(mPlayerRot)) };
	if (mKeyW)
		mPlayerAxis = { -direction.Y, -direction.X };

	if (mKeyS)
		mPlayerAxis = { direction.Y, direction.X };

	if (mKeyA)
		mPlayerAxis = { -direction.X, direction.Y };

	if (mKeyD)
		mPlayerAxis = { direction.X, -direction.Y };

	mPlayerPos += mPlayerAxis * anFloat2(mPlayerSpeed, mPlayerSpeed);
	mPlayerPos = Clamp2f(mPlayerPos, mPlayerMovementRectMin, mPlayerMovementRectMax);
	if (mPlayerPos.X == mPlayerMovementRectMin.X || mPlayerPos.X == mPlayerMovementRectMax.X)
		mCameraPos.X += mPlayerAxis.X * mPlayerSpeed;

	if (mPlayerPos.Y == mPlayerMovementRectMin.Y || mPlayerPos.Y == mPlayerMovementRectMax.Y)
		mCameraPos.Y += mPlayerAxis.Y * mPlayerSpeed;

	mCameraPos = Clamp2f(mCameraPos, anFloat2((mfWidth - mMapSize.X) * 0.5f, (mfHeight - mMapSize.Y) * 0.5f), anFloat2((mMapSize.X - mfWidth) * 0.5f, (mMapSize.Y - mfHeight) * 0.5f));

	mPlayerRot = -anRadiansToDegrees(anAtan2(mMousePos.Y - mPlayerPos.Y, mMousePos.X - mPlayerPos.X)) - 90.0f;

	if (mPlayerEyesClosed == 1)
	{
		if (mPlayerEyeCloseTimer >= 0.4f)
		{
			mPlayerEyesClosed = 0;
			mPlayerEyeCloseTimer = 0.0f;
		}
	}

	if (mPlayerEyesClosed == 0)
	{
		if (mPlayerEyeCloseTimer >= 3.5f)
		{
			mPlayerEyesClosed = 1;
			mPlayerEyeCloseTimer = 0.0f;
		}
	}
	
	mPlayerEyeCloseTimer += dt;
}

void GameState::LoadImages()
{
	// map
	mMapTexture = anLoadTexture("assets/map.png");

	const int iMapWidth = (int)mMapTexture->GetWidth();
	const int iMapHeight = (int)mMapTexture->GetHeight();

	mMapSize = { (float)iMapWidth, (float)iMapHeight };

	// player
	mPlayerTexture = anLoadTexture("assets/player.png");

	const int iPlayerWidth = (int)mPlayerTexture->GetWidth();
	const int iPlayerHeight = (int)mPlayerTexture->GetHeight();

	mPlayerSize = { (float)iPlayerWidth / 6.0f, (float)iPlayerHeight };
}

void GameState::InitializeVariables()
{
	const float playerMovementRectWidth = mfWidth * 0.75f;
	const float playerMovementRectHeight = mfHeight * 0.6f;

	mPlayerMovementRectMax = { playerMovementRectWidth * 0.5f, playerMovementRectHeight * 0.5f };
	mPlayerMovementRectMin = { playerMovementRectWidth * -0.5f, playerMovementRectHeight * -0.5f };
}
