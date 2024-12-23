#pragma once
#include<cmath>
#include<ratio>
#include<iostream>
#include<format>

namespace rz::units{

// Concepts
template<typename Dimension>
concept IsDimension = std::is_same_v<Dimension, std::ratio<Dimension::num, Dimension::den>>;

template<typename Time, typename Length, typename Mass, typename Current>
concept IsDimensionless = std::is_same_v<Time, std::ratio<0>> &&
                          std::is_same_v<Length, std::ratio<0>> &&
                          std::is_same_v<Mass, std::ratio<0>> &&
                          std::is_same_v<Current, std::ratio<0>>;


// Quantity class
template<IsDimension Time, IsDimension Length, IsDimension Mass, IsDimension Current>
class Quantity{
public:
    constexpr Quantity() = default;

    constexpr Quantity(double value) requires IsDimensionless<Time, Length, Mass, Current> : value(value) {}
    
    explicit constexpr Quantity(double value) : value(value) {}

    constexpr Quantity& operator+=(Quantity rhs){
        value += rhs.value;
        return *this;
    }
    
    constexpr Quantity& operator-=(Quantity rhs){
        value -= rhs.value;
        return *this;
    }

    constexpr double Convert(Quantity rhs) const{
        return value / rhs.value;
    }

    constexpr operator double() const requires IsDimensionless<Time, Length, Mass, Current>{
        return value;
    }

    explicit constexpr operator double() const{
        return value;
    }

    friend auto operator<=>(Quantity lhs, Quantity rhs) = default;
    
private:
    double value;
};


// Base Dimension
template<int32_t T, int32_t L, int32_t M, int32_t C>
using IntegralQuantity = Quantity<std::ratio<T>, std::ratio<L>, std::ratio<M>, std::ratio<C>>;


// Standard mathematical operations
template<IsDimension T, IsDimension L, IsDimension M, IsDimension C>
constexpr auto operator+(Quantity<T, L, M, C> lhs, Quantity<T, L, M, C> rhs){
    return Quantity<T, L, M, C>(static_cast<double>(lhs) + static_cast<double>(rhs));
}

template<IsDimension T, IsDimension L, IsDimension M, IsDimension C>
constexpr auto operator-(Quantity<T, L, M, C> quantity){
    return Quantity<T, L, M, C>(-static_cast<double>(quantity));
}

template<IsDimension T, IsDimension L, IsDimension M, IsDimension C>
constexpr auto operator-(Quantity<T, L, M, C> lhs, Quantity<T, L, M, C> rhs){
    return Quantity<T, L, M, C>(static_cast<double>(lhs) - static_cast<double>(rhs));
}

template<IsDimension T1, IsDimension L1, IsDimension M1, IsDimension C1, 
         IsDimension T2, IsDimension L2, IsDimension M2, IsDimension C2>
constexpr auto operator*(Quantity<T1, L1, M1, C1> lhs, Quantity<T2, L2, M2, C2> rhs){
    using NewT = std::ratio_add<T1, T2>;
    using NewL = std::ratio_add<L1, L2>;
    using NewM = std::ratio_add<M1, M2>;
    using NewC = std::ratio_add<C1, C2>;

    return Quantity<NewT, NewL, NewM, NewC>(static_cast<double>(lhs) * static_cast<double>(rhs));
}

template<IsDimension T, IsDimension L, IsDimension M, IsDimension C>
constexpr auto operator*(double lhs, Quantity<T, L, M, C> rhs){
    return Quantity<T, L, M, C>(lhs * static_cast<double>(rhs));
}

template<IsDimension T, IsDimension L, IsDimension M, IsDimension C>
constexpr auto operator*(Quantity<T, L, M, C> lhs, double rhs){
    return Quantity<T, L, M, C>(static_cast<double>(lhs) * rhs);
}

template<IsDimension T1, IsDimension L1, IsDimension M1, IsDimension C1, 
         IsDimension T2, IsDimension L2, IsDimension M2, IsDimension C2>
constexpr auto operator/(Quantity<T1, L1, M1, C1> lhs, Quantity<T2, L2, M2, C2> rhs){
    using NewT = std::ratio_subtract<T1, T2>;
    using NewL = std::ratio_subtract<L1, L2>;
    using NewM = std::ratio_subtract<M1, M2>;
    using NewC = std::ratio_subtract<C1, C2>;

    return Quantity<NewT, NewL, NewM, NewC>(static_cast<double>(lhs) / static_cast<double>(rhs));
}

template<IsDimension T, IsDimension L, IsDimension M, IsDimension C>
constexpr auto operator/(double lhs, Quantity<T, L, M, C> rhs){
    using NewT = std::ratio_subtract<std::ratio<0>, T>;
    using NewL = std::ratio_subtract<std::ratio<0>, L>;
    using NewM = std::ratio_subtract<std::ratio<0>, M>;
    using NewC = std::ratio_subtract<std::ratio<0>, C>;

    return Quantity<NewT, NewL, NewM, NewC>(lhs / static_cast<double>(rhs));
}

template<IsDimension T, IsDimension L, IsDimension M, IsDimension C>
constexpr auto operator/(Quantity<T, L, M, C> lhs, double rhs){
    return Quantity<T, L, M, C>(static_cast<double>(lhs) / rhs);
}

template<IsDimension Dimension>
static std::string format_dimension(const std::string& symbol){
    if(Dimension::num == 0){
        return "";
    }

    static std::string buffer;
    buffer.clear();
    
    buffer += symbol;

    if(Dimension::num == 1 && Dimension::den == 1){
        return buffer;
    }

    buffer += "^";
    buffer += std::to_string(Dimension::num);

    if(Dimension::den != 1){
        buffer += "/";
        buffer += std::to_string(Dimension::den);
    }

    return buffer;
}

template<IsDimension T, IsDimension L, IsDimension M, IsDimension C>
decltype(auto) operator<<(std::ostream& os, Quantity<T, L, M, C> quantity){
    os << static_cast<double>(quantity);
    os << format_dimension<T>("s");
    os << format_dimension<L>("m");
    os << format_dimension<M>("kg");
    os << format_dimension<C>("A");

    return os;
}


// cmath functions
template<IsDimension T, IsDimension L, IsDimension M, IsDimension C>
constexpr auto abs(Quantity<T, L, M, C> num){
    return Quantity<T, L, M, C>(std::abs(static_cast<double>(num)));
}

template<IsDimension T, IsDimension L, IsDimension M, IsDimension C>
constexpr auto lerp(Quantity<T, L, M, C> a, Quantity<T, L, M, C> b, double t){
    return Quantity<T, L, M, C>(std::lerp(static_cast<double>(a), static_cast<double>(b), t));
}


template<IsDimension T, IsDimension L, IsDimension M, IsDimension C, int32_t power>
constexpr auto pow(Quantity<T, L, M, C> base){
    using NewT = std::ratio_multiply<T, std::ratio<power>>;
    using NewL = std::ratio_multiply<L, std::ratio<power>>;
    using NewM = std::ratio_multiply<M, std::ratio<power>>;
    using NewC = std::ratio_multiply<C, std::ratio<power>>;

    return Quantity<NewT, NewL, NewM, NewC>(std::pow(static_cast<double>(base), power));
}

template<IsDimension T, IsDimension L, IsDimension M, IsDimension C, int32_t Num, int32_t Den>
constexpr auto pow(Quantity<T, L, M, C> base, std::ratio<Num, Den> power){
    using NewT = std::ratio_multiply<T, std::ratio<Num, Den>>;
    using NewL = std::ratio_multiply<L, std::ratio<Num, Den>>;
    using NewM = std::ratio_multiply<M, std::ratio<Num, Den>>;
    using NewC = std::ratio_multiply<C, std::ratio<Num, Den>>;

    const double exp = static_cast<double>(Num) / static_cast<double>(Den);
    return Quantity<NewT, NewL, NewM, NewC>(std::pow(static_cast<double>(base), exp));
}

template<IsDimension T, IsDimension L, IsDimension M, IsDimension C>
constexpr auto sqrt(Quantity<T, L, M, C> num){
    using NewT = std::ratio_divide<T, std::ratio<2>>;
    using NewL = std::ratio_divide<L, std::ratio<2>>;
    using NewM = std::ratio_divide<M, std::ratio<2>>;
    using NewC = std::ratio_divide<C, std::ratio<2>>;

    return Quantity<NewT, NewL, NewM, NewC>(std::sqrt(static_cast<double>(num)));
}

template<IsDimension T, IsDimension L, IsDimension M, IsDimension C>
constexpr auto cbrt(Quantity<T, L, M, C> num){
    using NewT = std::ratio_divide<T, std::ratio<3>>;
    using NewL = std::ratio_divide<L, std::ratio<3>>;
    using NewM = std::ratio_divide<M, std::ratio<3>>;
    using NewC = std::ratio_divide<C, std::ratio<3>>;

    return Quantity<NewT, NewL, NewM, NewC>(std::cbrt(static_cast<double>(num)));
}

template<IsDimension T, IsDimension L, IsDimension M, IsDimension C>
constexpr auto hypot(Quantity<T, L, M, C> x, Quantity<T, L, M, C> y){
    return Quantity<T, L, M, C>(std::hypot(static_cast<double>(x), static_cast<double>(y)));
}

} // namespace rz::units


// stl specializations
namespace std{

template<rz::units::IsDimension T, rz::units::IsDimension L, rz::units::IsDimension M, rz::units::IsDimension C> 
struct hash<rz::units::Quantity<T, L, M, C>>{
    size_t operator()(rz::units::Quantity<T, L, M, C> x) const{
        return hash<double>()(static_cast<double>(x));
    }
};

template<rz::units::IsDimension T, rz::units::IsDimension L, rz::units::IsDimension M, rz::units::IsDimension C>
struct numeric_limits<rz::units::Quantity<T, L, M, C>>{
private:
    using Quantity = rz::units::Quantity<T, L, M, C>;
    using Limits = std::numeric_limits<double>;

public:
    static constexpr bool is_specialized = true;

    static constexpr Quantity min() noexcept
    {
        return Quantity(Limits::min());
    }

    static constexpr Quantity max() noexcept
    {
        return Quantity(Limits::max());
    }

    static constexpr Quantity lowest() noexcept
    {
        return Quantity(Limits::lowest());
    }

    static constexpr int digits = Limits::digits;
    static constexpr int digits10 = Limits::digits10;
    static constexpr int max_digits10 = Limits::max_digits10;
    static constexpr bool is_signed = Limits::is_signed;
    static constexpr bool is_integer = false;
    static constexpr bool is_exact = false;
    static constexpr int radix = Limits::radix;
    static constexpr Quantity epsilon() noexcept { return Quantity(Limits::epsilon()); }
    static constexpr Quantity round_error() noexcept { return Quantity(Limits::round_error()); }
    static constexpr int min_exponent = Limits::min_exponent;
    static constexpr int min_exponent10 = Limits::min_exponent10;
    static constexpr int max_exponent = Limits::max_exponent;
    static constexpr int max_exponent10 = Limits::max_exponent10;
    static constexpr bool has_infinity = Limits::has_infinity;
    static constexpr bool has_quiet_NaN = Limits::has_quiet_NaN;
    static constexpr bool has_signaling_NaN = Limits::has_signaling_NaN;
    static constexpr float_denorm_style has_denorm = Limits::has_denorm;
    static constexpr bool has_denorm_loss = Limits::has_denorm_loss;
    static constexpr Quantity infinity() noexcept { return Quantity(Limits::infinity()); }
    static constexpr Quantity quiet_NaN() noexcept { return Quantity(Limits::quiet_NaN()); }
    static constexpr Quantity signaling_NaN() noexcept { return Quantity(Limits::signaling_NaN()); }
    static constexpr Quantity denorm_min() noexcept { return Quantity(Limits::denorm_min()); }
    static constexpr bool is_iec559 = Limits::is_iec559;
    static constexpr bool is_bounded = Limits::is_bounded;
    static constexpr bool is_modulo = false;
    static constexpr bool traps = Limits::traps;
    static constexpr bool tinyness_before = Limits::tinyness_before;
};

template<rz::units::IsDimension T, rz::units::IsDimension L, rz::units::IsDimension M, rz::units::IsDimension C>
struct formatter<rz::units::Quantity<T, L, M, C>>{
    constexpr auto parse(format_parse_context& ctx){
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(rz::units::Quantity<T, L, M, C> quantity, FormatContext& ctx) const{
        auto&& out = ctx.out();
        out = std::format_to(out, "{}", static_cast<double>(quantity));
        out = std::format_to(out, "{}", rz::units::format_dimension<T>("s"));
        out = std::format_to(out, "{}", rz::units::format_dimension<L>("m"));
        out = std::format_to(out, "{}", rz::units::format_dimension<M>("kg"));
        out = std::format_to(out, "{}", rz::units::format_dimension<C>("A"));

        return out;
    }
};

} // namespace std