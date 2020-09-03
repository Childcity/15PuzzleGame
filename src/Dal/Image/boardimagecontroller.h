#ifndef BOARDIMAGEPROCSSOR_H
#define BOARDIMAGEPROCSSOR_H

#include "Net/downloader.h"
#include "Dal/Image/Providers/flickrimageprovider.h"
#include "Utils/StdFutureWatcher/src/futurewatcher.hpp"

#include <QtConcurrent>
#include <QFuture>
#include <QBuffer>
#include <QPoint>
#include <any>


namespace Dal::Image {


using namespace childcity::StdFutureWatcher;

class BoardImageController : public QObject {
    Q_OBJECT

    using BoardImages = std::vector<QByteArray>;

public:
    BoardImageController(QObject *parent = nullptr);

    ~BoardImageController();

    void getBoardImagesAsync(const QPoint &dimensions);

    BoardImages getBoardImagesAcyncResult();

signals:
    void sigBoardImagesReady();

private:
    static std::future<QByteArray> partitionImages(const QImage &img, const QRect rect);

private:
    FutureWatcher<BoardImages> watcher_;
};


} // namespace Dal::Image
#endif // BOARDIMAGEPROCSSOR_H
