#ifndef __ERROR_LIST_HPP__
#define __ERROR_LIST_HPP__

#include <cstdint>

namespace NS_Error {

    /** Errors used for the engine logic */
    enum class Errors : int32_t {
        NONE = 0,
        FAILED = 1,
        ERROR_UNAVAILABLE,
        ERROR_UNCONFIGURED,
        ERROR_UNAUTHORIZED,
        ERROR_PARAMETER_RANGE_ERROR,
        ERROR_OUT_OF_MEMORY,
        ERROR_FILE_NOT_FOUND,
        ERROR_FILE_BAD_DRIVE,
        ERROR_FILE_BAD_PATH,
        ERROR_FILE_NO_PERMISSION,
        ERROR_FILE_ALREADY_IN_USE,
        ERROR_FILE_CANT_OPEN,
        ERROR_FILE_CANT_WRITE,
        ERROR_FILE_CANT_READ,
        ERROR_FILE_UNRECOGNIZED,
        ERROR_FILE_CORRUPT,
        ERROR_FILE_MISSING_DEPENDENCIES,
        ERROR_FILE_EOF,
        ERROR_CANT_OPEN,
        ERROR_CANT_CREATE,
        ERROR_QUERY_FAILED,
        ERROR_ALREADY_IN_USE,
        ERROR_LOCKED,
        ERROR_TIMEOUT,
        ERROR_CANT_CONNECT,
        ERROR_CANT_RESOLVE,
        ERROR_CONNECTION_ERROR,
        ERROR_CANT_ACQUIRE_RESOURCE,
        ERROR_CANT_FORK,
        ERROR_INVALID_DATA,
        ERROR_INVALID_PARAMETER,
        ERROR_ALREADY_EXISTS,
        ERROR_DOES_NOT_EXIST,
        ERROR_DATABASE_CANT_READ,
        ERROR_DATABASE_CANT_WRITE,
        ERROR_COMPILATION_FAILED,
        ERROR_METHOD_NOT_FOUND,
        ERROR_LINK_FAILED,
        ERROR_SCRIPT_FAILED,
        ERROR_CYCLIC_LINK,
        ERROR_INVALID_DECLARATION,
        ERROR_DUPLICATE_SYMBOL,
        ERROR_PARSE_ERROR,
        ERROR_BUSY,
        ERROR_SKIP,
        ERROR_HELP,
        ERROR_BUG,
        ERROR_PRINTER_ON_FIRE,
        ERROR_MAX = 49,
    };

    /** Error array to be used in the code */
    extern const char *error_names[];
}

#endif
