#pragma once

#include "Providers/imageprovidertype.h"
#include "Utils/StdFutureWatcher/src/futurewatcher.hpp"

class QSize;


namespace Dal::Image {

using namespace childcity::StdFutureWatcher;

class BoardImageController final: public QObject {
    Q_OBJECT

    using BoardImages = std::vector<QByteArray>;

public:
    BoardImageController(QObject *parent = nullptr) noexcept;
    ~BoardImageController();

    void getBoardImagesAsync(const QSize &dimensions, ImageProviderType providerType);
    BoardImages getBoardImagesAcyncResult();

signals:
    void sigBoardImagesReady();

private:
    static std::future<QByteArray> partitionImages(const QImage &img, const QRect rect);

private:
    FutureWatcher<BoardImages> watcher_;
};


} // namespace Dal::Image
