/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CCodeEditor.h
//  Author:			刘巍      
//  Version: 		1.0     
//  Date: 			2015/12/09
//  Description:	文本编辑控件，可显示代码行数并高亮显示部分行
//  Others:
//  History:
//    <author>      刘巍 
//    <time>        2015/12/09
//    <version>     1.0 
//    <desc>        build this moudle     
*************************************************/

#ifndef _TEACHING_BOX_C_CODE_EDITOR_H_
#define _TEACHING_BOX_C_CODE_EDITOR_H_

#include <QPlainTextEdit>
#include <QObject>
#include "CSingleTon.h"
#include "QColor"
#include "CTextFormat.h"

class CLineNumberArea;	/*左侧行号区域*/

class CCodeEditor : public QPlainTextEdit
{
	friend CSingleTon<CCodeEditor>;

	Q_OBJECT

	enum OperatorType
	{
		UPDATE_LINE,
		DELETE_LINE,
		INSERT_LINE,
	};
private:
	CCodeEditor(QWidget *parent = 0);
	CCodeEditor(const CCodeEditor&);
	CCodeEditor& operator=(const CCodeEditor&);
	~CCodeEditor();

public:
	static CCodeEditor* GetInstance();	/*获取唯一实例*/
	int GetPCLineNumber();	/*获取PC行号*/
	int GetLineNumberAreaWidth();		/*返回左侧区域宽度*/

	void ClearWrongLine();	/*清除错误行颜色*/

	void DealText();	/*处理文本*/
	void DeleteCurrentLine();	/*删除当前行*/

	void HighlightPCLine(const int lineNumber);	/*高亮显示PC行*/
	void InsertTextBeforeLine(const QString& strText);	/*插入文本*/
	void InsertTextBeforeLineUnsafely(const QString& strText);	/*插入文本（不进行语法检查）*/
	void PaintLineNumberArea(QPaintEvent *event);	/*绘制左侧行号区域*/

	void SaveFile();	/*保存文件*/

	void UpdateLineText(const QString& strNewText);	/*更新文本*/

public slots:
	void setPlainText(const QString &text);

	/*重定义父类函数*/
protected:
	void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
	void showEvent(QShowEvent *event)	Q_DECL_OVERRIDE;

	/*自定义槽函数*/
protected slots:
	void SlotUpdateCodeAreaWidth(int newBlockCount);	/*更新右侧区域宽度*/
	void SlotHighlightCurrentLine();	/*高亮显示某行*/
	void SlotUpdateLineNumberArea(const QRect &, int);	/*更新左侧区域*/

private:
	void FormatText(const QString& text); /*格式化文本*/
	void FormatCurrentText();	/*格式化当前文本*/
	bool ParseText();		/*解析文本并生成树*/

	void HighLightEditLine(const int lineNumber);	/*高亮显示编辑行*/
	void HighlightWrongLine(const int lineNumber);	/*高亮显示错误行*/

	void _DeleteCurrentLine();
	void _UpdateCurrentLine(const QString& text);
	void _InsertTextBeforeLine(const QString& text);

	void OperatorText(OperatorType operatorType, const QString& text = QString{});	/*操作文本*/
private:
	int m_textChangeTime = 0;
	QWidget *m_lineNumberArea;

	QList<QTextEdit::ExtraSelection> m_extraSelections;	/*所有颜色行*/
	QTextEdit::ExtraSelection m_selectSelection;	/*当前选择行*/
	QTextEdit::ExtraSelection m_pcSelection;	/*当前执行行*/
	QTextEdit::ExtraSelection m_wrongSelection;	/*错误代码行*/

	QColor COLOR_PC = QColor(Qt::blue).lighter(160);	/*蓝色，显示当前执行行*/
	QColor COLOR_WRONG = QColor(Qt::red).light(160);	/*红色，出错行*/
	QColor COLOR_EDIT = QColor(Qt::yellow).light(160);	/*黄色，当前编辑行*/
};

#endif