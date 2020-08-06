#ifndef GAMEBOARDMODEL_H
#define GAMEBOARDMODEL_H

#include "main.h"

#include <QAbstractListModel>
#include <QObject>
#include <algorithm>
#include <random>

class GameBoardModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(int dimension READ dimension WRITE setDimension NOTIFY sigDimensionChanged)
    Q_PROPERTY(int hiddenIndex READ hiddenIndex CONSTANT)

    template<class T>
    struct TileData {
        T Data {};
        explicit TileData() = default;
        explicit TileData(T data)
            : Data(data)
        {}
        TileData(const TileData &) = default;
        TileData & operator=(const TileData &) = delete;
        TileData & operator=(const T &data) {
            Data = data;
            return *this;
        }
        TileData(TileData &&other) noexcept
            : Data(std::move(other.Data)) {
            other.Data = 0;
        }
        void swap(TileData &other) noexcept { std::swap(Data, other.Data); }
        void friend swap(TileData &l, TileData &r) noexcept { l.swap(r); }
        friend QDebug & operator<<(QDebug &stream, const TileData &data) {
            stream << data.Data;
            return stream;
        }
    };

    using TileDataType = TileData<int>;

public:
    GameBoardModel(int dimension = defaultDimension_, QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
        setDimension(dimension);
    }

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex & = QModelIndex()) const override
    {
        return boardElements_.size();
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        QVariant value;

        if (index.isValid()) {
            if (role == Qt::DisplayRole) {
                value = boardElements_.at(index.row()).Data;
            }
        }

        return value;
    }

public:
    int dimension() const { return dimension_; }

    int hiddenIndex() const { return boardElements_.size() - 1; }

public slots:
    void setDimension(int dimension)
    {
        if (dimension == dimension_)
            return;

        dimension_ = dimension;

        updateBoard();

        sigDimensionChanged();
    }

signals:
    void sigDimensionChanged();

private:
    void updateBoard()
    {
        beginResetModel();

        {
            boardElements_.resize(dimension_ * dimension_);

            // fill with 1,2,3,4,5...
            std::iota(boardElements_.begin(), boardElements_.end(), 1);

            shaffleBoard();
            DEBUG(boardElements_)
        }

        endResetModel();
    }

    void shaffleBoard()
    {
        using std::chrono::system_clock;
        static auto seed = system_clock::now().time_since_epoch().count();
        static std::mt19937 generator(seed);
        std::shuffle(boardElements_.begin(), boardElements_.end(), generator);
    }

private:
    static constexpr size_t defaultDimension_ = 2;

    int dimension_;
    QVector<TileDataType> boardElements_;
};

#endif // GAMEBOARDMODEL_H
