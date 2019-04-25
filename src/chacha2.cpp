// chacha2.cpp: 定义控制台应用程序的入口点。
//
#include "fcgi_config.h"
#include "fcgiapp.h"
#include "stdafx.h"
#include "chacha2.h"
#include "logger.h"
#include "tinyxml2.h"
#include "query.h"
#include "utf.h"

static void InitProgram();
static void processXmlInput(char * content, std::string & ret);

int main(int argc, char ** argv)
{
	FCGX_Stream *in, *out, *err;
	FCGX_ParamArray envp;
	int count = 0;

	InitProgram();

	while (FCGX_Accept(&in, &out, &err, &envp) >= 0) 
	{
		char * content = NULL;
		char *contentLength = FCGX_GetParam("CONTENT_LENGTH", envp);
		int len = 0;

#if 0 // for test
/*
		FCGX_FPrintF(out, "Content-type: text/html\r\n"
			"\r\n"
			"<title>FastCGI Hello!</title>"
			"<h1>FastCGI Hello!</h1>"
			"<div>Request number %d running on host : %s </div>\n"
			"<div>QUERY_STRING : %s\n</div>"
			"<div>REMOTE_ADDR : %s\n</div>"
			"<div>REMOTE_PORT : %s\n</div>"
			"<div>REQUEST_METHOD : %s\n</div>"
			"<div>CONTENT_TYPE : %s\n</div>"
			"<div>CONTENT_LENGTH : %s\n</div>"
			"<div>SERVER_PROTOCOL : %s\n</div>"
			"<div>REQUEST_URI : %s\n</div>"
			"<div>SERVER_SOFTWARE : %s\n</div>",
			++count, getenv("SERVER_NAME"),getenv("QUERY_STRING"),
			getenv("REMOTE_ADDR"), getenv("REMOTE_PORT"), getenv("REQUEST_METHOD"),
			getenv("CONTENT_TYPE"),getenv("CONTENT_LENGTH"),getenv("REQUEST_URI"),
			getenv("SERVER_PROTOCOL"), getenv("SERVER_SOFTWARE")
		);
		*/
		FCGX_FPrintF(out, "Content-type: text/html\r\n\r\n %d, %d", ++count, getpid());
		continue;
#endif

		FCGX_FPrintF(out,
			"Content-type: text/html\r\n"
			"\r\n");

		if (contentLength != NULL)
			len = strtol(contentLength, NULL, 10);

		if (1)//(len > 0)  //(len > 0)  // (1) 
		{
#if 1  // TEST
			std::string testdata, testtext("5411");
			testdata += "<xml>";
			testdata += "<ToUserName><![CDATA[test_data1]]></ToUserName>";
			testdata += "<FromUserName><![CDATA[test_data2]]></FromUserName>";
			testdata += "<CreateTime>0</CreateTime>";
			testdata += "<MsgType><![CDATA[text]]></MsgType>";
			testdata += "<Content><![CDATA[" + testtext + "]]></Content>";
			testdata += "<FuncFlag>0</FuncFlag>";
			testdata += "</xml>";
			
			len = testdata.length() + 1;
#endif

			content = new char[len + 1];
			if (content == NULL)
			{
				FCGX_FPrintF(out, "No memory from standard input.<p>\n");
				continue;
			}

			memset(content, 0, len + 1);
#if 1	// TEST
			strcpy(content, testdata.c_str());
#else
			FCGX_GetStr(content, len, in);
#endif

			std::string retstr;
			processXmlInput(content, retstr);
			if (retstr.length())
			{
				// FCGX_FPrintF(out, retstr.c_str());  // 这个有长度限制
				
				char tmpstr[64];
			sprintf(tmpstr, "%d pid=%d", ++count, getpid());
			retstr += tmpstr;
			
			
				size_t outlen = retstr.length();
				for (size_t i = 0; i < outlen; i++) {
					FCGX_PutChar((retstr.c_str())[i], out);
				}
			}

			delete[] content;
		}
		else {
			FCGX_FPrintF(out, "No data from standard input.<p>\n");
		}

		//PrintEnv(out, "Request environment", envp);
		//PrintEnv(out, "Initial environment", environ);

		sync_all_logs();
	} 

	return 0;
}

static void InitProgram()
{
#ifdef WIN32
	setlocale(LC_ALL, ".936");
	srand((unsigned)GetCurrentProcessId());
#else
	srand((unsigned)getpid());
#endif
	//LogWriterInit();

	return;
}

static void load_xml_text(char * xml, std::map<std::string, std::string> & map)
{
	tinyxml2::XMLError err;
	tinyxml2::XMLDocument  doc;
	tinyxml2::XMLElement * root, *rootchild;

	err = doc.Parse(xml, strlen(xml));
	if (err)
	{
		return;
	}

	root = doc.RootElement();
	if (root == NULL || 0 != strcmp(root->Name(), "xml"))
	{
		//printf("RootElement \"SkyGeneralCfgs\" error \n");
		return;
	}

	rootchild = root->FirstChildElement();
	for (; rootchild != NULL; rootchild = rootchild->NextSiblingElement())
	{
		std::string name, text;
		name = rootchild->Name();
		text = rootchild->GetText();
		map.insert(std::make_pair(name, text));
	}
	
	return;
}

static void processXmlInput(char * content, std::string & ret)
{
	std::string from, to, msgtype, textcontext, event;
	std::map<std::string, std::string>  map;
	std::map<std::string, std::string>::iterator it;
	
	char time_s[64];

	load_xml_text(content, map);
	for (it = map.begin(); it != map.end(); it++)
	{
		if (0 == _stricmp(it->first.c_str(), "ToUserName"))
			to = it->second;
		else if (0 == _stricmp(it->first.c_str(), "FromUserName"))
			from = it->second;
		else if (0 == _stricmp(it->first.c_str(), "MsgType"))
			msgtype = it->second;
		else if (0 == _stricmp(it->first.c_str(), "Content"))
			textcontext = it->second;
		else if (0 == _stricmp(it->first.c_str(), "Event"))
			event = it->second;
	}

	logPrintWithSystemID("msgtype=%s, from=%s, Content=%s\n", msgtype.c_str(), from.c_str(), textcontext.c_str());

	if (0 == _stricmp(msgtype.c_str(), "text"))
	{
		ret += "<xml>\n";
		ret += "<ToUserName><![CDATA[" + from +  "]]></ToUserName>\n";
		ret += "<FromUserName><![CDATA[" + to + "]]></FromUserName>\n";
		sprintf(time_s, "%lld", (long long)(time(NULL)));
		ret += "<CreateTime>" + std::string(time_s) + "</CreateTime>\n";
		ret += "<MsgType><![CDATA[text]]></MsgType>\n";
		std::string queryRet;
		queryText(textcontext, queryRet);
		ret += "<Content>\n<![CDATA[" + queryRet + "]]>\n</Content>\n";
		ret += "<FuncFlag>0</FuncFlag>\n";
		ret += "</xml>";

		/*
		FILE * fp = fopen("C:\\Temp\\test111.txt", "wt");
		if (fp) {
			fwrite(ret.c_str(), 1, ret.length(), fp);
			fclose(fp);
		}*/
	}
	else if (0 == _stricmp(msgtype.c_str(), "event"))
	{
		const char * format = "<xml>"
			"<ToUserName><![CDATA[%s]]></ToUserName>"
			"<FromUserName><![CDATA[%s]]></FromUserName>"
			"<CreateTime>%lld</CreateTime>"
			"<MsgType><![CDATA[text]]></MsgType>"
			"<Content><![CDATA[%s]]></Content>"
			"<FuncFlag>0</FuncFlag>"
			"</xml>";

		if (0 == _stricmp(event.c_str(), "subscribe"))
		{
			char * buffer = new char[65536];
			char * rbtext = new char[65536];
			const wchar_t * rwtext = L"欢迎使用MCC查查公众号，直接在对话窗口输入POS消费小票的商户编号可以查询消费积分信息，或者输入商户编号的1-3，4-7，8-11位查询收单机构、地区码、MCC信息。";
			wstring_to_utf8(rbtext, 65536, rwtext);
			sprintf_s(buffer, 65536, format, from.c_str(), to.c_str(), time(NULL), rbtext);
			ret = buffer;
			delete[] buffer;
			delete[] rbtext;
		}
		else
			ret = "success";
	}
	else
	{
		ret += "<xml>";
		ret += "<ToUserName><![CDATA[" + from + "]]></ToUserName>";
		ret += "<FromUserName><![CDATA[" + to + "]]></FromUserName>";
		sprintf(time_s, "%lld", (long long)(time(NULL)));
		ret += "<CreateTime>" + std::string(time_s) + "</CreateTime>";
		ret += "<MsgType><![CDATA[" + msgtype +"]]></MsgType>";
		for (it = map.begin(); it != map.end(); it++)
		{
			std::string xmlT1;
			if (0 == _stricmp(it->first.c_str(), "ToUserName"))
				continue;
			if (0 == _stricmp(it->first.c_str(), "FromUserName"))
				continue;
			if (0 == _stricmp(it->first.c_str(), "CreateTime"))
				continue;
			if (0 == _stricmp(it->first.c_str(), "MsgType"))
				continue;

			xmlT1 += "<" + it->first + ">";
			xmlT1 += "<![CDATA[" + it->second + "]]>";
			xmlT1 += "</" + it->first + ">";
			ret += xmlT1;
		}
	}
}




