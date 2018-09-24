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

	//Test
	json_object_t* obj2 = json_object_get_object(jObject, "Window");

	return ret;
}

void JsonDoc::CleanUp()
{
	json_value_free(root);
}


json_object_t* JsonDoc::GetObj(const char* _name)
{
	json_object_t* obj = nullptr;
	obj = json_object_get_object(jObject, _name);

	return obj;
}

json_object_t* GetObjObj(json_object_t* _obj, const char* _name)
{
	return json_object_get_object(_obj, _name);
}

int	JsonDoc::GetIntFromObj(const char* _obj, const char* _name)
{
	json_object_t* obj = GetObj(_obj);
	return (int)json_object_get_number(obj, _name);
}

const char* JsonDoc::GetCharFromObj(const char* _obj, const char* _name)
{
	json_object_t* obj = GetObj(_obj);
	return json_object_dotget_string(obj, _name);
}

int	GetIntFromObj(json_object_t* _obj, const char* _name)
{
	return (int)json_object_get_number(_obj, _name);
}

const char* GetCharFromObj(json_object_t* _obj, const char* _name)
{
	return json_object_dotget_string(_obj, _name);
}


int GetObjIntValue(json_object_t* _obj, const char* _name)
{
	//Returns -1 if not correcty

	JSON_Value* v = json_object_get_value(_obj, _name);
	int i = -1;

	if (json_value_get_type(v) == JSONNumber)
	{
		i = json_value_get_number(v);
	}

	return i;
}