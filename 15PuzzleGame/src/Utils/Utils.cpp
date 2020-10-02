#include "Utils.h"


namespace Utils {


DebugPrinter::DebugPrinter(const char *file_, int line_)
    : file(file_)
    , line(line_)
{}

QDebug operator<<(QDebug stream, const DebugPrinter printer)
{
    stream << QDateTime::currentDateTime().toString("mm:ss:zzz").toLatin1().data()
           << (QByteArray(printer.file) + '[' + QByteArray::number(printer.line) + "]>").data();
    return stream;
}

void QObjectDeleteLater::operator()(QObject *o)
{
    if (o->parent()) {
        o->deleteLater();
    } else {
        delete o;
    }
}

size_t GetFastNotSecureRandomNum(size_t min, size_t max)
{
    using namespace std::chrono;
    unsigned now = static_cast<unsigned>(system_clock::now().time_since_epoch().count());
    ::srand(now);
    size_t rnd = static_cast<size_t>(::rand());
    return min + rnd % (max - min + 1);
}

QtMessageHandler CustomMessageHandler::GetDefaultMessageHandler()
{
    // Default message handler to be called to bypass all other warnings.
    static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(nullptr);
    return QT_DEFAULT_MESSAGE_HANDLER;
}

void CustomMessageHandler::Handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Suppress warning with cache, as it is bug: https://github.com/ariya/phantomjs/issues/13165
    // Suppress warning with QNetworkReplyImplPrivate https://github.com/raelgc/scudcloud/issues/546
    switch (type) {
        case QtDebugMsg: {
            if (! msg.contains("setCachingEnabled"))
                (*GetDefaultMessageHandler())(type, context, msg);
        } break;
        case QtWarningMsg: {
            if (! msg.contains("QNetworkReplyImpl", Qt::CaseInsensitive))
                (*GetDefaultMessageHandler())(type, context, msg);
        } break;
        case QtCriticalMsg: {
            if (! msg.contains("QNetworkReplyImpl", Qt::CaseInsensitive))
                (*GetDefaultMessageHandler())(type, context, msg);
        } break;
        default:
            (*GetDefaultMessageHandler())(type, context, msg);
            break;
    }
}


} // namespace Utils
