#include "pietscenesizeinput.h"

#include <QDialogButtonBox>
#include <QFormLayout>

PietSceneSizeInput::PietSceneSizeInput(int rows, int cols)
{
    setAttribute(Qt::WA_DeleteOnClose);

    rowBox = new QSpinBox;
    colBox = new QSpinBox;

    rowBox->setMaximum(9999);
    rowBox->setValue(rows);
    colBox->setMaximum(9999);
    colBox->setValue(cols);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok
                                                       | QDialogButtonBox::Cancel);

    QFormLayout *mainLayout = new QFormLayout;
    mainLayout->addRow(tr("Rows: "), rowBox);
    mainLayout->addRow(tr("Columns: "), colBox);
    mainLayout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, this, [this] {
        emit picked(rowBox->value(), colBox->value());
        close();
    });

    setLayout(mainLayout);



}
