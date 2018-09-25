#include "JsonDoc.h"



JsonDoc::JsonDoc()
{
}


JsonDoc::~JsonDoc()
{
}


bool JsonDoc::Init(const char* _path)
{
	bool ret = true;

	root = json_value_init_object();
	root = json_parse_file(_path);
	path = _path;


	if (json_value_get_type(root) == JSONNull) 
		ret = false;

	
	jObject = json_value_get_object(root);


	return ret;
}

void JsonDoc::CleanUp()
{
	json_value_free(root);
}

void JsonDoc::Save()
{
	
	size_t written = json_serialization_size(root);
	char* text = new char[written];
	json_serialize_to_buffer(root, text, written);
	
	delete text;
}

json_object_t* JsonDoc::GetObj(const char* _name)
{
	json_object_t* obj = nullptr;
	obj = json_object_get_object(jObject, _name);

	return obj;
}

json_object_t* JsonDoc::GetObjObj(json_object_t* _obj, const char* _name)
{
	return json_object_get_object(_obj, _name);
}



int JsonDoc::GetObjValueInt(json_object_t* _obj, const char* _name)
{
	//Returns -1 if not correct

	JSON_Value* v = json_object_get_value(_obj, _name);
	int i = -1;

	if (json_value_get_type(v) == JSONNumber)
	{
		i = json_value_get_number(v);
	}

	return i;
}

float JsonDoc::GetObjValueFloat(json_object_t* _obj, const char* _name)
{
	//Returns -1 if not correct

	JSON_Value* v = json_object_get_value(_obj, _name);
	float i = -1;

	if (json_value_get_type(v) == JSONNumber)
	{
		i = json_value_get_number(v);
	}

	return i;
}

const char* JsonDoc::GetObjValueString(json_object_t* _obj, const char* _name)
{

	JSON_Value* v = json_object_get_value(_obj, _name);
	const char* i = nullptr;

	if (json_value_get_type(v) == JSONString)
	{
		i = json_value_get_string(v);
	}

	return i;
}

bool JsonDoc::GetObjValueBool(json_object_t* _obj, const char* _name)
{
	JSON_Value* v = json_object_get_value(_obj, _name);
	bool i = false;

	if (json_value_get_type(v) == JSONBoolean)
	{
		i = json_value_get_boolean(v);
	}

	return i;
}