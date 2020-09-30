#ifndef TILEDATA_H
#define TILEDATA_H

#include <memory>
#include <QDebug>

namespace Dal {


struct TileData {
    Q_GADGET

    Q_PROPERTY(int Value MEMBER Value)
    Q_PROPERTY(QString Image MEMBER Image)

public:
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


#endif // TILEDATA_H
