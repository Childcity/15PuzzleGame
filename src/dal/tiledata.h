#ifndef TILEDATA_H
#define TILEDATA_H

#include <memory>
#include <QDebug>

namespace DAL {


struct TileData final {
    int Value {};

    int Image {};

    explicit TileData() = default;

    explicit TileData(int val);

    TileData(const TileData &) = default;

    TileData(TileData &&other) noexcept;

    TileData & operator=(const TileData &) = delete;

    TileData & operator=(int val);

    bool operator==(const TileData &other) const;

    bool operator<(const TileData &other) const;

    bool operator>(const TileData &other) const;

    void swap(TileData &other) noexcept;

    // Friend members

    friend void swap(TileData &l, TileData &r) noexcept;

    friend QDebug & operator<<(QDebug &stream, const TileData &other);
};


}

#endif // TILEDATA_H
