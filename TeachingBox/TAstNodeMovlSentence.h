#ifndef _TEACHING_BOX_C_T_MOVL_SENTENCE_H_
#define _TEACHING_BOX_C_T_MOVL_SENTENCE_H_

#include "TAstNode.h"
#include "DataStruct.h"

class TAstNodeMovlSentence:public TAstNode
{
public:
	TAstNodeMovlSentence(const std::shared_ptr<TToken> token = nullptr);
	~TAstNodeMovlSentence();

	static const std::shared_ptr<TAstNode> GetAstNode(TLexer* const lexer);

	virtual ValueReturned Execute() const override;

	virtual void ParseSemantic() const override;


private:
	static const std::shared_ptr<TAstNode> GetParameter(TLexer* const lexer);

	static void CheckComma(TLexer* const lexer);
	void CheckParameterType(std::shared_ptr<TAstNode> node, int type) const;
	void SendMovlData() const;
	tMovLParam GetMovlParameter() const;

	tAxesAllPositions GetPosition(const QString& name) const;
	tDynamicConstraint GetDynamic(const QString& name) const;
	tOverlapConstraint GetOverlap(const QString& name) const;
};

#endif