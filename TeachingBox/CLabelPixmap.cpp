#include "stdafx.h"
#include "CLabelPixmap.h"

CLabelPixmap::CLabelPixmap(const QString &text, QWidget* parent /*= 0*/) :QLabel(parent)
{
	m_pixmap=new QPixmap(text);
}

CLabelPixmap::CLabelPixmap(QWidget* parent /*= 0*/) : QLabel(parent)
{
	/*m_pixmap=new QPixmap;*/
}

CLabelPixmap::~CLabelPixmap()
{
	ClearImage();
}

void CLabelPixmap::resizeEvent(QResizeEvent *)
{
	ResizeImage();
	m_isResized = true;
}

void CLabelPixmap::ResizeImage()
{
	if (m_pixmap == NULL)
	{
		return;
	}
	else
	{
		QPixmap newPixmap = m_pixmap->scaled(this->width(), this->height(), Qt::KeepAspectRatio);
		this->clear();
		this->setPixmap(newPixmap);
	}
}

void CLabelPixmap::UpdateImage(const QString &text)
{
	ClearImage();
	m_pixmap = new QPixmap(text);

	if (m_isResized)
	{
		ResizeImage();
	}
}


void CLabelPixmap::ClearImage()
{
	if (m_pixmap!=NULL)
	{
		delete(m_pixmap);
		m_pixmap = NULL;
	}
}
