#include "stdafx.h"
#include "TAstNodeFactory.h"
#include "TLexer.h"
#include "TInterpreterException.h"
#include "TAstNode.h"
#include "TAstNodeOperator.h"
#include "TAstNodeAssignSentence.h"
#include "TAstNodeForSentence.h"
#include "TAstNodeRoot.h"
#include "TAstNodeIfSentence.h"
#include "TAstNodeMovlSentence.h"
#include "CScreenProject.h"
#include "TTokenWithValue.h"
#include "CFileManager.h"
#include "TAstNodeProgram.h"

const std::shared_ptr<TAstNode> TAstNodeFactory::CreateAst()
{
	std::shared_ptr<TAstNode> result(new TAstNodeRoot);
	QList<QString> fileNames = CScreenProject::GetInstance()->GetLoadedProjectAllFilesScopeName();
	for (const QString& var : fileNames)
	{
		QString fileText;
		CFileManager::GetFileText(fileText, CScreenProject::GetInstance()->GetAllFiles()[CScreenProject::GetInstance()->GetLoadedProjectName()][var]);

		TLexer lexer(fileText);
		std::shared_ptr<TToken> token(new TTokenWithValue<QString>(TToken::ID, 0, var));

		result->AddChild(TAstNodeProgram::GetAstNode(&lexer, token));
	}

	return result;
}

const std::shared_ptr<TAstNode> TAstNodeFactory::GetOneNode(TLexer* const lexer)
{
	auto token = lexer->PeekToken();

	switch (token->GetType())
	{
	case TToken::ID:
	{
		return TAstNodeAssignSentence::GetAstNode(lexer);
	}break;
	case TToken::STRUCTURE_FOR:
	{
		return TAstNodeForSentence::GetAstNode(lexer);
	}break;
	case TToken::STRUCTURE_IF:
	{
		return TAstNodeIfSentence::GetAstNode(lexer);
	}break;
	case TToken::MACRO_MOVL:
	{
		return TAstNodeMovlSentence::GetAstNode(lexer);
	}break;
	default:
	{
		return nullptr;
	}break;
	}
}

