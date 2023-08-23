#include "pietgraphicsscene.h"

#include <QGraphicsRectItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
PietGraphicsScene::PietGraphicsScene(QObject *parent)
    : QGraphicsScene{parent} {
    squareSize = 25;

    rowsAmount = 10;
    colsAmount = 10;

    pickedColor = QColor(0xFF, 0x00, 0x00);

    editMode = EditMode::PAINT;

    resetColors();
    drawSquares();
}

void PietGraphicsScene::drawSquares() {
    for (int row = 0; row < rowsAmount; ++row) {
        drawSquaresRow(row);
    }
}

void PietGraphicsScene::drawSquaresRow(int row) {
    for (int col = 0; col < colsAmount; ++col) {
        drawSquare(row, col);
    }
}

void PietGraphicsScene::drawSquaresCol(int col) {
    for (int row = 0; row < rowsAmount; ++row) {
        drawSquare(row, col);;
    }
}

void PietGraphicsScene::drawSquare(int row, int col) {
    QGraphicsRectItem *rect = new QGraphicsRectItem(col * squareSize, row * squareSize, squareSize, squareSize);
    rect->setBrush(QBrush(colors[row][col]));
    addItem(rect);
}

void PietGraphicsScene::resetColors() {
    colors.clear();

    for (int row = 0; row < rowsAmount; ++row) {
        colors.push_back(getColorVector(Qt::white, colsAmount));
    }
}

void PietGraphicsScene::resetChecks() {
    checks.clear();
    for (int row = 0; row < rowsAmount; ++row) {
        QVector <bool> rowChecks;
        for (int col = 0; col < colsAmount; ++col) {
            rowChecks.push_back(false);
        }
        checks.push_back(rowChecks);
    }
}

QImage PietGraphicsScene::getImage() const {
    QImage image(colsAmount, rowsAmount, QImage::Format_RGB16);

    for (int row = 0; row < rowsAmount; ++row) {
        for (int col = 0; col < colsAmount; ++col) {
            image.setPixelColor(col, row, colors[row][col]);
        }
    }

    return image;
}

void PietGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF scenePos = event->scenePos();

    const int row = static_cast<int>(scenePos.y()) / squareSize;
    const int col = static_cast<int>(scenePos.x()) / squareSize;

    if (row < 0|| col < 0
        || row >= rowsAmount
        || col >= colsAmount) {
        return;
    }

    switch (editMode) {
        case EditMode::PAINT:
            colors[row][col] = pickedColor;
            drawSquare(row, col);
            break;
        case EditMode::FILL:
            resetChecks();
            QColor fillColor = colors[row][col];
            fillSquare(row, col, fillColor);
            break;

    }
}

void PietGraphicsScene::fillSquare(int row, int col, const QColor &color) {
    if (row < 0 || col < 0
        || row >= rowsAmount
        || col >= colsAmount) {
            return;
    }
    if (colors[row][col] != color) {
            return;
    }
    if (checks[row][col]) {
        return;
    }

    checks[row][col] = true;
    colors[row][col] = pickedColor;
    drawSquare(row, col);


    fillSquare(row - 1, col,     color);
    fillSquare(row,     col - 1, color);
    fillSquare(row + 1, col,     color);
    fillSquare(row,     col + 1, color);
}

void PietGraphicsScene::addRow() {
    colors.push_back(getColorVector(Qt::white, colsAmount));

    drawSquaresRow(rowsAmount++);
}
void PietGraphicsScene::addColumn() {
    for (auto &colorVector : colors) {
        colorVector.push_back(Qt::white);
    }

    drawSquaresCol(colsAmount++);
}

void PietGraphicsScene::removeRow() {
    colors.pop_back();
    --rowsAmount;

    clear();
    drawSquares();

}
void PietGraphicsScene::removeColumn() {
    for (auto &colorVector : colors) {
        colorVector.pop_back();
    }
    --colsAmount;

    clear();
    drawSquares();

}
void PietGraphicsScene::setSize(int newRowsAmount, int newColsAmount) {
    setRowsAmount(newRowsAmount);
    setColsAmount(newColsAmount);
}

void PietGraphicsScene::setRowsAmount(int newRowsAmount) {
    const int minRowsAmount = std::min(rowsAmount, newRowsAmount);
    const int maxRowsAmount = std::max(rowsAmount, newRowsAmount);

    void (PietGraphicsScene::*rowFunc) ();
    if (minRowsAmount == rowsAmount) {
        rowFunc = &PietGraphicsScene::addRow;
    } else {
        rowFunc = &PietGraphicsScene::removeRow;
    }

    for (int i = minRowsAmount; i < maxRowsAmount; ++i) {
        (*this.*rowFunc)();
    }
}
void PietGraphicsScene::setColsAmount(int newColsAmount) {
    const int minColsAmount = std::min(colsAmount, newColsAmount);
    const int maxColsAmount = std::max(colsAmount, newColsAmount);

    void (PietGraphicsScene::*colFunc) ();
    if (minColsAmount == colsAmount) {
        colFunc = &PietGraphicsScene::addColumn;
    } else {
        colFunc = &PietGraphicsScene::removeColumn;
    }

    for (int i = minColsAmount; i < maxColsAmount; ++i) {
        (*this.*colFunc)();
    }
}
void PietGraphicsScene::setSquareSize(int newSquareSize) {
    squareSize = newSquareSize;

    clear();
    drawSquares();
}



void PietGraphicsScene::setEditMode(EditMode newEditMode) {
    editMode = newEditMode;
}

void PietGraphicsScene::pickColor(QColor color) {
    pickedColor = color;
}

int PietGraphicsScene::getRowsAmount() const {
    return rowsAmount;
}
int PietGraphicsScene::getColsAmount() const {
    return colsAmount;
}

QVector <QColor> PietGraphicsScene::getColorVector(const QColor &color, int len) {
    QVector <QColor> colorVector;
    for (int i = 0; i < len; ++i) {
        colorVector.push_back(color);
    }

    return colorVector;
}
