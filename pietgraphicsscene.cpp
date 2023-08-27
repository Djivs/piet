#include "pietgraphicsscene.h"

#include <QGraphicsRectItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
PietGraphicsScene::PietGraphicsScene(QObject *parent)
    : QGraphicsScene{parent} {
    squareSize = 25;

    rowsAmount = 15;
    colsAmount = 15;

    pickedColor = QColor(0xFF, 0x00, 0x00);
    baseColor = Qt::white;
    selectColor = Qt::magenta;
    lineColor = Qt::black;

    editMode = EditMode::PAINT;

    resetGrid();
    addGridItems();


    //loadFromImage(QImage("/home/dmitriy/Desktop/test.png"));
}

void PietGraphicsScene::selectSquare(int row, int col) {
    if (!isRowValid(row) || !isColValid(col)) {
        return;
    }
    QColor penColor;
    if (isRectSelected(row, col)) {
        penColor = lineColor;
    } else {
        penColor = selectColor;
    }

    grid[row][col]->setPen(QPen(penColor));
}

bool PietGraphicsScene::isRectSelected(int row, int col) {
    if (!isRowValid(row) || !isColValid(col)) {
        return false;
    }
    return grid[row][col]->pen().color() == selectColor;
}


void PietGraphicsScene::resetGrid() {
    grid.clear();

    for (int row = 0; row < rowsAmount; ++row) {
        grid.push_back(getRectRow(row));
    }
}

QGraphicsRectItem* PietGraphicsScene::getRect(int row, int col) {
    if (row < 0 || col < 0) {
        return nullptr;
    }

    QGraphicsRectItem *rect = new QGraphicsRectItem(col * squareSize, row * squareSize, squareSize, squareSize);
    rect->setBrush(QBrush(baseColor));

    return rect;
}

QVector <QGraphicsRectItem*> PietGraphicsScene::getRectRow(int row) {
    if (row < 0) {
        return QVector<QGraphicsRectItem*>();
    }

    QVector <QGraphicsRectItem*> rectRow;
    for (int col = 0; col < colsAmount; ++col) {
        rectRow.push_back(getRect(row, col));
    }
    return rectRow;
}

QColor PietGraphicsScene::getRectColor(int row, int col) const {
    if (!isRowValid(row) || !isColValid(col)) {
        return QColor();
    }

    return grid[row][col]->brush().color();
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
            image.setPixelColor(col, row, getRectColor(row, col));
        }
    }

    return image;
}

void PietGraphicsScene::loadFromImage(QImage img) {
    const QSize imageSize = img.size();
    setSize(imageSize.height(), imageSize.width());

    qDebug() << rowsAmount << colsAmount;

    for (int row = 0; row < rowsAmount; ++row) {
        for (int col = 0; col < colsAmount; ++col) {
            grid[row][col]->setBrush(img.pixelColor(col, row));
        }
    }
}

void PietGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF scenePos = event->scenePos();

    const int row = static_cast<int>(scenePos.y()) / squareSize;
    const int col = static_cast<int>(scenePos.x()) / squareSize;

    if (!isRowValid(row) || !isColValid(col)) {
        return;
    }

    switch (editMode) {
        case EditMode::PAINT:
            grid[row][col]->setBrush(QBrush(pickedColor));
            break;
        case EditMode::FILL:
        {
            resetChecks();
            QColor fillColor = getRectColor(row, col);
            fillSquare(row, col, fillColor);
        }
            break;
        case EditMode::SELECT:
            selectSquare(row, col);
            break;

    }
}

void PietGraphicsScene::fillSquare(int row, int col, const QColor &color) {
    if (!isRowValid(row) || !isColValid(col)) {
            return;
    }
    if (getRectColor(row, col) != color) {
            return;
    }
    if (checks[row][col]) {
        return;
    }

    checks[row][col] = true;
    grid[row][col]->setBrush(QBrush(pickedColor));


    fillSquare(row - 1, col,     color);
    fillSquare(row,     col - 1, color);
    fillSquare(row + 1, col,     color);
    fillSquare(row,     col + 1, color);
}

void PietGraphicsScene::addRow() {
    grid.push_back(getRectRow(rowsAmount));
    ++rowsAmount;

    addGridRow(rowsAmount - 1);
}
void PietGraphicsScene::addColumn() {
    for (int row = 0; row < rowsAmount; ++row) {
        grid[row].push_back(getRect(row, colsAmount));
    }
    ++colsAmount;

    addGridCol(colsAmount - 1);
}

void PietGraphicsScene::removeRow() {
    if (!rowsAmount) {
        return;
    }

    for (int col = 0; col < colsAmount; ++col) {
        removeItem(grid[rowsAmount - 1][col]);
    }

    grid.pop_back();
    --rowsAmount;
}
void PietGraphicsScene::removeColumn() {
    if (!colsAmount) {
        return;
    }

    for (int row = 0; row < rowsAmount; ++row) {
        removeItem(grid[row][colsAmount - 1]);
        grid[row].pop_back();
    }

    --colsAmount;
}

void PietGraphicsScene::removeColumns(int amount) {
    if (amount > colsAmount) {
        amount = colsAmount;
    }

    const int start = colsAmount - amount;
    const int end = colsAmount;

    for (int row = 0; row < rowsAmount; ++row) {
        for (int col = start; col < end; ++col) {
            removeItem(grid[row][col]);
        }
        grid[row].erase(grid[row].begin() + start, grid[row].end());
    }

    colsAmount -= amount;
}

void PietGraphicsScene::removeRows(int amount) {
    if (amount > rowsAmount) {
        amount = rowsAmount;
    }

    const int start = rowsAmount - amount;
    const int end = rowsAmount;

    for (int i = start; i < end; ++i) {
        for (int col = 0; col < colsAmount; ++col) {
            removeItem(grid[rowsAmount - 1][col]);
        }
        grid.pop_back();
        --rowsAmount;
    }
}

void PietGraphicsScene::setSize(int newRowsAmount, int newColsAmount) {
    setRowsAmount(newRowsAmount);
    setColsAmount(newColsAmount);
}

void PietGraphicsScene::setRowsAmount(int newRowsAmount) {
    if (newRowsAmount < 0 || newRowsAmount == rowsAmount) {
        return;
    }

    const int minRowsAmount = std::min(rowsAmount, newRowsAmount);
    const int maxRowsAmount = std::max(rowsAmount, newRowsAmount);

    if (minRowsAmount == newRowsAmount) {
        removeRows(maxRowsAmount - minRowsAmount);
    } else {
        for (int i = minRowsAmount; i < maxRowsAmount; ++i) {
            addRow();
        }
    }
}
void PietGraphicsScene::setColsAmount(int newColsAmount) {
    if (newColsAmount < 0 || newColsAmount == colsAmount) {
        return;
    }

    const int minColsAmount = std::min(colsAmount, newColsAmount);
    const int maxColsAmount = std::max(colsAmount, newColsAmount);

    if (minColsAmount == newColsAmount) {
        removeColumns(maxColsAmount - minColsAmount);
    } else {
        for (int i = minColsAmount; i < maxColsAmount; ++i) {
            addColumn();
        }
    }
}
void PietGraphicsScene::setSquareSize(int newSquareSize) {
    if (newSquareSize <= 0) {
        return;
    }

    squareSize = newSquareSize;

    for (int row = 0; row < rowsAmount; ++row) {
        for (int col = 0; col < colsAmount; ++col) {
            grid[row][col]->setRect(col * squareSize, row * squareSize, squareSize,squareSize);
        }
    }
}

void PietGraphicsScene::addGridItems() {
    for (auto &row : grid) {
        for (auto &rect : row) {
            addItem(rect);
        }
    }
}

void PietGraphicsScene::addGridRow(int row) {
    if (!isRowValid(row)) {
        return;
    }

    for (auto &rect: grid[row]) {
        addItem(rect);
    }
}
void PietGraphicsScene::addGridCol(int col) {
    if (!isColValid(col)) {
        return;
    }

    for (int row = 0; row < rowsAmount; ++row) {
        addItem(grid[row][col]);
    }
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

bool PietGraphicsScene::isRowValid(int row) const {
    return (row >= 0) and (row < rowsAmount);
}
bool PietGraphicsScene::isColValid(int col)  const{
    return (col >= 0) and (col < colsAmount);
}
