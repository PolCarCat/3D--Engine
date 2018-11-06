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

private:
	Timer RealTime;
	uint FrameCount;
	float GameTime;
	float Time_Scale;
	float Delta_Time;
	float Real_Delta_Time;
};