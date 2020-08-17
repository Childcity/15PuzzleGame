#ifndef TILEDATA_H
#define TILEDATA_H

#include <memory>
#include <QDebug>
#include <QObject>

namespace DAL {


struct TileData : public QObject {
    Q_OBJECT

    int m_value {};
    Q_PROPERTY(int value MEMBER value NOTIFY valueChanged )

signals:
    void valueChanged(int value);

public:
    int Value {};

    QByteArray Image {"qdwqd"};

    explicit TileData();

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

Q_DECLARE_METATYPE(DAL::TileData);


#endif // TILEDATA_H
