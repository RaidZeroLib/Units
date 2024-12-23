#pragma once
#include "Quantity.hpp"

namespace rz::units{

// Unit Definition
using Number = IntegralQuantity<0, 0, 0, 0>;
using Scalar = Number;


// Constants
constexpr Number pi = 3.14159265358979323846;
constexpr Number tau = 2.0 * pi;


// Specialized Functions
template<IsDimension T, IsDimension L, IsDimension M, IsDimension C>
constexpr auto lerp(Quantity<T, L, M, C> a, Quantity<T, L, M, C> b, Number t){
    return lerp(a, b, static_cast<double>(t));
}


// Literals
namespace literals{

constexpr auto operator""_pi(long double x){
    return Number(x * 3.14159265358979323846);
}

constexpr auto operator""_pi(unsigned long long x){
    return Number(x * 3.14159265358979323846);
}

constexpr auto operator""_tau(long double x){
    return Number(x * 2 * 3.14159265358979323846);
}

constexpr auto operator""_tau(unsigned long long x){
    return Number(x * 2 * 3.14159265358979323846);
}

} // namespace literals

} // namespace rz::units