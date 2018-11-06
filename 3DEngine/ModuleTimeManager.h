#include "Module.h"
#include "Timer.h"

class ModuleTimeManager :
	public Module
{
public:
	ModuleTimeManager(Application* app, bool start_enabled = true);
	~ModuleTimeManager();

	bool Start();

	update_status PreUpdate(float dt);

	update_status Update(float dt);

	update_status PostUpdate(float dt);

	void StartTimers();
	void Reset();

	float GetRealTime();
	float GetGameTime();
	float GetTimeScale();
	float GetDeltaTime();
	float GetRealDeltaTime();
	bool GetPlaying();

	void SetTimeScale(float ts);
	void SetPlaying(bool p);

private:
	Timer realTime;
	uint frameCount = 0;
	float gameTime = 0;
	float timeScale = 0;
	float deltaTime = 0;
	float realDeltaTime = 0;

	bool playing = false;
};