#include "downloader.h"

Downloader::Downloader(QObject *parent)
    : QObject(parent)
{}

Downloader::~Downloader()
{
    /*
     * We dont need next lins, because file_ and manager_ has parent 'this'
     *
     * if(file_ && file_->parent() == nullptr){
     *     file_->deleteLater();
     * }
     *
     * if(manager_ && manager_->parent() == nullptr){
     *     manager_->deleteLater();
     * }
     */

    if(reply_){
        reply_->deleteLater();
    }
}

void Downloader::download(const QUrl fileURL)
{
    downloadProgress_ = -1;

    if(file_){
        file_->deleteLater();
    }

    if(manager_){
        manager_->deleteLater();
    }

    if(reply_){
        reply_->deleteLater();
    }

    file_ = new QTemporaryFile(QDir::tempPath() + "/XXXXXX.zip", this);
    if(! file_->open()){
        qInfo() <<"Can't create a temporary file [" << file_->fileName() << "] for downloading:" << file_->errorString();
        emit sigComplite(false);
        return;
    }

    manager_ = new QNetworkAccessManager(this);

    downloadProgress_ = 0;

    QNetworkRequest request;
    request.setUrl(fileURL);
    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::downloadProgress, this, &Downloader::slotProgressChanged);
    connect(reply_, &QNetworkReply::readyRead, this, [this]{ file_->write(reply_->readAll()); });
    connect(manager_, &QNetworkAccessManager::finished, this, &Downloader::slotFinished);
    //connect(reply_, &QNetworkReply::finished, this, SLOT(onReplyFinished()));
}

bool Downloader::unZip(const QString destFolder, const QString password)
{
    QProcess unzip;
    unzip.start("unzip", QStringList{"-qo", "-P", password, file_->fileName(), "-d", destFolder});

    if (! unzip.waitForStarted(6000)){
        qInfo() << "Unzipping update error: " << unzip.errorString();
        return false;
    }

    if (! unzip.waitForFinished(6000)){
        qInfo() << "Unzipping update error: " << unzip.readAll();
        return false;
    }

    QByteArray ret(unzip.readAll());
    if(! ret.isEmpty()){
        // if somthing was return to output
        qInfo() << "Unexpected return from 'unzip'" << ret;
        return false;
    }

    return true;
}

void Downloader::slotFinished(QNetworkReply *reply)
{
    file_->close();// is needs because when 'file_' is opened it blocks 'unzip'. So if you need to use file_ -> open it!

    downloadProgress_ = -1;

    if(reply->error() == QNetworkReply::NoError){
        emit sigComplite(true);
    }else{
        qInfo() <<"Error during download update file:" << reply->errorString();
        emit sigComplite(false);
    }

    reply_->deleteLater();
}

void Downloader::slotProgressChanged(qint64 bytesRead, qint64 bytesTotal){
    bytesTotal = bytesTotal ? bytesTotal : 1;
    downloadProgress_ = (bytesRead*100.)/bytesTotal;
    emit sigProgressChanged(bytesRead, bytesTotal, downloadProgress_);
}

double Downloader::getDownloadProgress() const
{
    return downloadProgress_;
}
