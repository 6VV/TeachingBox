#include "stdafx.h"
#include "CScreenProject.h"
#include "QFileSystemModel"
#include "QTreeView"
#include "CScreenMain.h"
#include "CKeyBoard.h"
#include "CDatabaseManager.h"
#include "CWarningManager.h"

CScreenProject::CScreenProject(QWidget* parent /*= 0*/) :CScreenMainParent(parent)
{
	Init();
}

CScreenProject::~CScreenProject()
{
	qDebug() << "ScreenProject Destroy";
	if (m_btnList.size() > 0)
	{
		qDeleteAll(m_btnList);
		m_btnList.clear();
	}

	delete(m_widgetFile);
}

CScreenProject* CScreenProject::GetInstance()
{
	return CSingleTon<CScreenProject>::GetInstance();
}

bool CScreenProject::IsCreated()
{
	return CSingleTon<CScreenProject>::IsCreated();
}

/*获取打开的文档名*/
const QString& CScreenProject::GetOpenedFileName()
{
	return m_strFileOpened;
}

const QStringList& CScreenProject::GetLoadedFileNames()
{
	return m_strListFilesLoaded;
}

const QString& CScreenProject::GetStartFileName()
{
	return m_strStartFile;
}

const QString CScreenProject::GetLoadedProjectNameInDatabase()
{
	return m_strProjectLoaded;
}

const QStringList& CScreenProject::GetLoadedProjectAllFilesExactName()
{
	return m_strListFileNames;
}


const QList<QString> CScreenProject::GetLoadedProjectAllFilesScopeName()
{
	return m_mapAllFiles[m_strProjectLoaded].keys();
}

QMap<QString, QMap<QString, QString>>& CScreenProject::GetAllFiles()
{
	return m_mapAllFiles;
}

/*获取打开的项目名*/
const QString& CScreenProject::GetLoadedProjectName()
{
	return m_strProjectLoaded;
}

/*获取打开的文件路径*/
const QString CScreenProject::GetOpenedFilePath()
{
	QStringList list=m_strFileOpened.split('.');
	return GetOpenedProjectPath() + "/" + list.at(1)+".txt";
}

/*获取打开的项目路径*/
const QString CScreenProject::GetOpenedProjectPath()
{
	return PROJECT_FILE_PATH + "/" + m_strProjectLoaded;
}

bool CScreenProject::IsLoadProject()
{
	return m_isLoadProject;
}

void CScreenProject::RefreshProgramLineNumber(const QString& programName, const int lineNumber)
{
	if (programName!=m_strFileOpened)
	{
		OpenFile(programName);
	}
	CScreenPragram::GetInstance()->RefreshPCLineNumber(lineNumber);
}

void CScreenProject::KeyboardEdit()
{
	(this->*KeyboardCallBack)();
}

/*************************************************
//  Function: 		KeyboardNewProject
//  Description: 	新建项目
//  Calls:		 	QMessageBox::warning
//  Called By: 		CKeyboard::OnButtonConfirmClicked
//  Parameter:      const QString
//  Return: 		
//  Others: 		键盘确认按钮按下事件
*************************************************/
void CScreenProject::KeyboardNewProject()
{
	QString projectName = CKeyboard::GetInstance()->GetCurrentText();

	QRegExp regExp(CRegExpManager::STR_REG_STRING_NAME);
	/*若名称不正确*/
	if (!regExp.exactMatch(projectName))
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::WrongName);
		return;
	}
	else if (projectName.size()>VAR_NAME_MAX_LENGTH)
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::NameIsTooLong);
		return;
	}

	QDir dir(PROJECT_FILE_PATH);
	if (dir.exists(projectName))
	{
		CWarningManager::GetInstance()->Warning(this, "Already Exist:\n"+projectName);
		return;
	}
	else if (dir.mkdir(projectName))
	{
		UpdateTreeWidget();
		return;
	}
}

void CScreenProject::KeyboardNewProgram()
{
	QString programName = CKeyboard::GetInstance()->GetCurrentText();

	QRegExp regExp(CRegExpManager::STR_REG_STRING_NAME);
	/*若名称不正确*/
	if (!regExp.exactMatch(programName))
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::WrongName);
		return;
	}
	else if (programName.size() > VAR_NAME_MAX_LENGTH)
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::NameIsTooLong);
		return;
	}

	QModelIndex index = m_treeWidget->currentIndex();

	/*若索引无效*/
	if (!index.isValid())
	{
		return;
	}

	QModelIndex parent = index.parent();
	QString strProjectName;

	/*若为项目*/
	if (!parent.isValid())
	{
		//strProjectName = m_modelProject->index(index.row(), 0, parent).data().toString();
		strProjectName = m_treeWidget->currentItem()->text(0);
	}
	/*若为程序*/
	else if (!parent.parent().isValid())
	{
		//strProjectName = m_modelProject->index(parent.row(), 0,parent.parent()).data().toString();
		strProjectName = m_treeWidget->currentItem()->parent()->text(0);
	}
	else
	{
		return;
	}

	QString strFileName = PROJECT_FILE_PATH + "/" + strProjectName + "/" + programName+".txt";

	/*若文件存在*/
	if (QFile::exists(strFileName))
	{
		CWarningManager::GetInstance()->Warning(this, "Already Exist:\n" + programName);
		return;
	}
	else
	{
		QFile file(strFileName);
		if (file.open(QIODevice::WriteOnly))
		{
			file.write("EOF");
			file.close();
			//m_modelProject->UpdateData();
			UpdateTreeWidget();
			return;
		}
	}
}

void CScreenProject::DeleteProject()
{
    QString strProjectName = m_treeWidget->currentItem()->text(0);

	/*若当前项目已被加载*/
	if (strProjectName == m_strProjectLoaded)
	{
		CWarningManager::GetInstance()->Warning(this, CWarningManager::ForbidDelete);
		return;
	}

	/*删除文件*/
	QStringList strListScopes;
	strListScopes << strProjectName;
	QStringList strListFilePaths;
	for (auto var = m_mapAllFiles[strProjectName].begin(); var != m_mapAllFiles[strProjectName].end(); ++var)
	{
		strListScopes << var.key();
		strListFilePaths << var.value();
	}

    for (auto var : strListFilePaths)
	{
		QFile::remove(var);
	}

	QDir dir(PROJECT_FILE_PATH);
	dir.rmdir(strProjectName);

	/*清除数据库及内存数据*/
    for  (auto var : strListScopes)
	{
		CInterpreterAdapter::GetInstance()->DeleteDatabaseData(var);
	}

	/*更新树形控件*/
	UpdateTreeWidget();
}

void CScreenProject::DeleteFile()
{

}

void CScreenProject::ClearFileInfo()
{
	m_strListFilesLoaded.clear();
	m_strListFileNames.clear();
	//m_strListFilePaths.clear();
	m_strProjectLoaded.clear();
}

void CScreenProject::OpenFile(const QString& strFileName)
{
	/*若文件已被打开*/
	if (strFileName==m_strFileOpened)
	{
		CScreenMain::GetInstance()->ChangeToScreenProgram();
	}
	/*若文件未被打开*/
	else
	{
		m_strFileOpened = strFileName;
		CFileManager fileManager;
		QString strText;
		fileManager.GetFileText(strText, m_mapAllFiles[m_strProjectLoaded][strFileName]);
		CCodeEditor::GetInstance()->setPlainText(strText);
		CScreenMain::GetInstance()->ChangeToScreenProgram();
	}
}

void CScreenProject::Init()
{	
	m_interpreterManager = CInterpreterManager::GetInstance();

	InitLayout();
	InitSignalSlot();
	RefreshText();
}

void CScreenProject::InitSignalSlot()
{
	///*鼠标双击*/
	//connect(m_viewTree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(SlotOpenFile()));

	/*打开按钮*/
	connect(m_btnOpen, SIGNAL(clicked()), this, SLOT(SlotOnBtnOpenClicked()));

	/*加载按钮*/
	connect(m_btnLoad, SIGNAL(clicked()), this, SLOT(SlotOnBtnLoadClicked()));

	/*文件按钮*/
	connect(m_btnFile, SIGNAL(clicked()), this, SLOT(SlotOnBtnFileClicked()));

	/*关闭按钮*/
	connect(m_btnKill, SIGNAL(clicked()), this, SLOT(SlotOnBtnKillClicked()));

	/*刷新按钮*/
	connect(m_btnRefresh, SIGNAL(clicked()), this, SLOT(SlotOnBtnRefreshClicked()));

	/*文件按钮列表*/
    for (CButton* button : m_btnListFile)
	{
		connect(button, SIGNAL(clicked()), this, SLOT(SlotFile()));
	}

	/*树形控件点击*/
	connect(m_treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(SlotOnTreeWidgetItemChanged(QTreeWidgetItem*, int)));
}

void CScreenProject::InitLayout()
{
	//m_modelProject = CModelProject::GetInstance();

	/*树形视图*/
	//m_viewTree = new QTreeView;
	//m_viewTree->setModel(m_modelProject);
	m_treeWidget = new QTreeWidget;
	m_treeWidget->setColumnCount(3);
	//GetAllFilesFromPath(PROJECT_FILE_PATH, m_treeWidget->invisibleRootItem());
	UpdateTreeWidget();

	/*底部按钮*/
	m_btnLoad = new CButton;
	m_btnOpen = new CButton;
	m_btnKill = new CButton;
	m_btnInfo = new CButton;
	m_btnRefresh = new CButton;
	m_btnFile = new CButton;
	//m_btnBack = new CButton;

	/*底部按钮列表*/
	m_btnList.append(m_btnLoad);
	m_btnList.append(m_btnOpen);
	m_btnList.append(m_btnKill);
	m_btnList.append(m_btnInfo);
	m_btnList.append(m_btnRefresh);
	m_btnList.append(m_btnFile);
	m_btnList.append(m_btnBack);

	/*文件按钮*/
	m_btnFileNewProject = new CButton;
	m_btnFileNewProgram = new CButton;
	m_btnFileDelete = new CButton;
	m_btnFileExport = new CButton;
	m_btnFileImport = new CButton;
	
	m_btnListFile.append(m_btnFileNewProject);
	m_btnListFile.append(m_btnFileNewProgram);
	m_btnListFile.append(m_btnFileDelete);
	m_btnListFile.append(m_btnFileExport);
	m_btnListFile.append(m_btnFileImport);

	m_widgetFile = new CWidgetButtonListVertical(m_btnListFile);

	/*添加控件*/
	m_layoutCenter->addWidget(m_treeWidget);
	m_screenBottom->AddButtonList(m_btnList);
}

/*************************************************
//  Function: 		GetProjectFileName
//  Description: 	获取当前选取的文件及项目信息
//  Calls:		 	
//  Called By: 		SlotFile
					SlotOpenFile
//  Parameter:      
//  Return: 		
//  Others: 		只有在选取项目下的文件时，才获取文件及项目名并返回真
*************************************************/
//bool CScreenProject::GetFileName()
//{
//	QModelIndex index = m_treeWidget->currentIndex();
//
//	if (!index.isValid())
//	{
//		return false;
//	}
//
//	if (!index.parent().isValid()){
//		return false;
//	}
//
//	//QModelIndex indexParent = index.parent();
//	//QModelIndex indexProject = m_modelProject->index(indexParent.row(), 0, indexParent.parent());
//	//QModelIndex indexFile = m_modelProject->index(index.row(), 0, indexParent);
//
//	m_strProjectLoaded =m_treeWidget->currentItem()->parent()->text(0);
//	m_strFileOpened = m_treeWidget->currentItem()->text(0);
//
//	return true;
//}

/*************************************************
//  Function: 		GetProjectName
//  Description: 	获取选取的项目名
//  Calls:		 	
//  Called By: 		
//  Parameter:      
//  Return: 		
//  Others: 		若选取的为项目名，则获取项目名，若选取的为文件，则获取文件名及项目名
*************************************************/
//bool CScreenProject::GetProjectName()
//{
//	QModelIndex index = m_treeWidget->currentIndex();
//
//	/*若未进行任何选择*/
//	if (!index.isValid())
//	{
//		return false;
//	}
//
//	QModelIndex indexParent = index.parent();
//	/*若选择的为项目*/
//	if (!indexParent.isValid())
//	{
//		//m_strProjectLoaded = m_modelProject->index(index.row(),0,indexParent.parent()).data().toString();
//		m_strProjectLoaded = m_treeWidget->currentItem()->text(0);
//	}
//	/*若选择的为文件*/
//	else
//	{
//		/*	QModelIndex indexProject = m_modelProject->index(indexParent.row(), 0, indexParent.parent());
//			QModelIndex indexFile = m_modelProject->index(index.row(), 0, indexParent);*/
//
//		m_strProjectLoaded = m_treeWidget->currentItem()->parent()->text(0);
//		m_strFileOpened = m_treeWidget->currentItem()->text(0);
//	}
//	return true;
//}

/*************************************************
//  Function: 		RefreshText
//  Description: 	刷新控件文本，用于国际化支持
//  Calls:
//  Called By: 		changeEvent
//  Parameter:      void
//  Return: 		void
//  Others: 		本函数重写父类函数，父类函数在CScreenMainParent中定义
*************************************************/
void CScreenProject::RefreshText()
{

	QStringList rootData;
	rootData << QCoreApplication::translate(CLASS_NAME, "Projects")
		<< QCoreApplication::translate(CLASS_NAME, "State")
		<< QCoreApplication::translate(CLASS_NAME, "Settings");
	m_treeWidget->setHeaderLabels(rootData);

	/*底部按钮文本*/
	m_btnLoad->setText(QCoreApplication::translate("CScreenProject", "Load"));
	m_btnOpen->setText(QCoreApplication::translate("CScreenProject", "Open"));
	m_btnKill->setText(QCoreApplication::translate("CScreenProject", "Kill"));
	m_btnInfo->setText(QCoreApplication::translate("CScreenProject", "Info"));
	m_btnRefresh->setText(QCoreApplication::translate("CScreenProject", "Refresh"));
	m_btnFile->setText(QCoreApplication::translate("CScreenProject", "File"));
	m_btnBack->setText(QCoreApplication::translate("CScreenProject", "Back"));

	/*文件按钮文本*/
	m_btnFileNewProject->setText(QCoreApplication::translate(CLASS_NAME, "New Project"));
	m_btnFileNewProgram->setText(QCoreApplication::translate(CLASS_NAME, "New Program"));
	m_btnFileDelete->setText(QCoreApplication::translate(CLASS_NAME, "Delete"));
	m_btnFileExport->setText(QCoreApplication::translate(CLASS_NAME, "Export"));
	m_btnFileImport->setText(QCoreApplication::translate(CLASS_NAME, "Import"));
	
	/*刷新列名*/
	/*m_modelProject->UpdateRootData();*/
	//UpdateTreeWidget();
}

bool CScreenProject::IsCorrectName(const QString& strName)
{
	QChar firstChar = strName.at(0);

	if (strName.size()>64)
	{
		QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "Name is too long"));
	}

	bool isCorrect = true;

	/*若第一个为字符*/
	if (!((firstChar >= 'a' && firstChar <= 'z') || (firstChar >= 'A' && firstChar <= 'Z') || firstChar == '_'))
	{
		isCorrect = false;
	}
	else
	{
        for (auto var : strName)
		{
			/*若不是字母、数字或下划线*/
			if (!((var >= 'a' && var <= 'z')
				|| (var >= 'A' && var <= 'Z')
				|| (var >= '0' && var <= '9')
				|| var == '_'))
			{
				isCorrect = false;
				break;
			}
		}
	}

	/*若名字不正确*/
	if (!isCorrect)
	{
		QMessageBox::warning(this, "", QCoreApplication::translate(CLASS_NAME, "Please input a correct name"));
	}

	return isCorrect;
}

bool CScreenProject::LoadProject(QTreeWidgetItem* item)
{
	QModelIndex index = m_treeWidget->currentIndex();
	if (item==NULL)
	{
		return false;
	}

	if (!item->parent()==NULL)
	{
		return false;
	}

	m_strProjectLoaded = item->text(0);

	/*获取项目下所有文件名*/
	GetProjectFiles(PROJECT_FILE_PATH + "/" + m_strProjectLoaded, m_strListFileNames);

	/*从数据库中更新项目数据*/
	m_isLoadProject = true;
	UpdateDataFromDatabase();
	QString currentFileName;
	/*加载项目文本*/
	try
	{
		CFileManager fileManager;

        for (auto fileName : m_strListFileNames)
		{
			currentFileName = fileName;
			QString strText;
			QString strScope = m_strProjectLoaded + "." + fileName;
			fileManager.GetFileText(strText, m_mapAllFiles[m_strProjectLoaded][strScope]);
			m_interpreterManager->AddProgramNode(strScope.toStdString(), strText.toStdString());
		}
		
	}
	catch (CExceptionInterpreter& e)
	{
		m_interpreterManager->RemoveAllPrograms();
		m_isLoadProject = false; 
		ClearFileInfo();

		std::string warningInfo;
		m_interpreterManager->GetWarningInfo(e, warningInfo);
		CWarningManager::GetInstance()->Warning(this, currentFileName+"\n"+QString::number(e.GetLineNumber())+"\n"+QString::fromStdString(warningInfo));
		return false;
	}


	item->setText(1, QCoreApplication::translate(CLASS_NAME, "Loaded"));

	return true;
}

void CScreenProject::LoadFile(QTreeWidgetItem* item)
{
	/*若为无效节点*/
	if (item==NULL)
	{
		return;
	}
	/*若为项目节点*/
	if (item->parent()==NULL)
	{
		return;
	}
	

	m_strFileOpened = item->text(0);
	m_strListFilesLoaded.append(item->text(0));
	m_isLoadFile = true;

	item->setText(1, QCoreApplication::translate(CLASS_NAME,"Loaded"));
}

void CScreenProject::GetProjectFiles(const QString& strProjectPath, QStringList& strListFiles)
{

	QDir dir(strProjectPath);

		/*若不存在文件，则返回*/
		if (!dir.exists())
		{
			return;
		}

		dir.setFilter(QDir::Dirs | QDir::Files);
		dir.setSorting(QDir::DirsFirst);


		/*读取文件信息*/
		QFileInfoList list = dir.entryInfoList();
		int i = 0;
		bool bIsDir; /*是否是文件夹*/
		while (i < list.size())
		{
			QFileInfo fileInfo = list.at(i);
			bIsDir = fileInfo.isDir();

			if (fileInfo.fileName() == "." || fileInfo.fileName() == ".." || bIsDir || fileInfo.suffix() != QString("txt"))
			{
				i++;
				continue;
			}

			strListFiles << fileInfo.baseName();
			i++;
		}

}


void CScreenProject::GetAllFilesFromPath(const QString & path, QTreeWidgetItem* parent)
{
	QDir dir(path);

	/*若不存在文件，则返回*/
	if (!dir.exists())
	{
		return;
	}

	dir.setFilter(QDir::Dirs | QDir::Files);
	dir.setSorting(QDir::DirsFirst);


	/*读取文件信息*/
	QFileInfoList list = dir.entryInfoList();
	int i = 0;
	bool bIsDir; /*是否是文件夹*/
	while (i < list.size())
	{
		QFileInfo fileInfo = list.at(i);
		bIsDir = fileInfo.isDir();

		if (fileInfo.fileName() == "." | fileInfo.fileName() == ".." | (!bIsDir && fileInfo.suffix() != QString("txt")))
		{
			i++;
			continue;
		}

		QStringList files;
		files << fileInfo.baseName() << "---" << "";
		QTreeWidgetItem *childItem = new QTreeWidgetItem(parent, files);

		if (bIsDir)
		{
			GetAllFilesFromPath(fileInfo.filePath(), childItem);
		}
		i++;
	}
}

void CScreenProject::GetAllFilesFromTree()
{
	m_mapAllFiles.clear();

	QTreeWidgetItem* rootItem = m_treeWidget->invisibleRootItem();

	for (int i = 0; i < rootItem->childCount();++i)
	{
		QTreeWidgetItem* projectItem = rootItem->child(i);

		for (int j = 0; j < projectItem->childCount();++j)
		{
			m_mapAllFiles[projectItem->text(0)][projectItem->text(0)+"."+projectItem->child(j)->text(0)]
				= PROJECT_FILE_PATH + "/" + projectItem->text(0) + "/" + projectItem->child(j)->text(0)+".txt";
		}
	}
}

void CScreenProject::UpdateTreeWidget()
{
	m_treeWidget->clear();
	GetAllFilesFromPath(PROJECT_FILE_PATH, m_treeWidget->invisibleRootItem());
	GetAllFilesFromTree();

	/*若加载项目*/
	if (!m_strProjectLoaded.isEmpty())
	{
		QList<QTreeWidgetItem*> itemListProject=m_treeWidget->findItems(m_strProjectLoaded,Qt::MatchFixedString,0);
		itemListProject.at(0)->setText(1, QCoreApplication::translate(CLASS_NAME, "Loaded"));
	}
	/*若加载程序*/
	if (!m_strListFilesLoaded.isEmpty())
	{
		QTreeWidgetItem* itemListProject = m_treeWidget->findItems(m_strProjectLoaded, Qt::MatchFixedString, 0).at(0);
		for (int i = 0; i < itemListProject->childCount(); i++)
		{
            for (auto fileName : m_strListFilesLoaded)
			{
				if (itemListProject->child(i)->text(0) == fileName)
				{
					itemListProject->child(i)->setText(1, QCoreApplication::translate(CLASS_NAME, "Loaded"));
				}
			}
		}
	}
}

void CScreenProject::UpdateDataFromDatabase()
{
	CInterpreterAdapter::GetInstance()->UpdateValueFromDatabase();
}

/*重置视图各列宽度*/
void CScreenProject::resizeEvent(QResizeEvent *event)
{
	m_treeWidget->setColumnWidth(0, this->width() / 2);
	m_treeWidget->setColumnWidth(1, this->width() / 4);
}

/*打开文件并显示程序界面*/
void CScreenProject::SlotOpenFile()
{
	/*if (GetFileName())
	{
		CScreenMain::GetInstance()->ChangeToScreenProgram();
		CInterpreterAdapter::GetInstance()->UpdateValueFromDatabase();
	}*/
}

/*打开按钮按下*/
void CScreenProject::SlotOnBtnOpenClicked()
{
	SlotOnBtnLoadClicked();
	//QModelIndex index = m_viewTree->currentIndex();

	///*若为根节点*/
	//if (!index.isValid())
	//{
	//	return;
	//}

	///*若为项目节点*/
	//if (!index.parent().isValid()){
	//	return;
	//}

	///*若为文件节点*/
	//QModelIndex indexParent = index.parent();	
	//QModelIndex indexProject = m_modelProject->index(indexParent.row(), 0, indexParent.parent()); /*获取项目节点*/
	//QModelIndex indexFile = m_modelProject->index(index.row(), 0, indexParent);	/*获取文件节点*/

	//QString strProject = indexProject.data().toString();
	//QString strFile = strProject + "." + indexFile.data().toString();

	///*若没有项目被加载*/
	//if (!m_isLoadProject)
	//{
	//	LoadProject(indexProject);
	//	m_isLoadProject = true;
	//}

	//m_strProjectLoaded = strProject;
	//m_strFileOpened = strFile;

	//qDebug() << m_strFileOpened;
	//qDebug() << m_strProjectLoaded;

	//return;
}

void CScreenProject::SlotOnBtnLoadClicked()
{
	QModelIndex index = m_treeWidget->currentIndex();

	/*若为根节点*/
	if (!index.isValid())
	{
		return;
	}

	/*若为项目节点*/
	if (!index.parent().isValid()){
		//QModelIndex indexProject = m_treeWidget->;

		/*若没有项目被加载*/
		if (!m_isLoadProject)
		{
			/*若加载失败*/
			if (!LoadProject(m_treeWidget->currentItem()))
			{
				return;
			}
		}
		else
		{
			/*若该项目已被加载*/
			if (m_treeWidget->currentItem()->text(0)==m_strProjectLoaded)
			{
				return;
			}
			else
			{
				CWarningManager::GetInstance()->Warning(this, CWarningManager::AlreadyLoadOtherProject);
				return;
			}
		}
		return;
	}

	/*若为文件节点*/
	//QModelIndex indexParent = index.parent();
	//QModelIndex indexProject = m_modelProject->index(indexParent.row(), 0, indexParent.parent()); /*获取项目节点*/
	//QModelIndex indexFile = m_modelProject->index(index.row(), 0, indexParent);	/*获取文件节点*/

	QString strProject = m_treeWidget->currentItem()->parent()->text(0);
	QString strFile = strProject + "." + m_treeWidget->currentItem()->text(0);
	
	/*若没有项目被加载*/
	if (!m_isLoadProject)
	{
		if (!LoadProject(m_treeWidget->currentItem()->parent()))
		{
			return;
		}
	}

	/*设置为启动文件*/
	m_strStartFile = strFile;

	/*若已有文件被加载*/
	if (m_isLoadFile)
	{
		/*若文件已被加载*/
		if (m_strListFilesLoaded.contains(strFile))
		{
			OpenFile(strFile);
			return;
		}
	}

	/*加载文件*/
	LoadFile(m_treeWidget->currentItem());
	OpenFile(strFile);

	return;
}

/*文件按钮按下*/
void CScreenProject::SlotOnBtnFileClicked()
{
	m_widgetFile->show();
	m_widgetFile->SetBottomPosition(m_btnFile);
}

void CScreenProject::SlotOnBtnKillClicked()
{
	QTreeWidgetItem* item = m_treeWidget->currentItem();

	/*若为无效节点*/
	if (item==NULL)
	{
		return;
	}

	/*若为项目节点*/
	if (item->parent()==NULL)
	{
		/*若未打开该项目*/
		if (item->text(0)!=m_strProjectLoaded)
		{
			return;
		}

		ClearFileInfo();

		item->setText(1, "---"); 
		
		for (int i = 0; i < item->childCount();++i)
		{
			item->child(i)->setText(1, "---");
		}

		m_isLoadProject = false;
		return;
	}

	/*若为文件节点*/
	item->setText(1, "---");
	
	for (int i = 0; i < m_strListFilesLoaded.size();++i)
	{
		if (m_strListFilesLoaded.at(i)==item->text(0))
		{
			m_strListFilesLoaded.removeAt(i);
			break;
		}
	}
	
	if (m_strListFilesLoaded.isEmpty())
	{
		m_isLoadFile = false;
	}

	/*设置按钮禁用状态*/
	m_btnOpen->setEnabled(true);
	m_btnLoad->setEnabled(true);
}

void CScreenProject::SlotOnBtnRefreshClicked()
{
	UpdateTreeWidget();
}

/*文件按钮列表点击事件*/
void CScreenProject::SlotFile()
{
	const int typeNull = 0;
	const int typeProject = 1;
	const int typeProgram = 2;

	int typeSelected=0;
	QModelIndex index = m_treeWidget->currentIndex();

	if (!index.isValid())
	{
		typeSelected = 0;
	}
	else if (!index.parent().isValid())
	{
		typeSelected = typeProject;
	}
	else
	{
		typeSelected = typeProgram;
	}

	CButton* button = (CButton*)sender();

	/*新建项目*/
	if (button==m_btnFileNewProject)
	{
		CKeyboard* keyboard=CKeyboard::GetInstance();
		KeyboardCallBack = &CScreenProject::KeyboardNewProject;
		keyboard->SetKeyboardInterface(this);
		keyboard->show();
	}
	/*新建程序*/
	else if (button==m_btnFileNewProgram)
	{
		if (typeSelected==typeNull)
		{
			CWarningManager::GetInstance()->Warning(this, "Please select a project");
			return;
		}

		QModelIndex parent = index.parent();

		CKeyboard* keyboard = CKeyboard::GetInstance();
		KeyboardCallBack = &CScreenProject::KeyboardNewProgram;
		keyboard->SetKeyboardInterface(this);
		keyboard->show();
	}
	/*删除文件*/
	else if (button==m_btnFileDelete)
	{
		switch (typeSelected)
		{
		case typeNull:
		{
			CWarningManager::GetInstance()->Warning(this, "Please select a project");
			return;
		}break;
		case typeProject:
		{
			DeleteProject();
		}break;
		case typeProgram:
		{
			DeleteFile();
		}break;
		default:
			break;
		}
	}
	/*导出数据*/
	else if (button == m_btnFileExport)
	{
		/*if (GetFileName())
		{
			CXmlVariableManager manager;
			manager.SetProjectName(m_strProjectLoaded);
			manager.SetProgramName(m_strFileOpened);
			manager.WriteProgramVariable();
		}*/
	}
	else if (button==m_btnFileImport)
	{
		/*if (GetFileName())
		{

		}*/
	}

	m_widgetFile->hide();
}

void CScreenProject::SlotOnTreeWidgetItemChanged(QTreeWidgetItem * item, int column)
{
	/*若为无效节点*/
	if (item==NULL)
	{
		m_btnOpen->setEnabled(false);
		m_btnLoad->setEnabled(false);
	}
	/*若为项目节点*/
	else if (item->parent()==NULL)
	{
		m_btnOpen->setEnabled(false);

		/*若当前已加载程序*/
		if (m_isLoadProject)
		{
			m_btnLoad->setEnabled(false);
		}
		else
		{
			m_btnLoad->setEnabled(true);
		}
	}
	/*若为程序节点*/
	else
	{
		/*若已加载项目，且目标程序不在该项目内*/
		if (m_isLoadProject && item->parent()->text(0)!=m_strProjectLoaded)
		{
			m_btnOpen->setEnabled(false);
			m_btnLoad->setEnabled(false);
		}
		else
		{
			m_btnOpen->setEnabled(true);
			m_btnLoad->setEnabled(true);
		}
	}
}
