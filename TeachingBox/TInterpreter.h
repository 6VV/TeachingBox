#ifndef _TEACHING_BOX_T_INTERPRETER_H_
#define _TEACHING_BOX_T_INTERPRETER_H_

class TAstNode;

class TInterpreter
{
public:
	static void ExecuteAll(int lineNum);
	static void ExecuteOneLine(int lineNum);

	static void StopExecute();

	static void Parse(const QString& text);

private:
	static TAstNode* FindStartNode(std::shared_ptr<TAstNode> rootNode,int lineNum);
	static void InitContext(int lineNum);
};

#endif