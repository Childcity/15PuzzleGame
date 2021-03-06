#pragma once

#include <memory>
#include <QDebug>

namespace Dal {


struct TileData {
    int Value {};
    QString Image;

    explicit TileData();
    explicit TileData(int val);

    TileData(const TileData &) = default;
    TileData(TileData &&other) noexcept;

    TileData &operator=(const TileData &) = default;
    TileData &operator=(int val);
    bool operator==(const TileData &other) const;
    bool operator<(const TileData &other) const;
    bool operator>(const TileData &other) const;
    void swap(TileData &other) noexcept;

    // Friend members

    friend void swap(TileData &l, TileData &r) noexcept;
    friend QDebug &operator<<(QDebug &stream, const TileData &other);
};


} // namespace Dal

Q_DECLARE_METATYPE(Dal::TileData);
