#pragma once

#include <stdexcept>


namespace Net {


class OperationCanceledError : public std::runtime_error {
public:
    OperationCanceledError();
};


} // namespace Net
