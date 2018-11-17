#ifndef __COMPONENT_MATERIAL_H__
#define __COMPONENT_MATERIAL_H__

#include "Component.h"
#include "ResTexture.h"
#include "Color.h"


class ComponentMaterial :
	public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	bool Start();
	bool Update();
	void UpdateUI();
	bool CleanUp();
	bool Save(JSON_Object* json, JsonDoc* doc);
	bool Load(JSON_Object* json, JsonDoc* doc);

	void SetName(const char* n);
	void SetName(std::string n);
	const char* GetName() const;
	ResTexture* GetTexture() const;
	void SetTexture(ResTexture* tex);
	bool texEnabled = true;
	Color color;

private:

	ResTexture* tex = nullptr;
	std::string name;
};

#endif //__COMPONENT_MATERIAL__