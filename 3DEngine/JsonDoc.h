#pragma once
#include "Parson/parson.h"
#include <string>

class JsonDoc
{
public:
	JsonDoc();
	~JsonDoc();

	bool Init(const char* _path);
	void CleanUp();
	json_object_t* GetObj(const char* _name);
	json_object_t* GetObjObj(json_object_t* _obj, const char* _name);
	int	GetIntFromObj(const char* _obj, const char* _name);
	const char* GetCharFromObj(const char* _obj, const char* _name);
	int	GetIntFromObj(json_object_t* _obj, const char* _name);
	const char* GetCharFromObj(json_object_t* _obj, const char* _name);
	int GetObjIntValue(json_object_t* _obj);


private:
	std::string		path;
	json_value_t*	root;
	json_object_t*	jObject;

};

