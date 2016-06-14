#include "stdafx.h"
#include "TAstNode.h"
#include "TToken.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TGrammarParser.h"


TAstNode::TAstNode(const std::shared_ptr<TToken> token /*= nullptr*/)
	:m_token(token)
{

}

TAstNode::~TAstNode()
{

}

void TAstNode::AddChild(std::shared_ptr<TAstNode> child)
{
	child->m_parentNode = this;
	child->m_siblingNode = nullptr;

	if (!m_firstChild)
	{
		m_firstChild = child;
		m_endChild = child;
	}
	else
	{
		m_endChild->m_siblingNode = child;
		m_endChild = child;
	}
}


std::shared_ptr<TToken> TAstNode::GetToken() const
{
	return m_token;
}

std::shared_ptr<TAstNode> TAstNode::GetFirstChild() const
{
	return m_firstChild;
}

std::shared_ptr<TAstNode> TAstNode::GetEndChild() const
{
	return m_endChild;
}

std::shared_ptr<TAstNode> TAstNode::GetSibling() const
{
	return m_siblingNode;
}

TAstNode* TAstNode::GetParentNode() const
{
	return m_parentNode;
}

TAstNode::ValueReturned TAstNode::Execute() const
{
	return ValueReturned{};
}

void TAstNode::AddSentenceNodes(TLexer* const lexer, std::shared_ptr<TAstNode> parentNode)
{
	std::shared_ptr<TAstNode> childNode{};
	while (childNode = TGrammarParser::GetOneNode(lexer))
	{
		parentNode->AddChild(childNode);
	}
}

void TAstNode::CheckLineBreak(TLexer* const lexer)
{
	if (lexer->GetToken()->GetType() != TToken::SEPARATOR_EOL)
	{
		throw TInterpreterException(TInterpreterException::SENTENCE_SHOULD_END_WITH_LINE_BREAK, lexer->GetToken()->GetLineNumber());
	}
}

void TAstNode::CheckEofEol(TLexer* const lexer)
{
	if (!IsEofOrEol(lexer->GetToken()->GetType()))
	{
		throw TInterpreterException(TInterpreterException::SENTENCE_NOT_END_WITH_ABNORMAL_END, lexer->GetToken()->GetLineNumber());
	}

}

bool TAstNode::IsEofOrEol(const int type)
{
	if (type == TToken::SEPARATOR_EOF || type == TToken::SEPARATOR_EOL)
	{
		return true;
	}

	return false;
}

TAstNode* TAstNode::FindNextValidNode() const
{
	if (m_siblingNode != nullptr)
	{
		/*若兄弟节点与当前节点在同一行，则继续寻找下一节点*/
		if (m_siblingNode->GetToken()->GetLineNumber() == m_token->GetLineNumber())
		{
			return m_siblingNode->FindNextValidNode();
		}

		return m_siblingNode.get();
	}

	if (m_parentNode != nullptr)
	{
		return m_parentNode->FindNextValidNode();
	}

	return nullptr;
}
