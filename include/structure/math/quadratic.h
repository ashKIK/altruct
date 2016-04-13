#pragma once

#include "algorithm/math/base.h"

namespace altruct {
namespace math {

template<typename T, int ID, bool STATIC = true>
struct quadratic_members;

template<typename T, int ID>
struct quadratic_members<T, ID, false> {
	T D;
	quadratic_members(const T& D = 0) : D(D) {}
};

template<typename T, int ID>
struct quadratic_members<T, ID, true> {
	static T D;
	quadratic_members(const T& D = 0) {}
};

/**
 * a + b * sqrt(D)
 *
 * quadratic<int, -1> - gaussian integers
 * quadratic<double, -1> - complex numbers
 *
 * @param T - the underlying type
 * @param ID - ID of the quadratic type (useful when STATIC = true)
 * @param STATIC - whether D is a static or instance member
 */
template<typename T, int ID, bool STATIC = true>
class quadratic : public quadratic_members<T, ID, STATIC> {
public:
	T a, b;

	// construct from int, but only if T is not integral to avoid constructor clashing
	template <typename = std::enable_if_t<!std::is_integral<T>::value>>
	quadratic(int a) : a(a), b(0), quadratic_members(0) {}
	quadratic(const T& a = 0, const T& b = 0, const T& D = 0) : a(a), b(b), quadratic_members(D) {}
	quadratic(const quadratic& rhs) : a(rhs.a), b(rhs.b), quadratic_members(rhs.D) {}
	quadratic make(const T& a, const T& b) const { return quadratic(a, b, D); }

	bool operator == (const quadratic& rhs) const { return (a == rhs.a && b == rhs.b); }
	bool operator != (const quadratic& rhs) const { return (a != rhs.a || b != rhs.b); }
	bool operator <  (const quadratic& rhs) const { return (a < rhs.a || (a == rhs.a && b < rhs.b)); }
	bool operator >  (const quadratic& rhs) const { return (a > rhs.a || (a == rhs.a && b > rhs.b)); }
	bool operator <= (const quadratic& rhs) const { return (a < rhs.a || (a == rhs.a && b <= rhs.b)); }
	bool operator >= (const quadratic& rhs) const { return (a > rhs.a || (a == rhs.a && b >= rhs.b)); }
	
	quadratic  operator +  (const quadratic& rhs) const { quadratic t(*this); t += rhs; return t; }
	quadratic  operator -  (const quadratic& rhs) const { quadratic t(*this); t -= rhs; return t; }
	quadratic  operator -  ()                     const { quadratic t(-a, -b);          return t; }
	quadratic  operator *  (const quadratic& rhs) const { quadratic t(*this); t *= rhs; return t; }
	quadratic  operator /  (const quadratic& rhs) const { quadratic t(*this); t /= rhs; return t; }
	quadratic  operator %  (const quadratic& rhs) const { quadratic t(*this); t %= rhs; return t; }

	quadratic  operator *  (const T& rhs) const { quadratic t(*this); t *= rhs; return t; }
	quadratic  operator /  (const T& rhs) const { quadratic t(*this); t /= rhs; return t; }
	
	quadratic& operator += (const quadratic& rhs) { a += rhs.a; b += rhs.b; return *this; }
	quadratic& operator -= (const quadratic& rhs) { a -= rhs.a; b -= rhs.b; return *this; }
	quadratic& operator *= (const quadratic& rhs) { return *this = make(a * rhs.a + b * rhs.b * D, a * rhs.b + b * rhs.a); }
	quadratic& operator /= (const quadratic& rhs) { return *this = *this * rhs.conjugate() / rhs.norm(); }
	quadratic& operator %= (const quadratic& rhs) { return *this = *this - rhs * (*this / rhs); }
	
	quadratic& operator *= (const T& rhs) { a *= rhs; b *= rhs; return *this; }
	quadratic& operator /= (const T& rhs) { a /= rhs; b /= rhs; return *this; }

	quadratic conjugate() const { return make(a, -b); }
	T norm() const { return a * a - b * b * D; }
};

template<typename T>
using quadraticX = quadratic<T, 0, false>;

template<typename T, int ID>
T quadratic_members<T, ID, true>::D = (T)ID;

template<typename T, int ID, bool STATIC>
struct identityT<quadratic<T, ID, STATIC>> {
	static quadratic<T, ID, STATIC> of(const quadratic<T, ID, STATIC>& x) {
		return quadratic<T, ID, STATIC>(identityT<T>::of(x.a), zeroT<T>::of(x.b), x.D);
	}
};

template<typename T, int ID, bool STATIC>
struct zeroT<quadratic<T, ID, STATIC>> {
	static quadratic<T, ID, STATIC> of(const quadratic<T, ID, STATIC>& x) {
		return quadratic<T, ID, STATIC>(zeroT<T>::of(x.a), zeroT<T>::of(x.b), x.D);
	}
};

} // math
} // altruct