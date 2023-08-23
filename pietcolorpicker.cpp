#include "pietcolorpicker.h"

#include <QDebug>

PietColorPicker::PietColorPicker(QWidget *parent)
    : QWidget{parent} {
    rowButtonsAmount = 6;

    colors = { QColor(0xFF, 0xC0, 0xC0), QColor(0xFF, 0xFF, 0xC0), QColor(0xC0, 0xFF, 0xC0),
               QColor(0xC0, 0xFF, 0xFF), QColor(0xC0, 0xC0, 0xFF), QColor(0xFF, 0xC0, 0xFF),
               QColor(0xFF, 0x00, 0x00), QColor(0xFF, 0xFF, 0x00), QColor(0x00, 0xFF, 0x00),
               QColor(0x00, 0xFF, 0xFF), QColor(0x00, 0x00, 0xFF), QColor(0xFF, 0x00, 0xFF),
               QColor(0xC0, 0x00, 0x00), QColor(0xC0, 0xC0, 0x00), QColor(0x00, 0xC0, 0x00),
               QColor(0x00, 0xC0, 0xC0), QColor(0x00, 0x00, 0xC0), QColor(0xC0, 0x00, 0xC0),
               QColor(0xFF, 0xFF, 0xFF), QColor(0x00, 0x00, 0x00)};

    checkMarkIcon = QIcon(":/img/checkmark.png");

    setupUi();
}

QColor PietColorPicker::getPickedColor() const {
    for (QPushButton  *button : buttons) {
        if (button->isChecked()) {
            return button->palette().color(QPalette::Button);
        }
    }

    return QColor();
}

void PietColorPicker::setupUi() {
    initButtons();
    initButtonsLayout();

    setLayout(buttonsLayout);
}

void PietColorPicker::initButtons() {
    for (auto &color : colors) {
        QPushButton *button = new QPushButton;

        QPalette palette;
        palette.setColor(QPalette::Button, color);
        button->setPalette(palette);

        connect(button, &QPushButton::clicked, this, [this, button] {
            pickedColor = button->palette().color(QPalette::Button);

            emit colorPicked(pickedColor);
        });

        buttons.push_back(button);
    }
}
void PietColorPicker::initButtonsLayout() {
    buttonsLayout = new QGridLayout;
    buttonsLayout->setAlignment(Qt::AlignLeft);

    for (int i = 0; i < buttons.size(); ++i) {
        buttonsLayout->addWidget(buttons[i], i / rowButtonsAmount, i % rowButtonsAmount);
    }

}
