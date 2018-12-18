#ifndef __JSONDOC_H__
#define __JSONDOC_H__

#include "Parson/parson.h"
#include <string>
#include "Globals.h"
#include "Color.h"
#include "MathGeoLib/MathGeoLib.h"

class JsonDoc
{
public:
	JsonDoc();
	~JsonDoc();

	bool Init(const char* _path);
	bool Init();
	void CleanUp();
	void Save();
	
	json_object_t* GetRootObj();
	json_object_t* GetObj(const char* _name);
	JSON_Array* SetAr(const char* _name);
	JSON_Array* GetAr(const char* _name);
	json_object_t* GetObjObj(json_object_t* _obj, const char* _name);
	JSON_Array* GetObjAr(JSON_Object* parent, const char* _name);
	JSON_Object* SetObj(JSON_Object* parent, const char* name);
	JSON_Object* SetObj(JSON_Array* parent);
	JSON_Array* SetArray(JSON_Object* parent, const char* name);
	JSON_Object* GetArObj(JSON_Array* parent, uint index);

	int GetObjValueInt(json_object_t* _obj, const char* _name);
	float GetObjValueFloat(json_object_t* _obj, const char* _name);
	const char* GetObjValueString(json_object_t* _obj, const char* _name);
	bool GetObjValueBool(json_object_t* _obj, const char* _name);

	void SaveRange(JSON_Object * json,const char* name,  range<float> range);
	void SaveRange(JSON_Object * json, const char* name, range<int> range);
	void SaveColor(JSON_Object * json, const char* name, Color color);
	void SaveFloat3(JSON_Object * json, const char* name, float3 vect);

	range<float> LoadRange(JSON_Object * json, const char* name);
	Color LoadColor(JSON_Object * json, const char* name);
	float3 LoadFloat3(JSON_Object* json, const char* name);

private:
	std::string		path;
	json_value_t*	root = nullptr;
	json_object_t*	rootObj = nullptr;

};

#endif //__JSONDOC_H__