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
	float mfWidth = 0.0f;
	float mfHeight = 0.0f;

	anFloat2 mMousePos;

	float mPlayerRot = 0.0f;
	anFloat2 mPlayerSize = { 50.0f, 50.0f };
	anFloat2 mPlayerPos;
};

#endif
