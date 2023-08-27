#ifndef PIETEDITOR_H
#define PIETEDITOR_H

#include <QMainWindow>
#include <QGraphicsView>

#include "pietgraphicsscene.h"
#include "pietcolorpicker.h"

class PietEditor : public QMainWindow {
    Q_OBJECT

public:
    PietEditor(QMainWindow *parent = nullptr);
    ~PietEditor();
private:
    void setupUi();
private:
    PietGraphicsScene *scene;
    QGraphicsView *view;
    PietColorPicker *colorPicker;


};
#endif // PIETEDITOR_H
