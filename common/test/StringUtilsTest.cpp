#include "TestCommon.h"
#include "../src/stringUtils.cpp"
#include <climits>

void test_trim(const char* str, const char* verify)
{
    string result = StringUtils::trim(str);
    TEST_ASSERT(result == verify, "str=[%s] verify=[%s] result=[%s]", str, verify, result.c_str());
}

void test_trim_all()
{
    test_trim(" aa    ", "aa");
    test_trim(" aa", "aa");
    test_trim("aa    ", "aa");
    test_trim(" aa  \t  ", "aa");
}

void test_replace(const char* str, const char* oldStr, const char* newStr, const char* verify)
{
    string result = StringUtils::replace(str, oldStr, newStr);
    TEST_ASSERT(result == verify, "str=[%s] oldStr=[%s] newStr=[%s] verify=[%s] result=[%s]"
        , str, oldStr, newStr, verify, result.c_str());
}

void test_replace_all()
{
    test_replace("aaaaaaaaaaaaa", "b", "c", "aaaaaaaaaaaaa");
    test_replace("bbbbbbbbbbbbb", "b", "c", "ccccccccccccc");
    test_replace("aaabbaabbbbaa", "bb", "c", "aaacaaccaa");
}

void test_getLines(const char* str, bool ignoreBlank, size_t verify)
{
    list<string> result = StringUtils::getLines(str, ignoreBlank);
    TEST_ASSERT(result.size() == verify, "str=[%s] ignoreBlank=[%s] verify=[%d] result=[%d]"
        , str, ignoreBlank ? "true" : "false", verify, result.size());
}

void test_getLines_all()
{
    test_getLines("aaaaaaaa", true, 1);
    test_getLines("aaaaaaaa\nbbbbb", true, 2);
    test_getLines("aaaaaaaa\n\nbbbbb", true, 2);
    test_getLines("aaaaaaaa\n\nbbbbb\n\n", true, 2);
    test_getLines("\r\naaaaaaaa\n\nbbbbb\n\n", true, 2);
    
    test_getLines("aaaaaaaa", false, 1);
    test_getLines("aaaaaaaa\nbbbbb", false, 2);
    test_getLines("aaaaaaaa\n\nbbbbb", false, 3);
    test_getLines("aaaaaaaa\n\nbbbbb\n\n", false, 5);
    test_getLines("\r\naaaaaaaa\n\nbbbbb\n\n", false, 6);
}

void test_splitString(const char* str, const char* sep, size_t verify)
{
    list<string> result = StringUtils::splitString(str, sep);
    TEST_ASSERT(result.size() == verify, "str=[%s] sep=[%s] verify=[%d] result=[%d]"
        , str, sep, verify, result.size());
}

void test_splitString_all()
{
    test_splitString("bbbaaaacccc", ",", 1);
    test_splitString("bbb,aaaa,cccc", ",", 3);
    test_splitString(",bbb,aaaa,cccc", ",", 4);
    test_splitString(",bbb,aaaa,cccc,", ",", 5);
    test_splitString(",bbb,,aaaa,cccc,", ",", 6);
}

void test_itoa(int v, const char* verify)
{
    string str = StringUtils::itoa(v);
    TEST_ASSERT(str == verify, "v=[%d] verify=[%s] result=[%s]"
        , v, verify, str.c_str());
}

void test_itoa_all()
{
    test_itoa(0, "0");
    test_itoa(100, "100");
    test_itoa(-1, "-1");
    test_itoa(-100, "-100");
    test_itoa(INT_MAX, "2147483647");
    test_itoa(INT_MIN, "-2147483648");
}


void test_atoi(const char* str, int verify)
{
    int result = StringUtils::atoi(str);
    TEST_ASSERT(result == verify, "str=[%s] verify=[%d] result=[%d]"
        , str, verify, result);
}

void test_atoi_all()
{
    test_atoi("0", 0);
    test_atoi("100", 100);
    test_atoi("-1", -1);
    test_atoi("-100", -100);
    test_atoi("2147483647", INT_MAX);
    test_atoi("-2147483648", INT_MIN);

    //不支持16进制转换
    //test_atoi("0x1", 1);
    //test_atoi("0xFFFFFFFF", -1);
    //test_atoi("0x7FFFFFFF", INT_MAX);
    //test_atoi("0x80000000", INT_MIN);
}


void test_ltoa(long long v, const char* verify)
{
    string result = StringUtils::ltoa(v);
    TEST_ASSERT(result == verify, "verify=[%s] result=[%s]"
        , verify, result.c_str());
}

void test_ltoa_all()
{
    test_ltoa(0, "0");
    test_ltoa(100, "100");
    test_ltoa(-1, "-1");
    test_ltoa(-100, "-100");
    test_ltoa(INT_MAX, "2147483647");
    test_ltoa(INT_MIN, "-2147483648");
    test_ltoa(LONG_LONG_MAX, "9223372036854775807");
    test_ltoa(LONG_LONG_MIN, "-9223372036854775808");
}



void test_toLower(const char* str, const char* verify)
{
    string result = StringUtils::toLower(str);
    TEST_ASSERT(result == verify, "str=[%s] verify=[%s] result=[%s]"
        , str, verify, result.c_str());
}

void test_toLower_all()
{
    test_toLower("abc", "abc");
    test_toLower("ABC", "abc");
    test_toLower("AB你好C12", "ab你好c12");
}

void test_toUpper(const char* str, const char* verify)
{
    string result = StringUtils::toUpper(str);
    TEST_ASSERT(result == verify, "str=[%s] verify=[%s] result=[%s]"
        , str, verify, result.c_str());
}

void test_toUpper_all()
{
    test_toUpper("abc", "ABC");
    test_toUpper("ABC", "ABC");
    test_toUpper("AB你好C12", "AB你好C12");
}

void test_strncpy_z(unsigned int destLen, const char* src, const char* verify)
{
    char* dest = new char[destLen + 1];
    memset(dest, 0, destLen + 1);
    StringUtils::strncpy_z(dest, src, destLen);
    TEST_ASSERT(strcmp(dest, verify) == 0 && dest[destLen] == 0, "destLen=[%d] src=[%s] verify=[%s] result=[%s]"
        , destLen, src, verify, dest);
    delete dest;
}

void test_strncpy_z_all()
{
    test_strncpy_z(0, "aaaaaa", "");
    test_strncpy_z(1, "aaaaaa", "");
    test_strncpy_z(2, "aaaaaa", "a");
    test_strncpy_z(6, "aaaaaa", "aaaaa");
    test_strncpy_z(7, "aaaaaa", "aaaaaa");
    test_strncpy_z(10, "aaaaaa", "aaaaaa");
}

void test_strncat_z(const char* dest, unsigned int destLen, const char* src, const char* verify)
{
    char* dest2 = new char[destLen + 1];
    memset(dest2, 0, destLen + 1);
    strcpy(dest2, dest);
    StringUtils::strncat_z(dest2, src, destLen);
    TEST_ASSERT(strcmp(dest2, verify) == 0 && dest2[destLen] == 0, "dest=[%s] destLen=[%d] src=[%s] verify=[%s] result=[%s]"
        , dest, destLen, src, verify, dest2);
    delete dest2;
}

void test_strncat_z_all()
{
    test_strncat_z("a", 2, "123456", "a");
    test_strncat_z("a", 10, "123456", "a123456");
    test_strncat_z("a", 3, "123456", "a1");
    test_strncat_z("", 3, "123456", "12");
    test_strncat_z("", 10, "123456", "123456");
}

void test_string()
{
    const char* str = "bbbbbbbb";
    string a = "bbbbbbbb";
    string b = a;
    printf("str=%p a=%p b=%p\n", str, a.c_str(), b.c_str());
}

int main()
{
    test_itoa_all();
    test_atoi_all();
    test_ltoa_all();
    test_trim_all();
    test_replace_all();
    test_getLines_all();
    test_splitString_all();
    test_toLower_all();
    test_toUpper_all();
    test_strncpy_z_all();
    test_strncat_z_all();
    test_string();

    printf("all successed\n");
    return 0;
}
