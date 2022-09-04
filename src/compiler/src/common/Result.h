#pragma once
#include "Common.h"

// enum
// {
// 	ERROR_NOTFOUND_SYMBOL = -10005,
// 	//ERROR_ILLEGAL_USE = -10004,					//非法使用
// 	ERROR_NOTFOUND_PACKAGE_CLASS = -10003,
// 	ERROR_ILLEGAL_EXPRESSION = -10002,				//错误表达式
// 	//ERROR_UNEXPECTED_CHARACTER = -10001,			//异常字符
// 	ERROR_BEGIN = -10000,

// 	SUCCESS = 0,

// 	WARN_BEGIN = 10000,
// };

enum
{
	R_FAILED = -1,
	R_SUCCESS = 0
};

class Result
{
public:
	Result() = default;
	Result(int err, const string& msg = "")
	{
		this->error = err;
		this->errMsg = msg;
	}

	bool isSuccess() const
	{
		return error >= R_SUCCESS;
	}

	bool isError() const
	{
		return error < R_SUCCESS;
	}

	// bool isWarn()
	// {
	// 	return error > SUCCESS;
	// }

public:
	int error = R_SUCCESS;
	string errMsg;
};

//VERIFY_RESULT
#define VR(exp) \
{ \
	auto r = exp; \
	if (r.isError()) \
	{ \
		printf("return faild. file=%s:%d\n", __FILE__, __LINE__); \
		return r; \
	} \
}

