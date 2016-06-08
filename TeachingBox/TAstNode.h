#ifndef _TEACHING_BOX_T_AST_NODE_H_
#define _TEACHING_BOX_T_AST_NODE_H_

class TToken;
class TVariate;

class TAstNode
{
public:
	TAstNode(const std::shared_ptr<TToken> token = nullptr);
	virtual ~TAstNode();

public:
	void AddChild(std::shared_ptr<TAstNode> child);
	std::shared_ptr<TToken> GetToken() const;
	std::shared_ptr<TAstNode> GetFirstChild() const;
	std::shared_ptr<TAstNode> GetSibling() const;
	virtual std::shared_ptr<TVariate> Execute();

private:
	std::shared_ptr<TToken> m_token=nullptr;
	const TAstNode* m_parentNode = nullptr;	/*采用普通指针而非智能指针是为了避免循环引用*/
	std::shared_ptr<TAstNode> m_firstChild = nullptr;
	std::shared_ptr<TAstNode> m_endChild = nullptr;
	std::shared_ptr<TAstNode> m_siblingChild = nullptr;
};

#endif