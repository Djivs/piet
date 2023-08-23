#ifndef PIETCOLORPICKER_H
#define PIETCOLORPICKER_H

#include <QWidget>
#include <QColor>
#include <QVector>
#include <QPushButton>
#include <QGridLayout>

class PietColorPicker : public QWidget
{
    Q_OBJECT
public:
    explicit PietColorPicker(QWidget *parent = nullptr);

    QColor getPickedColor() const;
signals:
    void colorPicked(QColor);
private:
    void setupUi();

    void initButtons();
    void initButtonsLayout();
private:
    int rowButtonsAmount;

    QColor pickedColor;

    QIcon checkMarkIcon;

    QVector <QColor> colors;
    QVector <QPushButton *> buttons;
    QGridLayout *buttonsLayout;


};

#endif // PIETCOLORPICKER_H
