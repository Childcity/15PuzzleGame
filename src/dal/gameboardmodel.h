#ifndef GAMEBOARDMODEL_H
#define GAMEBOARDMODEL_H

#include "main.h"
#include "tiledata.h"

#include <QAbstractListModel>
#include <QObject>
#include <algorithm>
#include <random>

namespace DAL {


class GameBoardModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(int dimension READ dimension WRITE setDimension NOTIFY sigDimensionChanged)
    Q_PROPERTY(int hiddenIndex READ hiddenIndex CONSTANT)
    Q_PROPERTY(bool isGameWon READ isGameWon NOTIFY sigGameWonChanged)


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

    enum class ShiftDirection {Right, Left, Top, Bottom};
    Q_ENUM(ShiftDirection)

public:
    int dimension() const { return dimension_; }

    int hiddenIndex() const { return findHiddenIndex(); }

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
        const Position posToMove = getRowCol(index);
        const Position hidPos = getRowCol(hiddenIndex());

        if (! isMovable(posToMove, hidPos))
            return;

        beginResetModel();
        {
            ShiftDirection direction{};
            int count{};

            if (posToMove.first == hidPos.first) { // shift Left/Right
                count = posToMove.second - hidPos.second;
                direction = count > 0 ? ShiftDirection::Right : ShiftDirection::Left;
            } else if (posToMove.second == hidPos.second) { // shift Top/Bottom
                count = posToMove.first - hidPos.first;
                direction = count > 0 ? ShiftDirection::Bottom : ShiftDirection::Top;
            }

            count = count > 0 ? count : -count;
            shift2D(boardElements_, hidPos, count, direction);
        }
        endResetModel();

        if (isGameWon())
            emit sigGameWonChanged(true);
    }

signals:
    void sigDimensionChanged();

    void sigGameWonChanged(bool isGameWon);

private:
    Position getRowCol(int index) const { return {index / dimension_, index % dimension_}; }

    int getIndex(Position pos) const { return pos.first * dimension_ + pos.second; }

    void updateBoard()
    {
        beginResetModel();
        {
            boardElements_.resize(dimension_ * dimension_);

            // fill with 1,2,3,4,5,...,0
            std::iota(boardElements_.begin(), boardElements_.end() - 1, 1);
            boardElements_.last() = hiddenValue();

            shaffleBoard();
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

    bool isMovable(Position pos, Position hidPos) const
    {
        return  pos.second == hidPos.second || pos.first == hidPos.first;
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

    void shift2D(QVector<TileData> &vec, Position strPos, int count, ShiftDirection direction)
    {
        int row, col;
        std::tie(row, col) = strPos;

        assert(row < dimension_);
        assert(col < dimension_);

        if (direction == ShiftDirection::Right) {
            int strtIndx = col + 1;
            int endIndx = col + count;
            assert(endIndx < dimension_);

            for (int i = strtIndx; i <= endIndx; ++i) {
                vec[getIndex({row, i - 1})].swap(vec[getIndex({row, i})]);
            }
        } else if (direction == ShiftDirection::Left) {
            int strtIndx = col - 1;
            int endIndx = col - count;
            assert(endIndx >= 0);

            for (int i = strtIndx; i >= endIndx; --i) {
                vec[getIndex({row, i + 1})].swap(vec[getIndex({row, i})]);
            }
        } else if (direction == ShiftDirection::Bottom) {
            int strtIndx = row + 1;
            int endIndx = row + count;
            assert(endIndx < dimension_);

            for (int i = strtIndx; i <= endIndx; ++i) {
                vec[getIndex({i - 1, col})].swap(vec[getIndex({i, col})]);
            }
        } else if (direction == ShiftDirection::Top) {
            int strtIndx = row - 1;
            int endIndx = row - count;
            assert(endIndx >= 0);

            for (int i = strtIndx; i >= endIndx; --i) {
                vec[getIndex({i + 1, col})].swap(vec[getIndex({i, col})]);
            }
        }
    }

private:
    static constexpr int defaultDimension_ = 4;

    int dimension_;
    QVector<TileData> boardElements_;
};


}

#endif // GAMEBOARDMODEL_H
