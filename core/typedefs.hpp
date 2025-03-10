#ifndef __TYPEDEFS_HPP__
#define __TYPEDEFS_HPP__

#include <stddef.h>

#include "../platform/android/platform_config.hpp"
#include "./error/error_list.hpp"

#include <utility>

#if defined(_WIN32)
static_assert(_MSVC_LANG >= 201703L);
#endif

#ifndef _STR
#define _STR(m_x) #m_x
#define _MKSTR(m_x) _STR(m_x)
#endif

#ifndef _ALWAYS_INLINE_
#if defined(__GNUC__)
#define _ALWAYS_INLINE_ __attribute__((always_inline)) inline

#elif defined(_MSC_VER)
#define _ALWAYS_INLINE_ __forceinline

#else
#define _ALWAYS_INLINE_ inline
#endif
#endif

#ifndef _FORCE_INLINE_
#ifdef DEV_ENABLED
#define _FORCE_INLINE_ inline
#else
#define _FORCE_INLINE_ _ALWAYS_INLINE_
#endif
#endif

#ifndef _ALLOW_DISCARD_
#define _ALLOW_DISCARD_ (void)
#endif

#ifdef _WIN32
#undef min
#undef max
#undef ERROR
#undef DELETE
#undef MessageBox
#undef Error
#undef OK
#undef CONNECT_DEFERRED
#undef MemoryBarrier
#undef MONO_FONT
#endif

#undef ABS
#undef SIGN
#undef MIN
#undef MAX
#undef CLAMP

template <typename T>
constexpr T ABS(T m_v) {
	return m_v < 0 ? -m_v : m_v;
}

template <typename T>
constexpr const T SIGN(const T m_v) {
	return m_v > 0 ? +1.0f : (m_v < 0 ? -1.0f : 0.0f);
}

template <typename T, typename T2>
constexpr auto MIN(const T m_a, const T2 m_b) {
	return m_a < m_b ? m_a : m_b;
}

template <typename T, typename T2>
constexpr auto MAX(const T m_a, const T2 m_b) {
	return m_a > m_b ? m_a : m_b;
}

template <typename T, typename T2, typename T3>
constexpr auto CLAMP(const T m_a, const T2 m_min, const T3 m_max) {
	return m_a < m_min ? m_min : (m_a > m_max ? m_max : m_a);
}

#ifndef SWAP
#define SWAP(m_x, m_y) std::swap((m_x), (m_y))
#endif

template <typename T>
inline bool is_power_of_2(const T x) {
	return x && ((x & (x - 1)) == 0);
}

static _FORCE_INLINE_ unsigned int next_power_of_2(unsigned int x) {
	if (x == 0) {
		return 0;
	}

	--x;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;

	return ++x;
}

static _FORCE_INLINE_ unsigned int previous_power_of_2(unsigned int x) {
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return x - (x >> 1);
}

static _FORCE_INLINE_ unsigned int closest_power_of_2(unsigned int x) {
	unsigned int nx = next_power_of_2(x);
	unsigned int px = previous_power_of_2(x);
	return (nx - x) > (x - px) ? px : nx;
}

static inline int get_shift_from_power_of_2(unsigned int p_bits) {
	for (unsigned int i = 0; i < 32; i++) {
		if (p_bits == (unsigned int)(1 << i)) {
			return i;
		}
	}

	return -1;
}

template <typename T>
static _FORCE_INLINE_ T nearest_power_of_2_templated(T x) {
	--x;

	size_t num = get_shift_from_power_of_2(sizeof(T)) + 3;

	for (size_t i = 0; i < num; i++) {
		x |= x >> (1 << i);
	}

	return ++x;
}

static inline unsigned int nearest_shift(unsigned int p_number) {
	for (int i = 30; i >= 0; i--) {
		if (p_number & (1 << i)) {
			return i + 1;
		}
	}

	return 0;
}

template <typename T>
constexpr T floor_log2(T x) {
	return x < 2 ? x : 1 + floor_log2(x >> 1);
}

template <typename T>
constexpr T get_num_bits(T x) {
	return floor_log2(x);
}

#if defined(__GNUC__)
#define BSWAP16(x) __builtin_bswap16(x)
#define BSWAP32(x) __builtin_bswap32(x)
#define BSWAP64(x) __builtin_bswap64(x)
#elif defined(_MSC_VER)
#define BSWAP16(x) _byteswap_ushort(x)
#define BSWAP32(x) _byteswap_ulong(x)
#define BSWAP64(x) _byteswap_uint64(x)
#else
static inline uint16_t BSWAP16(uint16_t x) {
	return (x >> 8) | (x << 8);
}

static inline uint32_t BSWAP32(uint32_t x) {
	return ((x << 24) | ((x << 8) & 0x00FF0000) | ((x >> 8) & 0x0000FF00) | (x >> 24));
}

static inline uint64_t BSWAP64(uint64_t x) {
	x = (x & 0x00000000FFFFFFFF) << 32 | (x & 0xFFFFFFFF00000000) >> 32;
	x = (x & 0x0000FFFF0000FFFF) << 16 | (x & 0xFFFF0000FFFF0000) >> 16;
	x = (x & 0x00FF00FF00FF00FF) << 8 | (x & 0xFF00FF00FF00FF00) >> 8;
	return x;
}
#endif

template <typename T>
struct Comparator {
	_ALWAYS_INLINE_ bool operator()(const T &p_a, const T &p_b) const { return (p_a < p_b); }
};

void _global_lock();
void _global_unlock();

struct _GlobalLock {
	_GlobalLock() { _global_lock(); }
	~_GlobalLock() { _global_unlock(); }
};

#define GLOBAL_LOCK_FUNCTION _GlobalLock _global_lock_;

#if defined(__GNUC__)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) x
#define unlikely(x) x
#endif

#if defined(__GNUC__)
#define _PRINTF_FORMAT_ATTRIBUTE_2_0 __attribute__((format(printf, 2, 0)))
#define _PRINTF_FORMAT_ATTRIBUTE_2_3 __attribute__((format(printf, 2, 3)))
#else
#define _PRINTF_FORMAT_ATTRIBUTE_2_0
#define _PRINTF_FORMAT_ATTRIBUTE_2_3
#endif

#define CAST_INT_TO_UCHAR_PTR(ptr) ((uint8_t *)(uintptr_t)(ptr))

template <size_t... Is>
struct IndexSequence {};

template <size_t N, size_t... Is>
struct BuildIndexSequence : BuildIndexSequence<N - 1, N - 1, Is...> {};

template <size_t... Is>
struct BuildIndexSequence<0, Is...> : IndexSequence<Is...> {};

#define MAX_RECURSION 100

#ifdef DEBUG_ENABLED
#define DEBUG_METHODS_ENABLED
#endif

#define __GDARG_PLACEHOLDER_1 false,
#define __gd_take_second_arg(__ignored, val, ...) val
#define ____gd_is_defined(arg1_or_junk) __gd_take_second_arg(arg1_or_junk true, false)
#define ___gd_is_defined(val) ____gd_is_defined(__GDARG_PLACEHOLDER_##val)
#define GD_IS_DEFINED(x) ___gd_is_defined(x)

#endif
