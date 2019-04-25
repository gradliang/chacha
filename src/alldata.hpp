


#pragma once


struct jifeninfo
{
	const wchar_t*			desc;
	const wchar_t*			undesired;
};

struct mccinfo
{
	unsigned int	crc32; 
	const wchar_t*	scode;					// MCC码
	const wchar_t*	largeClass;				// 大类
	const wchar_t*	desc;					// 描述
	const wchar_t*	fee;					// 费率
	const wchar_t*	old96;					// 96费改前的费率
	const wchar_t*	warning;				// 警告信息
	const wchar_t*	detail;					// 详情
	const jifeninfo *jifen;					// 积分情况
}; 

#define MCC_HASH_TABLE_SIZE			(331)

extern const int mcc_bucket_size[];
extern const mccinfo * const mccinfo_buckets[];
#define AREA_MAX_SIZE				(10000)
struct areainfo {
	const wchar_t*    scode; 
	int				icode; 
	const wchar_t*	display; 
}; 

extern const unsigned char area_bucket_size[];
extern const areainfo * const areainfo_buckets[];
#define BANK_HASH_TABLE_SIZE			(673) 
struct bankinfo {
	const wchar_t*    scode; 
	const wchar_t*	display; 
	unsigned int	crc32; 
}; 
extern const unsigned char bank_bucket_size[];
extern const bankinfo * const bankinfo_buckets[];
struct BankcommHeiShop
{
	const char *	str; 
	unsigned		crc32; 
}; 
#define HEISHOP_HASH_TABLE_SIZE			(2729)

extern const int BankcommHeiShop_bucket_size[]; 
extern const BankcommHeiShop *  const BankcommHeiShop_buckets[];
struct TianshuDBUnit
{
	const char *	str; 
	unsigned		crc32; 
}; 
#define TIANSHU_HASH_TABLE_SIZE			(10949)

#define TIANSHU_COUNT						(3)

struct tianshu_db_struct { 
	const TianshuDBUnit * const * const buckets; 
	const int * const bucket_size; 
	const wchar_t * const month; 
}; 
extern const struct tianshu_db_struct tianshu_db[];
#define AD_COUNT  (2) 
extern const wchar_t * const ad_texts[];   

