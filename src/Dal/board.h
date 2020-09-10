#ifndef BOARD_H
#define BOARD_H

#include "Dal/Image/boardimagecontroller.h"
#include "tiledata.h"


namespace Dal {


class Board : public QObject {
    Q_OBJECT

    using Position = std::pair<int, int>;

    enum class ShiftDirection
    {
        Right,
        Left,
        Top,
        Bottom
    };

public:
    Board(int dimension, QObject *parent = nullptr);

    ~Board() override;

    void move(int index);

    int tilesNumber() const;

    int dimension() const;

    int hiddenValue() const;

    int hiddenIndex() const;

    bool isMovable(Position pos, Position hidPos) const;

    bool isMovable(int index) const;

    bool isGameWon() const;

    TileData operator[](int index);

signals:
    void sigImagesCached();

    void sigCachingError(QString errorString);

private:
    Position getRowCol(int index) const;

    int getIndex(Position pos) const;

    void shaffleBoard();

    bool isBoardSolvable() const;

    int findHiddenIndex() const;

    void shift2D(QVector<TileData> &vec, Position strtPos, int count, ShiftDirection direction);

private:
    int dimension_;
    QVector<TileData> boardElements_;
    Image::BoardImageController imgController_;
};


} // namespace Dal

#endif // BOARD_H
