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
    Q_PROPERTY(bool isGameWon READ isGameWon NOTIFY sigGameWonChanged)

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
        bool operator>(const TileData &other) const {
            return Value > other.Value;
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

    int hiddenValue() const { return 0; }

    bool isGameWon() const
    {
      return boardElements_.first().Value != hiddenValue()
             && std::is_sorted(boardElements_.begin(), boardElements_.end() - 1);
    }

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
            emit sigGameWonChanged(true);
    }

signals:
    void sigDimensionChanged();

    void sigGameWonChanged(bool isGameWon);

private:
    Position getRowCol(int index) const { return {index / defaultDimension_, index % defaultDimension_}; }

    void updateBoard()
    {
        beginResetModel();
        {
            boardElements_.resize(dimension_ * dimension_);

            // fill with 1,2,3,4,5,...,0
            std::iota(boardElements_.begin(), boardElements_.end() - 1, 1);
            boardElements_.last() = hiddenValue();

            shaffleBoard();

            hiddenIndex_ = findHiddenIndex();
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
        } while ((! isBoardSolvable()) || isGameWon());
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
      // Count inversions in given puzzle
      int invCount = 0;
      for (int i = 0; i < boardElements_.size() - 1; i++)
      {
        for (int j = i + 1; j < boardElements_.size(); j++)
        {
          // count pairs(i, j) such that i appears
          // before j, but i > j.
          if (boardElements_[j].Value && boardElements_[i].Value
              && boardElements_[i] > boardElements_[j])
          {
            invCount++;
          }
        }
      }

      auto isOdd = [](int val){ return (val & 1); };
      auto isEven = [&isOdd](int val){ return ! isOdd(val); };

      // If grid is odd, return true if inversion
      // count is even.
      if (isOdd(dimension_)) {
        return isEven(invCount);
      } else {
        // grid is even
        const int hiddenIndex = findHiddenIndex();
        const int pos = getRowCol(hiddenIndex).first;
        return isOdd(pos) ? isEven(invCount) : isOdd(invCount);
      }
    }

    int findHiddenIndex() const
    {
      // find and update hidden index
      const auto it = std::find(boardElements_.begin(), boardElements_.end(), TileData(hiddenValue()));
      assert(it != boardElements_.end());
      return static_cast<int>(std::distance(boardElements_.begin(), it));
    }

private:
    static constexpr int defaultDimension_ = 2;

    int dimension_;
    int hiddenIndex_;
    QVector<TileData> boardElements_;
};

#endif // GAMEBOARDMODEL_H
