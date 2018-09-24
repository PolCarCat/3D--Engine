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
	json_object_t* GetObject(const char* _name);
	int	GetIntFromObj(const char* _obj, const char* _name);
	const char* GetCharFromObj(const char* _obj, const char* _name);


private:
	std::string		path;
	json_value_t*	root;
	json_object_t*	jObject;
	json_array_t*	jArray;
};

