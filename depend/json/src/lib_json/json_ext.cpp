//
//  "$Id$"
//
//  Copyright (c)1992-2012, ZheJiang Dahua Technology Stock CO.LTD.
//  All Rights Reserved.
//
//	Description:
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//


#include <json/ext.h>
#include <json/writer.h>
#include <json/reader.h>


namespace Json {

////////////////////////////////////////////////////////////////////////////////

struct FastWriterWrapper::FastWriterWrapperInternal
{
	std::string	m_string;
	FastWriter	m_writer;

	FastWriterWrapperInternal()
	{
	}
};

FastWriterWrapper::FastWriterWrapper(Json::Value const& root, size_t reserve)
{
	m_internal = new FastWriterWrapperInternal();
	m_internal->m_string.reserve(reserve);
	m_internal->m_string = m_internal->m_writer.write(root);
}

FastWriterWrapper::~FastWriterWrapper()
{
	delete m_internal;
}

const char* FastWriterWrapper::data() const
{
	return m_internal->m_string.c_str();
}

size_t FastWriterWrapper::size() const
{
	return m_internal->m_string.size();
}

////////////////////////////////////////////////////////////////////////////////

struct StyledWriterWrapper::StyledWriterWrapperInternal
{
	std::string	m_string;
	StyledWriter	m_writer;

	StyledWriterWrapperInternal()
	{
	}
};

StyledWriterWrapper::StyledWriterWrapper(Json::Value const& root, size_t reserve)
{
	m_internal = new StyledWriterWrapperInternal();
	m_internal->m_string.reserve(reserve);
	m_internal->m_string = m_internal->m_writer.write(root);
}

StyledWriterWrapper::~StyledWriterWrapper()
{
	delete m_internal;
}

const char* StyledWriterWrapper::data() const
{
	return m_internal->m_string.c_str();
}

size_t StyledWriterWrapper::size() const
{
	return m_internal->m_string.size();
}

////////////////////////////////////////////////////////////////////////////////

struct ReaderWrapper::ReaderWrapperInternal
{
	Reader		m_reader;
	std::string	m_errmsg;
	bool		m_errmsg_invalid;
};

/// 构造函数
ReaderWrapper::ReaderWrapper()
{
	m_internal = new ReaderWrapperInternal();
	m_internal->m_errmsg_invalid = false;
}

/// 析构函数
ReaderWrapper::~ReaderWrapper()
{
	delete m_internal;
}

/// \brief Read a Value from a <a HREF="http://www.json.org">JSON</a> document.
bool ReaderWrapper::parse(const char *beginDoc, const char *endDoc, Value &root, bool collectComments)
{
	m_internal->m_errmsg_invalid = true;
	return m_internal->m_reader.parse(beginDoc, endDoc, root, collectComments);
}

/// \brief Returns a user friendly string that list errors in the parsed document.
const char* ReaderWrapper::getFormattedErrorMessages() const
{
	if (m_internal->m_errmsg_invalid)
	{
		m_internal->m_errmsg = m_internal->m_reader.getFormatedErrorMessages();
		m_internal->m_errmsg_invalid = false;
	}

	return m_internal->m_errmsg.c_str();
}

////////////////////////////////////////////////////////////////////////////////


struct MemberNames::MemberNamesInternal
{
	Value::Members members;
};

/// 构造函数
MemberNames::MemberNames(Json::Value const& value)
{
	m_internal = new MemberNamesInternal();
	m_internal->members = value.getMemberNames();
}

/// 析构函数
MemberNames::~MemberNames()
{
	delete m_internal;
}

/// 取成员个数
size_t MemberNames::count() const
{
	return m_internal->members.size();
}

/// 取成员名称
char const* MemberNames::operator[](size_t index) const
{
	return m_internal->members[index].c_str();
}

} // namespace Json


