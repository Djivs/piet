#ifndef PIETGRAPHICSSCENE_H
#define PIETGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QVector>
#include <QImage>

enum class EditMode {PAINT, FILL, SELECT};

class PietGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PietGraphicsScene(QObject *parent = nullptr);

    QImage getImage() const;

    void addRow();
    void addColumn();
    void removeRow();
    void removeColumn();
    void removeColumns(int amount);
    void removeRows(int amount);

    void loadFromImage(QImage img);

    void setSize(int newRowsAmount, int newColsAmount);
    void setRowsAmount(int newRowsAmount);
    void setColsAmount(int newColsAmount);

    void setEditMode(EditMode newEditMode);
    void setSquareSize(int newSquareSize);

    int getRowsAmount() const;
    int getColsAmount() const;
public slots:
    void pickColor(QColor color);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    void selectSquare(int row, int col);

    void addGridItems();
    void addGridRow(int row);
    void addGridCol(int col);

    void resetGrid();
    void resetChecks();

    void fillSquare(int row, int col, const QColor &color);

    QGraphicsRectItem *getRect(int row, int col);
    QVector <QGraphicsRectItem*> getRectRow(int row);
    QColor getRectColor(int row, int col) const;
    bool isRectSelected(int row, int col);

    bool isRowValid(int row) const;
    bool isColValid(int col) const;
private:
    EditMode editMode;

    int rowsAmount;
    int colsAmount;

    int squareSize;

    QColor pickedColor;
    QColor baseColor;
    QColor selectColor;
    QColor lineColor;

    QVector <QVector <QGraphicsRectItem *>> grid;

    QVector <QVector <bool>> checks;

};

#endif // PIETGRAPHICSSCENE_H
