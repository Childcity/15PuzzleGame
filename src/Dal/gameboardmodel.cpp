#include "gameboardmodel.h"


Dal::GameBoardModel::GameBoardModel(QObject *parent)
    : QAbstractListModel(parent)
{
    createBoard(3);
}

int Dal::GameBoardModel::rowCount(const QModelIndex &) const
{
    return board_->tilesNumber();
}

QVariant Dal::GameBoardModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid()) {
        if (role == Qt::DisplayRole) {
            return QVariant::fromValue((*board_)[index.row()]);
        }
    }

    return {};
}

int Dal::GameBoardModel::dimension() const
{
    return board_->dimension();
}

int Dal::GameBoardModel::hiddenIndex() const
{
    return board_->hiddenIndex();
}

bool Dal::GameBoardModel::isGameWon() const
{
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
    board_ = qt_make_unique<Board>(dimension, this);
    connect(&*board_, &Board::sigCachingError, this, &GameBoardModel::sigGameBoardError);
    connect(&*board_, &Board::sigImagesCached, this, [this] {
        beginResetModel();
        endResetModel();
    });
}
