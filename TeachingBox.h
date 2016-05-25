#ifndef TEACHING_BOX_H
#define TEACHING_BOX_H

#define BOX_WIDTH 800
#define BOX_HEIGHT 600

#include <QWidget>

class TeachingBox : public QWidget
{
    Q_OBJECT

public:
    TeachingBox(QWidget *parent = 0);
    ~TeachingBox();

public:
    void init();    //
};

#endif // WIDGET_H
