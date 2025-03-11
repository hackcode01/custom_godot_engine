#ifndef __ERROR_MACROS_HPP__
#define __ERROR_MACROS_HPP__

#pragma warning(disable : 4464)

#include "../object/object_id.hpp"
#include "../typedefs.hpp"

#include <atomic>

class String;

enum class ErrorHandlerType {
    ERROR_HANDLER_ERROR,
    ERROR_HANDLER_WARNING,
    ERROR_HANDLER_SCRIPT,
    ERROR_HANDLER_SHADER,
};

/** Pointer to the error handler printing function.
 *  Reassign to any function to have errors printed.
 *  Parameters: userdata, function, file, line, error, explanation, type.
 */
typedef void (*ErrorHandlerFunction)(void *, const char *, const char *,
                                     int32_t p_line, const char *, const char *,
                                     bool p_editor_notify, ErrorHandlerType p_type);

struct ErrorHandlerList {
    ErrorHandlerFunction error_function = nullptr;
    void *user_data = nullptr;

    ErrorHandlerList *next = nullptr;

    ErrorHandlerList() {}
};

void add_error_handler(ErrorHandlerList *p_handler);
void remove_error_handler(const ErrorHandlerList *p_handler);

/** Functions used by the error macros. */
void _error_print_error(const char *p_function, const char *p_file, int p_line,
                        const char *p_error, bool p_editor_notify = false,
                        ErrorHandlerType p_type = ErrorHandlerType::ERROR_HANDLER_ERROR);

void _error_print_error(const char *p_function, const char *p_file, int32_t p_line,
                        const String &p_error, bool p_editor_notify = false,
                        ErrorHandlerType p_type = ErrorHandlerType::ERROR_HANDLER_ERROR);

void _error_print_error(const char *p_function, const char *p_file, int32_t p_line,
                        const char *p_error, const char *p_message,
                        bool p_editor_notify = false,
                        ErrorHandlerType p_type = ErrorHandlerType::ERROR_HANDLER_ERROR);

void _error_print_error(const char *p_function, const char *p_file, int32_t p_line,
                        const String &p_error, const char *p_message,
                        bool p_editor_notify = false,
                        ErrorHandlerType p_type = ErrorHandlerType::ERROR_HANDLER_ERROR);

void _error_print_error(const char *p_function, const char *p_file, int32_t p_line,
                        const char *p_error, const String &p_message,
                        bool p_editor_notify = false,
                        ErrorHandlerType p_type = ErrorHandlerType::ERROR_HANDLER_ERROR);

void _error_print_error(const char *p_function, const char *p_file, int32_t p_line,
                        const String &p_error, const String &p_message,
                        bool p_editor_notify = false,
                        ErrorHandlerType p_type = ErrorHandlerType::ERROR_HANDLER_ERROR);

void _error_print_error_asap(const String &p_error,
                             ErrorHandlerType p_type = ErrorHandlerType::ERROR_HANDLER_ERROR);

void _error_print_index_error(const char *p_function, const char *p_file, int32_t p_line,
                              int64_t p_index, int64_t p_size, const char *p_index_str,
                              const char *p_size_str, const char *p_message = "",
                              bool p_editor_notify = false, bool fatal = false);

void _error_print_index_error(const char *p_function, const char *p_file, int32_t p_line,
                              int64_t p_index, int64_t p_size, const char *p_index_str,
                              const char *p_size_str, const String &p_message,
                              bool p_editor_notify = false, bool fatal = false);

void _error_flush_stdout();

void _physics_interpolation_warning(const char *p_function, const char *p_file,
                                    int32_t p_line, ObjectID p_id, const char *p_warn_string);

#ifdef __GNUC__
/** #define FUNCTION_STR __PRETTY_FUNCTION__ - too annoying */
#define FUNCTION_STR __FUNCTION__
#else
#define FUNCTION_STR __FUNCTION__
#endif

#ifdef _MSC_VER
/**
 * Don't use GENERATE_TRAP() directly, should only be used be the macros below.
 */
#define GENERATE_TRAP() __debugbreak()
#else
/**
 * Don't use GENERATE_TRAP() directly, should only be used be the macros below.
 */
#define GENERATE_TRAP() __builtin_trap()
#endif

/**
 * NS_Error macros.
 * WARNING: These macros work in the opposite way to assert().
 *
 * Unlike exceptions and asserts, these macros try to maintain consistency and stability.
 * In most cases, bugs and/or invalid data are not fatal. They should never allow a perfectly
 * running application to fail or crash.
 * Always try to return processable data, so the engine can keep running well.
 * Use the _MSG versions to print a meaningful message to help with debugging.
 *
 * The `((void)0)` no-op statement is used as a trick to force us to put a semicolon after
 * those macros, making them look like proper statements.
 * The if wrappers are used to ensure that the macro replacement does not trigger unexpected
 * issues when expanded e.g. after an `if (cond) ERR_FAIL();` without braces.
 */

/** Index out of bounds error macros.
 *  These macros should be used instead of `ERR_FAIL_COND` for bounds checking.
 */

/** Integer index out of bounds error macros. */

/**
 * Try using `ERROR_FAIL_INDEX_MSG`.
 * Only use this macro if there is no sensible error message.
 *
 * Ensures an integer index `m_index` is less than `m_size` and greater than or equal to 0.
 * If not, the current function returns.
 */
#define ERROR_FAIL_INDEX(m_index, m_size)                                                                         \
    if (unlikely((m_index) < 0 || (m_index) >= (m_size)))                                                         \
    {                                                                                                             \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size)); \
        return;                                                                                                   \
    }                                                                                                             \
    else                                                                                                          \
    {                                                                                                             \
        ((void)0)                                                                                                 \
    }

/**
 * Ensures an integer index `m_index` is less than `m_size` and greater than or equal to 0.
 * If not, prints `m_msg` and the current function returns.
 */
#define ERROR_FAIL_INDEX_MSG(m_index, m_size, m_msg)                                                                     \
    if (unlikely((m_index) < 0 || (m_index) >= (m_size)))                                                                \
    {                                                                                                                    \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), m_msg); \
        return;                                                                                                          \
    }                                                                                                                    \
    else                                                                                                                 \
    {                                                                                                                    \
        ((void)0)                                                                                                        \
    }

/**
 * Same as `ERROR_FAIL_INDEX_MSG` but also notifies the editor.
 */
#define ERROR_FAIL_INDEX_EDMSG(m_index, m_size, m_msg)                                                                         \
    if (unlikely((m_index) < 0 || (m_index) >= (m_size)))                                                                      \
    {                                                                                                                          \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), m_msg, true); \
        return;                                                                                                                \
    }                                                                                                                          \
    else                                                                                                                       \
    {                                                                                                                          \
        ((void)0)                                                                                                              \
    }

/**
 * Try using `ERROR_FAIL_INDEX_V_MSG`.
 * Only use this macro if there is no sensible error message.
 *
 * Ensures an integer index `m_index` is less than `m_size` and greater than or equal to 0.
 * If not, the current function returns `m_retval`.
 */
#define ERROR_FAIL_INDEX_V(m_index, m_size, m_retval)                                                             \
    if (unlikely((m_index) < 0 || (m_index) >= (m_size)))                                                         \
    {                                                                                                             \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size)); \
        return m_retval;                                                                                          \
    }                                                                                                             \
    else                                                                                                          \
    {                                                                                                             \
        ((void)0)                                                                                                 \
    }

/**
 * Ensures an integer index `m_index` is less than `m_size` and greater than or equal to 0.
 * If not, prints `m_msg` and the current function returns `m_retval`.
 */
#define ERROR_FAIL_INDEX_V_MSG(m_index, m_size, m_retval, m_msg)                                                         \
    if (unlikely((m_index) < 0 || (m_index) >= (m_size)))                                                                \
    {                                                                                                                    \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), m_msg); \
        return m_retval;                                                                                                 \
    }                                                                                                                    \
    else                                                                                                                 \
    {                                                                                                                    \
        ((void)0)                                                                                                        \
    }

/**
 * Same as `ERROR_FAIL_INDEX_V_MSG` but also notifies the editor.
 */
#define ERROR_FAIL_INDEX_V_EDMSG(m_index, m_size, m_retval, m_msg)                                                             \
    if (unlikely((m_index) < 0 || (m_index) >= (m_size)))                                                                      \
    {                                                                                                                          \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), m_msg, true); \
        return m_retval;                                                                                                       \
    }                                                                                                                          \
    else                                                                                                                       \
    {                                                                                                                          \
        ((void)0)                                                                                                              \
    }

/**
 * Try using `ERROR_FAIL_INDEX_MSG` or `ERROR_FAIL_INDEX_V_MSG`.
 * Only use this macro if there is no sensible fallback i.e. the error is unrecoverable, and
 * there is no sensible error message.
 *
 * Ensures an integer index `m_index` is less than `m_size` and greater than or equal to 0.
 * If not, the application crashes.
 */
#define CRASH_BAD_INDEX(m_index, m_size)                                                                                           \
    if (unlikely((m_index) < 0 || (m_index) >= (m_size)))                                                                          \
    {                                                                                                                              \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), "", false, true); \
        _error_flush_stdout();                                                                                                     \
        GENERATE_TRAP();                                                                                                           \
    }                                                                                                                              \
    else                                                                                                                           \
    {                                                                                                                              \
        ((void)0)                                                                                                                  \
    }

/**
 * Try using `ERROR_FAIL_INDEX_MSG` or `ERROR_FAIL_INDEX_V_MSG`.
 * Only use this macro if there is no sensible fallback i.e. the error is unrecoverable.
 *
 * Ensures an integer index `m_index` is less than `m_size` and greater than or equal to 0.
 * If not, prints `m_msg` and the application crashes.
 */
#define CRASH_BAD_INDEX_MSG(m_index, m_size, m_msg)                                                                                   \
    if (unlikely((m_index) < 0 || (m_index) >= (m_size)))                                                                             \
    {                                                                                                                                 \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), m_msg, false, true); \
        _error_flush_stdout();                                                                                                        \
        GENERATE_TRAP();                                                                                                              \
    }                                                                                                                                 \
    else                                                                                                                              \
    {                                                                                                                                 \
        ((void)0)                                                                                                                     \
    }

/** Unsigned integer index out of bounds error macros. */

/**
 * Try using `ERROR_FAIL_UNSIGNED_INDEX_MSG`.
 * Only use this macro if there is no sensible error message.
 *
 * Ensures an unsigned integer index `m_index` is less than `m_size`.
 * If not, the current function returns.
 */
#define ERROR_FAIL_UNSIGNED_INDEX(m_index, m_size)                                                                \
    if (unlikely((m_index) >= (m_size)))                                                                          \
    {                                                                                                             \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size)); \
        return;                                                                                                   \
    }                                                                                                             \
    else                                                                                                          \
    {                                                                                                             \
        ((void)0)                                                                                                 \
    }

/**
 * Ensures an unsigned integer index `m_index` is less than `m_size`.
 * If not, prints `m_msg` and the current function returns.
 */
#define ERROR_FAIL_UNSIGNED_INDEX_MSG(m_index, m_size, m_msg)                                                            \
    if (unlikely((m_index) >= (m_size)))                                                                                 \
    {                                                                                                                    \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), m_msg); \
        return;                                                                                                          \
    }                                                                                                                    \
    else                                                                                                                 \
    {                                                                                                                    \
        ((void)0)                                                                                                        \
    }

/**
 * Same as `ERROR_FAIL_UNSIGNED_INDEX_MSG` but also notifies the editor.
 */
#define ERROR_FAIL_UNSIGNED_INDEX_EDMSG(m_index, m_size, m_msg)                                                                \
    if (unlikely((m_index) >= (m_size)))                                                                                       \
    {                                                                                                                          \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), m_msg, true); \
        return;                                                                                                                \
    }                                                                                                                          \
    else                                                                                                                       \
    {                                                                                                                          \
        ((void)0)                                                                                                              \
    }

/**
 * Try using `ERROR_FAIL_UNSIGNED_INDEX_V_MSG`.
 * Only use this macro if there is no sensible error message.
 *
 * Ensures an unsigned integer index `m_index` is less than `m_size`.
 * If not, the current function returns `m_retval`.
 */
#define ERROR_FAIL_UNSIGNED_INDEX_V(m_index, m_size, m_retval)                                                    \
    if (unlikely((m_index) >= (m_size)))                                                                          \
    {                                                                                                             \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size)); \
        return m_retval;                                                                                          \
    }                                                                                                             \
    else                                                                                                          \
    {                                                                                                             \
        ((void)0)                                                                                                 \
    }

/**
 * Ensures an unsigned integer index `m_index` is less than `m_size`.
 * If not, prints `m_msg` and the current function returns `m_retval`.
 */
#define ERROR_FAIL_UNSIGNED_INDEX_V_MSG(m_index, m_size, m_retval, m_msg)                                                \
    if (unlikely((m_index) >= (m_size)))                                                                                 \
    {                                                                                                                    \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), m_msg); \
        return m_retval;                                                                                                 \
    }                                                                                                                    \
    else                                                                                                                 \
    {                                                                                                                    \
        ((void)0)                                                                                                        \
    }

/**
 * Same as `ERROR_FAIL_UNSIGNED_INDEX_V_EDMSG` but also notifies the editor.
 */
#define ERROR_FAIL_UNSIGNED_INDEX_V_EDMSG(m_index, m_size, m_retval, m_msg)                                                    \
    if (unlikely((m_index) >= (m_size)))                                                                                       \
    {                                                                                                                          \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), m_msg, true); \
        return m_retval;                                                                                                       \
    }                                                                                                                          \
    else                                                                                                                       \
        ((void)0)

/**
 * Try using `ERROR_FAIL_UNSIGNED_INDEX_MSG` or `ERROR_FAIL_UNSIGNED_INDEX_V_MSG`.
 * Only use this macro if there is no sensible fallback i.e. the error is unrecoverable, and
 * there is no sensible error message.
 *
 * Ensures an unsigned integer index `m_index` is less than `m_size`.
 * If not, the application crashes.
 */
#define CRASH_BAD_UNSIGNED_INDEX(m_index, m_size)                                                                                  \
    if (unlikely((m_index) >= (m_size)))                                                                                           \
    {                                                                                                                              \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), "", false, true); \
        _error_flush_stdout();                                                                                                     \
        GENERATE_TRAP();                                                                                                           \
    }                                                                                                                              \
    else                                                                                                                           \
        ((void)0)

/**
 * Try using `ERROR_FAIL_UNSIGNED_INDEX_MSG` or `ERROR_FAIL_UNSIGNED_INDEX_V_MSG`.
 * Only use this macro if there is no sensible fallback i.e. the error is unrecoverable.
 *
 * Ensures an unsigned integer index `m_index` is less than `m_size`.
 * If not, prints `m_msg` and the application crashes.
 */
#define CRASH_BAD_UNSIGNED_INDEX_MSG(m_index, m_size, m_msg)                                                                          \
    if (unlikely((m_index) >= (m_size)))                                                                                              \
    {                                                                                                                                 \
        _error_print_index_error(FUNCTION_STR, __FILE__, __LINE__, m_index, m_size, _STR(m_index), _STR(m_size), m_msg, false, true); \
        _error_flush_stdout();                                                                                                        \
        GENERATE_TRAP();                                                                                                              \
    }                                                                                                                                 \
    else                                                                                                                              \
        ((void)0)

/** Null reference error macros. */

/**
 * Try using `ERROR_FAIL_NULL_MSG`.
 * Only use this macro if there is no sensible error message.
 *
 * Ensures a pointer `m_param` is not null.
 * If it is null, the current function returns.
 */
#define ERROR_FAIL_NULL(m_param)                                                                          \
    if (unlikely(m_param == nullptr))                                                                     \
    {                                                                                                     \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null."); \
        return;                                                                                           \
    }                                                                                                     \
    else                                                                                                  \
        ((void)0)

/**
 * Ensures a pointer `m_param` is not null.
 * If it is null, prints `m_msg` and the current function returns.
 */
#define ERROR_FAIL_NULL_MSG(m_param, m_msg)                                                                      \
    if (unlikely(m_param == nullptr))                                                                            \
    {                                                                                                            \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null.", m_msg); \
        return;                                                                                                  \
    }                                                                                                            \
    else                                                                                                         \
        ((void)0)

/**
 * Same as `ERROR_FAIL_NULL_MSG` but also notifies the editor.
 */
#define ERROR_FAIL_NULL_EDMSG(m_param, m_msg)                                                                          \
    if (unlikely(m_param == nullptr))                                                                                  \
    {                                                                                                                  \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null.", m_msg, true); \
        return;                                                                                                        \
    }                                                                                                                  \
    else                                                                                                               \
        ((void)0)

/**
 * Try using `ERROR_FAIL_NULL_V_MSG`.
 * Only use this macro if there is no sensible error message.
 *
 * Ensures a pointer `m_param` is not null.
 * If it is null, the current function returns `m_retval`.
 */
#define ERROR_FAIL_NULL_V(m_param, m_retval)                                                              \
    if (unlikely(m_param == nullptr))                                                                     \
    {                                                                                                     \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null."); \
        return m_retval;                                                                                  \
    }                                                                                                     \
    else                                                                                                  \
        ((void)0)

/**
 * Ensures a pointer `m_param` is not null.
 * If it is null, prints `m_msg` and the current function returns `m_retval`.
 */
#define ERROR_FAIL_NULL_V_MSG(m_param, m_retval, m_msg)                                                          \
    if (unlikely(m_param == nullptr))                                                                            \
    {                                                                                                            \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null.", m_msg); \
        return m_retval;                                                                                         \
    }                                                                                                            \
    else                                                                                                         \
        ((void)0)

/**
 * Same as `ERROR_FAIL_NULL_V_MSG` but also notifies the editor.
 */
#define ERROR_FAIL_NULL_V_EDMSG(m_param, m_retval, m_msg)                                                              \
    if (unlikely(m_param == nullptr))                                                                                  \
    {                                                                                                                  \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Parameter \"" _STR(m_param) "\" is null.", m_msg, true); \
        return m_retval;                                                                                               \
    }                                                                                                                  \
    else                                                                                                               \
        ((void)0)

/**
 * Try using `ERROR_FAIL_COND_MSG`.
 * Only use this macro if there is no sensible error message.
 * If checking for null use ERROR_FAIL_NULL_MSG instead.
 * If checking index bounds use ERROR_FAIL_INDEX_MSG instead.
 *
 * Ensures `m_cond` is false.
 * If `m_cond` is true, the current function returns.
 */
#define ERROR_FAIL_COND(m_cond)                                                                          \
    if (unlikely(m_cond))                                                                                \
    {                                                                                                    \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true."); \
        return;                                                                                          \
    }                                                                                                    \
    else                                                                                                 \
        ((void)0)

/**
 * Ensures `m_cond` is false.
 * If `m_cond` is true, prints `m_msg` and the current function returns.
 *
 * If checking for null use ERROR_FAIL_NULL_MSG instead.
 * If checking index bounds use ERROR_FAIL_INDEX_MSG instead.
 */
#define ERROR_FAIL_COND_MSG(m_cond, m_msg)                                                                      \
    if (unlikely(m_cond))                                                                                       \
    {                                                                                                           \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true.", m_msg); \
        return;                                                                                                 \
    }                                                                                                           \
    else                                                                                                        \
        ((void)0)

/**
 * Same as `ERROR_FAIL_COND_MSG` but also notifies the editor.
 */
#define ERROR_FAIL_COND_EDMSG(m_cond, m_msg)                                                                          \
    if (unlikely(m_cond))                                                                                             \
    {                                                                                                                 \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true.", m_msg, true); \
        return;                                                                                                       \
    }                                                                                                                 \
    else                                                                                                              \
        ((void)0)

/**
 * Try using `ERROR_FAIL_COND_V_MSG`.
 * Only use this macro if there is no sensible error message.
 * If checking for null use ERROR_FAIL_NULL_V_MSG instead.
 * If checking index bounds use ERROR_FAIL_INDEX_V_MSG instead.
 *
 * Ensures `m_cond` is false.
 * If `m_cond` is true, the current function returns `m_retval`.
 */
#define ERROR_FAIL_COND_V(m_cond, m_retval)                                                                                         \
    if (unlikely(m_cond))                                                                                                           \
    {                                                                                                                               \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Returning: " _STR(m_retval)); \
        return m_retval;                                                                                                            \
    }                                                                                                                               \
    else                                                                                                                            \
        ((void)0)

/**
 * Ensures `m_cond` is false.
 * If `m_cond` is true, prints `m_msg` and the current function returns `m_retval`.
 *
 * If checking for null use ERROR_FAIL_NULL_V_MSG instead.
 * If checking index bounds use ERROR_FAIL_INDEX_V_MSG instead.
 */
#define ERROR_FAIL_COND_V_MSG(m_cond, m_retval, m_msg)                                                                                     \
    if (unlikely(m_cond))                                                                                                                  \
    {                                                                                                                                      \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Returning: " _STR(m_retval), m_msg); \
        return m_retval;                                                                                                                   \
    }                                                                                                                                      \
    else                                                                                                                                   \
        ((void)0)

/**
 * Same as `ERROR_FAIL_COND_V_MSG` but also notifies the editor.
 */
#define ERROR_FAIL_COND_V_EDMSG(m_cond, m_retval, m_msg)                                                                                         \
    if (unlikely(m_cond))                                                                                                                        \
    {                                                                                                                                            \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Returning: " _STR(m_retval), m_msg, true); \
        return m_retval;                                                                                                                         \
    }                                                                                                                                            \
    else                                                                                                                                         \
        ((void)0)

/**
 * Try using `ERROR_CONTINUE_MSG`.
 * Only use this macro if there is no sensible error message.
 *
 * Ensures `m_cond` is false.
 * If `m_cond` is true, the current loop continues.
 */
#define ERROR_CONTINUE(m_cond)                                                                                       \
    if (unlikely(m_cond))                                                                                            \
    {                                                                                                                \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Continuing."); \
        continue;                                                                                                    \
    }                                                                                                                \
    else                                                                                                             \
        ((void)0)

/**
 * Ensures `m_cond` is false.
 * If `m_cond` is true, prints `m_msg` and the current loop continues.
 */
#define ERROR_CONTINUE_MSG(m_cond, m_msg)                                                                                   \
    if (unlikely(m_cond))                                                                                                   \
    {                                                                                                                       \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Continuing.", m_msg); \
        continue;                                                                                                           \
    }                                                                                                                       \
    else                                                                                                                    \
        ((void)0)

/**
 * Same as `ERROR_CONTINUE_MSG` but also notifies the editor.
 */
#define ERROR_CONTINUE_EDMSG(m_cond, m_msg)                                                                                       \
    if (unlikely(m_cond))                                                                                                         \
    {                                                                                                                             \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Continuing.", m_msg, true); \
        continue;                                                                                                                 \
    }                                                                                                                             \
    else                                                                                                                          \
        ((void)0)

/**
 * Try using `ERROR_BREAK_MSG`.
 * Only use this macro if there is no sensible error message.
 *
 * Ensures `m_cond` is false.
 * If `m_cond` is true, the current loop breaks.
 */
#define ERROR_BREAK(m_cond)                                                                                        \
    if (unlikely(m_cond))                                                                                          \
    {                                                                                                              \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Breaking."); \
        break;                                                                                                     \
    }                                                                                                              \
    else                                                                                                           \
        ((void)0)

/**
 * Ensures `m_cond` is false.
 * If `m_cond` is true, prints `m_msg` and the current loop breaks.
 */
#define ERROR_BREAK_MSG(m_cond, m_msg)                                                                                    \
    if (unlikely(m_cond))                                                                                                 \
    {                                                                                                                     \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Breaking.", m_msg); \
        break;                                                                                                            \
    }                                                                                                                     \
    else                                                                                                                  \
        ((void)0)

/**
 * Same as `ERROR_BREAK_MSG` but also notifies the editor.
 */
#define ERROR_BREAK_EDMSG(m_cond, m_msg)                                                                                        \
    if (unlikely(m_cond))                                                                                                       \
    {                                                                                                                           \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Condition \"" _STR(m_cond) "\" is true. Breaking.", m_msg, true); \
        break;                                                                                                                  \
    }                                                                                                                           \
    else                                                                                                                        \
        ((void)0)

/**
 * Try using `ERROR_FAIL_COND_MSG` or `ERROR_FAIL_COND_V_MSG`.
 * Only use this macro if there is no sensible fallback i.e. the error is unrecoverable, and
 * there is no sensible error message.
 *
 * Ensures `m_cond` is false.
 * If `m_cond` is true, the application crashes.
 */
#define CRASH_COND(m_cond)                                                                                      \
    if (unlikely(m_cond))                                                                                       \
    {                                                                                                           \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "FATAL: Condition \"" _STR(m_cond) "\" is true."); \
        _error_flush_stdout();                                                                                  \
        GENERATE_TRAP();                                                                                        \
    }                                                                                                           \
    else                                                                                                        \
        ((void)0)

/**
 * Try using `ERROR_FAIL_COND_MSG` or `ERROR_FAIL_COND_V_MSG`.
 * Only use this macro if there is no sensible fallback i.e. the error is unrecoverable.
 *
 * Ensures `m_cond` is false.
 * If `m_cond` is true, prints `m_msg` and the application crashes.
 */
#define CRASH_COND_MSG(m_cond, m_msg)                                                                                  \
    if (unlikely(m_cond))                                                                                              \
    {                                                                                                                  \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "FATAL: Condition \"" _STR(m_cond) "\" is true.", m_msg); \
        _error_flush_stdout();                                                                                         \
        GENERATE_TRAP();                                                                                               \
    }                                                                                                                  \
    else                                                                                                               \
        ((void)0)

/** Generic error macros. */

/**
 * Try using `ERROR_FAIL_COND_MSG` or `ERROR_FAIL_MSG`.
 * Only use this macro if more complex error detection or recovery is required, and
 * there is no sensible error message.
 *
 * The current function returns.
 */
#define ERROR_FAIL()                                                                     \
    if (true)                                                                            \
    {                                                                                    \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Method/function failed."); \
        return;                                                                          \
    }                                                                                    \
    else                                                                                 \
        ((void)0)

/**
 * Try using `ERROR_FAIL_COND_MSG`.
 * Only use this macro if more complex error detection or recovery is required.
 *
 * Prints `m_msg`, and the current function returns.
 */
#define ERROR_FAIL_MSG(m_msg)                                                                   \
    if (true)                                                                                   \
    {                                                                                           \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Method/function failed.", m_msg); \
        return;                                                                                 \
    }                                                                                           \
    else                                                                                        \
        ((void)0)

/**
 * Same as `ERROR_FAIL_MSG` but also notifies the editor.
 */
#define ERROR_FAIL_EDMSG(m_msg)                                                                       \
    if (true)                                                                                         \
    {                                                                                                 \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Method/function failed.", m_msg, true); \
        return;                                                                                       \
    }                                                                                                 \
    else                                                                                              \
        ((void)0)

/**
 * Try using `ERROR_FAIL_COND_V_MSG` or `ERROR_FAIL_V_MSG`.
 * Only use this macro if more complex error detection or recovery is required, and
 * there is no sensible error message.
 *
 * The current function returns `m_retval`.
 */
#define ERROR_FAIL_V(m_retval)                                                                                      \
    if (true)                                                                                                       \
    {                                                                                                               \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Method/function failed. Returning: " _STR(m_retval)); \
        return m_retval;                                                                                            \
    }                                                                                                               \
    else                                                                                                            \
        ((void)0)

/**
 * Try using `ERROR_FAIL_COND_V_MSG`.
 * Only use this macro if more complex error detection or recovery is required.
 *
 * Prints `m_msg`, and the current function returns `m_retval`.
 */
#define ERROR_FAIL_V_MSG(m_retval, m_msg)                                                                                  \
    if (true)                                                                                                              \
    {                                                                                                                      \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Method/function failed. Returning: " _STR(m_retval), m_msg); \
        return m_retval;                                                                                                   \
    }                                                                                                                      \
    else                                                                                                                   \
        ((void)0)

/**
 * Same as `ERROR_FAIL_V_MSG` but also notifies the editor.
 */
#define ERROR_FAIL_V_EDMSG(m_retval, m_msg)                                                                                      \
    if (true)                                                                                                                    \
    {                                                                                                                            \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "Method/function failed. Returning: " _STR(m_retval), m_msg, true); \
        return m_retval;                                                                                                         \
    }                                                                                                                            \
    else                                                                                                                         \
        ((void)0)

/**
 * Try using `ERROR_FAIL_COND_MSG`, `ERROR_FAIL_COND_V_MSG`, `ERROR_CONTINUE_MSG` or `ERROR_BREAK_MSG`.
 * Only use this macro at the start of a function that has not been implemented yet, or
 * if more complex error detection or recovery is required.
 *
 * Prints `m_msg`.
 */
#define ERROR_PRINT(m_msg) \
    _error_print_error(FUNCTION_STR, __FILE__, __LINE__, m_msg)

/**
 * Same as `ERROR_PRINT` but also notifies the editor.
 */
#define ERROR_PRINT_ED(m_msg) \
    _error_print_error(FUNCTION_STR, __FILE__, __LINE__, m_msg, true)

/**
 * Prints `m_msg` once during the application lifetime.
 */
#define ERROR_PRINT_ONCE(m_msg)                                          \
    if (true)                                                            \
    {                                                                    \
        static bool first_print = true;                                  \
        if (first_print)                                                 \
        {                                                                \
            _error_print_error(FUNCTION_STR, __FILE__, __LINE__, m_msg); \
            first_print = false;                                         \
        }                                                                \
    }                                                                    \
    else                                                                 \
        ((void)0)

/**
 * Same as `ERROR_PRINT_ONCE` but also notifies the editor.
 */
#define ERROR_PRINT_ONCE_ED(m_msg)                                             \
    if (true)                                                                  \
    {                                                                          \
        static bool first_print = true;                                        \
        if (first_print)                                                       \
        {                                                                      \
            _error_print_error(FUNCTION_STR, __FILE__, __LINE__, m_msg, true); \
            first_print = false;                                               \
        }                                                                      \
    }                                                                          \
    else                                                                       \
        ((void)0)

/** Print warning message macros. */

/**
 * Prints `m_msg`.
 *
 * If warning about deprecated usage, use `WARNING_DEPRECATED` or `WARNING_DEPRECATED_MSG` instead.
 */
#define WARNING_PRINT(m_msg) \
    _error_print_error(FUNCTION_STR, __FILE__, __LINE__, m_msg, false, ERR_HANDLER_WARNING)

/**
 * Same as `WARNING_PRINT` but also notifies the editor.
 */
#define WARNING_PRINT_ED(m_msg) \
    _error_print_error(FUNCTION_STR, __FILE__, __LINE__, m_msg, true, ERR_HANDLER_WARNING)

/**
 * Prints `m_msg` once during the application lifetime.
 *
 * If warning about deprecated usage, use `WARN_DEPRECATED` or `WARN_DEPRECATED_MSG` instead.
 */
#define WARNING_PRINT_ONCE(m_msg)                                                                    \
    if (true)                                                                                        \
    {                                                                                                \
        static bool first_print = true;                                                              \
        if (first_print)                                                                             \
        {                                                                                            \
            _error_print_error(FUNCTION_STR, __FILE__, __LINE__, m_msg, false, ERR_HANDLER_WARNING); \
            first_print = false;                                                                     \
        }                                                                                            \
    }                                                                                                \
    else                                                                                             \
        ((void)0)

/**
 * Same as `WARNING_PRINT_ONCE` but also notifies the editor.
 */
#define WARNING_PRINT_ONCE_ED(m_msg)                                                                \
    if (true)                                                                                       \
    {                                                                                               \
        static bool first_print = true;                                                             \
        if (first_print)                                                                            \
        {                                                                                           \
            _error_print_error(FUNCTION_STR, __FILE__, __LINE__, m_msg, true, ERR_HANDLER_WARNING); \
            first_print = false;                                                                    \
        }                                                                                           \
    }                                                                                               \
    else                                                                                            \
        ((void)0)

/**
 * Warns about `m_msg` only when verbose mode is enabled.
 */
#define WARNING_VERBOSE(m_msg)          \
    {                                   \
        if (is_print_verbose_enabled()) \
        {                               \
            WARN_PRINT(m_msg);          \
        }                               \
    }

/** Print deprecated warning message macros. */

/**
 * Warns that the current function is deprecated.
 */
#define WARNING_DEPRECATED                                                                                                                                          \
    if (true)                                                                                                                                                       \
    {                                                                                                                                                               \
        static std::atomic<bool> warning_shown;                                                                                                                     \
        if (!warning_shown.load())                                                                                                                                  \
        {                                                                                                                                                           \
            _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "This method has been deprecated and will be removed in the future.", false, ERR_HANDLER_WARNING); \
            warning_shown.store(true);                                                                                                                              \
        }                                                                                                                                                           \
    }                                                                                                                                                               \
    else                                                                                                                                                            \
        ((void)0)

/**
 * Warns that the current function is deprecated and prints `m_msg`.
 */
#define WARNING_DEPRECATED_MSG(m_msg)                                                                                                                                      \
    if (true)                                                                                                                                                              \
    {                                                                                                                                                                      \
        static std::atomic<bool> warning_shown;                                                                                                                            \
        if (!warning_shown.load())                                                                                                                                         \
        {                                                                                                                                                                  \
            _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "This method has been deprecated and will be removed in the future.", m_msg, false, ERR_HANDLER_WARNING); \
            warning_shown.store(true);                                                                                                                                     \
        }                                                                                                                                                                  \
    }                                                                                                                                                                      \
    else                                                                                                                                                                   \
        ((void)0)

/**
 * Do not use.
 * If the application should never reach this point use CRASH_NOW_MSG(m_msg) to explain why.
 *
 * The application crashes.
 */
#define CRASH_NOW()                                                                             \
    if (true)                                                                                   \
    {                                                                                           \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "FATAL: Method/function failed."); \
        _error_flush_stdout();                                                                  \
        GENERATE_TRAP();                                                                        \
    }                                                                                           \
    else                                                                                        \
        ((void)0)

/**
 * Only use if the application should never reach this point.
 *
 * Prints `m_msg`, and then the application crashes.
 */
#define CRASH_NOW_MSG(m_msg)                                                                           \
    if (true)                                                                                          \
    {                                                                                                  \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "FATAL: Method/function failed.", m_msg); \
        _error_flush_stdout();                                                                         \
        GENERATE_TRAP();                                                                               \
    }                                                                                                  \
    else                                                                                               \
        ((void)0)

/**
 * Note: IN MOST CASES YOU SHOULD NOT USE THIS MACRO.
 * Do not use unless you understand the trade-offs.
 *
 * DEV macros will be compiled out in releases, they are wrapped in DEV_ENABLED.
 *
 * Prefer WARNINGS / ERR_FAIL macros (which fail without crashing) - ERR_FAIL should be used in most cases.
 * Then CRASH_NOW_MSG macros (on rare occasions where error cannot be recovered).
 *
 * DEV_ASSERT should generally only be used when both of the following conditions are met:
 * 1) Bottleneck code where a check in release would be too expensive.
 * 2) Situations where the check would fail obviously and straight away during the maintenance of the code
 *    (i.e. strict conditions that should be true no matter what)
 *    and that can't fail for other contributors once the code is finished and merged.
 */
#ifdef DEV_ENABLED
#define DEV_ASSERT(m_cond)                                                                                                \
    if (unlikely(!(m_cond)))                                                                                              \
    {                                                                                                                     \
        _error_print_error(FUNCTION_STR, __FILE__, __LINE__, "FATAL: DEV_ASSERT failed  \"" _STR(m_cond) "\" is false."); \
        _error_flush_stdout();                                                                                            \
        GENERATE_TRAP();                                                                                                  \
    }                                                                                                                     \
    else                                                                                                                  \
    {                                                                                                                     \
        ((void)0)                                                                                                         \
    }
#else
#define DEV_ASSERT(m_cond)
#endif

#ifdef DEV_ENABLED
#define DEV_CHECK_ONCE(m_cond)                                                     \
    if (unlikely(!(m_cond)))                                                       \
    {                                                                              \
        ERROR_PRINT_ONCE("DEV_CHECK_ONCE failed  \"" _STR(m_cond) "\" is false."); \
    }                                                                              \
    else                                                                           \
    {                                                                              \
        ((void)0)                                                                  \
    }
#else
#define DEV_CHECK_ONCE(m_cond)
#endif

/**
 * Physics Interpolation warnings.
 * These are spam protection warnings.
 */
#define PHYSICS_INTERPOLATION_NODE_WARNING(m_object_id, m_string) \
    _physics_interpolation_warning(FUNCTION_STR, __FILE__, __LINE__, m_object_id, m_string)

#define PHYSICS_INTERPOLATION_WARNING(m_string) \
    _physics_interpolation_warning(FUNCTION_STR, __FILE__, __LINE__, ObjectID(UINT64_MAX), m_string)

#endif
