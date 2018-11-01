#pragma once
#include "Parson/parson.h"
#include <string>
#include "Globals.h"

class JsonDoc
{
public:
	JsonDoc();
	~JsonDoc();

	bool Init(const char* _path);
	void CleanUp();
	void Save();
	
	json_object_t* GetRootObj();
	json_object_t* GetObj(const char* _name);
	JSON_Array* GetAr(const char* _name);
	json_object_t* GetObjObj(json_object_t* _obj, const char* _name);
	JSON_Object* SetObj(JSON_Object* parent, const char* name);
	JSON_Object* SetObj(JSON_Array* parent);
	JSON_Array* SetArray(JSON_Object* parent, const char* name);
	JSON_Object* GetArObj(JSON_Array* parent, uint index);

	int GetObjValueInt(json_object_t* _obj, const char* _name);
	float GetObjValueFloat(json_object_t* _obj, const char* _name);
	const char* GetObjValueString(json_object_t* _obj, const char* _name);
	bool GetObjValueBool(json_object_t* _obj, const char* _name);



private:
	std::string		path;
	json_value_t*	root;
	json_object_t*	rootObj;

};

