#ifndef _TEACHING_BOX_T_MACRO_DATA_H_
#define _TEACHING_BOX_T_MACRO_DATA_H_

class TMacroData
{
public:
	template<typename T>
	static void Send(const T& value, const int commandId,const int lineNum,const long long programAddress);

private:
	static 
	struct MacroData
	{
		char data[1024];
	};
	static void Send(const MacroData& data);

	template<typename T>
	static const MacroData GetData(const T& value, const int commandId, const int lineNum, const long long programAddress);

	static void AddLock(MacroData& data,int& offset);
	static void AddCommandAttribute(MacroData& data, int& offset, const int commandId, const int length, const int lineNum, const long long programAddress);

	template<typename T>
	static void AddCommand(MacroData& data, int& offset, const T& value);
};

template<typename T>
void TMacroData::Send(const T& value, const int commandId, const int lineNum, const long long programAddress)
{
	Send(GetData(value, commandId,lineNum,programAddress));
}

template<typename T>
const TMacroData::MacroData TMacroData::GetData(const T& value, const int commandId, const int lineNum, const long long programAddress)
{
	MacroData data;
	int offset = 0;
	AddLock(data, offset);
	AddCommandAttribute(data, offset, commandId, sizeof(T),lineNum,programAddress);
	AddCommand(data, offset, value);

	return data;
}

template<typename T>
void TMacroData::AddCommand(MacroData& data, int& offset, const T& value)
{
	*(T*)(data.data + offset) = value;
	offset += sizeof(T);
}

#endif