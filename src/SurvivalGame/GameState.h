#ifndef TEST_STATE_H_
#define TEST_STATE_H_

#include "State/anState.h"

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

	float mfWidth = 0.0f;
	float mfHeight = 0.0f;

	anFloat2 mCameraPos;
	
	anFloat2 mMousePos;

	bool mKeyW = false;
	bool mKeyS = false;
	bool mKeyA = false;
	bool mKeyD = false;

	float mPlayerRot = 0.0f;
	anFloat2 mPlayerSize = { 50.0f, 50.0f };
	anFloat2 mPlayerPos;
	anFloat2 mPlayerAxis;
	anFloat2 mPlayerMovementRectMax;
	anFloat2 mPlayerMovementRectMin;

	anTexture* mMapTexture = nullptr;
	anFloat2 mMapSize;
	anFloat2 mMapPos;

	void PlayerMovement(float dt);
	void LoadImages();
	void InitializeVariables();
};

#endif
