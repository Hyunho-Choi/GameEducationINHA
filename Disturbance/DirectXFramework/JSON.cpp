#include "stdafx.h"
#include "JSON.h"

void JSON::LoadJSON(string FullPath)
{
	FILE *fp;
	fopen_s(&fp, FullPath.c_str(), "rb");

	char buffer[65536];
	FileReadStream is(fp, buffer, sizeof(buffer));
	if(mDocument.ParseStream(is).HasParseError())
	{
		cout << "Parsing error" << endl;
	}
	fclose(fp);
	//cout << buffer;
}

void JSON::WriteJSON(string FullPath)
{
	FILE *fp;
	fopen_s(&fp, FullPath.c_str(), "wb");

	//StringBuffer s;
	//Writer<StringBuffer> writer(s);

	char buffer[65536];
	FileWriteStream os(fp, buffer, sizeof(buffer));
	PrettyWriter<FileWriteStream> writer(os);

	writer.StartObject();
	writer.Key("hello");
	writer.String("world");
	writer.Key("t");
	writer.Bool(true);
	writer.Key("f");
	writer.Bool(false);
	writer.Key("n");
	writer.Key("i");
	writer.Key("pi");
	writer.Double(3.1416);
	writer.Key("ÇÑ±Û");
	writer.Int(123);
	writer.EndObject();

	fclose(fp);
}
