#ifndef TEST_STATE_H_
#define TEST_STATE_H_

#include "State/anState.h"

namespace PlayerHappinessStatus
{
	enum : anUInt32
	{
		Normal = 0,
		Happy,
		Unhappy
	};
}

class GameState : public anState
{
public:
	GameState(anApplication* app);
	~GameState();

	void Initialize() override;
	void Update(float dt) override;
	void OnEvent(const anEvent& event) override;
	void Render(anRenderer& renderer) override;
private:
	bool mIsLoading = false;
	int mLoadingProccess = 0;
	anFloat2 mLoadingProccessBarSize = { 200.0f, 30.0f };

	float mfWidth = 0.0f;
	float mfHeight = 0.0f;

	anFloat2 mCameraPos;
	
	anFloat2 mMousePos;

	bool mKeyW = false;
	bool mKeyS = false;
	bool mKeyA = false;
	bool mKeyD = false;

	float mPlayerRot = 0.0f;
	anFloat2 mPlayerSize;
	anFloat2 mPlayerPos;
	anFloat2 mPlayerAxis;
	anFloat2 mPlayerMovementRectMax;
	anFloat2 mPlayerMovementRectMin;
	float mPlayerMinSpeed = 1.8f;
	float mPlayerMaxSpeed = 2.5f;
	float mPlayerSpeed = mPlayerMinSpeed;
	anUInt32 mPlayerHappinessStatus = PlayerHappinessStatus::Normal;
	anTexture* mPlayerTexture;
	anUInt32 mPlayerEyesClosed = 0;
	float mPlayerEyeCloseTimer = 0.0f;

	anTexture* mMapTexture = nullptr;
	anFloat2 mMapSize;
	anFloat2 mMapPos;

	void PlayerMovement(float dt);
	void LoadImages();
	void InitializeVariables();
};

#endif
