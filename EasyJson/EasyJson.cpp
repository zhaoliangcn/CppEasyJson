// EasyJson.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CppEasyJson.h"
#include "encode.hpp"
const char * JSONSTRING = "{/*this is test comment*/\"firstName\":\"Bret\\b\\t\\\"t\\u9001\"}";
const char * JSONSTRING1 = "//thiis commentline \
{\"firstName\":\"Brett\",/*this is test comment*/\"lastName\" : \"McLaughlin\",\"email\" : \"aaaa\" }";
const char * JSONSTRING2 = "{\"people\":[{\"firstName\":\"Brett\",\"lastName\":\"McLaughlin\",\"email\":\"aaaa\"},{\"firstName\":\"Jason\",\"lastName\":\"Hunter\",\"email\":\"bbbb\"},{\"firstName\":\"Elliotte\",\"lastName\":\"Harold\",\"email\":\"cccc\"}]}";
const char * JSONSTRING3 = "{ \"name\":\"中国\",\"province\":[{\"name\":\"黑龙江\",\"cities\": {\"city\": [\"哈尔滨\",\"大庆\"]}},{\"name\": \"广东\",\"cities\": {\"city\": [\"广州\", \"深圳\", \"珠海\"]}},{\"name\": \"台湾\",\"cities\": {\"city\": [\"台北\", \"高雄\"]}},{\"name\":\"新疆\",\"cities\":{\"city\":[\"乌鲁木齐\"]}}]}";
const char * JSONSTRING33 = "{ \"name\":\"china\",\"province\":[{\"name\":\"heilongjiang\",\"cities\": {\"city\": [\"harbin\",\"daqing\"]}},{\"name\": \"guangdong\",\"cities\": {\"city\": [\"guangzhou\", \"shenzhen\", \"zhuhai\"]}},{\"name\": \"taiwan\",\"cities\": {\"city\": [\"taibei\", \"gaoxiong\"]}},{\"name\":\"xinjiang\",\"cities\":{\"city\":[\"wulumuqi\"]}}]}";
const char * JSONSTRING4 = "{ \"name\":\"china\",\"province\":[{\"name\":\"heilongjiang\",\"cities\": {\"city\": [\"harbin\",\"daqing\"]}}]}";
const char * JSONSTRING5 = " {\"key\": 0}";
const char * JSONSTRING6 = "{\"key\": \"x\"}";
const char * JSONSTRING7 = "{\"key\": []}";
const char * JSONSTRING8 = "{\"key\": {\"subkey\": 2901}}";
const char * JSONSTRING9 = "{\"key\": 0,\"otherkey\" : 12}";
const char * JSONSTRING10 = "{\"key1\": true,\"key2\":false,\"key3\":333,\"key4\":3.14,\"key5\":\"key5\"}";
const char * JSONSTRING11 = "{\"abc\":{\"x\": true,\"y\": false},\"key\":[1,2,true,false,4,4.90241,{},22013,14],\"xxx\":{\"y\":{ },\"z\":{ },\"abc\":{},\"def\":null},\"qwr\": [48559,{},\"abc\",true]}";
const char * JSONSTRING12 = "{ \"type\": \"FeatureCollection\",\"features\" : [ { \"type\": \"Feature\",\"geometry\" : {\"type\": \"Point\", \"coordinates\" : [102.0, 0.5]},\"properties\" : {\"prop0\": \"value0\"}},{ \"type\": \"Feature\",\"geometry\" : {\"type\": \"LineString\",\"coordinates\" : [	[102.0, 0.0],[103.0, 1.0],[104.0, 0.0],[105.0, 1.0]	]},\"properties\": {\"prop0\": \"value0\",\"prop1\" : 0.0	}},{\"type\": \"Feature\",\"geometry\" : {\"type\": \"Polygon\",\"coordinates\" : [	[[100.0, 0.0],[101.0, 0.0],[101.0, 1.0],[100.0, 1.0],[100.0, 0.0]]]},\"properties\": {\"prop0\": \"value0\",\"prop1\" : {\"this\": \"that\"}}}]}";
const char * JSONSTRING14 = "{\"jsontext\": \"{\\\"object with 1 member\\\":[\\\"array with 1 element\\\"]}\" }";
void Test25()
{
	CppEasyJson ejson;
	ejson.ParseFile("F:\\CppEasyJson\\good\\c-comment.json");
	printf("%s", UToA(ejson.ToString()).c_str());
	ejson.Release();
}
void Test24()
{
	CppEasyJson ejson;
	ejson.ParseFile("F:\\CppEasyJson\\good\\complex0.json");
	printf("%s", UToA(ejson.ToString()).c_str());
	ejson.Release();
}


void Test23()
{
	CppEasyJson ejson;
	ejson.ParseFile("F:\\CppEasyJson\\good\\multiobj.json");
	printf("%s", UToA(ejson.ToString()).c_str());
	ejson.Release();
}

void Test22()
{
	CppEasyJson ejson;
	ejson.ParseFile("F:\\CppEasyJson\\good\\utf8.json");
	printf("%s", UToA(ejson.ToString()).c_str());
	ejson.Release();
}

void Test21()
{
	CppEasyJson ejson;
	ejson.ParseFile("F:\\CppEasyJson\\good\\yaml-comment.json");
	printf("%s", UToA(ejson.ToString()).c_str());
	ejson.Release();
}

void Test20()
{
	CppEasyJson ejson;
	ejson.ParseString("{abcd:1234}");
	printf("%s", UToA(ejson.ToString()).c_str());
	ejson.Release();
}
void Test19()
{
	CppEasyJson ejson;
	JsonNode * root = ejson.GetRoot();
	if (root == NULL)
	{
		root = ejson.CreateJsonNode(NODE_OBJECT);
		ejson.SetRoot(root);
		ejson.AppendValue(root, "hello", "json");
		ejson.AppendValue(root, "cn", "中文也可以显示");
		ejson.AppendValue(root, "number1", 1024);
		ejson.AppendValue(root, "", 1048576);
		ejson.AppendValue(root, "", "这个字符串没有KEY");
		ejson.AppendValue(root, "stringwithEscape", "string\r\b\n\t\f\\");
		std::string temp;
		temp += char(0x1F);
		temp += char(0x11);
		ejson.AppendValue(root, "controlstring", (char *)temp.c_str());

		JsonNode * node = ejson.CreateJsonNode(NODE_OBJECT);
		ejson.AppendValue(node, "hahaha", "json");
		ejson.AppendValue(node, "number2", 65535);
		ejson.AppendNullValue(node, "iamnull");
		
		ejson.AppendObjectValue(root, "subnode",node);	

		JsonNode * node2 = ejson.CreateJsonNode(NODE_ARRAY);
		ejson.AppendValue(node2, "float1",1.0);
		ejson.AppendValue(node2, "float2", 2.0);
		ejson.AppendArrayValue(root, "subnodearray", node2);


		ejson.AppendValue("subnode", "number3", 27015);
		ejson.AppendValue("subnodearray", "float3", 3.0);


	}
	
	printf("%s\n", UToA(ejson.ToString()).c_str());

	CppEasyJson ejson2;
	ejson2 = ejson;
	printf("%s\n", UToA(ejson2.ToString()).c_str());

	CppEasyJson ejson3;
	ejson3 = ejson.GetNode("subnode");
	printf("%s\n", UToA(ejson3.ToString()).c_str());

	ejson.Release();
	ejson2.Release(); 
	ejson3.Release();
}

void Test15()
{
	CppEasyJson ejson;
	ejson.ParseFile("F:\\CppEasyJson\\jtest.json");
	bool bvalue;
	ejson.GetValue("abc.x", bvalue);
	printf("%s\n", bvalue ? "true" : "false");
	ejson.Release();
}
void Test14()
{
	CppEasyJson ejson;
	ejson.ParseString(JSONSTRING14);
	char cvalue[256] = { 0 };
	ejson.GetValue("jsontext", cvalue, 256);
	printf("%s\n", cvalue);
	ejson.Release();
}
void Test12()
{
	CppEasyJson ejson;
	ejson.ParseString(JSONSTRING12);	
	char cvalue[256] = { 0 };
	ejson.GetValue("type", cvalue, 256);
	printf("%s\n", cvalue);
	ejson.GetValue("features[0].geometry.type", cvalue, 256);
	printf("%s\n", cvalue);
}
void Test11()
{
	CppEasyJson ejson;
	ejson.ParseString(JSONSTRING11);
	bool bvalue;
	ejson.GetValue("abc.x", bvalue);
	printf("%s\n", bvalue ? "true" : "false");
	printf("%s\n", UToA(ejson.ToString()).c_str());
	ejson.Release();
	//char cvalue[256] = { 0 };
	//ejson.GetValue("key5", cvalue, 256);
	//printf("%s\n", cvalue);
}
void Test10()
{
	CppEasyJson ejson;
	ejson.ParseString(JSONSTRING10);
	bool bvalue;
	ejson.GetValue("key1", bvalue);
	printf("%s\n", bvalue? "true":"false");
	ejson.GetValue("key2", bvalue);
	printf("%s\n", bvalue ? "true": "false");
	int value = 0;
	ejson.GetValue("key3", value);
	printf("%d\n", value);
	double dvalue = 0;
	ejson.GetValue("key4",dvalue);
	printf("%f\n", dvalue);
	char cvalue[256] = { 0 };
	ejson.GetValue("key5", cvalue, 256);
	printf("%s\n", cvalue);
}
void Test9()
{
	CppEasyJson ejson;
	ejson.ParseString(JSONSTRING9);
	int value=0;
	ejson.GetValue("key", value);
	printf("%d\n", value);
	ejson.GetValue("otherkey", value);
	printf("%d\n", value);
}
void Test8()
{
	CppEasyJson ejson;
	ejson.ParseString(JSONSTRING8);
	int value = 0;
	ejson.GetValue("key.subkey", value);
	printf("%d\n", value);
}
void Test7()
{
	CppEasyJson ejson;
	ejson.ParseString(JSONSTRING7);
	char Name[256] = { 0 };
	ejson.GetValue("key", Name, 256);
	printf("%s\n", Name);
}
void Test6()
{
	CppEasyJson ejson;
	ejson.ParseString(JSONSTRING6);
	char Name[256] = { 0 };
	ejson.GetValue("key", Name, 256);
	printf("%s\n", Name);
}
void Test5()
{
	CppEasyJson ejson;
	ejson.ParseString(JSONSTRING5);
	int value = 0;
	ejson.GetValue("key", value);
	printf("%d\n", value);
}
void Test0()
{
	CppEasyJson ejson;
	ejson.ParseString(JSONSTRING);
	char Name[256] = { 0 };
	ejson.GetValue("firstName", Name, 256);
	printf("%s\n", Name);
}
void Test1()
{
	CppEasyJson ejson;
	ejson.ParseString(JSONSTRING1);
	char Name[256] = { 0 };
	ejson.GetValue("firstName", Name, 256);
	printf("%s\n", Name);
}
void Test2()
{
	CppEasyJson ejson;
	ejson.ParseString(JSONSTRING2);
	char Name[256] = { 0 };
	//ejson.GetValue("people.[0].firstName", Name, 256);
	ejson.GetValue("people[0].firstName", Name, 256);
	printf("%s\n", Name);
	ejson.GetValue("people[0].lastName", Name, 256);
	printf("%s\n", Name);
	ejson.GetValue("people[0].email", Name, 256);
	printf("%s\n", Name);
	ejson.GetValue("people[1].firstName", Name, 256);
	printf("%s\n", Name);
	ejson.GetValue("people[1].lastName", Name, 256);
	printf("%s\n", Name);
	ejson.GetValue("people[1].email", Name, 256);
	printf("%s\n", Name);
	ejson.GetValue("people[2].firstName", Name, 256);
	printf("%s\n", Name);
	ejson.GetValue("people[2].lastName", Name, 256);
	printf("%s\n", Name);
	ejson.GetValue("people[2].email", Name, 256);
	printf("%s\n", Name);
	ejson.GetValue("people[3].email", Name, 256);
	printf("%s\n", Name);
}
void Test3()
{
	CppEasyJson ejson;
	ejson.ParseString(JSONSTRING4);
	char Name[256] = { 0 };
	//ejson.GetValue("name", Name, 256);
	//printf("%s\n", Name);
	//ejson.GetValue("province.[0].name", Name, 256);
	//printf("%s\n", Name);
	ejson.GetValue("province[0].cities.city[0]", Name, 256);
	printf("%s\n", Name);
	
}
void Test33()
{
	CppEasyJson ejson;
	ejson.ParseString(JSONSTRING33);
	char Name[256] = { 0 };
	//ejson.GetValue("province.[0].cities.city.[0].harbin", Name, 256);
	//printf("%s\n", Name);
	ejson.GetValue("province[0].name", Name, 256);
	printf("%s\n", Name);
	ejson.GetValue("province[1].name", Name, 256);
	printf("%s\n", Name);
	ejson.GetValue("province[2].name", Name, 256);
	printf("%s\n", Name);
	ejson.GetValue("province[3].name", Name, 256);
	printf("%s\n", Name);
}
void Test13()
{
	CppEasyJson ejson;
	//ejson.ParseFile("F:\\CppEasyJson\\test\\pass3.json");
	char Name[256] = { 0 };

	//ejson.GetValue("JSON Test Pattern pass3.The outermost value", Name, 256);
	//printf("%s\n", Name);
	//ejson.GetValue("JSON Test Pattern pass3.In this test", Name, 256);
	//printf("%s\n", Name);
	//ejson.Release();
	ejson.ParseFile("F:\\CppEasyJson\\test\\pass1.json");
	
	//ejson.GetValue("[1].object with 1 member[0]", Name, 256);
	//printf("%s\n", Name);

	int value = 0;
	ejson.GetValue("[8]. s p a c e d [0]", value);
	printf("%d\n", value);
	ejson.GetValue("[8]. s p a c e d [1]", value);
	printf("%d\n", value);
	ejson.GetValue("[8]. s p a c e d [2]", value);
	printf("%d\n", value);
	ejson.GetValue("[8]. s p a c e d [3]", value);
	printf("%d\n", value);
	ejson.GetValue("[12]", value);
	printf("%d\n", value);
	ejson.GetValue("[19]", Name, 256);
	printf("%s\n", Name);
	ejson.GetValue("[8].alpha", Name, 256);
	printf("%s\n", Name);
	ejson.GetValue("[8].ALPHA", Name, 256);
	printf("%s\n", Name);
	ejson.GetValue("[8].jsontext", Name, 256);
	printf("%s\n", Name);	
	ejson.GetValue("[8].quotes", Name, 256);
	printf("%s\n", Name);	
	ejson.GetValue("[8].url", Name, 256);
	printf("%s\n", Name);	
	ejson.GetValue("[8].address", Name, 256);
	printf("%s\n", Name);	
	ejson.GetValue("[18]", value);
	printf("%d\n", value);
	ejson.Release();
	
}
void TestJsonFile()
{
	CppEasyJson ejson;
	ejson.ParseFile("E:\\1.json");
	ejson.SaveToFile("E:\\2.json");

}
int main()
{
//	TestJsonFile();
	Test19();
	Test25();
	//Test24();
	//Test23();
	//Test22();
	//Test21();
	//Test20();
	//Test19();
	//Test15();
	//Test11();
	/*
	//Test18();
	Test17();
	Test16();
	
	///
	Test0();
	Test1();
	Test2();
	Test3();
	Test5();
	Test6();
	////Test7();
	Test8();
	Test9();
	Test10();
	Test33();
	
	Test12();
	Test13();
	Test14();
	*/
	
	
	
    return 0;
}

