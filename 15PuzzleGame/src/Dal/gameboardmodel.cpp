#include "gameboardmodel.h"


Dal::GameBoardModel::GameBoardModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int Dal::GameBoardModel::rowCount(const QModelIndex &) const
{
    assert(board_);
    return board_->tilesNumber();
}

QVariant Dal::GameBoardModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid()) {
        if (role == Qt::DisplayRole) {
            assert(board_);
            return QVariant::fromValue((*board_)[index.row()]);
        }
    }

    return {};
}

int Dal::GameBoardModel::dimension() const
{
    assert(board_);
    return board_->dimension();
}

int Dal::GameBoardModel::hiddenIndex() const
{
    assert(board_);
    return board_->hiddenIndex();
}

bool Dal::GameBoardModel::isGameWon() const
{
    if (! board_)
        return false;

    return board_->isGameWon();
}

void Dal::GameBoardModel::resetBoard()
{
    setDimension(dimension());
}

void Dal::GameBoardModel::setDimension(int dimension)
{
    beginResetModel();
    {
        createBoard(dimension);
    }
    endResetModel();

    emit sigDimensionChanged();
    emit sigGameWonChanged(false);
}

void Dal::GameBoardModel::move(int index)
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

void Dal::GameBoardModel::createBoard(int dimension)
{
    board_ = Utils::qt_make_unique<Board>(dimension, this);
    connect(&*board_, &Board::sigCachingError, this, &GameBoardModel::sigGameBoardError);
    connect(&*board_, &Board::sigImagesCached, this, [this] {
        beginResetModel();
        endResetModel();
    });
}
