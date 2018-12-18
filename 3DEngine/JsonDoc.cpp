#include "JsonDoc.h"
#include "Application.h"
#include <fstream>


#include "mmgr/mmgr.h"


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
	if (root == nullptr)
		root = json_value_init_object();

	path = _path;


	if (json_value_get_type(root) == JSONNull) 
		ret = false;

	
	rootObj = json_value_get_object(root);


	return ret;
}

bool JsonDoc::Init()
{
	//Generates a root obj in memory
	bool ret = true;
	root = json_value_init_object();

	if (json_value_get_type(root) == JSONNull)
		ret = false;

	rootObj = json_value_get_object(root);


	return ret;
}

void JsonDoc::CleanUp()
{
	json_value_free(root);
	path.clear();
	root = nullptr;
	rootObj = nullptr;
}

void JsonDoc::Save()
{

	if (json_serialize_to_file_pretty(root, path.c_str()) == JSONFailure)
	{
		char* str = json_serialize_to_string_pretty(root);
		App->fileSystem.SaveFile(path.c_str(), str, strlen(str));
	}
	//json_value_free(root);
	

}

json_object_t* JsonDoc::GetObj(const char* _name)
{
	json_object_t* obj = nullptr;
	obj = json_object_get_object(rootObj, _name);

	return obj;
}

JSON_Array* JsonDoc::SetAr(const char* _name)
{
	return SetArray(rootObj, _name);
}

JSON_Array* JsonDoc::GetAr(const char* _name)
{
	return json_object_dotget_array(rootObj, _name);
}

json_object_t* JsonDoc::GetObjObj(json_object_t* _obj, const char* _name)
{
	return json_object_get_object(_obj, _name);
}

JSON_Object* JsonDoc::GetArObj(JSON_Array* parent, uint index)
{
	return json_array_get_object(parent, index);
}

JSON_Array* JsonDoc::GetObjAr(JSON_Object* parent, const char* _name)
{
	return json_object_dotget_array(parent, _name);
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

void JsonDoc::SaveRange(JSON_Object * json, const char* name, range<float> range)
{
	std::string min = name;
	min += "Min";

	std::string max = name;
	max += "Max";

	json_object_dotset_number(json, min.c_str(), range.min);
	json_object_dotset_number(json, max.c_str(), range.max);
}

void JsonDoc::SaveRange(JSON_Object * json, const char* name, range<int> range)
{
	std::string min = name;
	min += "Min";

	std::string max = name;
	max += "Max";

	json_object_dotset_number(json, min.c_str(), range.min);
	json_object_dotset_number(json, max.c_str(), range.max);
}

void JsonDoc::SaveColor(JSON_Object * json, const char* name, Color color)
{

	JSON_Array* col = SetArray(json, name);
	json_array_append_number(col, color.r);
	json_array_append_number(col, color.g);
	json_array_append_number(col, color.b);
	json_array_append_number(col, color.a);
}

void JsonDoc::SaveFloat3(JSON_Object * json, const char * name, float3 vect)
{
	JSON_Array* array = SetArray(json, name);
	json_array_append_number(array, vect.x);
	json_array_append_number(array, vect.y);
	json_array_append_number(array, vect.z);

}

range<float> JsonDoc::LoadRange(JSON_Object * json, const char* name)
{
	range<float> ret;
	std::string min = name;
	min += "Min";

	std::string max = name;
	max += "Max";

	ret.min = json_object_dotget_number(json, min.c_str());
	ret.max = json_object_dotget_number(json, max.c_str());
	
	return ret;
}

Color JsonDoc::LoadColor(JSON_Object * json, const char* name)
{
	Color ret;


	JSON_Array* pos = GetObjAr(json, name);
	ret.r = json_array_get_number(pos, 0);
	ret.g = json_array_get_number(pos, 1);
	ret.b = json_array_get_number(pos, 2);
	ret.a = json_array_get_number(pos, 3);

	return ret;
}

float3 JsonDoc::LoadFloat3(JSON_Object * json, const char * name)
{
	float3 ret;

	JSON_Array* pos = GetObjAr(json, name);
	ret.x = json_array_get_number(pos, 0);
	ret.y = json_array_get_number(pos, 1);
	ret.z = json_array_get_number(pos, 2);

	return ret;
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