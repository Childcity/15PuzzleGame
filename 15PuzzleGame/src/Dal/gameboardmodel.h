#ifndef GAMEBOARDMODEL_H
#define GAMEBOARDMODEL_H

#include "Utils/Utils.h"
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
    GameBoardModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

public:
    int dimension() const;

    int hiddenIndex() const;

    bool isGameWon() const;

public slots:
    void resetBoard();

    void setDimension(int dimension);

    void move(int index);

signals:
    void sigDimensionChanged();

    void sigGameWonChanged(bool isGameWon);

    void sigGameBoardError(QString errorString);

private:
    void createBoard(int dimension = 2);

private:
    Utils::qt_unique_ptr<Board> board_;
};


} // namespace Dal

#endif // GAMEBOARDMODEL_H
