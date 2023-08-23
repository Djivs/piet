#ifndef PIETWIDGET_H
#define PIETWIDGET_H

#include <QWidget>
#include <QGraphicsView>

#include "pietgraphicsscene.h"
#include "pietcolorpicker.h"

class PietWidget : public QWidget
{
    Q_OBJECT

public:
    PietWidget(QWidget *parent = nullptr);
    ~PietWidget();
private:
    void setupUi();
private:
    PietGraphicsScene *scene;
    QGraphicsView *view;
    PietColorPicker *colorPicker;


};
#endif // PIETWIDGET_H
