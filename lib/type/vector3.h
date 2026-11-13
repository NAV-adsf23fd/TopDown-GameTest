#pragma once
#include <cstdint>
#include <type_traits>

// Auto up-type, prevent type-overflow
template <typename T> struct NextWider {
  using type = T; // default
};

template <> struct NextWider<std::int8_t> {
  using type = std::int16_t;
};
template <> struct NextWider<std::int16_t> {
  using type = std::int32_t;
};
template <> struct NextWider<std::int32_t> {
  using type = std::int64_t;
};
template <> struct NextWider<std::uint8_t> {
  using type = std::uint16_t;
};
template <> struct NextWider<std::uint16_t> {
  using type = std::uint32_t;
};
template <> struct NextWider<std::uint32_t> {
  using type = std::uint64_t;
};

template <typename T> using NextWider_t = typename NextWider<T>::type;

template <typename T>
inline constexpr bool is_valid_vec_scalar_v =
    std::is_arithmetic_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool>;

template <typename T, typename U>
using BestType_t =
    std::conditional_t<(std::is_integral_v<T> && std::is_integral_v<U> &&
                        std::is_signed_v<T> == std::is_signed_v<U>),
                       std::conditional_t<(sizeof(T) >= sizeof(U)), T, U>,
                       std::common_type_t<T, U>>;
// end

template <typename T> struct Vec3 {
  static_assert(is_valid_vec_scalar_v<T>, "Vec3 Only allows numeric types");
  T x, y, z;

  using Param = std::conditional_t<(sizeof(T) * 3 <= 16), Vec3, const Vec3 &>;

  template <typename OtherT> constexpr explicit operator Vec3<OtherT>() const {
    return {static_cast<OtherT>(x), static_cast<OtherT>(y),
            static_cast<OtherT>(z)};
  }

  constexpr bool operator==(const Vec3 &) const = default;

  constexpr Vec3 &operator+=(Param other) {
    x = static_cast<T>(x + other.x);
    y = static_cast<T>(y + other.y);
    z = static_cast<T>(z + other.z);
    return *this;
  }

  constexpr Vec3 &operator-=(Param other) {
    x = static_cast<T>(x - other.x);
    y = static_cast<T>(y - other.y);
    z = static_cast<T>(z - other.z);
    return *this;
  }

  constexpr Vec3 &operator*=(T scalar) {
    x = static_cast<T>(x * scalar);
    y = static_cast<T>(y * scalar);
    z = static_cast<T>(z * scalar);
    return *this;
  }

  constexpr Vec3 &operator%=(Param b) {
    *this = *this % b;
    return *this;
  }

  constexpr Vec3 operator-() const {
    return {static_cast<T>(-x), static_cast<T>(-y), static_cast<T>(-z)};
  }

  // type promoting
  constexpr auto operator+() const {
    using WiderT = NextWider_t<T>;
    return Vec3<WiderT>{static_cast<WiderT>(x), static_cast<WiderT>(y),
                        static_cast<WiderT>(z)};
  }

  // using friend like this make operator overload behave as if writen
  // outsize struct scope. I use Param for b but not a because I want "a + b
  // + c" to work properly (As "a + b + c = (a+b) + c", (a+b) is rvalue, so
  // we cant use & for ref)
  friend constexpr Vec3 operator+(Vec3 a, Param b) {
    a += b;
    return a;
  }

  friend constexpr Vec3 operator-(Vec3 a, Param b) {
    a -= b;
    return a;
  }

  friend constexpr Vec3 operator*(Vec3 a, T scalar) {
    a *= scalar;
    return a;
  }

  friend constexpr Vec3 operator*(T scalar, Vec3 a) { return a * scalar; }

  // dot product
  friend constexpr T operator*(Param a, Param b) {
    return static_cast<T>(a.x * b.x + a.y * b.y + a.z * b.z);
  }

  // cross product
  friend constexpr Vec3 operator%(Param a, Param b) {
    return Vec3{static_cast<T>(a.y * b.z - b.y * a.z),
                static_cast<T>(a.z * b.x - b.z * a.x),
                static_cast<T>(a.x * b.y - b.x * a.y)};
  }
};

// multi-type (Chỉ cho phép nhân chéo kiểu scalar khi U là số thực để tránh v8 *
// 2 bị phình lên 32-bit)
template <typename T, typename U,
          typename = std::enable_if_t<std::is_floating_point_v<U> &&
                                      !std::is_same_v<T, U>>>
constexpr auto operator*(const Vec3<T> &a, U scalar) {
  using ResT = BestType_t<T, U>;
  return Vec3<ResT>{
      static_cast<ResT>(static_cast<ResT>(a.x) * static_cast<ResT>(scalar)),
      static_cast<ResT>(static_cast<ResT>(a.y) * static_cast<ResT>(scalar)),
      static_cast<ResT>(static_cast<ResT>(a.z) * static_cast<ResT>(scalar))};
}

template <typename T, typename U,
          typename = std::enable_if_t<std::is_floating_point_v<U> &&
                                      !std::is_same_v<T, U>>>
constexpr auto operator*(U scalar, const Vec3<T> &a) {
  return a * scalar;
}

template <typename T, typename U,
          typename = std::enable_if_t<!std::is_same_v<T, U>>>
constexpr auto operator+(const Vec3<T> &a, const Vec3<U> &b) {
  using CommonT = BestType_t<T, U>;
  return Vec3<CommonT>{static_cast<CommonT>(static_cast<CommonT>(a.x) +
                                            static_cast<CommonT>(b.x)),
                       static_cast<CommonT>(static_cast<CommonT>(a.y) +
                                            static_cast<CommonT>(b.y)),
                       static_cast<CommonT>(static_cast<CommonT>(a.z) +
                                            static_cast<CommonT>(b.z))};
}

template <typename T, typename U,
          typename = std::enable_if_t<!std::is_same_v<T, U>>>
constexpr auto operator-(const Vec3<T> &a, const Vec3<U> &b) {
  using CommonT = BestType_t<T, U>;
  return Vec3<CommonT>{static_cast<CommonT>(static_cast<CommonT>(a.x) -
                                            static_cast<CommonT>(b.x)),
                       static_cast<CommonT>(static_cast<CommonT>(a.y) -
                                            static_cast<CommonT>(b.y)),
                       static_cast<CommonT>(static_cast<CommonT>(a.z) -
                                            static_cast<CommonT>(b.z))};
}

template <typename T, typename U,
          typename = std::enable_if_t<!std::is_same_v<T, U>>>
constexpr auto operator*(const Vec3<T> &a, const Vec3<U> &b) {
  using CommonT = BestType_t<T, U>;
  return static_cast<CommonT>(
      static_cast<CommonT>(a.x) * static_cast<CommonT>(b.x) +
      static_cast<CommonT>(a.y) * static_cast<CommonT>(b.y) +
      static_cast<CommonT>(a.z) * static_cast<CommonT>(b.z));
}

template <typename T, typename U,
          typename = std::enable_if_t<!std::is_same_v<T, U>>>
constexpr auto operator%(const Vec3<T> &a, const Vec3<U> &b) {
  using CommonT = BestType_t<T, U>;
  return Vec3<CommonT>{
      static_cast<CommonT>(
          static_cast<CommonT>(a.y) * static_cast<CommonT>(b.z) -
          static_cast<CommonT>(b.y) * static_cast<CommonT>(a.z)),
      static_cast<CommonT>(
          static_cast<CommonT>(a.z) * static_cast<CommonT>(b.x) -
          static_cast<CommonT>(b.z) * static_cast<CommonT>(a.x)),
      static_cast<CommonT>(
          static_cast<CommonT>(a.x) * static_cast<CommonT>(b.y) -
          static_cast<CommonT>(b.x) * static_cast<CommonT>(a.y))};
}

using Vector3i8 = Vec3<std::int8_t>;
using Vector3ui8 = Vec3<std::uint8_t>;
using Vector3i16 = Vec3<std::int16_t>;
using Vector3i32 = Vec3<std::int32_t>;
using Vector3i64 = Vec3<std::int64_t>;
using Vector3f = Vec3<float>;
using Vector3d = Vec3<double>;
