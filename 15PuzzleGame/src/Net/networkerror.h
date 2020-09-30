#ifndef NETWORKERROR_H
#define NETWORKERROR_H

#include <stdexcept>

#include <QNetworkReply>
#include <QMetaEnum>

namespace Net {


class NetworkError : public std::runtime_error {
public:
    using NetErrorCode = QNetworkReply::NetworkError;

    NetworkError(NetErrorCode errorCode);

    NetErrorCode ErrorCode;
};


} // namespace Net

#endif // NETWORKERROR_H
