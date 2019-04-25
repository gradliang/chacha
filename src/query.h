#ifndef QUERY__H__
#define QUERY__H__

#include <string>
#include <vector>

void queryText(std::string & textcontext, std::string & ret);

std::wstring queryBank(wchar_t * str);
std::wstring findBank(wchar_t * str);
std::wstring queryArea(wchar_t * str1, wchar_t * bank);
std::wstring findArea(wchar_t * str, wchar_t * bank);
std::wstring queryMccCode(wchar_t * str);
std::wstring findMCC(wchar_t * str);
std::wstring getAdText();
std::wstring processRequestText(wchar_t * text);
int findInTianShu(const wchar_t * shanghuhao, std::vector<std::wstring> & findret);
int findInBommHeiShops(const wchar_t * shanghuhao, std::vector<std::wstring> & findret);

#endif // !QUERY__H__
