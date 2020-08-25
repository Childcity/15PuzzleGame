#ifndef GAMEBOARDMODEL_H
#define GAMEBOARDMODEL_H

#include "main.h"
#include "board.h"
#include "tiledata.h"

#include <QAbstractListModel>
#include <QObject>
#include <algorithm>
#include <random>

namespace Dal {


class GameBoardModel : public QAbstractListModel {
    Q_OBJECT

    Q_PROPERTY(int dimension READ dimension WRITE setDimension NOTIFY sigDimensionChanged)
    Q_PROPERTY(int hiddenIndex READ hiddenIndex CONSTANT)
    Q_PROPERTY(bool isGameWon READ isGameWon NOTIFY sigGameWonChanged)

public:
    GameBoardModel(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
        board_ = qt_make_unique<Board>(4, this);
        connect(&*board_, &Board::sigImagesCached, this, [this]{
            beginResetModel();
            endResetModel();
        });
    }

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex & = QModelIndex()) const override { return board_->tilesNumber(); }

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (index.isValid()) {
            if (role == Qt::DisplayRole) {
                return QVariant::fromValue((*board_)[index.row()]);
            }
        }

        return {};
    }

public:
    int dimension() const { return board_->dimension(); }

    int hiddenIndex() const { return board_->hiddenIndex(); }

    bool isGameWon() const { return board_->isGameWon(); }

public slots:
    void setDimension(int dimension)
    {
        if (dimension == board_->dimension())
            return;

        beginResetModel();
        {
            board_ = qt_make_unique<Board>(dimension, this);
        }
        endResetModel();

        emit sigDimensionChanged();
        emit sigGameWonChanged(false);
    }

    void move(int index)
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

signals:
    void sigDimensionChanged();

    void sigGameWonChanged(bool isGameWon);

private:
    qt_unique_ptr<Board> board_;
};


}

#endif // GAMEBOARDMODEL_H
