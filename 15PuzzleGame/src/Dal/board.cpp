#include "board.h"
#include "main.h"
#include "Net/networkerror.h"

#include <QSize>
#include <QVector>
#include <algorithm>
#include <random>


namespace Dal {


Board::Board(QObject *parent) noexcept
    : QObject(parent)
    , dimension_(-1)
    , imageProviderType_(Image::ImageProviderType::Invalid)
    , boardElements_(0)
{
}

Board::Board(int dimension, Image::ImageProviderType imageProviderType, QObject *parent) noexcept
    : QObject(parent)
    , dimension_(dimension)
    , imageProviderType_(imageProviderType)
    , boardElements_(dimension * dimension)
{
    reset();
}

Board::~Board()
{
    DEBUG("~Board");
}

void Board::reset()
{
    if (! isValid()) {
        return;
    }

    // get Async images for board
    {
        imgController_ = std::make_unique<Image::BoardImageController>();
        connect(&*imgController_, &Image::BoardImageController::sigBoardImagesReady,
                this, &Board::slotImageReady, Qt::QueuedConnection);

        // run acync task
        imgController_->getBoardImagesAsync({ dimension_, dimension_ }, imageProviderType_);
    }

    // fill with 1,2,3,4,5,...,0
    std::iota(boardElements_.begin(), boardElements_.end(), 1);
    boardElements_.back().Value = hiddenValue();

    shaffleBoard();
}

void Board::move(int index)
{
    const Position posToMove = getRowCol(index);
    const Position hiddenPos = getRowCol(hiddenIndex());

    ShiftDirection direction {};
    int count {};

    if (posToMove.first == hiddenPos.first) { // shift Left/Right
        count = posToMove.second - hiddenPos.second;
        // if count is positive, then posToMove to the Left of the hidden tile
        // so we should move all tiles to the Right
        // else to the Left
        direction = count > 0 ? ShiftDirection::Right : ShiftDirection::Left;
    } else if (posToMove.second == hiddenPos.second) { // shift Top/Bottom
        count = posToMove.first - hiddenPos.first;
        // if count is positive, then posToMove to the Top of the hidden tile
        // so we should move all tiles to the Bottom
        // else to the Top
        direction = count > 0 ? ShiftDirection::Bottom : ShiftDirection::Top;
    }

    assert(count != 0);

    count = std::abs(count);
    shift2D(hiddenPos, count, direction);
}

int Board::tilesNumber() const
{
    return boardElements_.size();
}

int Board::dimension() const
{
    return dimension_;
}

Image::ImageProviderType Board::imageProviderType() const
{
    return imageProviderType_;
}

int Board::hiddenValue() const
{
    return 0;
}

int Board::hiddenIndex() const
{
    return findHiddenIndex();
}

bool Board::isMovable(Board::Position pos, Board::Position hidPos) const
{
    return pos.second == hidPos.second || pos.first == hidPos.first;
}

bool Board::isMovable(int index) const
{
    return index >= 0 && static_cast<size_t>(index) < boardElements_.size() &&
           isMovable(getRowCol(index), getRowCol(hiddenIndex()));
}

bool Board::isGameWon() const
{
    return isValid() && boardElements_.front().Value != hiddenValue() &&
           std::is_sorted(boardElements_.cbegin(), boardElements_.cend() - 1);
}

bool Board::isValid() const
{
    return dimension_ > 0 &&
           imageProviderType_ != Image::ImageProviderType::Invalid &&
           boardElements_.size() > 0;
}

TileData Board::operator[](int index)
{
    return boardElements_[index];
}

void Board::slotImageReady()
{
    std::vector<QByteArray> imgs;

    try {
        imgs = imgController_->getBoardImagesAcyncResult();
    } catch (Net::NetworkError &ex) {
        emit sigCachingError(ex.what());
        return;
    }

    assert(imgs.size() == boardElements_.size());

    // update boardElements_ with new images
    for (auto &tile : boardElements_) {
        if (tile.Value == hiddenValue())
            continue;

        size_t isz = static_cast<size_t>(tile.Value - 1); // get index of image for current tile
        tile.Image = std::move(imgs[isz]);
    }

    emit sigImagesCached();
}

Board::Position Board::getRowCol(int index) const
{
    return { index / dimension_, index % dimension_ };
}

int Board::getIndex(Board::Position pos) const
{
    return pos.first * dimension_ + pos.second;
}

void Board::shaffleBoard()
{
    using std::chrono::system_clock;
    static const auto seed = system_clock::now().time_since_epoch().count();
    static std::mt19937 generator((std::random_device()() ^ seed));

    do {
        std::shuffle(boardElements_.begin(), boardElements_.end(), generator);
    } while ((! isBoardSolvable()) || isGameWon());
}

bool Board::isBoardSolvable() const
{
    // Count inversions in given puzzle
    size_t invCount = 0;
    for (size_t i = 0; i < boardElements_.size() - 1; i++)
    {
        for (size_t j = i + 1; j < boardElements_.size(); j++)
        {
            // count pairs(i, j) such that i appears
            // before j, but i > j.
            if (boardElements_[j].Value && boardElements_[i].Value && boardElements_[i] > boardElements_[j])
            {
                invCount++;
            }
        }
    }

    auto isOdd = [](int val) { return (val & 1); };
    auto isEven = [&isOdd](int val) { return ! isOdd(val); };

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

int Board::findHiddenIndex() const
{
    // find and update hidden index
    const auto it = std::find(boardElements_.cbegin(), boardElements_.cend(), TileData(hiddenValue()));
    assert(it != boardElements_.cend());
    return static_cast<int>(std::distance(boardElements_.cbegin(), it));
}

void Board::shift2D(const Board::Position strtPos, int count, Board::ShiftDirection direction)
{
    const auto &[row, col] = strtPos;
    auto &vec = boardElements_;

    assert(row < dimension_);
    assert(col < dimension_);

    if (direction == ShiftDirection::Right) {
        int strtIndx = col + 1;
        int endIndx = col + count;
        assert(endIndx < dimension_);

        for (int i = strtIndx; i <= endIndx; ++i) {
            vec[getIndex({ row, i - 1 })].swap(vec[getIndex({ row, i })]);
        }
    } else if (direction == ShiftDirection::Left) {
        int strtIndx = col - 1;
        int endIndx = col - count;
        assert(endIndx >= 0);

        for (int i = strtIndx; i >= endIndx; --i) {
            vec[getIndex({ row, i + 1 })].swap(vec[getIndex({ row, i })]);
        }
    } else if (direction == ShiftDirection::Bottom) {
        int strtIndx = row + 1;
        int endIndx = row + count;
        assert(endIndx < dimension_);

        for (int i = strtIndx; i <= endIndx; ++i) {
            vec[getIndex({ i - 1, col })].swap(vec[getIndex({ i, col })]);
        }
    } else if (direction == ShiftDirection::Top) {
        int strtIndx = row - 1;
        int endIndx = row - count;
        assert(endIndx >= 0);

        for (int i = strtIndx; i >= endIndx; --i) {
            vec[getIndex({ i + 1, col })].swap(vec[getIndex({ i, col })]);
        }
    }
}


} // namespace Dal
