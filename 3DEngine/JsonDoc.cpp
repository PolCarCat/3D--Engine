#include "JsonDoc.h"
#include <fstream>


JsonDoc::JsonDoc()
{
}


JsonDoc::~JsonDoc()
{
}


bool JsonDoc::Init(const char* _path)
{
	bool ret = true;
	

	std::ifstream infile(_path);

	if (!infile.good())
	{
		std::ofstream file(_path);
	
	}


	root = json_value_init_object();
	root = json_parse_file(_path);

	path = _path;


	if (json_value_get_type(root) == JSONNull) 
		ret = false;

	
	rootObj = json_value_get_object(root);


	return ret;
}

void JsonDoc::CleanUp()
{
	json_value_free(root);
	json_object_clear(rootObj);
}

void JsonDoc::Save()
{

	json_serialize_to_file_pretty(root, path.c_str());
	//json_value_free(root);
	

}

json_object_t* JsonDoc::GetObj(const char* _name)
{
	json_object_t* obj = nullptr;
	obj = json_object_get_object(rootObj, _name);

	return obj;
}

JSON_Array* JsonDoc::GetAr(const char* _name)
{
	return SetArray(rootObj, _name);
}

json_object_t* JsonDoc::GetObjObj(json_object_t* _obj, const char* _name)
{
	return json_object_get_object(_obj, _name);
}

JSON_Object* JsonDoc::GetArObj(JSON_Array* parent, uint index)
{
	return json_array_get_object(parent, index);
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


json_object_t* JsonDoc::GetRootObj()
{
	return rootObj;
}

JSON_Object* JsonDoc::SetObj(JSON_Object* parent, const char* name)
{	
	json_object_set_value(parent, name, json_value_init_object());
	return GetObjObj(parent, name);
}

JSON_Object* JsonDoc::SetObj(JSON_Array* parent)
{
	json_array_append_value(parent, json_value_init_object());
	uint size =	json_array_get_count(parent);
	return GetArObj(parent, size - 1);
}

JSON_Array* JsonDoc::SetArray(JSON_Object* parent, const char* name)
{
	JSON_Value* va = json_value_init_array();
	JSON_Array* array = json_value_get_array(va);
	json_object_set_value(parent, name, va);

	return array;
}