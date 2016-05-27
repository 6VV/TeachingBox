#ifndef _TEACHING_BOX_C_SCREEN_PROJECT_H_
#define _TEACHING_BOX_C_SCREEN_PROJECT_H_

/*************************************************
//  Copyright (C), 2015-2016, CS&S. Co., Ltd.
//  File name: 	    CScreenProject.h
//  Author:			刘巍
//  Version: 		1.0
//  Date: 			2015/11/24
//  Description:	用于显示项目界面
//  Others:
//  History:
//    <author>      刘巍
//    <time>        2015/11/24
//    <version>     1.0
//    <desc>        build this moudle
*************************************************/

#include "QBoxLayout"
#include "Init.h"
#include "QWidget"
#include "CTreeModel.h"
#include "QTreeView"
#include "CModelProject.h"
#include "CScreenMainParent.h"
#include "CSingleTon.h"
#include "CWidgetButtonListVertical.h"
#include "QMap"
#include "CKeyBoardInterface.h"
#include "CInterpreterManager.h"
#include "CFileManager.h"

class CScreenProject :public CScreenMainParent,public CKeyBoardInterface
{
	friend class CSingleTon<CScreenProject>;	/*单例模式*/

	Q_OBJECT
private:
	CScreenProject(QWidget* parent = 0);
	CScreenProject(const CScreenProject&);
	CScreenProject& operator=(const CScreenProject&);
	~CScreenProject();

	/*自定义公有函数*/
public:
	static CScreenProject* GetInstance();
	static bool IsCreated();

	const QString& GetOpenedFileName();					/*获取打开的文件名（项目名+文件名）*/
	const QStringList& GetLoadedFileNames();			/*获取加载的文件名（项目名+文件名）*/
	const QString& GetStartFileName();					/*获取启动文件名（项目名+文件名）*/
	const QString GetLoadedProjectNameInDatabase();		/*获取在数据库中存储的项目名*/
	const QStringList& GetLoadedProjectAllFilesExactName();		/*获取项目下所有文件名（文件名）*/
	const QList<QString> GetLoadedProjectAllFilesScopeName();		/*获取项目下所有文件名（项目名+文件名）*/
	//const QStringList& GetProjectFilePaths();			/*获取项目下所有文件路径*/

	QMap<QString, QMap<QString, QString>>& GetAllFiles();	/*获取所有文件路径*/
	const QString& GetLoadedProjectName();	/*获取打开的项目名称*/
	const QString GetOpenedFilePath();		/*获取打开的文件路径*/
	const QString GetOpenedProjectPath();	/*获取打开的项目路径*/

	bool IsLoadProject();	/*是否有加载项目*/

	void RefreshProgramLineNumber(const QString& programName, const int lineNumber);

	void KeyboardEdit() Q_DECL_OVERRIDE;	/*键盘回调函数 */
	
	typedef void(CScreenProject::*KeyboardOperator)(void);	/*当前操作*/
	KeyboardOperator KeyboardCallBack;

	/*自定义私有函数*/
private:
	void Init();
	void InitSignalSlot();
	void InitLayout();

	//bool GetFileName();		/*获取当前选取的文件及项目名*/
	//bool GetProjectName();	/*获取选取的项目名*/

	void RefreshText() Q_DECL_OVERRIDE;

	bool IsCorrectName(const QString& strName);	/*是否是正确的名称*/

	bool LoadProject(QTreeWidgetItem*);		/*加载项目*/
	void LoadFile(QTreeWidgetItem*);		/*加载文件*/

	void GetProjectFiles(const QString& strProjectPath, QStringList& strListFiles);

	void GetAllFilesFromPath(const QString & path, QTreeWidgetItem* parent);
	void GetAllFilesFromTree();

	void UpdateTreeWidget();

	void KeyboardNewProject();	/*新建项目*/
	void KeyboardNewProgram();	/*新建程序*/

	void DeleteProject();	/*删除项目*/
	void DeleteFile();		/*删除文件*/

	void ClearFileInfo();	/*清除加载的文件信息*/

	void OpenFile(const QString& strFileName);	/*打开文件，并切换视图*/

	/*重写父类函数*/
protected:
	void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

	/*槽函数*/
protected slots:
	void SlotOpenFile();
	void SlotOnBtnOpenClicked();
	void SlotOnBtnLoadClicked();
	void SlotOnBtnFileClicked();
	void SlotOnBtnKillClicked();
	void SlotOnBtnRefreshClicked();
	void SlotFile();

	void SlotOnTreeWidgetItemChanged(QTreeWidgetItem * item, int column);

private:
	//CModelProject* m_modelProject;
	//QTreeView* m_viewTree;

	QTreeWidget* m_treeWidget;

	bool m_isLoadProject = false;	/*是否已有项目被加载*/
	bool m_isLoadFile = false;		/*是否已有文件被加载*/

	QMap<QString, QMap<QString, QString>> m_mapAllFiles; /*所有文件,（项目名、文件名、文件路径）*/

	QString m_strFileOpened;			/*被打开的文件（项目名+文件名）*/
	QString m_strStartFile;				/*启动文件*/

	QStringList m_strListFilesLoaded;	/*被加载的文件（项目名+文件名）*/
	QString m_strProjectLoaded;			/*被加载的项目*/

	QStringList m_strListFileNames;		/*项目下所有文件名（项目名+文件名）*/
	//QStringList m_strListFilePaths;		/*项目下所有文件路径*/

	CInterpreterManager* m_interpreterManager;	/*解释器管理器*/
private:
	const char* CLASS_NAME = "CScreenProject";

	/*底部按钮*/
	CButton* m_btnLoad;
	CButton* m_btnOpen;
	CButton* m_btnKill;
	CButton* m_btnInfo;
	CButton* m_btnRefresh;
	CButton* m_btnFile;
	//CButton* m_btnBack;

	/*文件指令按钮*/
	CWidgetButtonListVertical* m_widgetFile;
	QList<CButton*> m_btnListFile;
	CButton* m_btnFileNewProject;
	CButton* m_btnFileNewProgram;
	CButton* m_btnFileDelete;
	CButton* m_btnFileExport;
	CButton* m_btnFileImport;
};

#endif