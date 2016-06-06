#ifndef _TEACHING_BOX_T_TOKEN_WITH_VALUE_H_
#define _TEACHING_BOX_T_TOKEN_WITH_VALUE_H_

#include "TToken.h"

template<typename T>
class TTokenWithValue:public TToken
{
public:
	TTokenWithValue(const TokenType type,const int lineNumber,const T& value);
	~TTokenWithValue(){};
	
	const T GetValue() const;

private:
	T m_value;
};

template<typename T>
const T TTokenWithValue<T>::GetValue() const
{
	return m_value;
}

template<typename T>
TTokenWithValue<T>::TTokenWithValue(const TokenType type, const int lineNumber, const T& value)
	:TToken(type, lineNumber), m_value(value)
{
}

#endif