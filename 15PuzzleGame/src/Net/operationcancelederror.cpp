#include "operationcancelederror.h"

Net::OperationCanceledError::OperationCanceledError()
    : std::runtime_error("Canceled")
{}
