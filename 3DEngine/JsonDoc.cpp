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


json_object_t* JsonDoc::GetObject(const char* _name)
{
	json_object_t* obj = nullptr;
	//obj = json_object_get_object(jObject, _name);

	bool end = false;
	int i = 0;
	while (!end)
	{
		
	}
	
	return obj;
}

int	JsonDoc::GetIntFromObj(const char* _obj, const char* _name)
{
	json_object_t* obj = GetObject(_obj);
	return (int)json_object_get_number(obj, _name);
}

const char* JsonDoc::GetCharFromObj(const char* _obj, const char* _name)
{
	json_object_t* obj = GetObject(_obj);
	return json_object_dotget_string(obj, _name);
}


void print_commits_info(const char *username, const char *repo) {
	JSON_Value *root_value;
	JSON_Array *commits;
	JSON_Object *commit;
	size_t i;

	char curl_command[512];
	char cleanup_command[256];
	char output_filename[] = "commits.json";

	/* it ain't pretty, but it's not a libcurl tutorial */
	sprintf(curl_command,
		"curl -s \"https://api.github.com/repos/%s/%s/commits\" > %s",
		username, repo, output_filename);
	sprintf(cleanup_command, "rm -f %s", output_filename);
	system(curl_command);

	/* parsing json and validating output */
	root_value = json_parse_file(output_filename);
	if (json_value_get_type(root_value) != JSONArray) {
		system(cleanup_command);
		return;
	}

	/* getting array from root value and printing commit info */
	commits = json_value_get_array(root_value);
	printf("%-10.10s %-10.10s %s\n", "Date", "SHA", "Author");


	/* cleanup code */
	json_value_free(root_value);
	system(cleanup_command);
}

void persistence_example(void) {
	JSON_Value *schema = json_parse_string("{\"name\":\"\"}");
	JSON_Value *user_data = json_parse_file("user_data.json");
	char buf[256];
	const char *name = NULL;
	if (user_data == NULL || json_validate(schema, user_data) != JSONSuccess) {
		puts("Enter your name:");
		scanf("%s", buf);
		user_data = json_value_init_object();
		json_object_set_string(json_object(user_data), "name", buf);
		json_serialize_to_file(user_data, "user_data.json");
	}
	name = json_object_get_string(json_object(user_data), "name");
	printf("Hello, %s.", name);
	json_value_free(schema);
	json_value_free(user_data);
	return;
}