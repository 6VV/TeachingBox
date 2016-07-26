#ifndef _TEACHING_BOX_T_AST_NODE_H_
#define _TEACHING_BOX_T_AST_NODE_H_

#include <memory>

class TToken;
class TVariate;
class TLexer;

class TAstNode
{
public:
	struct ValueReturned
	{
		double value;
		int type;
	};

public:
	TAstNode(const std::shared_ptr<TToken> token = nullptr);
	virtual ~TAstNode();

public:
	void AddChild(std::shared_ptr<TAstNode> child);
	virtual TAstNode* FindNextValidNode() const;

	std::shared_ptr<TToken> GetToken() const;
	std::shared_ptr<TAstNode> GetFirstChild() const;
	std::shared_ptr<TAstNode> GetEndChild() const;
	std::shared_ptr<TAstNode> GetSibling() const;
	TAstNode* GetParentNode() const;

	virtual ValueReturned Execute() const;
	virtual void ParseSemantic() const{};

protected:
	static void AddSentenceNodes(TLexer* const lexer, std::shared_ptr<TAstNode> parentNode);

	static void CheckLineBreak(TLexer* const lexer);
	static void CheckEofEol(TLexer* const lexer);

	static bool IsEofOrEol(const int type);

	static void SkipEol(TLexer* const lexer);

protected:
	std::shared_ptr<TToken> m_token=nullptr;
	TAstNode* m_parentNode = nullptr;	/*采用普通指针而非智能指针是为了避免循环引用*/
	std::shared_ptr<TAstNode> m_firstChild = nullptr;
	std::shared_ptr<TAstNode> m_endChild = nullptr;
	std::shared_ptr<TAstNode> m_siblingNode = nullptr;
};

#endif