#ifndef STANDARDVIEW_H
#define STANDARDVIEW_H

#include <QtGui/QWidget>
#include "ui_StandardView.h"

class StandardView : public QWidget
{
    Q_OBJECT

public:
    StandardView(QWidget *parent = 0);
    virtual ~StandardView();

private:
    Ui::StandardViewClass ui;
};

#endif // STANDARDVIEW_H
