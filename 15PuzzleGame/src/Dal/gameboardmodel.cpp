#include "gameboardmodel.h"

#include <algorithm>
#include <random>


namespace Dal {


GameBoardModel::GameBoardModel(QObject *parent)
    : QAbstractListModel(parent)
{
    board_ = qt_make_unique<Board>();
}

int GameBoardModel::rowCount(const QModelIndex &) const
{
    assert(board_);
    return board_->tilesNumber();
}

QVariant GameBoardModel::data(const QModelIndex &index, int role) const
{
    assert(board_);

    if (index.isValid()) {
        if (role == Qt::DisplayRole) {
            return QVariant::fromValue((*board_)[index.row()]);
        }
    }

    return {};
}

int GameBoardModel::dimension() const
{
    assert(board_);
    return board_->dimension();
}

void GameBoardModel::setDimension(int dimension)
{
    createBoard(dimension, imageProvider());
    emit sigDimensionChanged(dimension);
    emit sigGameWonChanged(false);
}

int GameBoardModel::hiddenIndex() const
{
    return board_->hiddenIndex();
}

bool GameBoardModel::isGameWon() const
{
    return board_->isGameWon();
}

Image::ImageProviderType GameBoardModel::imageProvider() const
{
    return board_->imageProviderType();
}

void GameBoardModel::setImageProvider(ImageProviderType imageProvider)
{
    createBoard(dimension(), imageProvider);
    emit sigImageProviderChanged(imageProvider);
    emit sigGameWonChanged(false);
}

void GameBoardModel::resetBoard()
{
    createBoard(dimension(), imageProvider());
    emit sigGameWonChanged(false);
}

void GameBoardModel::move(int index)
{
    assert(board_);
    if (! board_->isMovable(index))
        return;

    beginResetModel();
    {
        board_->move(index);
    }
    endResetModel();

    if (board_->isGameWon()) {
        emit sigGameWonChanged(true);
    } else {
        emit sigGameWonChanged(false);
    }
}

void GameBoardModel::createBoard(int dimension, ImageProviderType imageProvider)
{
    beginResetModel();

    board_ = qt_make_unique<Board>(dimension, imageProvider, this);
    connect(&*board_, &Board::sigCachingError, this, &GameBoardModel::sigGameBoardError);
    connect(&*board_, &Board::sigImagesCached, this, [this] {
        // For stopping empty image animation and reloading new images
        // we must notify qml about changes
        beginResetModel();
        endResetModel();
    });

    endResetModel();
}


} // namespace Dal
