#pragma once

#include "Dal/Image/boardimagecontroller.h"
#include "Dal/Image/Providers/imageprovidertype.h"
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
    Board(QObject *parent = nullptr) noexcept;
    Board(int dimension, Image::ImageProviderType imageProviderType, QObject *parent = nullptr) noexcept;
    ~Board() override;

    void reset();
    void move(int index);
    int tilesNumber() const;
    int dimension() const;
    Image::ImageProviderType imageProviderType() const;

    int hiddenValue() const;
    int hiddenIndex() const;

    bool isMovable(Position pos, Position hidPos) const;
    bool isMovable(int index) const;

    bool isGameWon() const;
    bool isValid() const;

    TileData &operator[](int index);

signals:
    void sigImagesCached();
    void sigCachingError(QString errorString);

private slots:
    void slotImageReady();

private:
    Position getRowCol(int index) const;
    int getIndex(Position pos) const;
    void shaffleBoard();
    bool isBoardSolvable() const;
    int findHiddenIndex() const;

    void shift2D(const Position strtPos, int count, ShiftDirection direction);

private:
    int dimension_;
    Image::ImageProviderType imageProviderType_;
    std::vector<TileData> boardElements_;
    std::unique_ptr<Image::BoardImageController> imgController_;
};


} // namespace Dal
