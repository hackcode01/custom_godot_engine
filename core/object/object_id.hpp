#ifndef __OBJECT_ID_HPP__
#define __OBJECT_ID_HPP__

#pragma warning(disable : 4464)

#include "../typedefs.hpp"

class ObjectID {

public:
    _ALWAYS_INLINE_ bool is_ref_counted() const { return (m_id & (uint64_t(1) << 63)) != 0; }
    _ALWAYS_INLINE_ bool is_valid() const { return m_id != 0; }
    _ALWAYS_INLINE_ bool is_null() const { return m_id == 0; }
    _ALWAYS_INLINE_ operator uint64_t() const { return m_id; }
    _ALWAYS_INLINE_ operator int64_t() const { return (int64_t)m_id; }

    _ALWAYS_INLINE_ bool operator==(const ObjectID &p_id) const { return m_id == p_id.m_id; }
    _ALWAYS_INLINE_ bool operator!=(const ObjectID &p_id) const { return m_id != p_id.m_id; }
    _ALWAYS_INLINE_ bool operator<(const ObjectID &p_id) const { return m_id < p_id.m_id; }

    _ALWAYS_INLINE_ void operator=(int64_t p_int64) { m_id = (uint64_t)p_int64; }
    _ALWAYS_INLINE_ void operator=(uint64_t p_uint64) { m_id = p_uint64; }

    _ALWAYS_INLINE_ ObjectID() {}
    _ALWAYS_INLINE_ explicit ObjectID(const uint64_t p_id) { m_id = p_id; }
    _ALWAYS_INLINE_ explicit ObjectID(const int64_t p_id) { m_id = (uint64_t)p_id; } 

private:
    uint64_t m_id{0};
};

#endif
