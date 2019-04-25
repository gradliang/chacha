#ifndef UTF_H____
#define UTF_H____

int utf8_to_wstring(wchar_t * u16s, size_t widesize, const char * u8s);
int wstring_to_utf8(char * u8s, size_t u8size, const wchar_t * u16s);

bool checkIsLetterOrNumber(const wchar_t * str);
bool checkIsNumber(const wchar_t * str);
int strcmp_16_8(const wchar_t * src, const char * dst);
unsigned int crc32_hash(const unsigned char* pbData, size_t size);
void capitalization(wchar_t * str);

#ifndef WIN32
int _wtoi(const wchar_t * str);
int _stricmp(const char * dst, const char* src);
#define  sprintf_s   snprintf
#endif


#endif // !UTF_H____














