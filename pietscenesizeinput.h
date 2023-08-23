#ifndef PIETSCENESIZEINPUT_H
#define PIETSCENESIZEINPUT_H

#include <QDialog>
#include <QSpinBox>

class PietSceneSizeInput : public QDialog
{
    Q_OBJECT
public:
    explicit PietSceneSizeInput(int rows, int cols);
signals:
    void picked(int rows, int cols);
private:
    QSpinBox *rowBox;
    QSpinBox *colBox;

};

#endif // PIETSCENESIZEINPUT_H
