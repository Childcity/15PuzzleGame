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

    struct TileData final {
        int Value {};
        explicit TileData() = default;
        explicit TileData(int val)
            : Value(val)
        {}
        TileData(const TileData &) = default;
        TileData & operator=(const TileData &) = delete;
        TileData & operator=(const int &val) {
            Value = val;
            return *this;
        }
        bool operator==(const TileData &other) const {
            return Value == other.Value;
        }
        bool operator<(const TileData &other) const {
            return Value < other.Value;
        }
        TileData(TileData &&other) noexcept
            : Value(std::move(other.Value)) {
            other.Value = 0;
        }
        void swap(TileData &other) noexcept { std::swap(Value, other.Value); }
        void friend swap(TileData &l, TileData &r) noexcept { l.swap(r); }
        friend QDebug & operator<<(QDebug &stream, const TileData &other) {
            stream << other.Value;
            return stream;
        }
    };

    using Position = std::pair<int, int>;

public:
    GameBoardModel(int dimension = defaultDimension_, QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
        setDimension(dimension);
    }

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex & = QModelIndex()) const override { return boardElements_.size(); }

    QVariant data(const QModelIndex &index, int role) const override
    {
        QVariant value;

        if (index.isValid()) {
            if (role == Qt::DisplayRole) {
                value = boardElements_.at(index.row()).Value;
            }
        }

        return value;
    }

public:
    int dimension() const { return dimension_; }

    int hiddenIndex() const { return hiddenIndex_; }

public slots:
    void setDimension(int dimension)
    {
        if (dimension == dimension_)
            return;

        dimension_ = dimension;

        updateBoard();

        sigDimensionChanged();
    }

    void move(int index)
    {
        {
            const Position posToMove = getRowCol(index);
            if (! isMovable(posToMove))
                return;
        }

        beginResetModel();
        {
            boardElements_[index].swap(boardElements_[hiddenIndex_]);
            hiddenIndex_ = index;
        }
        endResetModel();

        if (isGameWon())
            emit sigGameWon();
    }

signals:
    void sigDimensionChanged();

    void sigGameWon();

private:
    Position getRowCol(int index) const { return {index / defaultDimension_, index % defaultDimension_}; }

    void updateBoard()
    {
        beginResetModel();
        {
            boardElements_.resize(dimension_ * dimension_);

            // fill with 1,2,3,4,5...
            std::iota(boardElements_.begin(), boardElements_.end(), 1);

            const auto hiddenElement = boardElements_.last();
            shaffleBoard();

            // find and update hidden index
            const auto it = std::find(boardElements_.begin(), boardElements_.end(), hiddenElement);
            assert(it != boardElements_.end());
            hiddenIndex_ = static_cast<int>(std::distance(boardElements_.begin(), it));
        }
        endResetModel();
    }

    void shaffleBoard()
    {
        using std::chrono::system_clock;
        static auto seed = system_clock::now().time_since_epoch().count();
        static std::mt19937 generator(seed);

        do {
            std::shuffle(boardElements_.begin(), boardElements_.end(), generator);
            DEBUG("isBoardSolvable" <<isBoardSolvable())
        } while (! isBoardSolvable());
    }

    bool isMovable(Position pos) const
    {
        const Position hidPos = getRowCol(hiddenIndex());
        return  // for Column condition (if culumn the same)
                (pos.second == hidPos.second && ((pos.first - 1) == hidPos.first || (pos.first + 1) == hidPos.first))
                // for Raw condition (if raw the same)
                || (pos.first == hidPos.first && ((pos.second - 1) == hidPos.second || (pos.second + 1) == hidPos.second));
    }

    bool isBoardSolvable() const
    {
        // https://www.cs.bham.ac.uk/~mdr/teaching/modules04/java2/TilesSolvability.html
        int parity = 0;

        int row = 0; // the current row we are on
        int blankRow = 0; // the row with the blank tile

        for (int i = 0; i < dimension_; i++) {
            if (i % dimension_ == 0) { // advance to next row
                row++;
            }
            if (boardElements_[i].Value == 0) { // the blank tile
                blankRow = row; // save the row on which encountered
                continue;
            }
            for (int j = i + 1; j < dimension_; j++) {
                if (boardElements_[i].Value > boardElements_[j].Value && boardElements_[j].Value != 0)
                    parity++;
            }
        }

        if (dimension_ % 2 == 0) { // even grid
            if (blankRow % 2 == 0) { // blank on odd row; counting from bottom
                return parity % 2 == 0;
            } else { // blank on even row; counting from bottom
                return parity % 2 != 0;
            }
        } else { // odd grid
            return parity % 2 == 0;
        }
    }

    bool isGameWon() { return std::is_sorted(boardElements_.begin(), boardElements_.end()); }

private:
    static constexpr int defaultDimension_ = 2;

    int dimension_;
    int hiddenIndex_;
    QVector<TileData> boardElements_;
};

#endif // GAMEBOARDMODEL_H
