#ifndef PIETGRAPHICSSCENE_H
#define PIETGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QVector>
#include <QImage>

enum class EditMode {PAINT, FILL};

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
    void drawSquares();
    void drawSquaresRow(int row);
    void drawSquaresCol(int col);
    void drawSquare(int row, int col);

    void resetColors();
    void resetChecks();

    void fillSquare(int row, int col, const QColor &color);

    static QVector <QColor> getColorVector(const QColor &color, int len);
private:
    EditMode editMode;

    int rowsAmount;
    int colsAmount;

    int squareSize;

    QColor pickedColor;

    QVector <QVector <QColor>> colors;

    QVector <QVector <bool>> checks;

};

#endif // PIETGRAPHICSSCENE_H
