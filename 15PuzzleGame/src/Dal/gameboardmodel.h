#pragma once

#include "main.h"
#include "tiledata.h"
#include "board.h"

#include <QAbstractListModel>


namespace Dal {


class GameBoardModel : public QAbstractListModel {
    Q_OBJECT
    using ImageProviderType = Image::ImageProviderType;

public:
    Q_PROPERTY(int dimension READ dimension WRITE setDimension NOTIFY sigDimensionChanged)
    Q_PROPERTY(ImageProviderType imageProvider READ imageProvider WRITE setImageProvider NOTIFY sigImageProviderChanged)
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
    void setDimension(int dimension);

    int hiddenIndex() const;
    bool isGameWon() const;

    ImageProviderType imageProvider() const;
    void setImageProvider(ImageProviderType imageProvider);

public slots:
    void resetBoard();
    void move(int index);

signals:
    void sigDimensionChanged(int dimension);
    void sigGameWonChanged(bool isGameWon);
    void sigGameBoardError(QString errorString);

    void sigImageProviderChanged(ImageProviderType imageProvider);

private:
    void createBoard(int dimension, ImageProviderType imageProvider);

private:
    qt_unique_ptr<Board> board_;
    int dimension_;
    Image::ImageProviderType imageProviderType_;
};


} // namespace Dal
