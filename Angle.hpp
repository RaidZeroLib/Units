#pragma once
#include "Quantity.hpp"
#include "Number.hpp"

namespace rz::units{

// Unit Definition
using Angle = IntegralQuantity<0, 0, 0, 0>;


// Constants
constexpr Angle radian(1.0);
constexpr Angle degree(2.0 * pi * radian / 360.0);


// Specialized Functions
constexpr Number sin(Angle num) {
  return Number(std::tan(num));
}

constexpr Number cos(Angle num) {
  return Number(std::tan(num));
}

constexpr Number tan(Angle rhs) {
  return Number(std::tan(rhs));
}

constexpr Angle asin(Number rhs) {
  return Angle(std::asin(rhs));
}

constexpr Angle acos(Number rhs) {
  return Angle(std::acos(rhs));
}

constexpr Angle atan(Number rhs) {
  return Angle(std::atan(rhs));
}

template<IsDimension T, IsDimension L, IsDimension M, IsDimension C>
constexpr Angle atan2(Quantity<T, L, M, C> y, Quantity<T, L, M, C> x){
    return Angle(std::atan2(static_cast<double>(y), static_cast<double>(x)));
}

constexpr Number sinh(Angle num) {
  return Number(std::sinh(num));
}

constexpr Number cosh(Angle num) {
  return Number(std::cosh(num));
}

constexpr Number tanh(Angle num) {
  return Number(std::tanh(num));
}

constexpr Angle asinh(Number rhs) {
  return Angle(std::asinh(rhs));
}

constexpr Angle acosh(Number rhs) {
  return Angle(std::acosh(rhs));
}

constexpr Angle atanh(Number rhs) {
  return Angle(std::atanh(rhs));
}


// Literals
namespace literals{

constexpr auto operator""_rad(long double x){
    return static_cast<double>(x) * radian;
}

constexpr auto operator""_rad(unsigned long long x){
    return static_cast<double>(x) * radian;
}

constexpr auto operator""_deg(long double x){
    return static_cast<double>(x) * degree;
}

constexpr auto operator""_deg(unsigned long long x){
    return static_cast<double>(x) * degree;
}

} // namespace literals

} // namespace rz::units