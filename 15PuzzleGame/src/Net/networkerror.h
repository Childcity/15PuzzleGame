#pragma once

#include <QNetworkReply>
#include <stdexcept>


namespace Net {


class NetworkError : public std::runtime_error {
public:
    using NetErrorCode = QNetworkReply::NetworkError;

    NetworkError(NetErrorCode errorCode);

    NetErrorCode ErrorCode;
};


} // namespace Net
