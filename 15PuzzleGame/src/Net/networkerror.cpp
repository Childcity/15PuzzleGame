#include "networkerror.h"

#include <QMetaEnum>

Net::NetworkError::NetworkError(Net::NetworkError::NetErrorCode errorCode)
    : std::runtime_error(
          QMetaEnum::fromType<NetErrorCode>().valueToKey(errorCode))
    , ErrorCode(errorCode)
{}
