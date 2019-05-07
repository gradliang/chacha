#include "stdafx.h"
#include "query.h"
#include "utf.h"
#include "mohu.h"
#include "alldata.hpp"

static bool haveLetter(const wchar_t *str);

void queryText(std::string & textcontext, std::string & ret)
{
	wchar_t * wtext;
	char * btext;
	unsigned wlen, blen;

	wlen = textcontext.length() * 4 + 1;
	wtext = new wchar_t[wlen];

	utf8_to_wstring(wtext, wlen, textcontext.c_str());
	std::wstring wret = processRequestText(wtext);

	blen = wret.length() * 4 + 1;
	btext = new char[blen];
	wstring_to_utf8(btext, blen, wret.c_str());
	ret = btext;

	delete[] btext;
	delete[] wtext;

	return  ;
}

std::wstring processRequestText(wchar_t * text)
{
	int len;
	bool existArea = false;				// 是否存在地区吗
	bool existMCC = false;				// 是否存在MCC
	bool existBank = false;				// 是否存在机构号
	bool fullbianhao = false;			// 完整商户号
	wchar_t strArea[8] = { 0 };			// 
	wchar_t strMCC[8] = { 0 };			// 
	wchar_t strBank[8] = { 0 };			// 
	std::wstring ret;

	////wprintf(L"processRequest text = \"%s\"\n", text);

	len = (int)wcslen(text);
	if (len == 3)
	{
		wcscpy(strBank, text);
	}
	else if (len == 4)
	{
		wcscpy(strMCC, text);
		wcscpy(strArea, text);
	}
	else if (len == 15)
	{
		wcsncpy(strBank, &text[0], 3);
		wcsncpy(strArea, &text[3], 4);
		wcsncpy(strMCC, &text[7], 4);
		fullbianhao = true;
	}
	else if (len > 3 && len < 7)
	{
		wcsncpy(strBank, &text[0], 3);
		wcsncpy(strArea, &text[0], 4);
	}
	else if (len >= 7 && len < 11)
	{
		wcsncpy(strBank, &text[0], 3);
		wcsncpy(strArea, &text[3], 4);
	}
	else if (len >= 11)
	{
		wcsncpy(strBank, &text[0], 3);
		wcsncpy(strArea, &text[3], 4);
		wcsncpy(strMCC, &text[7], 4);
		fullbianhao = true;
	}

	if (strArea[0] != 0 && checkIsLetterOrNumber(strArea))
		existArea = true;
	if (strMCC[0] != 0 && checkIsLetterOrNumber(strMCC))
		existMCC = true;
	if (strBank[0] != 0 && checkIsLetterOrNumber(strBank))
		existBank = true;

	bool boMoHuFindOK = false;
	if (existArea == false && existMCC == false && existBank == false)
		boMoHuFindOK = MoHu_find(text, ret);
	if (boMoHuFindOK)
		return ret;

	ret += L"◆◆◆◆查询开始◆◆◆◆\n";

	if (existBank)
		ret += queryBank(strBank);
	if (existArea)
		ret += queryArea(strArea, strBank);
	if (existArea)
	{
		ret += queryMccCode(strMCC);
	}
	if (fullbianhao)
	{
		std::vector<std::wstring> findret;
		std::wstring bianhao(text, 15);
		findInTianShu(bianhao.c_str(), findret);
		for (size_t t = 0; t < findret.size(); t++)
			ret += findret[t];
		findInBommHeiShops(bianhao.c_str(), findret);
		for (size_t t = 0; t < findret.size(); t++)
			ret += findret[t];
	}

	ret += L"◆◆◆◆查询结束◆◆◆◆\n";

	ret += getAdText();

	return ret;
}

std::wstring queryBank(wchar_t * str)
{
	std::wstring ret;

	ret += L"【收单机构】:";
	ret += str;
	ret += L"\n";
	ret += findBank(str);
	ret += L"\n";

	return ret;
}

std::wstring findBank(wchar_t * str)
{
	unsigned crc, idx;
	const bankinfo * pInfos;
	const bankinfo * it;
	unsigned bucketSize;

	unsigned short * pstr16;
	
#ifdef WIN32
	pstr16 = str;
#else
	int k = 0;
	unsigned short bankstr16[8];
	memset(bankstr16, 0, sizeof(bankstr16));
	while (str[k] && k < 8)
	{
		bankstr16[k] = (unsigned short) str[k];
		k++;
	}
	pstr16 = bankstr16;
#endif // WIN32

	crc = crc32_hash((unsigned char*)pstr16, wcslen(str) * sizeof(wchar_t));
	idx = crc % BANK_HASH_TABLE_SIZE;
	pInfos = bankinfo_buckets[idx];
	bucketSize = bank_bucket_size[idx];

	if (pInfos)
	{
		for (unsigned i = 0; i < bucketSize; i++)
		{
			it = &pInfos[i];
			if (it->crc32 == crc)
			{
				if (0 == wcscmp(it->scode, str))
				{
					return it->display;
				}
			}
		}
	}

	if (str[0] == '9' && str[1] == '9')
		return L"二清支付机构";
	if ((str[0] == '4' || str[0] == '5' || str[0] == '6') && (str[1] >= '0' && str[1] <= '9') && (str[2] >= '0' && str[2] <= '9'))
		return L"地方性银行机构";

	return L"找不到";
}

std::wstring queryArea(wchar_t * str, wchar_t * bank)
{
	std::wstring ret;

	ret += L"【地区代码】:";
	ret += str;
	ret += L"\n";
	ret += findArea(str, bank);
	//ret += L"\n";

	return ret;
}

std::wstring findArea(wchar_t * str1, wchar_t * bank)
{
	int areaId;
	bool isNumber;
	const areainfo * it;
	const areainfo * infos;
	unsigned bucketSize, i;
	std::wstring ret;
	wchar_t str[16];

	memset(str, 0, sizeof(str));
	wcsncpy(str, str1, sizeof(str) / sizeof(wchar_t) - 1);
	capitalization(str);

	if (0 == wcscmp(bank, L"847") && str[0] == L'A')
	{
		return L"中付支付自编\n";
	}

	if (0 == wcscmp(bank, L"887") && haveLetter(str))
	{
		return L"瑞银信自编\n";
	}

	isNumber = checkIsNumber(str);
	if (!isNumber)
		return L"找不到\n";

	areaId = _wtoi(str);
	if (areaId < AREA_MAX_SIZE)
	{
		infos = areainfo_buckets[areaId];
		bucketSize = area_bucket_size[areaId];
		if (infos && bucketSize)
		{
			for (i = 0; i < bucketSize; i++)
			{
				it = &infos[i];
				ret += it->display;
				ret += L"\n";
			}
			return ret;
		}
		else
		{
			return L"找不到\n";
		}
	}
	return L"找不到\n";
}

std::wstring queryMccCode(wchar_t * str)
{
	std::wstring ret;

	ret += L"【MCC代码】:";
	ret += str;
	ret += L"\n";
	ret += findMCC(str);
	ret += L"\n";

	return ret;
}

std::wstring findMCC(wchar_t * str)
{
	unsigned crc32, idx;
	const mccinfo * it;
	const mccinfo * infos;
	unsigned i, bucketSize;
	unsigned short * pstr16;
	
#ifdef WIN32
	pstr16 = str;
#else
	int k = 0;
	unsigned short mccstr16[8];
	memset(mccstr16, 0, sizeof(mccstr16));
	while (str[k] && k < 8)
	{
		mccstr16[k] = (unsigned short) str[k];
		k++;
	}
	pstr16 = mccstr16;
#endif // WIN32

	crc32 = crc32_hash((unsigned char*)pstr16, wcslen(str) * sizeof(unsigned short));
	idx = crc32 % MCC_HASH_TABLE_SIZE;
	infos = mccinfo_buckets[idx];
	bucketSize = mcc_bucket_size[idx];

	if (infos) 
	{
		for (i = 0; i < bucketSize; i++)
		{
			it = &infos[i];
			if (it->crc32 == crc32)
			{
				if (0 == wcscmp(it->scode, str))
				{
					// found
					goto found;
				}
			}
		}
	}
	return L"找不到";

found:
	std::wstring ret;

	ret += it->desc;
	ret += L"\n【商户大类】:\n";
	ret += it->largeClass;
	ret += L"\n【费率】:\n";
	ret += std::wstring(it->fee) + L" , 96费改前为" + it->old96;
	if (it->warning != L"")
	{
		ret += L"\n【警告信息】:\n";
		ret += it->warning;
	}
	ret += L"\n【商户详细信息】:\n";
	ret += it->detail;
	if (it->jifen != NULL)
	{
		if (it->jifen->desc != L"")
		{
			ret += L"\n【积分情况】:\n";
			ret += it->jifen->desc;
		}
		if (it->jifen->undesired != L"")
		{
			ret += L"\n【无积分的银行】:\n";
			ret += it->jifen->undesired;
		}
	}

	return ret;
}

int findInBommHeiShops(const wchar_t * shanghuhao, std::vector<std::wstring> & findret)
{
	unsigned crc32, findID;
	char bianhao_c[16];
	bool found = false;
	const BankcommHeiShop * it;
	const BankcommHeiShop * infos;
	unsigned i, bucketSize;

	findret.clear();
	findret.push_back(L"【交通银行黑名单】:\n");

	memset(bianhao_c, 0, sizeof(bianhao_c));
	for (i = 0; i < 15 && i < wcslen(shanghuhao); i++)
	{
		bianhao_c[i] = (char)shanghuhao[i];
	}

	crc32 = crc32_hash((unsigned char*)bianhao_c, strlen(bianhao_c));
	findID = crc32 % HEISHOP_HASH_TABLE_SIZE;
	bucketSize = BankcommHeiShop_bucket_size[findID];
	infos = BankcommHeiShop_buckets[findID];

	for (i = 0; i < bucketSize; i++)
	{
		it = &infos[i];
		if (crc32 == it->crc32)
		{
			if (0 == strcmp_16_8(shanghuhao, it->str))
			{
				found = true;
				break;
			}
		}
	}

	std::wstring record;
	if (found)
	{
		record = L"2018-07数据: ";
		record += L"不幸中招\n";
	}
	else
	{
		record = L"2018-07数据: ";
		record += L"未收录\n";
	}

	findret.push_back(record);

	return 1;
}


int findInTianShu(const wchar_t * shanghuhao, std::vector<std::wstring> & findret)
{
	size_t idOfDB, i;
	unsigned crc32, findID;
	char bianhao_c[16];

	findret.clear();

	findret.push_back(L"【招行天书黑名单】:\n");

	memset(bianhao_c, 0, sizeof(bianhao_c));
	for (i = 0; i < 15 && i < wcslen(shanghuhao); i++)
	{
		bianhao_c[i] = (char)shanghuhao[i];
	}

	crc32 = crc32_hash((unsigned char*)bianhao_c, strlen(bianhao_c));
	findID = crc32 % TIANSHU_HASH_TABLE_SIZE;

	for (idOfDB = 0; idOfDB < TIANSHU_COUNT; idOfDB++)
	{
		bool found = false;
		const tianshu_db_struct * db;
		unsigned bucketSize;

		db = &tianshu_db[idOfDB];
		bucketSize = db->bucket_size[findID];
		auto infos = db->buckets[findID];

		for (i = 0; i < bucketSize; i++)
		{
			auto it = &infos[i];
			if (crc32 == it->crc32)
			{
				if (0 == strcmp_16_8(shanghuhao, it->str))
				{
					found = true;
					break;
				}
			}
		}

		std::wstring record;
		if (found)
		{
			record = db->month;
			record += L": ";
			record += L"不幸中招\n";
		}
		else
		{
			record = db->month;
			record += L": ";
			record += L"未收录\n";
		}

		findret.push_back(record);
	}


	return 1;
}

std::wstring getAdText()
{
	int n;

	n = rand();
	n %= AD_COUNT;

	return ad_texts[n];
}

static bool haveLetter(const wchar_t *str)
{
	while (*str)
	{
		if (((*str) >= 'A' && (*str) <= 'Z') || ((*str) >= 'a' && (*str) <= 'z'))
			return true;
		str++;
	}
	return false;
}
