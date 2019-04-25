#include "stdafx.h"
#include "query.h"
#include "utf.h"
#include "mohu.h"

static bool match_mcc(const wchar_t * text, std::wstring & retstr);
static bool match_area(const wchar_t * text, std::wstring & retstr);
static bool match_bank(const wchar_t * text, std::wstring & retstr);

bool MoHu_find(const wchar_t * text, std::wstring & retstr)
{
	if (0 == wcsncmp(text, L"建议", wcslen(L"建议")) ||
		0 == wcsncmp(text, L"提议", wcslen(L"提议")) ||
		0 == wcsncmp(text, L"纠错", wcslen(L"纠错"))
		)
	{
		retstr = L"谢谢你的建议与纠错";
		return true;
	}

	if (wcsstr(text, L"跳码"))
	{
		retstr = L"跳码是指你刷正常商户，不良机器偷偷地向银行报告你刷了劣质低费率商户，向你收取正常费用，但是向银行缴纳超低费用，从中大量获利，并严重伤害持卡人利益。";
		return true;
	}

	if (0 == wcscmp(text, L"封顶"))
	{
		retstr = L"公立医院8062、公立学校8211、信用卡还款9498 等为封顶类商户";
		return true;
	}

	if (match_mcc(text, retstr))
		return true;
	if (match_bank(text, retstr))
		return true;
	if (match_area(text, retstr))
		return true;

	return false;
}

static bool match_mcc(const wchar_t * text, std::wstring & retstr)
{
	if (0 == wcscmp(text, L"超市"))
	{
		retstr = L"5411";
		return true;
	}
	if (0 == wcscmp(text, L"加油") || 0 == wcscmp(text, L"加油站"))
	{
		retstr = L"5541";
		return true;
	}
	if (0 == wcscmp(text, L"珠宝"))
	{
		retstr = L"5094";
		return true;
	}
	if (0 == wcscmp(text, L"餐饮") || 0 == wcscmp(text, L"饭店"))
	{
		retstr = L"5811";
		return true;
	}
	if (0 == wcscmp(text, L"KTV") || 0 == wcscmp(text, L"ktv") || 0 == wcscmp(text, L"歌舞厅"))
	{
		retstr = L"7911";
		return true;
	}
	if (0 == wcscmp(text, L"酒吧"))
	{
		retstr = L"5813";
		return true;
	}
	if (0 == wcscmp(text, L"酒店") || 0 == wcscmp(text, L"旅馆") || 0 == wcscmp(text, L"旅店"))
	{
		retstr = L"7011";
		return true;
	}
	if (0 == wcscmp(text, L"美容") || 0 == wcscmp(text, L"美容院") || 0 == wcscmp(text, L"SPA") || 0 == wcscmp(text, L"spa")
		|| 0 == wcscmp(text, L"保健") || 0 == wcscmp(text, L"大保健"))
	{
		retstr = L"7298";
		return true;
	}
	if (0 == wcscmp(text, L"高尔夫"))
	{
		retstr = L"7992";
		return true;
	}
	if (0 == wcscmp(text, L"女装"))
	{
		retstr = L"5621";
		return true;
	}
	if (0 == wcscmp(text, L"服装"))
	{
		retstr = L"5691";
		return true;
	}
	if (0 == wcscmp(text, L"化妆品"))
	{
		retstr = L"5977";
		return true;
	}
	if (0 == wcscmp(text, L"手机") || 0 == wcscmp(text, L"手机店"))
	{
		retstr = L"4812";
		return true;
	}
	if (0 == wcscmp(text, L"眼镜") || 0 == wcscmp(text, L"眼镜店"))
	{
		retstr = L"8043";
		return true;
	}
	if (0 == wcscmp(text, L"水族馆") || 0 == wcscmp(text, L"海洋馆"))
	{
		retstr = L"7998";
		return true;
	}
	if (0 == wcscmp(text, L"宠物") || 0 == wcscmp(text, L"宠物店"))
	{
		retstr = L"5995";
		return true;
	}
	if (0 == wcscmp(text, L"箱包") || 0 == wcscmp(text, L"皮具店"))
	{
		retstr = L"5948";
		return true;
	}
	if (0 == wcscmp(text, L"香烟") || 0 == wcscmp(text, L"烟酒"))
	{
		retstr = L"5993";
		return true;
	}
	if (0 == wcscmp(text, L"百货") || 0 == wcscmp(text, L"百货商场") || 0 == wcscmp(text, L"商场"))
	{
		retstr = L"5311";
		return true;
	}
	if (0 == wcscmp(text, L"旅行社"))
	{
		retstr = L"4722";
		return true;
	}
	if (0 == wcscmp(text, L"航空") || 0 == wcscmp(text, L"机票"))
	{
		retstr = L"4511";
		return true;
	}
	if (0 == wcscmp(text, L"房产"))
	{
		retstr = L"1520";
		return true;
	}
	if (0 == wcscmp(text, L"电器"))
	{
		retstr = L"5722";
		return true;
	}
	if (0 == wcscmp(text, L"医院"))
	{
		retstr = L"8062";
		return true;
	}
	if (0 == wcscmp(text, L"学校"))
	{
		retstr = L"8211";
		return true;
	}
	if (0 == wcscmp(text, L"信用卡还款"))
	{
		retstr = L"9498";
		return true;
	}


	return false;
}

static bool match_area(const wchar_t * text, std::wstring & retstr)
{
	return false;
}

static bool match_bank(const wchar_t * text, std::wstring & retstr)
{
	if (0 == wcscmp(text, L"邮政") || 0 == wcscmp(text, L"邮政储蓄"))
	{
		retstr = L"100";
		return true;
	}
	if (0 == wcscmp(text, L"中国工商银行") || 0 == wcscmp(text, L"工行") || 
		0 == wcscmp(text, L"工商") || 0 == wcscmp(text, L"工商银行") || 0 == wcscmp(text, L"大妈"))
	{
		retstr = L"102";
		return true;
	}
	if (0 == wcscmp(text, L"中国农业银行") || 0 == wcscmp(text, L"农行") ||
		0 == wcscmp(text, L"农业") || 0 == wcscmp(text, L"农业银行") || 0 == wcscmp(text, L"农妇"))
	{
		retstr = L"103";
		return true;
	}
	if (0 == wcscmp(text, L"中国银行") || 0 == wcscmp(text, L"中行") ||
		0 == wcscmp(text, L"中银") || 0 == wcscmp(text, L"砖行"))
	{
		retstr = L"104";
		return true;
	}
	if (0 == wcscmp(text, L"中国建设银行") || 0 == wcscmp(text, L"建行") ||
		0 == wcscmp(text, L"建设银行") || 0 == wcscmp(text, L"建设"))
	{
		retstr = L"105";
		return true;
	}
	if (0 == wcscmp(text, L"交通银行") || 0 == wcscmp(text, L"交行") ||
		0 == wcscmp(text, L"交通") || 0 == wcscmp(text, L"小娇"))
	{
		retstr = L"301";
		return true;
	}
	if (0 == wcscmp(text, L"中信银行") || 0 == wcscmp(text, L"中信") ||
		0 == wcscmp(text, L"铁公鸡") )
	{
		retstr = L"302";
		return true;
	}
	if (0 == wcscmp(text, L"中国光大银行") || 0 == wcscmp(text, L"光大银行") ||
		0 == wcscmp(text, L"光大"))
	{
		retstr = L"303";
		return true;
	}
	if (0 == wcscmp(text, L"华夏银行") || 0 == wcscmp(text, L"华夏"))
	{
		retstr = L"304";
		return true;
	}
	if (0 == wcscmp(text, L"中国民生银行") || 0 == wcscmp(text, L"民生银行") ||
		0 == wcscmp(text, L"民生"))
	{
		retstr = L"305";
		return true;
	}
	if (0 == wcscmp(text, L"广发银行") || 0 == wcscmp(text, L"广发") ||
		0 == wcscmp(text, L"废行"))
	{
		retstr = L"306";
		return true;
	}
	if (0 == wcscmp(text, L"平安银行") || 0 == wcscmp(text, L"平安"))
	{
		retstr = L"307";
		return true;
	}
	if (0 == wcscmp(text, L"招商银行") || 0 == wcscmp(text, L"招商")
		|| 0 == wcscmp(text, L"招行"))
	{
		retstr = L"308";
		return true;
	}
	if (0 == wcscmp(text, L"兴业银行") || 0 == wcscmp(text, L"兴业"))
	{
		retstr = L"309";
		return true;
	}
	if (0 == wcscmp(text, L"浦发银行") || 0 == wcscmp(text, L"浦发")
		|| 0 == wcscmp(text, L"猴子"))
	{
		retstr = L"310";
		return true;
	}
	if (0 == wcscmp(text, L"恒丰银行") || 0 == wcscmp(text, L"恒丰"))
	{
		retstr = L"311";
		return true;
	}

	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////

	if (0 == wcscmp(text, L"卡友"))
	{
		retstr = L"801";
		return true;
	}
	if (0 == wcscmp(text, L"快钱"))
	{
		retstr = L"812";
		return true;
	}
	if (0 == wcscmp(text, L"钱袋网"))
	{
		retstr = L"817";
		return true;
	}
	if (0 == wcscmp(text, L"富友"))
	{
		retstr = L"818";
		return true;
	}
	if (0 == wcscmp(text, L"盛付通"))
	{
		retstr = L"820";
		return true;
	}
	if (0 == wcscmp(text, L"通联支付") || 0 == wcscmp(text, L"通联"))
	{
		retstr = L"821";
		return true;
	}
	if (0 == wcscmp(text, L"拉卡拉"))
	{
		retstr = L"822";
		return true;
	}
	if (0 == wcscmp(text, L"汇付") || 0 == wcscmp(text, L"汇付天下"))
	{
		retstr = L"823";
		return true;
	}
	if (0 == wcscmp(text, L"银盛") || 0 == wcscmp(text, L"银盛支付"))
	{
		retstr = L"826";
		return true;
	}
	if (0 == wcscmp(text, L"杉德"))
	{
		retstr = L"827";
		return true;
	}
	if (0 == wcscmp(text, L"联动优势") || 0 == wcscmp(text, L"联动"))
	{
		retstr = L"829";
		return true;
	}
	if (0 == wcscmp(text, L"易生") || 0 == wcscmp(text, L"易生支付"))
	{
		retstr = L"831";
		return true;
	}
	if (0 == wcscmp(text, L"海科融通") || 0 == wcscmp(text, L"海科"))
	{
		retstr = L"833";
		return true;
	}
	if (0 == wcscmp(text, L"现代") || 0 == wcscmp(text, L"现代金控") || 0 == wcscmp(text, L"金控"))
	{
		retstr = L"834";
		return true;
	}
	if (0 == wcscmp(text, L"随行付") )
	{
		retstr = L"836";
		return true;
	}
	if (0 == wcscmp(text, L"点佰趣") || 0 == wcscmp(text, L"开店宝"))
	{
		retstr = L"843";
		return true;
	}
	if (0 == wcscmp(text, L"中付") || 0 == wcscmp(text, L"中付支付"))
	{
		retstr = L"847";
		return true;
	}
	if (0 == wcscmp(text, L"钱宝") || 0 == wcscmp(text, L"钱宝支付"))
	{
		retstr = L"848";
		return true;
	}
	if (0 == wcscmp(text, L"嘉联") || 0 == wcscmp(text, L"嘉联支付"))
	{
		retstr = L"849";
		return true;
	}
	if (0 == wcscmp(text, L"付临门"))
	{
		retstr = L"850";
		return true;
	}
	if (0 == wcscmp(text, L"瀚银"))
	{
		retstr = L"853";
		return true;
	}
	if (0 == wcscmp(text, L"国通星驿") || 0 == wcscmp(text, L"星驿付"))
	{
		retstr = L"857";
		return true;
	}
	if (0 == wcscmp(text, L"和融通"))
	{
		retstr = L"864";
		return true;
	}
	if (0 == wcscmp(text, L"中汇"))
	{
		retstr = L"868";
		return true;
	}
	if (0 == wcscmp(text, L"瑞银信"))
	{
		retstr = L"887";
		return true;
	}
	if (0 == wcscmp(text, L"乐刷"))
	{
		retstr = L"890";
		return true;
	}
	if (0 == wcscmp(text, L"盛迪嘉"))
	{
		retstr = L"893";
		return true;
	}
	if (0 == wcscmp(text, L"银联商务"))
	{
		retstr = L"898";
		return true;
	}
	if (0 == wcscmp(text, L"畅捷通") || 0 == wcscmp(text, L"畅捷支付") || 0 == wcscmp(text, L"畅捷"))
	{
		retstr = L"900";
		return true;
	}

	return false;
}



