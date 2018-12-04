#ifndef __COMPONENTBILLBOARD_H__
#define __COMPONENTBILLBOARD_H__

#include "Component.h"

class ResMesh;
class ResTexture;
class ComponentCamera;
class ComponentTransform;

enum Alignment {
	SCREEN_ALIGN,
	WORLD_ALIGN,
	AXIAL_ALIGN
};

class ComponentBillboard :
	public Component
{
public:
	ComponentBillboard();
	~ComponentBillboard();

	bool Start();
	bool Update();
	void UpdateUI();
	bool CleanUp();
	void FaceCamera();

	void SetTexture(ResTexture* t);

	Alignment alignment = SCREEN_ALIGN;
	ComponentTransform* transform = nullptr;

private:

	ResMesh* mesh = nullptr;
	ResTexture* tex = nullptr;

};

#endif // !__COMPONENTBILLBOARD_H__