#pragma once
#include "Singleton.h"
#include "JSON/rapidjson/document.h"
#include "JSON/rapidjson/filereadstream.h"
#include "JSON/rapidjson/filewritestream.h"
#include "JSON/rapidjson/prettywriter.h"

using namespace rapidjson;

#define gJSON JSON::GetInstance()

class JSON : public Singleton<JSON>
{
public:
	void LoadJSON(string FullPath);
	void WriteJSON(string FullPath);
	void PrintJSON()
	{
		
	}

	Document mDocument;
private:

};

