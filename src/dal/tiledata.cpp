#include "tiledata.h"

#include <memory>

namespace Dal {


TileData::TileData()
{
    qRegisterMetaType<Dal::TileData>("Dal::TileData");
}

TileData::TileData(int val)
  : TileData()
{
    Value = val;
}

TileData::TileData(TileData &&other) noexcept
    : Value(std::move(other.Value))
{
    other.Value = 0;
}

bool TileData::operator==(const TileData &other) const { return Value == other.Value; }

bool TileData::operator<(const TileData &other) const { return Value < other.Value; }

bool TileData::operator>(const TileData &other) const { return Value > other.Value; }

void TileData::swap(TileData &other) noexcept { std::swap(Value, other.Value); }

// Friend members

void swap(TileData &l, TileData &r) noexcept { l.swap(r); }

QDebug & operator<<(QDebug &stream, const TileData &other)
{
    stream << other.Value;
    return stream;
}


}
