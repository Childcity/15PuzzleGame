#ifndef OPERATIONCANCELEDERROR_H
#define OPERATIONCANCELEDERROR_H

#include <stdexcept>


namespace Net {


class OperationCanceledError : public std::runtime_error {
public:
    OperationCanceledError();
};


} // namespace Net

#endif // OPERATIONCANCELEDERROR_H
