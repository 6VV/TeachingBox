#include "stdafx.h"
#include "CModelProject.h"
#include "QDir"
#include "Init.h"



CModelProject::CModelProject(QObject* parent/*=0*/) :CTreeModel(parent)
{
	UpdateData();
	
}

CModelProject::~CModelProject()
{

}

CModelProject* CModelProject::GetInstance()
{
	return CSingleTon<CModelProject>::GetInstance();
}

bool CModelProject::IsCreated()
{
	return CSingleTon<CModelProject>::IsCreated();
}

void CModelProject::UpdateData()
{
	beginResetModel();
	/*废弃旧的模型数据*/
	if (rootItem)
	{
		rootItem->ClearChildren();
	}
	
	//rootItem = NULL;

	UpdateRootData();

	//setupModelData(rootItem);
	GetFileName(PROJECT_FILE_PATH,rootItem);
	endResetModel();
}

/*刷新列名*/
void CModelProject::UpdateRootData()
{
	/*列名*/
	QList<QVariant> rootData;
	rootData << QCoreApplication::translate("CModelProject", "Projects")
		<< QCoreApplication::translate("CModelProject", "State")
		<< QCoreApplication::translate("CModelProject", "Settings");

	if (rootItem==NULL)
	{
		rootItem = new CTreeItem(rootData);
	}
	else
	{
		rootItem->SetItemData(rootData);
	}
}


bool CModelProject::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
{
	if (!index.isValid())
	{
		return false;
	}

	///*添加图标*/
	//if (role == Qt::DecorationRole&&index.column() == 0)
	//	return QIcon(":/Image/Image/Light_Green.png");

	QList<QVariant> rootData;
	rootData << QCoreApplication::translate("CModelProject", "Projects")
		<< QCoreApplication::translate("CModelProject", "State")
		<< QCoreApplication::translate("CModelProject", "Settings");

		CTreeItem *item = static_cast<CTreeItem*>(index.internalPointer());
		item->SetItemData(rootData);

		UpdateData();

	return true;
}

void CModelProject::GetFileName(const QString & path, CTreeItem* parent)
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

		QList<QVariant> files;
		files << fileInfo.baseName() << "---" << "";
		CTreeItem *childItem = new CTreeItem(files, parent);
		parent->appendChild(childItem);

		if (bIsDir)
		{
			GetFileName(fileInfo.filePath(), childItem);
		}
		i++;
	}
}
