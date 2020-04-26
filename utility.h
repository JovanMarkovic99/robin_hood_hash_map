#pragma once
// std::...
#include <utility>

namespace jvn
{

// A function object that invokes operator== on Ty type
template <class Ty>
struct equal_to 
{
    constexpr bool operator()(const Ty& lhs, const Ty& rhs) const { return lhs == rhs; }
};

// A custom pair implementation that is is_trivially_copyable
template <class Ty1, class Ty2>
struct pair 
{
    using first_type    = Ty1;
    using second_type   = Ty2;

    template <typename U1 = Ty1, typename U2 = Ty2,
                std::enable_if_t<std::is_default_constructible<U1>::value &&
                std::is_default_constructible<U2>::value, int> = 0>
    constexpr pair() noexcept(noexcept(U1()) && noexcept(U2()))
        :first(),
        second() 
        {}

    constexpr pair(pair const& p) noexcept(
        noexcept(Ty1(std::declval<const Ty1&>())) && 
        noexcept(Ty2(std::declval<const Ty2&>())))
        :first(p.first), 
        second(p.second) 
        {}

    constexpr pair(pair&& p) noexcept(
        noexcept(Ty1(std::move(std::declval<Ty1&&>()))) &&
        noexcept(Ty2(std::move(std::declval<Ty2&&>()))))
        :first(std::move(p.first)),
        second(std::move(p.second))
        {}

    constexpr pair(const Ty1& f, const Ty2& s) noexcept(
        noexcept(Ty1(std::declval<const Ty1&>())) && 
        noexcept(Ty2(std::declval<const Ty2&>())))
        :first(f), 
        second(s) 
        {}

    constexpr pair(Ty1&& f, Ty2&& s) noexcept(
        noexcept(Ty1(std::move(std::declval<Ty1&&>()))) &&
        noexcept(Ty2(std::move(std::declval<Ty2&&>()))))
        :first(std::move(f)), 
        second(std::move(s)) 
        {}   

    template <typename U1, typename U2>
    constexpr pair(U1&& a, U2&& b) noexcept(
        noexcept(Ty1(std::forward<U1>(std::declval<U1&&>()))) &&
        noexcept(Ty2(std::forward<U2>(std::declval<U2&&>()))))
        : first(std::forward<U1>(a))
        , second(std::forward<U2>(b)) {}

    first_type first;
    second_type second;
};

template <class Ty1, class Ty2>
inline constexpr bool operator==(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <class Ty1, class Ty2>
inline constexpr bool operator!=(const pair<Ty1, Ty2>& lhs, const pair<Ty1, Ty2>& rhs) { return !(lhs == rhs); }

// TODO : Add no except boundary
template <class Ty1, class Ty2>
inline void swap(pair<Ty1, Ty2>& lhs, pair<Ty1, Ty2>& rhs) noexcept
{
    // Enable ADL
    using std::swap;
    swap(lhs.first, rhs.first);
    swap(lhs.second, rhs.second);
}

} // namespace jvn
