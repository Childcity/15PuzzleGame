#ifndef BOARD_H
#define BOARD_H

#include "tiledata.h"

#include <QVector>
#include <algorithm>
#include <random>

namespace DAL {


class Board {
    using Position = std::pair<int, int>;

    enum class ShiftDirection { Right, Left, Top, Bottom };

public:
    using Ptr = std::unique_ptr<Board>;

public:
    Board(int dimension = defaultDimension_)
        : dimension_(dimension)
    {
        boardElements_.resize(dimension_ * dimension_);

        // fill with 1,2,3,4,5,...,0
        std::iota(boardElements_.begin(), boardElements_.end() - 1, 1);
        boardElements_.last() = hiddenValue();

        shaffleBoard();
    }

    void move(int index)
    {
        const Position posToMove = getRowCol(index);
        const Position hidPos = getRowCol(hiddenIndex());

        ShiftDirection direction{};
        int count{};

        if (posToMove.first == hidPos.first) { // shift Left/Right
            count = posToMove.second - hidPos.second;
            // if count is positive, then posToMove to the Left of the hidden tile
            // so we should move all tiles to the Right
            // else to the Left
            direction = count > 0 ? ShiftDirection::Right : ShiftDirection::Left;
        } else if (posToMove.second == hidPos.second) { // shift Top/Bottom
            count = posToMove.first - hidPos.first;
            // if count is positive, then posToMove to the Top of the hidden tile
            // so we should move all tiles to the Bottom
            // else to the Top
            direction = count > 0 ? ShiftDirection::Bottom : ShiftDirection::Top;
        }

        assert(count != 0);

        count = count > 0 ? count : -count;
        shift2D(boardElements_, hidPos, count, direction);
    }

    int tilesNumber() const { return boardElements_.size(); }

    int dimension() const { return dimension_; }

    int hiddenValue() const { return 0; }

    int hiddenIndex() const { return findHiddenIndex(); }

    bool isMovable(Position pos, Position hidPos) const
    {
        return  pos.second == hidPos.second || pos.first == hidPos.first;
    }

    bool isMovable(int index) const
    {
        return index >= 0 && index < boardElements_.size()
                && isMovable(getRowCol(index), getRowCol(hiddenIndex()));
    }

    bool isGameWon() const
    {
      return boardElements_.first().Value != hiddenValue()
             && std::is_sorted(boardElements_.begin(), boardElements_.end() - 1);
    }

    TileData operator[](int index) { return boardElements_.at(index); }

private:
    Position getRowCol(int index) const { return {index / dimension_, index % dimension_}; }

    int getIndex(Position pos) const { return pos.first * dimension_ + pos.second; }

    void shaffleBoard()
    {
        using std::chrono::system_clock;
        static auto seed = system_clock::now().time_since_epoch().count();
        static std::mt19937 generator(seed);

        do {
            std::shuffle(boardElements_.begin(), boardElements_.end(), generator);
            //DEBUG("isBoardSolvable" <<isBoardSolvable())
        } while ((! isBoardSolvable()) || isGameWon());
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

    void shift2D(QVector<TileData> &vec, Position strtPos, int count, ShiftDirection direction)
    {
        int row, col;
        std::tie(row, col) = strtPos;

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
    static constexpr int defaultDimension_ = 2;

    int dimension_;
    QVector<TileData> boardElements_;
};


}

#endif // BOARD_H
