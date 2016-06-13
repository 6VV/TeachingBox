#ifndef _TEACHING_BOX_T_AST_NODE_H_
#define _TEACHING_BOX_T_AST_NODE_H_

class TToken;
class TVariate;
#include <memory>

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
	std::shared_ptr<TToken> GetToken() const;
	std::shared_ptr<TAstNode> GetFirstChild() const;
	std::shared_ptr<TAstNode> GetSibling() const;
	virtual ValueReturned Execute() const;
	virtual void ParseSemantic() const{};

protected:
	static bool IsEofOrEol(const int type);
	TAstNode* FindNextValidNode() const;

protected:
	std::shared_ptr<TToken> m_token=nullptr;
	TAstNode* m_parentNode = nullptr;	/*采用普通指针而非智能指针是为了避免循环引用*/
	std::shared_ptr<TAstNode> m_firstChild = nullptr;
	std::shared_ptr<TAstNode> m_endChild = nullptr;
	std::shared_ptr<TAstNode> m_siblingNode = nullptr;
};

#endif