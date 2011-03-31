#ifndef STANDARDVIEW_H
#define STANDARDVIEW_H

#include <QtGui/QWidget>
#include "ui_standardview.h"

class StandardView : public QWidget, public Ui::StandardView
{
    Q_OBJECT

public:
    StandardView(QWidget *parent = 0);
    virtual ~StandardView();


};

#endif // STANDARDVIEW_H
