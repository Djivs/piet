#include "pieteditor.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QImage>
#include <QDebug>
#include <QGroupBox>
#include <QRadioButton>
#include <QSlider>
#include <QLabel>

#include "pietscenesizeinput.h"

PietEditor::PietEditor(QMainWindow *parent)
    : QMainWindow(parent) {
    setupUi();
}

PietEditor::~PietEditor() {
}

void PietEditor::setupUi() {
    scene = new PietGraphicsScene;
    view = new QGraphicsView;
    view->setScene(scene);

    colorPicker = new PietColorPicker;
    connect(colorPicker, &PietColorPicker::colorPicked, scene, &PietGraphicsScene::pickColor);

    QRadioButton *paintButton = new QRadioButton(tr("Paint"));
    paintButton->setChecked(true);
    QRadioButton *fillButton = new QRadioButton(tr("Fill"));
    QRadioButton *selectButton = new QRadioButton(tr("Select"));

    connect(paintButton, &QRadioButton::clicked, this, [this] {
        scene->setEditMode(EditMode::PAINT);
    });
    connect(fillButton, &QRadioButton::clicked, this, [this] {
        scene->setEditMode(EditMode::FILL);
    });
    connect(selectButton, &QRadioButton::clicked, this, [this] {
        scene->setEditMode(EditMode::SELECT);
    });

    QVBoxLayout *editModeLayout = new QVBoxLayout;
    editModeLayout->addWidget(paintButton);
    editModeLayout->addWidget(fillButton);
    editModeLayout->addWidget(selectButton);

    QGroupBox *editModeBox = new QGroupBox;
    editModeBox->setLayout(editModeLayout);
    editModeBox->setTitle(tr("Edit mode:"));

    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setMaximum(100);
    slider->setMinimum(1);
    slider->setValue(25);
    connect(slider, &QSlider::sliderMoved, this, [this] (int pos){
        scene->setSquareSize(pos);
    });

    QHBoxLayout *sliderLayout = new QHBoxLayout;
    sliderLayout->addWidget(new QLabel(tr("Square size:")));
    sliderLayout->addWidget(slider);

    QVBoxLayout *layout1_0 = new QVBoxLayout;
    layout1_0->addWidget(editModeBox);
    layout1_0->addLayout(sliderLayout);

    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(colorPicker);
    layout1->addLayout(layout1_0);

    QPushButton *exportButton = new QPushButton(tr("Export"));
    QPushButton *setCanvasSizeButton = new QPushButton(tr("Set canvas size"));
    QPushButton *addRowButton = new QPushButton(tr("Add row"));
    QPushButton *addColumnButton = new QPushButton(tr("Add column"));
    QPushButton *removeRowButton = new QPushButton(tr("Remove row"));
    QPushButton *removeColumnButton = new QPushButton(tr("Remove column"));

    connect(exportButton, &QPushButton::clicked, this, [this] {
        const QString fileName = QFileDialog::getSaveFileName();
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "can't open file :(";
            return;
        }

        QImage image = scene->getImage();


        if (image.save(&file, "png", 100)) {
            qDebug() << "image saving: success";
        } else {
            qDebug() << "image saving: fail";
        }

        file.close();

    });
    connect(setCanvasSizeButton, &QPushButton::clicked, this, [this] {
        PietSceneSizeInput *input = new PietSceneSizeInput(scene->getRowsAmount(), scene->getColsAmount());
        input->show();
        connect(input, &PietSceneSizeInput::picked, this, [this] (int rows, int cols) {
            scene->setSize(rows, cols);
        });
    });
    connect(addRowButton, &QPushButton::clicked, this, [this] {
        scene->addRow();
    });
    connect(addColumnButton, &QPushButton::clicked, this, [this] {
        scene->addColumn();
    });
    connect(removeRowButton, &QPushButton::clicked, this, [this] {
        scene->removeRow();
    });
    connect(removeColumnButton, &QPushButton::clicked, this, [this] {
        scene->removeColumn();
    });

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(exportButton);
    buttonsLayout->addWidget(setCanvasSizeButton);
    buttonsLayout->addWidget(addRowButton);
    buttonsLayout->addWidget(addColumnButton);
    buttonsLayout->addWidget(removeRowButton);
    buttonsLayout->addWidget(removeColumnButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addLayout(layout1);
    mainLayout->addWidget(view);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);

    setCentralWidget(centralWidget);
}
