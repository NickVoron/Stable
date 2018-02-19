// Copyright (C) 2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//



#ifndef NLOHMANN_JSON_HPP
#define NLOHMANN_JSON_HPP

#include <algorithm> 
#include <array> 
#include <cassert> 
#include <cctype> 
#include <ciso646> 
#include <cmath> 
#include <cstddef> 
#include <cstdint> 
#include <cstdlib> 
#include <cstring> 
#include <forward_list> 
#include <functional> 
#include <initializer_list> 
#include <iomanip> 
#include <iostream> 
#include <iterator> 
#include <limits> 
#include <locale> 
#include <map> 
#include <memory> 
#include <numeric> 
#include <sstream> 
#include <stdexcept> 
#include <string> 
#include <type_traits> 
#include <utility> 
#include <vector> 


#if defined(__clang__)
    #if (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__) < 30400
        #error "unsupported Clang version - see https:
    #endif
#elif defined(__GNUC__)
    #if (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__) < 40900
        #error "unsupported GCC version - see https:
    #endif
#endif


#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wfloat-equal"
#endif


#if defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdocumentation"
#endif


#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
    #define JSON_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
    #define JSON_DEPRECATED __declspec(deprecated)
#else
    #define JSON_DEPRECATED
#endif


#if not defined(JSON_NOEXCEPTION) || defined(__EXCEPTIONS)
    #define JSON_THROW(exception) throw exception
    #define JSON_TRY try
    #define JSON_CATCH(exception) catch(exception)
#else
    #define JSON_THROW(exception) std::abort()
    #define JSON_TRY if(true)
    #define JSON_CATCH(exception) if(false)
#endif


namespace nlohmann
{


namespace detail
{





enum class value_t : uint8_t
{
    null,            
    object,          
    array,           
    string,          
    boolean,         
    number_integer,  
    number_unsigned, 
    number_float,    
    discarded        
};


inline bool operator<(const value_t lhs, const value_t rhs) noexcept
{
    static constexpr std::array<uint8_t, 8> order = {{
            0, 
            3, 
            4, 
            5, 
            1, 
            2, 
            2, 
            2, 
        }
    };

    
    if (lhs == value_t::discarded or rhs == value_t::discarded)
    {
        return false;
    }

    return order[static_cast<std::size_t>(lhs)] <
           order[static_cast<std::size_t>(rhs)];
}







template<bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template<typename T>
using uncvref_t = typename std::remove_cv<typename std::remove_reference<T>::type>::type;


template<typename T>
using is_unscoped_enum =
    std::integral_constant<bool, std::is_convertible<T, int>::value and
    std::is_enum<T>::value>;


template<class...> struct conjunction : std::true_type {};
template<class B1> struct conjunction<B1> : B1 {};
template<class B1, class... Bn>
struct conjunction<B1, Bn...> : std::conditional<bool(B1::value), conjunction<Bn...>, B1>::type {};

template<class B> struct negation : std::integral_constant < bool, !B::value > {};


template<unsigned N> struct priority_tag : priority_tag < N - 1 > {};
template<> struct priority_tag<0> {};






template<value_t> struct external_constructor;

template<>
struct external_constructor<value_t::boolean>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, typename BasicJsonType::boolean_t b) noexcept
    {
        j.m_type = value_t::boolean;
        j.m_value = b;
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::string>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, const typename BasicJsonType::string_t& s)
    {
        j.m_type = value_t::string;
        j.m_value = s;
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::number_float>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, typename BasicJsonType::number_float_t val) noexcept
    {
        
        if (not std::isfinite(val))
        {
            j = BasicJsonType{};
        }
        else
        {
            j.m_type = value_t::number_float;
            j.m_value = val;
        }
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::number_unsigned>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, typename BasicJsonType::number_unsigned_t val) noexcept
    {
        j.m_type = value_t::number_unsigned;
        j.m_value = val;
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::number_integer>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, typename BasicJsonType::number_integer_t val) noexcept
    {
        j.m_type = value_t::number_integer;
        j.m_value = val;
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::array>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, const typename BasicJsonType::array_t& arr)
    {
        j.m_type = value_t::array;
        j.m_value = arr;
        j.assert_invariant();
    }

    template<typename BasicJsonType, typename CompatibleArrayType,
             enable_if_t<not std::is_same<CompatibleArrayType,
                                          typename BasicJsonType::array_t>::value,
                         int> = 0>
    static void construct(BasicJsonType& j, const CompatibleArrayType& arr)
    {
        using std::begin;
        using std::end;
        j.m_type = value_t::array;
        j.m_value.array = j.template create<typename BasicJsonType::array_t>(begin(arr), end(arr));
        j.assert_invariant();
    }
};

template<>
struct external_constructor<value_t::object>
{
    template<typename BasicJsonType>
    static void construct(BasicJsonType& j, const typename BasicJsonType::object_t& obj)
    {
        j.m_type = value_t::object;
        j.m_value = obj;
        j.assert_invariant();
    }

    template<typename BasicJsonType, typename CompatibleObjectType,
             enable_if_t<not std::is_same<CompatibleObjectType,
                                          typename BasicJsonType::object_t>::value,
                         int> = 0>
    static void construct(BasicJsonType& j, const CompatibleObjectType& obj)
    {
        using std::begin;
        using std::end;

        j.m_type = value_t::object;
        j.m_value.object = j.template create<typename BasicJsonType::object_t>(begin(obj), end(obj));
        j.assert_invariant();
    }
};







#define NLOHMANN_JSON_HAS_HELPER(type)                                        \
    template<typename T> struct has_##type {                                  \
    private:                                                                  \
        template<typename U, typename = typename U::type>                     \
        static int detect(U &&);                                              \
        static void detect(...);                                              \
    public:                                                                   \
        static constexpr bool value =                                         \
                std::is_integral<decltype(detect(std::declval<T>()))>::value; \
    }

NLOHMANN_JSON_HAS_HELPER(mapped_type);
NLOHMANN_JSON_HAS_HELPER(key_type);
NLOHMANN_JSON_HAS_HELPER(value_type);
NLOHMANN_JSON_HAS_HELPER(iterator);

#undef NLOHMANN_JSON_HAS_HELPER


template<bool B, class RealType, class CompatibleObjectType>
struct is_compatible_object_type_impl : std::false_type {};

template<class RealType, class CompatibleObjectType>
struct is_compatible_object_type_impl<true, RealType, CompatibleObjectType>
{
    static constexpr auto value =
        std::is_constructible<typename RealType::key_type,
        typename CompatibleObjectType::key_type>::value and
        std::is_constructible<typename RealType::mapped_type,
        typename CompatibleObjectType::mapped_type>::value;
};

template<class BasicJsonType, class CompatibleObjectType>
struct is_compatible_object_type
{
    static auto constexpr value = is_compatible_object_type_impl <
                                  conjunction<negation<std::is_same<void, CompatibleObjectType>>,
                                  has_mapped_type<CompatibleObjectType>,
                                  has_key_type<CompatibleObjectType>>::value,
                                  typename BasicJsonType::object_t, CompatibleObjectType >::value;
};

template<typename BasicJsonType, typename T>
struct is_basic_json_nested_type
{
    static auto constexpr value = std::is_same<T, typename BasicJsonType::iterator>::value or
                                  std::is_same<T, typename BasicJsonType::const_iterator>::value or
                                  std::is_same<T, typename BasicJsonType::reverse_iterator>::value or
                                  std::is_same<T, typename BasicJsonType::const_reverse_iterator>::value or
                                  std::is_same<T, typename BasicJsonType::json_pointer>::value;
};

template<class BasicJsonType, class CompatibleArrayType>
struct is_compatible_array_type
{
    static auto constexpr value =
        conjunction<negation<std::is_same<void, CompatibleArrayType>>,
        negation<is_compatible_object_type<
        BasicJsonType, CompatibleArrayType>>,
        negation<std::is_constructible<typename BasicJsonType::string_t,
        CompatibleArrayType>>,
        negation<is_basic_json_nested_type<BasicJsonType, CompatibleArrayType>>,
        has_value_type<CompatibleArrayType>,
        has_iterator<CompatibleArrayType>>::value;
};

template<bool, typename, typename>
struct is_compatible_integer_type_impl : std::false_type {};

template<typename RealIntegerType, typename CompatibleNumberIntegerType>
struct is_compatible_integer_type_impl<true, RealIntegerType, CompatibleNumberIntegerType>
{
    
    using RealLimits = std::numeric_limits<RealIntegerType>;
    using CompatibleLimits = std::numeric_limits<CompatibleNumberIntegerType>;

    static constexpr auto value =
        std::is_constructible<RealIntegerType,
        CompatibleNumberIntegerType>::value and
        CompatibleLimits::is_integer and
        RealLimits::is_signed == CompatibleLimits::is_signed;
};

template<typename RealIntegerType, typename CompatibleNumberIntegerType>
struct is_compatible_integer_type
{
    static constexpr auto value =
        is_compatible_integer_type_impl <
        std::is_integral<CompatibleNumberIntegerType>::value and
        not std::is_same<bool, CompatibleNumberIntegerType>::value,
        RealIntegerType, CompatibleNumberIntegerType > ::value;
};



template<typename BasicJsonType, typename T>
struct has_from_json
{
  private:
    
    template<typename U, typename = enable_if_t<std::is_same<void, decltype(uncvref_t<U>::from_json(
                 std::declval<BasicJsonType>(), std::declval<T&>()))>::value>>
    static int detect(U&&);
    static void detect(...);

  public:
    static constexpr bool value = std::is_integral<decltype(
                                      detect(std::declval<typename BasicJsonType::template json_serializer<T, void>>()))>::value;
};



template<typename BasicJsonType, typename T>
struct has_non_default_from_json
{
  private:
    template <
        typename U,
        typename = enable_if_t<std::is_same<
                                   T, decltype(uncvref_t<U>::from_json(std::declval<BasicJsonType>()))>::value >>
    static int detect(U&&);
    static void detect(...);

  public:
    static constexpr bool value = std::is_integral<decltype(detect(
                                      std::declval<typename BasicJsonType::template json_serializer<T, void>>()))>::value;
};


template<typename BasicJsonType, typename T>
struct has_to_json
{
  private:
    template<typename U, typename = decltype(uncvref_t<U>::to_json(
                 std::declval<BasicJsonType&>(), std::declval<T>()))>
    static int detect(U&&);
    static void detect(...);

  public:
    static constexpr bool value = std::is_integral<decltype(detect(
                                      std::declval<typename BasicJsonType::template json_serializer<T, void>>()))>::value;
};






template<typename BasicJsonType, typename T, enable_if_t<
             std::is_same<T, typename BasicJsonType::boolean_t>::value, int> = 0>
void to_json(BasicJsonType& j, T b) noexcept
{
    external_constructor<value_t::boolean>::construct(j, b);
}

template<typename BasicJsonType, typename CompatibleString,
         enable_if_t<std::is_constructible<typename BasicJsonType::string_t,
                     CompatibleString>::value, int> = 0>
void to_json(BasicJsonType& j, const CompatibleString& s)
{
    external_constructor<value_t::string>::construct(j, s);
}

template<typename BasicJsonType, typename FloatType,
         enable_if_t<std::is_floating_point<FloatType>::value, int> = 0>
void to_json(BasicJsonType& j, FloatType val) noexcept
{
    external_constructor<value_t::number_float>::construct(j, static_cast<typename BasicJsonType::number_float_t>(val));
}

template <
    typename BasicJsonType, typename CompatibleNumberUnsignedType,
    enable_if_t<is_compatible_integer_type<typename BasicJsonType::number_unsigned_t,
                CompatibleNumberUnsignedType>::value, int> = 0 >
void to_json(BasicJsonType& j, CompatibleNumberUnsignedType val) noexcept
{
    external_constructor<value_t::number_unsigned>::construct(j, static_cast<typename BasicJsonType::number_unsigned_t>(val));
}

template <
    typename BasicJsonType, typename CompatibleNumberIntegerType,
    enable_if_t<is_compatible_integer_type<typename BasicJsonType::number_integer_t,
                CompatibleNumberIntegerType>::value, int> = 0 >
void to_json(BasicJsonType& j, CompatibleNumberIntegerType val) noexcept
{
    external_constructor<value_t::number_integer>::construct(j, static_cast<typename BasicJsonType::number_integer_t>(val));
}

template<typename BasicJsonType, typename UnscopedEnumType,
         enable_if_t<is_unscoped_enum<UnscopedEnumType>::value, int> = 0>
void to_json(BasicJsonType& j, UnscopedEnumType e) noexcept
{
    external_constructor<value_t::number_integer>::construct(j, e);
}

template <
    typename BasicJsonType, typename CompatibleArrayType,
    enable_if_t <
        is_compatible_array_type<BasicJsonType, CompatibleArrayType>::value or
        std::is_same<typename BasicJsonType::array_t, CompatibleArrayType>::value,
        int > = 0 >
void to_json(BasicJsonType& j, const  CompatibleArrayType& arr)
{
    external_constructor<value_t::array>::construct(j, arr);
}

template <
    typename BasicJsonType, typename CompatibleObjectType,
    enable_if_t<is_compatible_object_type<BasicJsonType, CompatibleObjectType>::value,
                int> = 0 >
void to_json(BasicJsonType& j, const  CompatibleObjectType& arr)
{
    external_constructor<value_t::object>::construct(j, arr);
}







template<typename BasicJsonType, typename ArithmeticType,
         enable_if_t<std::is_arithmetic<ArithmeticType>::value and
                     not std::is_same<ArithmeticType,
                                      typename BasicJsonType::boolean_t>::value,
                     int> = 0>
void get_arithmetic_value(const BasicJsonType& j, ArithmeticType& val)
{
    switch (static_cast<value_t>(j))
    {
        case value_t::number_unsigned:
        {
            val = static_cast<ArithmeticType>(
                      *j.template get_ptr<const typename BasicJsonType::number_unsigned_t*>());
            break;
        }
        case value_t::number_integer:
        {
            val = static_cast<ArithmeticType>(
                      *j.template get_ptr<const typename BasicJsonType::number_integer_t*>());
            break;
        }
        case value_t::number_float:
        {
            val = static_cast<ArithmeticType>(
                      *j.template get_ptr<const typename BasicJsonType::number_float_t*>());
            break;
        }
        default:
        {
            JSON_THROW(
                std::domain_error("type must be number, but is " + j.type_name()));
        }
    }
}

template<typename BasicJsonType>
void from_json(const BasicJsonType& j, typename BasicJsonType::boolean_t& b)
{
    if (not j.is_boolean())
    {
        JSON_THROW(std::domain_error("type must be boolean, but is " + j.type_name()));
    }
    b = *j.template get_ptr<const typename BasicJsonType::boolean_t*>();
}

template<typename BasicJsonType>
void from_json(const BasicJsonType& j, typename BasicJsonType::string_t& s)
{
    if (not j.is_string())
    {
        JSON_THROW(std::domain_error("type must be string, but is " + j.type_name()));
    }
    s = *j.template get_ptr<const typename BasicJsonType::string_t*>();
}

template<typename BasicJsonType>
void from_json(const BasicJsonType& j, typename BasicJsonType::number_float_t& val)
{
    get_arithmetic_value(j, val);
}

template<typename BasicJsonType>
void from_json(const BasicJsonType& j, typename BasicJsonType::number_unsigned_t& val)
{
    get_arithmetic_value(j, val);
}

template<typename BasicJsonType>
void from_json(const BasicJsonType& j, typename BasicJsonType::number_integer_t& val)
{
    get_arithmetic_value(j, val);
}

template<typename BasicJsonType, typename UnscopedEnumType,
         enable_if_t<is_unscoped_enum<UnscopedEnumType>::value, int> = 0>
void from_json(const BasicJsonType& j, UnscopedEnumType& e)
{
    typename std::underlying_type<UnscopedEnumType>::type val = e;
    get_arithmetic_value(j, val);
    e = static_cast<UnscopedEnumType>(val);
}

template<typename BasicJsonType>
void from_json(const BasicJsonType& j, typename BasicJsonType::array_t& arr)
{
    if (not j.is_array())
    {
        JSON_THROW(std::domain_error("type must be array, but is " + j.type_name()));
    }
    arr = *j.template get_ptr<const typename BasicJsonType::array_t*>();
}


template<typename BasicJsonType, typename T, typename Allocator>
void from_json(const BasicJsonType& j, std::forward_list<T, Allocator>& l)
{
    
    
    if (j.is_null())
    {
        JSON_THROW(std::domain_error("type must be array, but is " + j.type_name()));
    }
    if (not std::is_same<T, BasicJsonType>::value)
    {
        if (not j.is_array())
        {
            JSON_THROW(std::domain_error("type must be array, but is " + j.type_name()));
        }
    }
    for (auto it = j.rbegin(), end = j.rend(); it != end; ++it)
    {
        l.push_front(it->template get<T>());
    }
}

template<typename BasicJsonType, typename CompatibleArrayType>
void from_json_array_impl(const BasicJsonType& j, CompatibleArrayType& arr, priority_tag<0>)
{
    using std::begin;
    using std::end;

    std::transform(j.begin(), j.end(),
                   std::inserter(arr, end(arr)), [](const BasicJsonType & i)
    {
        
        
        return i.template get<typename CompatibleArrayType::value_type>();
    });
}

template<typename BasicJsonType, typename CompatibleArrayType>
auto from_json_array_impl(const BasicJsonType& j, CompatibleArrayType& arr, priority_tag<1>)
-> decltype(
    arr.reserve(std::declval<typename CompatibleArrayType::size_type>()),
    void())
{
    using std::begin;
    using std::end;

    arr.reserve(j.size());
    std::transform(
        j.begin(), j.end(), std::inserter(arr, end(arr)), [](const BasicJsonType & i)
    {
        
        
        return i.template get<typename CompatibleArrayType::value_type>();
    });
}

template<typename BasicJsonType, typename CompatibleArrayType,
         enable_if_t<is_compatible_array_type<BasicJsonType, CompatibleArrayType>::value and
                     not std::is_same<typename BasicJsonType::array_t, CompatibleArrayType>::value, int> = 0>
void from_json(const BasicJsonType& j, CompatibleArrayType& arr)
{
    if (j.is_null())
    {
        JSON_THROW(std::domain_error("type must be array, but is " + j.type_name()));
    }

    
    if (not std::is_same<typename CompatibleArrayType::value_type, BasicJsonType>::value)
    {
        if (not j.is_array())
        {
            JSON_THROW(std::domain_error("type must be array, but is " + j.type_name()));
        }
    }
    from_json_array_impl(j, arr, priority_tag<1> {});
}

template<typename BasicJsonType, typename CompatibleObjectType,
         enable_if_t<is_compatible_object_type<BasicJsonType, CompatibleObjectType>::value, int> = 0>
void from_json(const BasicJsonType& j, CompatibleObjectType& obj)
{
    if (not j.is_object())
    {
        JSON_THROW(std::domain_error("type must be object, but is " + j.type_name()));
    }

    auto inner_object = j.template get_ptr<const typename BasicJsonType::object_t*>();
    using std::begin;
    using std::end;
    
    
    
    obj = CompatibleObjectType(begin(*inner_object), end(*inner_object));
}





template<typename BasicJsonType, typename ArithmeticType,
         enable_if_t <
             std::is_arithmetic<ArithmeticType>::value and
             not std::is_same<ArithmeticType, typename BasicJsonType::number_unsigned_t>::value and
             not std::is_same<ArithmeticType, typename BasicJsonType::number_integer_t>::value and
             not std::is_same<ArithmeticType, typename BasicJsonType::number_float_t>::value and
             not std::is_same<ArithmeticType, typename BasicJsonType::boolean_t>::value,
             int> = 0>
void from_json(const BasicJsonType& j, ArithmeticType& val)
{
    switch (static_cast<value_t>(j))
    {
        case value_t::number_unsigned:
        {
            val = static_cast<ArithmeticType>(*j.template get_ptr<const typename BasicJsonType::number_unsigned_t*>());
            break;
        }
        case value_t::number_integer:
        {
            val = static_cast<ArithmeticType>(*j.template get_ptr<const typename BasicJsonType::number_integer_t*>());
            break;
        }
        case value_t::number_float:
        {
            val = static_cast<ArithmeticType>(*j.template get_ptr<const typename BasicJsonType::number_float_t*>());
            break;
        }
        case value_t::boolean:
        {
            val = static_cast<ArithmeticType>(*j.template get_ptr<const typename BasicJsonType::boolean_t*>());
            break;
        }
        default:
        {
            JSON_THROW(std::domain_error("type must be number, but is " + j.type_name()));
        }
    }
}

struct to_json_fn
{
  private:
    template<typename BasicJsonType, typename T>
    auto call(BasicJsonType& j, T&& val, priority_tag<1>) const noexcept(noexcept(to_json(j, std::forward<T>(val))))
    -> decltype(to_json(j, std::forward<T>(val)), void())
    {
        return to_json(j, std::forward<T>(val));
    }

    template<typename BasicJsonType, typename T>
    void call(BasicJsonType&, T&&, priority_tag<0>) const noexcept
    {
        static_assert(sizeof(BasicJsonType) == 0,
                      "could not find to_json() method in T's namespace");
    }

  public:
    template<typename BasicJsonType, typename T>
    void operator()(BasicJsonType& j, T&& val) const
    noexcept(noexcept(std::declval<to_json_fn>().call(j, std::forward<T>(val), priority_tag<1> {})))
    {
        return call(j, std::forward<T>(val), priority_tag<1> {});
    }
};

struct from_json_fn
{
  private:
    template<typename BasicJsonType, typename T>
    auto call(const BasicJsonType& j, T& val, priority_tag<1>) const
    noexcept(noexcept(from_json(j, val)))
    -> decltype(from_json(j, val), void())
    {
        return from_json(j, val);
    }

    template<typename BasicJsonType, typename T>
    void call(const BasicJsonType&, T&, priority_tag<0>) const noexcept
    {
        static_assert(sizeof(BasicJsonType) == 0,
                      "could not find from_json() method in T's namespace");
    }

  public:
    template<typename BasicJsonType, typename T>
    void operator()(const BasicJsonType& j, T& val) const
    noexcept(noexcept(std::declval<from_json_fn>().call(j, val, priority_tag<1> {})))
    {
        return call(j, val, priority_tag<1> {});
    }
};


template<typename T>
struct static_const
{
    static constexpr T value{};
};

template<typename T>
constexpr T static_const<T>::value;
} 



namespace
{
constexpr const auto& to_json = detail::static_const<detail::to_json_fn>::value;
constexpr const auto& from_json = detail::static_const<detail::from_json_fn>::value;
}



template<typename = void, typename = void>
struct adl_serializer
{
    
    template<typename BasicJsonType, typename ValueType>
    static void from_json(BasicJsonType&& j, ValueType& val) noexcept(
        noexcept(::nlohmann::from_json(std::forward<BasicJsonType>(j), val)))
    {
        ::nlohmann::from_json(std::forward<BasicJsonType>(j), val);
    }

    
    template<typename BasicJsonType, typename ValueType>
    static void to_json(BasicJsonType& j, ValueType&& val) noexcept(
        noexcept(::nlohmann::to_json(j, std::forward<ValueType>(val))))
    {
        ::nlohmann::to_json(j, std::forward<ValueType>(val));
    }
};



template <
    template<typename U, typename V, typename... Args> class ObjectType = std::map,
    template<typename U, typename... Args> class ArrayType = std::vector,
    class StringType = std::string,
    class BooleanType = bool,
    class NumberIntegerType = std::int64_t,
    class NumberUnsignedType = std::uint64_t,
    class NumberFloatType = double,
    template<typename U> class AllocatorType = std::allocator,
    template<typename T, typename SFINAE = void> class JSONSerializer = adl_serializer
    >
class basic_json
{
  private:
    template<detail::value_t> friend struct detail::external_constructor;
    
    using basic_json_t = basic_json<ObjectType, ArrayType, StringType,
          BooleanType, NumberIntegerType, NumberUnsignedType, NumberFloatType,
          AllocatorType, JSONSerializer>;

  public:
    using value_t = detail::value_t;
    
    template<typename U> class iter_impl;
    template<typename Base> class json_reverse_iterator;
    class json_pointer;
    template<typename T, typename SFINAE>
    using json_serializer = JSONSerializer<T, SFINAE>;

    
    
    

    
    
    
    

    
    using value_type = basic_json;

    
    using reference = value_type&;
    
    using const_reference = const value_type&;

    
    using difference_type = std::ptrdiff_t;
    
    using size_type = std::size_t;

    
    using allocator_type = AllocatorType<basic_json>;

    
    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

    
    using iterator = iter_impl<basic_json>;
    
    using const_iterator = iter_impl<const basic_json>;
    
    using reverse_iterator = json_reverse_iterator<typename basic_json::iterator>;
    
    using const_reverse_iterator = json_reverse_iterator<typename basic_json::const_iterator>;

    


    
    static allocator_type get_allocator()
    {
        return allocator_type();
    }

    
    static basic_json meta()
    {
        basic_json result;

        result["copyright"] = "(C) 2013-2017 Niels Lohmann";
        result["name"] = "JSON for Modern C++";
        result["url"] = "https:
        result["version"] =
        {
            {"string", "2.1.0"},
            {"major", 2},
            {"minor", 1},
            {"patch", 0},
        };

#ifdef _WIN32
        result["platform"] = "win32";
#elif defined __linux__
        result["platform"] = "linux";
#elif defined __APPLE__
        result["platform"] = "apple";
#elif defined __unix__
        result["platform"] = "unix";
#else
        result["platform"] = "unknown";
#endif

#if defined(__clang__)
        result["compiler"] = {{"family", "clang"}, {"version", __clang_version__}};
#elif defined(__ICC) || defined(__INTEL_COMPILER)
        result["compiler"] = {{"family", "icc"}, {"version", __INTEL_COMPILER}};
#elif defined(__GNUC__) || defined(__GNUG__)
        result["compiler"] = {{"family", "gcc"}, {"version", std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__) + "." + std::to_string(__GNUC_PATCHLEVEL__)}};
#elif defined(__HP_cc) || defined(__HP_aCC)
        result["compiler"] = "hp"
#elif defined(__IBMCPP__)
        result["compiler"] = {{"family", "ilecpp"}, {"version", __IBMCPP__}};
#elif defined(_MSC_VER)
        result["compiler"] = {{"family", "msvc"}, {"version", _MSC_VER}};
#elif defined(__PGI)
        result["compiler"] = {{"family", "pgcpp"}, {"version", __PGI}};
#elif defined(__SUNPRO_CC)
        result["compiler"] = {{"family", "sunpro"}, {"version", __SUNPRO_CC}};
#else
        result["compiler"] = {{"family", "unknown"}, {"version", "unknown"}};
#endif

#ifdef __cplusplus
        result["compiler"]["c++"] = std::to_string(__cplusplus);
#else
        result["compiler"]["c++"] = "unknown";
#endif
        return result;
    }


    
    
    

    
    
    
    

    
    using object_t = ObjectType<StringType,
          basic_json,
          std::less<StringType>,
          AllocatorType<std::pair<const StringType,
          basic_json>>>;

    
    using array_t = ArrayType<basic_json, AllocatorType<basic_json>>;

    
    using string_t = StringType;

    
    using boolean_t = BooleanType;

    
    using number_integer_t = NumberIntegerType;

    
    using number_unsigned_t = NumberUnsignedType;

    
    using number_float_t = NumberFloatType;

    

  private:

    
    template<typename T, typename... Args>
    static T* create(Args&& ... args)
    {
        AllocatorType<T> alloc;
        auto deleter = [&](T * object)
        {
            alloc.deallocate(object, 1);
        };
        std::unique_ptr<T, decltype(deleter)> object(alloc.allocate(1), deleter);
        alloc.construct(object.get(), std::forward<Args>(args)...);
        assert(object != nullptr);
        return object.release();
    }

    
    
    

    
    union json_value
    {
        
        object_t* object;
        
        array_t* array;
        
        string_t* string;
        
        boolean_t boolean;
        
        number_integer_t number_integer;
        
        number_unsigned_t number_unsigned;
        
        number_float_t number_float;

        
        json_value() = default;
        
        json_value(boolean_t v) noexcept : boolean(v) {}
        
        json_value(number_integer_t v) noexcept : number_integer(v) {}
        
        json_value(number_unsigned_t v) noexcept : number_unsigned(v) {}
        
        json_value(number_float_t v) noexcept : number_float(v) {}
        
        json_value(value_t t)
        {
            switch (t)
            {
                case value_t::object:
                {
                    object = create<object_t>();
                    break;
                }

                case value_t::array:
                {
                    array = create<array_t>();
                    break;
                }

                case value_t::string:
                {
                    string = create<string_t>("");
                    break;
                }

                case value_t::boolean:
                {
                    boolean = boolean_t(false);
                    break;
                }

                case value_t::number_integer:
                {
                    number_integer = number_integer_t(0);
                    break;
                }

                case value_t::number_unsigned:
                {
                    number_unsigned = number_unsigned_t(0);
                    break;
                }

                case value_t::number_float:
                {
                    number_float = number_float_t(0.0);
                    break;
                }

                case value_t::null:
                {
                    break;
                }

                default:
                {
                    if (t == value_t::null)
                    {
                        JSON_THROW(std::domain_error("961c151d2e87f2686a955a9be24d316f1362bf21 2.1.0")); 
                    }
                    break;
                }
            }
        }

        
        json_value(const string_t& value)
        {
            string = create<string_t>(value);
        }

        
        json_value(const object_t& value)
        {
            object = create<object_t>(value);
        }

        
        json_value(const array_t& value)
        {
            array = create<array_t>(value);
        }
    };

    
    void assert_invariant() const
    {
        assert(m_type != value_t::object or m_value.object != nullptr);
        assert(m_type != value_t::array or m_value.array != nullptr);
        assert(m_type != value_t::string or m_value.string != nullptr);
    }

  public:
    
    
    

    
    enum class parse_event_t : uint8_t
    {
        
        object_start,
        
        object_end,
        
        array_start,
        
        array_end,
        
        key,
        
        value
    };

    
    using parser_callback_t = std::function<bool(int depth,
                              parse_event_t event,
                              basic_json& parsed)>;


    
    
    

    
    
    
    

    
    basic_json(const value_t value_type)
        : m_type(value_type), m_value(value_type)
    {
        assert_invariant();
    }

    
    basic_json(std::nullptr_t = nullptr) noexcept
        : basic_json(value_t::null)
    {
        assert_invariant();
    }

    
    template<typename CompatibleType, typename U = detail::uncvref_t<CompatibleType>,
             detail::enable_if_t<not std::is_base_of<std::istream, U>::value and
                                 not std::is_same<U, basic_json_t>::value and
                                 not detail::is_basic_json_nested_type<
                                     basic_json_t, U>::value and
                                 detail::has_to_json<basic_json, U>::value,
                                 int> = 0>
    basic_json(CompatibleType && val) noexcept(noexcept(JSONSerializer<U>::to_json(
                std::declval<basic_json_t&>(), std::forward<CompatibleType>(val))))
    {
        JSONSerializer<U>::to_json(*this, std::forward<CompatibleType>(val));
        assert_invariant();
    }

    
    basic_json(std::initializer_list<basic_json> init,
               bool type_deduction = true,
               value_t manual_type = value_t::array)
    {
        
        
        bool is_an_object = std::all_of(init.begin(), init.end(),
                                        [](const basic_json & element)
        {
            return element.is_array() and element.size() == 2 and element[0].is_string();
        });

        
        if (not type_deduction)
        {
            
            if (manual_type == value_t::array)
            {
                is_an_object = false;
            }

            
            if (manual_type == value_t::object and not is_an_object)
            {
                JSON_THROW(std::domain_error("cannot create object from initializer list"));
            }
        }

        if (is_an_object)
        {
            
            m_type = value_t::object;
            m_value = value_t::object;

            std::for_each(init.begin(), init.end(), [this](const basic_json & element)
            {
                m_value.object->emplace(*(element[0].m_value.string), element[1]);
            });
        }
        else
        {
            
            m_type = value_t::array;
            m_value.array = create<array_t>(init);
        }

        assert_invariant();
    }

    
    static basic_json array(std::initializer_list<basic_json> init =
                                std::initializer_list<basic_json>())
    {
        return basic_json(init, false, value_t::array);
    }

    
    static basic_json object(std::initializer_list<basic_json> init =
                                 std::initializer_list<basic_json>())
    {
        return basic_json(init, false, value_t::object);
    }

    
    basic_json(size_type cnt, const basic_json& val)
        : m_type(value_t::array)
    {
        m_value.array = create<array_t>(cnt, val);
        assert_invariant();
    }

    
    template<class InputIT, typename std::enable_if<
                 std::is_same<InputIT, typename basic_json_t::iterator>::value or
                 std::is_same<InputIT, typename basic_json_t::const_iterator>::value, int>::type = 0>
    basic_json(InputIT first, InputIT last)
    {
        assert(first.m_object != nullptr);
        assert(last.m_object != nullptr);

        
        if (first.m_object != last.m_object)
        {
            JSON_THROW(std::domain_error("iterators are not compatible"));
        }

        
        m_type = first.m_object->m_type;

        
        switch (m_type)
        {
            case value_t::boolean:
            case value_t::number_float:
            case value_t::number_integer:
            case value_t::number_unsigned:
            case value_t::string:
            {
                if (not first.m_it.primitive_iterator.is_begin() or not last.m_it.primitive_iterator.is_end())
                {
                    JSON_THROW(std::out_of_range("iterators out of range"));
                }
                break;
            }

            default:
            {
                break;
            }
        }

        switch (m_type)
        {
            case value_t::number_integer:
            {
                m_value.number_integer = first.m_object->m_value.number_integer;
                break;
            }

            case value_t::number_unsigned:
            {
                m_value.number_unsigned = first.m_object->m_value.number_unsigned;
                break;
            }

            case value_t::number_float:
            {
                m_value.number_float = first.m_object->m_value.number_float;
                break;
            }

            case value_t::boolean:
            {
                m_value.boolean = first.m_object->m_value.boolean;
                break;
            }

            case value_t::string:
            {
                m_value = *first.m_object->m_value.string;
                break;
            }

            case value_t::object:
            {
                m_value.object = create<object_t>(first.m_it.object_iterator,
                                                  last.m_it.object_iterator);
                break;
            }

            case value_t::array:
            {
                m_value.array = create<array_t>(first.m_it.array_iterator,
                                                last.m_it.array_iterator);
                break;
            }

            default:
            {
                JSON_THROW(std::domain_error("cannot use construct with iterators from " + first.m_object->type_name()));
            }
        }

        assert_invariant();
    }

    
    JSON_DEPRECATED
    explicit basic_json(std::istream& i, const parser_callback_t cb = nullptr)
    {
        *this = parser(i, cb).parse();
        assert_invariant();
    }

    
    
    

    
    basic_json(const basic_json& other)
        : m_type(other.m_type)
    {
        
        other.assert_invariant();

        switch (m_type)
        {
            case value_t::object:
            {
                m_value = *other.m_value.object;
                break;
            }

            case value_t::array:
            {
                m_value = *other.m_value.array;
                break;
            }

            case value_t::string:
            {
                m_value = *other.m_value.string;
                break;
            }

            case value_t::boolean:
            {
                m_value = other.m_value.boolean;
                break;
            }

            case value_t::number_integer:
            {
                m_value = other.m_value.number_integer;
                break;
            }

            case value_t::number_unsigned:
            {
                m_value = other.m_value.number_unsigned;
                break;
            }

            case value_t::number_float:
            {
                m_value = other.m_value.number_float;
                break;
            }

            default:
            {
                break;
            }
        }

        assert_invariant();
    }

    
    basic_json(basic_json&& other) noexcept
        : m_type(std::move(other.m_type)),
          m_value(std::move(other.m_value))
    {
        
        other.assert_invariant();

        
        other.m_type = value_t::null;
        other.m_value = {};

        assert_invariant();
    }

    
    reference& operator=(basic_json other) noexcept (
        std::is_nothrow_move_constructible<value_t>::value and
        std::is_nothrow_move_assignable<value_t>::value and
        std::is_nothrow_move_constructible<json_value>::value and
        std::is_nothrow_move_assignable<json_value>::value
    )
    {
        
        other.assert_invariant();

        using std::swap;
        swap(m_type, other.m_type);
        swap(m_value, other.m_value);

        assert_invariant();
        return *this;
    }

    
    ~basic_json()
    {
        assert_invariant();

        switch (m_type)
        {
            case value_t::object:
            {
                AllocatorType<object_t> alloc;
                alloc.destroy(m_value.object);
                alloc.deallocate(m_value.object, 1);
                break;
            }

            case value_t::array:
            {
                AllocatorType<array_t> alloc;
                alloc.destroy(m_value.array);
                alloc.deallocate(m_value.array, 1);
                break;
            }

            case value_t::string:
            {
                AllocatorType<string_t> alloc;
                alloc.destroy(m_value.string);
                alloc.deallocate(m_value.string, 1);
                break;
            }

            default:
            {
                
                break;
            }
        }
    }

    

  public:
    
    
    

    
    
    

    
    string_t dump(const int indent = -1) const
    {
        std::stringstream ss;

        if (indent >= 0)
        {
            dump(ss, true, static_cast<unsigned int>(indent));
        }
        else
        {
            dump(ss, false, 0);
        }

        return ss.str();
    }

    
    constexpr value_t type() const noexcept
    {
        return m_type;
    }

    
    constexpr bool is_primitive() const noexcept
    {
        return is_null() or is_string() or is_boolean() or is_number();
    }

    
    constexpr bool is_structured() const noexcept
    {
        return is_array() or is_object();
    }

    
    constexpr bool is_null() const noexcept
    {
        return m_type == value_t::null;
    }

    
    constexpr bool is_boolean() const noexcept
    {
        return m_type == value_t::boolean;
    }

    
    constexpr bool is_number() const noexcept
    {
        return is_number_integer() or is_number_float();
    }

    
    constexpr bool is_number_integer() const noexcept
    {
        return m_type == value_t::number_integer or m_type == value_t::number_unsigned;
    }

    
    constexpr bool is_number_unsigned() const noexcept
    {
        return m_type == value_t::number_unsigned;
    }

    
    constexpr bool is_number_float() const noexcept
    {
        return m_type == value_t::number_float;
    }

    
    constexpr bool is_object() const noexcept
    {
        return m_type == value_t::object;
    }

    
    constexpr bool is_array() const noexcept
    {
        return m_type == value_t::array;
    }

    
    constexpr bool is_string() const noexcept
    {
        return m_type == value_t::string;
    }

    
    constexpr bool is_discarded() const noexcept
    {
        return m_type == value_t::discarded;
    }

    
    constexpr operator value_t() const noexcept
    {
        return m_type;
    }

    

  private:
    
    
    

    
    boolean_t get_impl(boolean_t* ) const
    {
        if (is_boolean())
        {
            return m_value.boolean;
        }

        JSON_THROW(std::domain_error("type must be boolean, but is " + type_name()));
    }

    
    object_t* get_impl_ptr(object_t* ) noexcept
    {
        return is_object() ? m_value.object : nullptr;
    }

    
    constexpr const object_t* get_impl_ptr(const object_t* ) const noexcept
    {
        return is_object() ? m_value.object : nullptr;
    }

    
    array_t* get_impl_ptr(array_t* ) noexcept
    {
        return is_array() ? m_value.array : nullptr;
    }

    
    constexpr const array_t* get_impl_ptr(const array_t* ) const noexcept
    {
        return is_array() ? m_value.array : nullptr;
    }

    
    string_t* get_impl_ptr(string_t* ) noexcept
    {
        return is_string() ? m_value.string : nullptr;
    }

    
    constexpr const string_t* get_impl_ptr(const string_t* ) const noexcept
    {
        return is_string() ? m_value.string : nullptr;
    }

    
    boolean_t* get_impl_ptr(boolean_t* ) noexcept
    {
        return is_boolean() ? &m_value.boolean : nullptr;
    }

    
    constexpr const boolean_t* get_impl_ptr(const boolean_t* ) const noexcept
    {
        return is_boolean() ? &m_value.boolean : nullptr;
    }

    
    number_integer_t* get_impl_ptr(number_integer_t* ) noexcept
    {
        return is_number_integer() ? &m_value.number_integer : nullptr;
    }

    
    constexpr const number_integer_t* get_impl_ptr(const number_integer_t* ) const noexcept
    {
        return is_number_integer() ? &m_value.number_integer : nullptr;
    }

    
    number_unsigned_t* get_impl_ptr(number_unsigned_t* ) noexcept
    {
        return is_number_unsigned() ? &m_value.number_unsigned : nullptr;
    }

    
    constexpr const number_unsigned_t* get_impl_ptr(const number_unsigned_t* ) const noexcept
    {
        return is_number_unsigned() ? &m_value.number_unsigned : nullptr;
    }

    
    number_float_t* get_impl_ptr(number_float_t* ) noexcept
    {
        return is_number_float() ? &m_value.number_float : nullptr;
    }

    
    constexpr const number_float_t* get_impl_ptr(const number_float_t* ) const noexcept
    {
        return is_number_float() ? &m_value.number_float : nullptr;
    }

    
    template<typename ReferenceType, typename ThisType>
    static ReferenceType get_ref_impl(ThisType& obj)
    {
        
        using PointerType = typename std::add_pointer<ReferenceType>::type;

        
        auto ptr = obj.template get_ptr<PointerType>();

        if (ptr != nullptr)
        {
            return *ptr;
        }

        JSON_THROW(std::domain_error("incompatible ReferenceType for get_ref, actual type is " +
                                     obj.type_name()));
    }

  public:
    
    
    

    
    template <
        typename BasicJsonType,
        detail::enable_if_t<std::is_same<typename std::remove_const<BasicJsonType>::type,
                                         basic_json_t>::value,
                            int> = 0 >
    basic_json get() const
    {
        return *this;
    }

    
    template <
        typename ValueTypeCV,
        typename ValueType = detail::uncvref_t<ValueTypeCV>,
        detail::enable_if_t <
            not std::is_same<basic_json_t, ValueType>::value and
            detail::has_from_json<basic_json_t, ValueType>::value and
            not detail::has_non_default_from_json<basic_json_t, ValueType>::value,
            int > = 0 >
    ValueType get() const noexcept(noexcept(
                                       JSONSerializer<ValueType>::from_json(std::declval<const basic_json_t&>(), std::declval<ValueType&>())))
    {
        
        
        
        static_assert(not std::is_reference<ValueTypeCV>::value,
                      "get() cannot be used with reference types, you might want to use get_ref()");
        static_assert(std::is_default_constructible<ValueType>::value,
                      "types must be DefaultConstructible when used with get()");

        ValueType ret;
        JSONSerializer<ValueType>::from_json(*this, ret);
        return ret;
    }

    
    template <
        typename ValueTypeCV,
        typename ValueType = detail::uncvref_t<ValueTypeCV>,
        detail::enable_if_t<not std::is_same<basic_json_t, ValueType>::value and
                            detail::has_non_default_from_json<basic_json_t,
                                    ValueType>::value, int> = 0 >
    ValueType get() const noexcept(noexcept(
                                       JSONSerializer<ValueTypeCV>::from_json(std::declval<const basic_json_t&>())))
    {
        static_assert(not std::is_reference<ValueTypeCV>::value,
                      "get() cannot be used with reference types, you might want to use get_ref()");
        return JSONSerializer<ValueTypeCV>::from_json(*this);
    }

    
    template<typename PointerType, typename std::enable_if<
                 std::is_pointer<PointerType>::value, int>::type = 0>
    PointerType get() noexcept
    {
        
        return get_ptr<PointerType>();
    }

    
    template<typename PointerType, typename std::enable_if<
                 std::is_pointer<PointerType>::value, int>::type = 0>
    constexpr const PointerType get() const noexcept
    {
        
        return get_ptr<PointerType>();
    }

    
    template<typename PointerType, typename std::enable_if<
                 std::is_pointer<PointerType>::value, int>::type = 0>
    PointerType get_ptr() noexcept
    {
        
        using pointee_t = typename std::remove_const<typename
                          std::remove_pointer<typename
                          std::remove_const<PointerType>::type>::type>::type;
        
        static_assert(
            std::is_same<object_t, pointee_t>::value
            or std::is_same<array_t, pointee_t>::value
            or std::is_same<string_t, pointee_t>::value
            or std::is_same<boolean_t, pointee_t>::value
            or std::is_same<number_integer_t, pointee_t>::value
            or std::is_same<number_unsigned_t, pointee_t>::value
            or std::is_same<number_float_t, pointee_t>::value
            , "incompatible pointer type");

        
        return get_impl_ptr(static_cast<PointerType>(nullptr));
    }

    
    template<typename PointerType, typename std::enable_if<
                 std::is_pointer<PointerType>::value and
                 std::is_const<typename std::remove_pointer<PointerType>::type>::value, int>::type = 0>
    constexpr const PointerType get_ptr() const noexcept
    {
        
        using pointee_t = typename std::remove_const<typename
                          std::remove_pointer<typename
                          std::remove_const<PointerType>::type>::type>::type;
        
        static_assert(
            std::is_same<object_t, pointee_t>::value
            or std::is_same<array_t, pointee_t>::value
            or std::is_same<string_t, pointee_t>::value
            or std::is_same<boolean_t, pointee_t>::value
            or std::is_same<number_integer_t, pointee_t>::value
            or std::is_same<number_unsigned_t, pointee_t>::value
            or std::is_same<number_float_t, pointee_t>::value
            , "incompatible pointer type");

        
        return get_impl_ptr(static_cast<const PointerType>(nullptr));
    }

    
    template<typename ReferenceType, typename std::enable_if<
                 std::is_reference<ReferenceType>::value, int>::type = 0>
    ReferenceType get_ref()
    {
        
        return get_ref_impl<ReferenceType>(*this);
    }

    
    template<typename ReferenceType, typename std::enable_if<
                 std::is_reference<ReferenceType>::value and
                 std::is_const<typename std::remove_reference<ReferenceType>::type>::value, int>::type = 0>
    ReferenceType get_ref() const
    {
        
        return get_ref_impl<ReferenceType>(*this);
    }

    
    template < typename ValueType, typename std::enable_if <
                   not std::is_pointer<ValueType>::value and
                   not std::is_same<ValueType, typename string_t::value_type>::value
#ifndef _MSC_VER  
                   and not std::is_same<ValueType, std::initializer_list<typename string_t::value_type>>::value
#endif
                   , int >::type = 0 >
    operator ValueType() const
    {
        
        return get<ValueType>();
    }

    


    
    
    

    
    
    

    
    reference at(size_type idx)
    {
        
        if (is_array())
        {
            JSON_TRY
            {
                return m_value.array->at(idx);
            }
            JSON_CATCH (std::out_of_range&)
            {
                
                JSON_THROW(std::out_of_range("array index " + std::to_string(idx) + " is out of range"));
            }
        }
        else
        {
            JSON_THROW(std::domain_error("cannot use at() with " + type_name()));
        }
    }

    
    const_reference at(size_type idx) const
    {
        
        if (is_array())
        {
            JSON_TRY
            {
                return m_value.array->at(idx);
            }
            JSON_CATCH (std::out_of_range&)
            {
                
                JSON_THROW(std::out_of_range("array index " + std::to_string(idx) + " is out of range"));
            }
        }
        else
        {
            JSON_THROW(std::domain_error("cannot use at() with " + type_name()));
        }
    }

    
    reference at(const typename object_t::key_type& key)
    {
        
        if (is_object())
        {
            JSON_TRY
            {
                return m_value.object->at(key);
            }
            JSON_CATCH (std::out_of_range&)
            {
                
                JSON_THROW(std::out_of_range("key '" + key + "' not found"));
            }
        }
        else
        {
            JSON_THROW(std::domain_error("cannot use at() with " + type_name()));
        }
    }

    
    const_reference at(const typename object_t::key_type& key) const
    {
        
        if (is_object())
        {
            JSON_TRY
            {
                return m_value.object->at(key);
            }
            JSON_CATCH (std::out_of_range&)
            {
                
                JSON_THROW(std::out_of_range("key '" + key + "' not found"));
            }
        }
        else
        {
            JSON_THROW(std::domain_error("cannot use at() with " + type_name()));
        }
    }

    
    reference operator[](size_type idx)
    {
        
        if (is_null())
        {
            m_type = value_t::array;
            m_value.array = create<array_t>();
            assert_invariant();
        }

        
        if (is_array())
        {
            
            if (idx >= m_value.array->size())
            {
                m_value.array->insert(m_value.array->end(),
                                      idx - m_value.array->size() + 1,
                                      basic_json());
            }

            return m_value.array->operator[](idx);
        }

        JSON_THROW(std::domain_error("cannot use operator[] with " + type_name()));
    }

    
    const_reference operator[](size_type idx) const
    {
        
        if (is_array())
        {
            return m_value.array->operator[](idx);
        }

        JSON_THROW(std::domain_error("cannot use operator[] with " + type_name()));
    }

    
    reference operator[](const typename object_t::key_type& key)
    {
        
        if (is_null())
        {
            m_type = value_t::object;
            m_value.object = create<object_t>();
            assert_invariant();
        }

        
        if (is_object())
        {
            return m_value.object->operator[](key);
        }

        JSON_THROW(std::domain_error("cannot use operator[] with " + type_name()));
    }

    
    const_reference operator[](const typename object_t::key_type& key) const
    {
        
        if (is_object())
        {
            assert(m_value.object->find(key) != m_value.object->end());
            return m_value.object->find(key)->second;
        }

        JSON_THROW(std::domain_error("cannot use operator[] with " + type_name()));
    }

    
    template<typename T, std::size_t n>
    reference operator[](T * (&key)[n])
    {
        return operator[](static_cast<const T>(key));
    }

    
    template<typename T, std::size_t n>
    const_reference operator[](T * (&key)[n]) const
    {
        return operator[](static_cast<const T>(key));
    }

    
    template<typename T>
    reference operator[](T* key)
    {
        
        if (is_null())
        {
            m_type = value_t::object;
            m_value = value_t::object;
            assert_invariant();
        }

        
        if (is_object())
        {
            return m_value.object->operator[](key);
        }

        JSON_THROW(std::domain_error("cannot use operator[] with " + type_name()));
    }

    
    template<typename T>
    const_reference operator[](T* key) const
    {
        
        if (is_object())
        {
            assert(m_value.object->find(key) != m_value.object->end());
            return m_value.object->find(key)->second;
        }

        JSON_THROW(std::domain_error("cannot use operator[] with " + type_name()));
    }

    
    template<class ValueType, typename std::enable_if<
                 std::is_convertible<basic_json_t, ValueType>::value, int>::type = 0>
    ValueType value(const typename object_t::key_type& key, ValueType default_value) const
    {
        
        if (is_object())
        {
            
            const auto it = find(key);
            if (it != end())
            {
                return *it;
            }

            return default_value;
        }
        else
        {
            JSON_THROW(std::domain_error("cannot use value() with " + type_name()));
        }
    }

    
    string_t value(const typename object_t::key_type& key, const char* default_value) const
    {
        return value(key, string_t(default_value));
    }

    
    template<class ValueType, typename std::enable_if<
                 std::is_convertible<basic_json_t, ValueType>::value, int>::type = 0>
    ValueType value(const json_pointer& ptr, ValueType default_value) const
    {
        
        if (is_object())
        {
            
            JSON_TRY
            {
                return ptr.get_checked(this);
            }
            JSON_CATCH (std::out_of_range&)
            {
                return default_value;
            }
        }

        JSON_THROW(std::domain_error("cannot use value() with " + type_name()));
    }

    
    string_t value(const json_pointer& ptr, const char* default_value) const
    {
        return value(ptr, string_t(default_value));
    }

    
    reference front()
    {
        return *begin();
    }

    
    const_reference front() const
    {
        return *cbegin();
    }

    
    reference back()
    {
        auto tmp = end();
        --tmp;
        return *tmp;
    }

    
    const_reference back() const
    {
        auto tmp = cend();
        --tmp;
        return *tmp;
    }

    
    template<class IteratorType, typename std::enable_if<
                 std::is_same<IteratorType, typename basic_json_t::iterator>::value or
                 std::is_same<IteratorType, typename basic_json_t::const_iterator>::value, int>::type
             = 0>
    IteratorType erase(IteratorType pos)
    {
        
        if (this != pos.m_object)
        {
            JSON_THROW(std::domain_error("iterator does not fit current value"));
        }

        IteratorType result = end();

        switch (m_type)
        {
            case value_t::boolean:
            case value_t::number_float:
            case value_t::number_integer:
            case value_t::number_unsigned:
            case value_t::string:
            {
                if (not pos.m_it.primitive_iterator.is_begin())
                {
                    JSON_THROW(std::out_of_range("iterator out of range"));
                }

                if (is_string())
                {
                    AllocatorType<string_t> alloc;
                    alloc.destroy(m_value.string);
                    alloc.deallocate(m_value.string, 1);
                    m_value.string = nullptr;
                }

                m_type = value_t::null;
                assert_invariant();
                break;
            }

            case value_t::object:
            {
                result.m_it.object_iterator = m_value.object->erase(pos.m_it.object_iterator);
                break;
            }

            case value_t::array:
            {
                result.m_it.array_iterator = m_value.array->erase(pos.m_it.array_iterator);
                break;
            }

            default:
            {
                JSON_THROW(std::domain_error("cannot use erase() with " + type_name()));
            }
        }

        return result;
    }

    
    template<class IteratorType, typename std::enable_if<
                 std::is_same<IteratorType, typename basic_json_t::iterator>::value or
                 std::is_same<IteratorType, typename basic_json_t::const_iterator>::value, int>::type
             = 0>
    IteratorType erase(IteratorType first, IteratorType last)
    {
        
        if (this != first.m_object or this != last.m_object)
        {
            JSON_THROW(std::domain_error("iterators do not fit current value"));
        }

        IteratorType result = end();

        switch (m_type)
        {
            case value_t::boolean:
            case value_t::number_float:
            case value_t::number_integer:
            case value_t::number_unsigned:
            case value_t::string:
            {
                if (not first.m_it.primitive_iterator.is_begin() or not last.m_it.primitive_iterator.is_end())
                {
                    JSON_THROW(std::out_of_range("iterators out of range"));
                }

                if (is_string())
                {
                    AllocatorType<string_t> alloc;
                    alloc.destroy(m_value.string);
                    alloc.deallocate(m_value.string, 1);
                    m_value.string = nullptr;
                }

                m_type = value_t::null;
                assert_invariant();
                break;
            }

            case value_t::object:
            {
                result.m_it.object_iterator = m_value.object->erase(first.m_it.object_iterator,
                                              last.m_it.object_iterator);
                break;
            }

            case value_t::array:
            {
                result.m_it.array_iterator = m_value.array->erase(first.m_it.array_iterator,
                                             last.m_it.array_iterator);
                break;
            }

            default:
            {
                JSON_THROW(std::domain_error("cannot use erase() with " + type_name()));
            }
        }

        return result;
    }

    
    size_type erase(const typename object_t::key_type& key)
    {
        
        if (is_object())
        {
            return m_value.object->erase(key);
        }

        JSON_THROW(std::domain_error("cannot use erase() with " + type_name()));
    }

    
    void erase(const size_type idx)
    {
        
        if (is_array())
        {
            if (idx >= size())
            {
                JSON_THROW(std::out_of_range("array index " + std::to_string(idx) + " is out of range"));
            }

            m_value.array->erase(m_value.array->begin() + static_cast<difference_type>(idx));
        }
        else
        {
            JSON_THROW(std::domain_error("cannot use erase() with " + type_name()));
        }
    }

    


    
    
    

    
    

    
    iterator find(typename object_t::key_type key)
    {
        auto result = end();

        if (is_object())
        {
            result.m_it.object_iterator = m_value.object->find(key);
        }

        return result;
    }

    
    const_iterator find(typename object_t::key_type key) const
    {
        auto result = cend();

        if (is_object())
        {
            result.m_it.object_iterator = m_value.object->find(key);
        }

        return result;
    }

    
    size_type count(typename object_t::key_type key) const
    {
        
        return is_object() ? m_value.object->count(key) : 0;
    }

    


    
    
    

    
    

    
    iterator begin() noexcept
    {
        iterator result(this);
        result.set_begin();
        return result;
    }

    
    const_iterator begin() const noexcept
    {
        return cbegin();
    }

    
    const_iterator cbegin() const noexcept
    {
        const_iterator result(this);
        result.set_begin();
        return result;
    }

    
    iterator end() noexcept
    {
        iterator result(this);
        result.set_end();
        return result;
    }

    
    const_iterator end() const noexcept
    {
        return cend();
    }

    
    const_iterator cend() const noexcept
    {
        const_iterator result(this);
        result.set_end();
        return result;
    }

    
    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    
    const_reverse_iterator rbegin() const noexcept
    {
        return crbegin();
    }

    
    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    
    const_reverse_iterator rend() const noexcept
    {
        return crend();
    }

    
    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(cend());
    }

    
    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(cbegin());
    }

  private:
    
    template<typename IteratorType> class iteration_proxy;

  public:
    
    static iteration_proxy<iterator> iterator_wrapper(reference cont)
    {
        return iteration_proxy<iterator>(cont);
    }

    
    static iteration_proxy<const_iterator> iterator_wrapper(const_reference cont)
    {
        return iteration_proxy<const_iterator>(cont);
    }

    


    
    
    

    
    

    
    bool empty() const noexcept
    {
        switch (m_type)
        {
            case value_t::null:
            {
                
                return true;
            }

            case value_t::array:
            {
                
                return m_value.array->empty();
            }

            case value_t::object:
            {
                
                return m_value.object->empty();
            }

            default:
            {
                
                return false;
            }
        }
    }

    
    size_type size() const noexcept
    {
        switch (m_type)
        {
            case value_t::null:
            {
                
                return 0;
            }

            case value_t::array:
            {
                
                return m_value.array->size();
            }

            case value_t::object:
            {
                
                return m_value.object->size();
            }

            default:
            {
                
                return 1;
            }
        }
    }

    
    size_type max_size() const noexcept
    {
        switch (m_type)
        {
            case value_t::array:
            {
                
                return m_value.array->max_size();
            }

            case value_t::object:
            {
                
                return m_value.object->max_size();
            }

            default:
            {
                
                return size();
            }
        }
    }

    


    
    
    

    
    

    
    void clear() noexcept
    {
        switch (m_type)
        {
            case value_t::number_integer:
            {
                m_value.number_integer = 0;
                break;
            }

            case value_t::number_unsigned:
            {
                m_value.number_unsigned = 0;
                break;
            }

            case value_t::number_float:
            {
                m_value.number_float = 0.0;
                break;
            }

            case value_t::boolean:
            {
                m_value.boolean = false;
                break;
            }

            case value_t::string:
            {
                m_value.string->clear();
                break;
            }

            case value_t::array:
            {
                m_value.array->clear();
                break;
            }

            case value_t::object:
            {
                m_value.object->clear();
                break;
            }

            default:
            {
                break;
            }
        }
    }

    
    void push_back(basic_json&& val)
    {
        
        if (not(is_null() or is_array()))
        {
            JSON_THROW(std::domain_error("cannot use push_back() with " + type_name()));
        }

        
        if (is_null())
        {
            m_type = value_t::array;
            m_value = value_t::array;
            assert_invariant();
        }

        
        m_value.array->push_back(std::move(val));
        
        val.m_type = value_t::null;
    }

    
    reference operator+=(basic_json&& val)
    {
        push_back(std::move(val));
        return *this;
    }

    
    void push_back(const basic_json& val)
    {
        
        if (not(is_null() or is_array()))
        {
            JSON_THROW(std::domain_error("cannot use push_back() with " + type_name()));
        }

        
        if (is_null())
        {
            m_type = value_t::array;
            m_value = value_t::array;
            assert_invariant();
        }

        
        m_value.array->push_back(val);
    }

    
    reference operator+=(const basic_json& val)
    {
        push_back(val);
        return *this;
    }

    
    void push_back(const typename object_t::value_type& val)
    {
        
        if (not(is_null() or is_object()))
        {
            JSON_THROW(std::domain_error("cannot use push_back() with " + type_name()));
        }

        
        if (is_null())
        {
            m_type = value_t::object;
            m_value = value_t::object;
            assert_invariant();
        }

        
        m_value.object->insert(val);
    }

    
    reference operator+=(const typename object_t::value_type& val)
    {
        push_back(val);
        return *this;
    }

    
    void push_back(std::initializer_list<basic_json> init)
    {
        if (is_object() and init.size() == 2 and init.begin()->is_string())
        {
            const string_t key = *init.begin();
            push_back(typename object_t::value_type(key, *(init.begin() + 1)));
        }
        else
        {
            push_back(basic_json(init));
        }
    }

    
    reference operator+=(std::initializer_list<basic_json> init)
    {
        push_back(init);
        return *this;
    }

    
    template<class... Args>
    void emplace_back(Args&& ... args)
    {
        
        if (not(is_null() or is_array()))
        {
            JSON_THROW(std::domain_error("cannot use emplace_back() with " + type_name()));
        }

        
        if (is_null())
        {
            m_type = value_t::array;
            m_value = value_t::array;
            assert_invariant();
        }

        
        m_value.array->emplace_back(std::forward<Args>(args)...);
    }

    
    template<class... Args>
    std::pair<iterator, bool> emplace(Args&& ... args)
    {
        
        if (not(is_null() or is_object()))
        {
            JSON_THROW(std::domain_error("cannot use emplace() with " + type_name()));
        }

        
        if (is_null())
        {
            m_type = value_t::object;
            m_value = value_t::object;
            assert_invariant();
        }

        
        auto res = m_value.object->emplace(std::forward<Args>(args)...);
        
        auto it = begin();
        it.m_it.object_iterator = res.first;

        
        return {it, res.second};
    }

    
    iterator insert(const_iterator pos, const basic_json& val)
    {
        
        if (is_array())
        {
            
            if (pos.m_object != this)
            {
                JSON_THROW(std::domain_error("iterator does not fit current value"));
            }

            
            iterator result(this);
            result.m_it.array_iterator = m_value.array->insert(pos.m_it.array_iterator, val);
            return result;
        }

        JSON_THROW(std::domain_error("cannot use insert() with " + type_name()));
    }

    
    iterator insert(const_iterator pos, basic_json&& val)
    {
        return insert(pos, val);
    }

    
    iterator insert(const_iterator pos, size_type cnt, const basic_json& val)
    {
        
        if (is_array())
        {
            
            if (pos.m_object != this)
            {
                JSON_THROW(std::domain_error("iterator does not fit current value"));
            }

            
            iterator result(this);
            result.m_it.array_iterator = m_value.array->insert(pos.m_it.array_iterator, cnt, val);
            return result;
        }

        JSON_THROW(std::domain_error("cannot use insert() with " + type_name()));
    }

    
    iterator insert(const_iterator pos, const_iterator first, const_iterator last)
    {
        
        if (not is_array())
        {
            JSON_THROW(std::domain_error("cannot use insert() with " + type_name()));
        }

        
        if (pos.m_object != this)
        {
            JSON_THROW(std::domain_error("iterator does not fit current value"));
        }

        
        if (first.m_object != last.m_object)
        {
            JSON_THROW(std::domain_error("iterators do not fit"));
        }

        if (first.m_object == this or last.m_object == this)
        {
            JSON_THROW(std::domain_error("passed iterators may not belong to container"));
        }

        
        iterator result(this);
        result.m_it.array_iterator = m_value.array->insert(
                                         pos.m_it.array_iterator,
                                         first.m_it.array_iterator,
                                         last.m_it.array_iterator);
        return result;
    }

    
    iterator insert(const_iterator pos, std::initializer_list<basic_json> ilist)
    {
        
        if (not is_array())
        {
            JSON_THROW(std::domain_error("cannot use insert() with " + type_name()));
        }

        
        if (pos.m_object != this)
        {
            JSON_THROW(std::domain_error("iterator does not fit current value"));
        }

        
        iterator result(this);
        result.m_it.array_iterator = m_value.array->insert(pos.m_it.array_iterator, ilist);
        return result;
    }

    
    void swap(reference other) noexcept (
        std::is_nothrow_move_constructible<value_t>::value and
        std::is_nothrow_move_assignable<value_t>::value and
        std::is_nothrow_move_constructible<json_value>::value and
        std::is_nothrow_move_assignable<json_value>::value
    )
    {
        std::swap(m_type, other.m_type);
        std::swap(m_value, other.m_value);
        assert_invariant();
    }

    
    void swap(array_t& other)
    {
        
        if (is_array())
        {
            std::swap(*(m_value.array), other);
        }
        else
        {
            JSON_THROW(std::domain_error("cannot use swap() with " + type_name()));
        }
    }

    
    void swap(object_t& other)
    {
        
        if (is_object())
        {
            std::swap(*(m_value.object), other);
        }
        else
        {
            JSON_THROW(std::domain_error("cannot use swap() with " + type_name()));
        }
    }

    
    void swap(string_t& other)
    {
        
        if (is_string())
        {
            std::swap(*(m_value.string), other);
        }
        else
        {
            JSON_THROW(std::domain_error("cannot use swap() with " + type_name()));
        }
    }

    

  public:
    
    
    

    
    

    
    friend bool operator==(const_reference lhs, const_reference rhs) noexcept
    {
        const auto lhs_type = lhs.type();
        const auto rhs_type = rhs.type();

        if (lhs_type == rhs_type)
        {
            switch (lhs_type)
            {
                case value_t::array:
                {
                    return *lhs.m_value.array == *rhs.m_value.array;
                }
                case value_t::object:
                {
                    return *lhs.m_value.object == *rhs.m_value.object;
                }
                case value_t::null:
                {
                    return true;
                }
                case value_t::string:
                {
                    return *lhs.m_value.string == *rhs.m_value.string;
                }
                case value_t::boolean:
                {
                    return lhs.m_value.boolean == rhs.m_value.boolean;
                }
                case value_t::number_integer:
                {
                    return lhs.m_value.number_integer == rhs.m_value.number_integer;
                }
                case value_t::number_unsigned:
                {
                    return lhs.m_value.number_unsigned == rhs.m_value.number_unsigned;
                }
                case value_t::number_float:
                {
                    return lhs.m_value.number_float == rhs.m_value.number_float;
                }
                default:
                {
                    return false;
                }
            }
        }
        else if (lhs_type == value_t::number_integer and rhs_type == value_t::number_float)
        {
            return static_cast<number_float_t>(lhs.m_value.number_integer) == rhs.m_value.number_float;
        }
        else if (lhs_type == value_t::number_float and rhs_type == value_t::number_integer)
        {
            return lhs.m_value.number_float == static_cast<number_float_t>(rhs.m_value.number_integer);
        }
        else if (lhs_type == value_t::number_unsigned and rhs_type == value_t::number_float)
        {
            return static_cast<number_float_t>(lhs.m_value.number_unsigned) == rhs.m_value.number_float;
        }
        else if (lhs_type == value_t::number_float and rhs_type == value_t::number_unsigned)
        {
            return lhs.m_value.number_float == static_cast<number_float_t>(rhs.m_value.number_unsigned);
        }
        else if (lhs_type == value_t::number_unsigned and rhs_type == value_t::number_integer)
        {
            return static_cast<number_integer_t>(lhs.m_value.number_unsigned) == rhs.m_value.number_integer;
        }
        else if (lhs_type == value_t::number_integer and rhs_type == value_t::number_unsigned)
        {
            return lhs.m_value.number_integer == static_cast<number_integer_t>(rhs.m_value.number_unsigned);
        }

        return false;
    }

    
    template<typename ScalarType, typename std::enable_if<
                 std::is_scalar<ScalarType>::value, int>::type = 0>
    friend bool operator==(const_reference lhs, const ScalarType rhs) noexcept
    {
        return (lhs == basic_json(rhs));
    }

    
    template<typename ScalarType, typename std::enable_if<
                 std::is_scalar<ScalarType>::value, int>::type = 0>
    friend bool operator==(const ScalarType lhs, const_reference rhs) noexcept
    {
        return (basic_json(lhs) == rhs);
    }

    
    friend bool operator!=(const_reference lhs, const_reference rhs) noexcept
    {
        return not (lhs == rhs);
    }

    
    template<typename ScalarType, typename std::enable_if<
                 std::is_scalar<ScalarType>::value, int>::type = 0>
    friend bool operator!=(const_reference lhs, const ScalarType rhs) noexcept
    {
        return (lhs != basic_json(rhs));
    }

    
    template<typename ScalarType, typename std::enable_if<
                 std::is_scalar<ScalarType>::value, int>::type = 0>
    friend bool operator!=(const ScalarType lhs, const_reference rhs) noexcept
    {
        return (basic_json(lhs) != rhs);
    }

    
    friend bool operator<(const_reference lhs, const_reference rhs) noexcept
    {
        const auto lhs_type = lhs.type();
        const auto rhs_type = rhs.type();

        if (lhs_type == rhs_type)
        {
            switch (lhs_type)
            {
                case value_t::array:
                {
                    return *lhs.m_value.array < *rhs.m_value.array;
                }
                case value_t::object:
                {
                    return *lhs.m_value.object < *rhs.m_value.object;
                }
                case value_t::null:
                {
                    return false;
                }
                case value_t::string:
                {
                    return *lhs.m_value.string < *rhs.m_value.string;
                }
                case value_t::boolean:
                {
                    return lhs.m_value.boolean < rhs.m_value.boolean;
                }
                case value_t::number_integer:
                {
                    return lhs.m_value.number_integer < rhs.m_value.number_integer;
                }
                case value_t::number_unsigned:
                {
                    return lhs.m_value.number_unsigned < rhs.m_value.number_unsigned;
                }
                case value_t::number_float:
                {
                    return lhs.m_value.number_float < rhs.m_value.number_float;
                }
                default:
                {
                    return false;
                }
            }
        }
        else if (lhs_type == value_t::number_integer and rhs_type == value_t::number_float)
        {
            return static_cast<number_float_t>(lhs.m_value.number_integer) < rhs.m_value.number_float;
        }
        else if (lhs_type == value_t::number_float and rhs_type == value_t::number_integer)
        {
            return lhs.m_value.number_float < static_cast<number_float_t>(rhs.m_value.number_integer);
        }
        else if (lhs_type == value_t::number_unsigned and rhs_type == value_t::number_float)
        {
            return static_cast<number_float_t>(lhs.m_value.number_unsigned) < rhs.m_value.number_float;
        }
        else if (lhs_type == value_t::number_float and rhs_type == value_t::number_unsigned)
        {
            return lhs.m_value.number_float < static_cast<number_float_t>(rhs.m_value.number_unsigned);
        }
        else if (lhs_type == value_t::number_integer and rhs_type == value_t::number_unsigned)
        {
            return lhs.m_value.number_integer < static_cast<number_integer_t>(rhs.m_value.number_unsigned);
        }
        else if (lhs_type == value_t::number_unsigned and rhs_type == value_t::number_integer)
        {
            return static_cast<number_integer_t>(lhs.m_value.number_unsigned) < rhs.m_value.number_integer;
        }

        
        
        
        return operator<(lhs_type, rhs_type);
    }

    
    friend bool operator<=(const_reference lhs, const_reference rhs) noexcept
    {
        return not (rhs < lhs);
    }

    
    friend bool operator>(const_reference lhs, const_reference rhs) noexcept
    {
        return not (lhs <= rhs);
    }

    
    friend bool operator>=(const_reference lhs, const_reference rhs) noexcept
    {
        return not (lhs < rhs);
    }

    


    
    
    

    
    

    
    friend std::ostream& operator<<(std::ostream& o, const basic_json& j)
    {
        
        const bool pretty_print = (o.width() > 0);
        const auto indentation = (pretty_print ? o.width() : 0);

        
        o.width(0);

        
        j.dump(o, pretty_print, static_cast<unsigned int>(indentation));

        return o;
    }

    
    friend std::ostream& operator>>(const basic_json& j, std::ostream& o)
    {
        return o << j;
    }

    


    
    
    

    
    

    
    template<class T, std::size_t N>
    static basic_json parse(T (&array)[N],
                            const parser_callback_t cb = nullptr)
    {
        
        return parse(std::begin(array), std::end(array), cb);
    }

    
    template<typename CharT, typename std::enable_if<
                 std::is_pointer<CharT>::value and
                 std::is_integral<typename std::remove_pointer<CharT>::type>::value and
                 sizeof(typename std::remove_pointer<CharT>::type) == 1, int>::type = 0>
    static basic_json parse(const CharT s,
                            const parser_callback_t cb = nullptr)
    {
        return parser(reinterpret_cast<const char*>(s), cb).parse();
    }

    
    static basic_json parse(std::istream& i,
                            const parser_callback_t cb = nullptr)
    {
        return parser(i, cb).parse();
    }

    
    static basic_json parse(std::istream&& i,
                            const parser_callback_t cb = nullptr)
    {
        return parser(i, cb).parse();
    }

    
    template<class IteratorType, typename std::enable_if<
                 std::is_base_of<
                     std::random_access_iterator_tag,
                     typename std::iterator_traits<IteratorType>::iterator_category>::value, int>::type = 0>
    static basic_json parse(IteratorType first, IteratorType last,
                            const parser_callback_t cb = nullptr)
    {
        
        
        assert(std::accumulate(first, last, std::pair<bool, int>(true, 0),
                               [&first](std::pair<bool, int> res, decltype(*first) val)
        {
            res.first &= (val == *(std::next(std::addressof(*first), res.second++)));
            return res;
        }).first);

        
        static_assert(sizeof(typename std::iterator_traits<IteratorType>::value_type) == 1,
                      "each element in the iterator range must have the size of 1 byte");

        
        
        if (std::distance(first, last) <= 0)
        {
            return parser("").parse();
        }

        return parser(first, last, cb).parse();
    }

    
    template<class ContiguousContainer, typename std::enable_if<
                 not std::is_pointer<ContiguousContainer>::value and
                 std::is_base_of<
                     std::random_access_iterator_tag,
                     typename std::iterator_traits<decltype(std::begin(std::declval<ContiguousContainer const>()))>::iterator_category>::value
                 , int>::type = 0>
    static basic_json parse(const ContiguousContainer& c,
                            const parser_callback_t cb = nullptr)
    {
        
        return parse(std::begin(c), std::end(c), cb);
    }

    
    friend std::istream& operator<<(basic_json& j, std::istream& i)
    {
        j = parser(i).parse();
        return i;
    }

    
    friend std::istream& operator>>(std::istream& i, basic_json& j)
    {
        j = parser(i).parse();
        return i;
    }

    

    
    
    

    
    

  private:
    template<typename T>
    static void add_to_vector(std::vector<uint8_t>& vec, size_t bytes, const T number)
    {
        assert(bytes == 1 or bytes == 2 or bytes == 4 or bytes == 8);

        switch (bytes)
        {
            case 8:
            {
                vec.push_back(static_cast<uint8_t>((static_cast<uint64_t>(number) >> 070) & 0xff));
                vec.push_back(static_cast<uint8_t>((static_cast<uint64_t>(number) >> 060) & 0xff));
                vec.push_back(static_cast<uint8_t>((static_cast<uint64_t>(number) >> 050) & 0xff));
                vec.push_back(static_cast<uint8_t>((static_cast<uint64_t>(number) >> 040) & 0xff));
                
            }

            case 4:
            {
                vec.push_back(static_cast<uint8_t>((number >> 030) & 0xff));
                vec.push_back(static_cast<uint8_t>((number >> 020) & 0xff));
                
            }

            case 2:
            {
                vec.push_back(static_cast<uint8_t>((number >> 010) & 0xff));
                
            }

            case 1:
            {
                vec.push_back(static_cast<uint8_t>(number & 0xff));
                break;
            }
        }
    }

    
    template<typename T>
    static T get_from_vector(const std::vector<uint8_t>& vec, const size_t current_index)
    {
        if (current_index + sizeof(T) + 1 > vec.size())
        {
            JSON_THROW(std::out_of_range("cannot read " + std::to_string(sizeof(T)) + " bytes from vector"));
        }

        T result;
        auto* ptr = reinterpret_cast<uint8_t*>(&result);
        for (size_t i = 0; i < sizeof(T); ++i)
        {
            *ptr++ = vec[current_index + sizeof(T) - i];
        }
        return result;
    }

    
    static void to_msgpack_internal(const basic_json& j, std::vector<uint8_t>& v)
    {
        switch (j.type())
        {
            case value_t::null:
            {
                
                v.push_back(0xc0);
                break;
            }

            case value_t::boolean:
            {
                
                v.push_back(j.m_value.boolean ? 0xc3 : 0xc2);
                break;
            }

            case value_t::number_integer:
            {
                if (j.m_value.number_integer >= 0)
                {
                    
                    
                    
                    
                    if (j.m_value.number_unsigned < 128)
                    {
                        
                        add_to_vector(v, 1, j.m_value.number_unsigned);
                    }
                    else if (j.m_value.number_unsigned <= std::numeric_limits<uint8_t>::max())
                    {
                        
                        v.push_back(0xcc);
                        add_to_vector(v, 1, j.m_value.number_unsigned);
                    }
                    else if (j.m_value.number_unsigned <= std::numeric_limits<uint16_t>::max())
                    {
                        
                        v.push_back(0xcd);
                        add_to_vector(v, 2, j.m_value.number_unsigned);
                    }
                    else if (j.m_value.number_unsigned <= std::numeric_limits<uint32_t>::max())
                    {
                        
                        v.push_back(0xce);
                        add_to_vector(v, 4, j.m_value.number_unsigned);
                    }
                    else if (j.m_value.number_unsigned <= std::numeric_limits<uint64_t>::max())
                    {
                        
                        v.push_back(0xcf);
                        add_to_vector(v, 8, j.m_value.number_unsigned);
                    }
                }
                else
                {
                    if (j.m_value.number_integer >= -32)
                    {
                        
                        add_to_vector(v, 1, j.m_value.number_integer);
                    }
                    else if (j.m_value.number_integer >= std::numeric_limits<int8_t>::min() and j.m_value.number_integer <= std::numeric_limits<int8_t>::max())
                    {
                        
                        v.push_back(0xd0);
                        add_to_vector(v, 1, j.m_value.number_integer);
                    }
                    else if (j.m_value.number_integer >= std::numeric_limits<int16_t>::min() and j.m_value.number_integer <= std::numeric_limits<int16_t>::max())
                    {
                        
                        v.push_back(0xd1);
                        add_to_vector(v, 2, j.m_value.number_integer);
                    }
                    else if (j.m_value.number_integer >= std::numeric_limits<int32_t>::min() and j.m_value.number_integer <= std::numeric_limits<int32_t>::max())
                    {
                        
                        v.push_back(0xd2);
                        add_to_vector(v, 4, j.m_value.number_integer);
                    }
                    else if (j.m_value.number_integer >= std::numeric_limits<int64_t>::min() and j.m_value.number_integer <= std::numeric_limits<int64_t>::max())
                    {
                        
                        v.push_back(0xd3);
                        add_to_vector(v, 8, j.m_value.number_integer);
                    }
                }
                break;
            }

            case value_t::number_unsigned:
            {
                if (j.m_value.number_unsigned < 128)
                {
                    
                    add_to_vector(v, 1, j.m_value.number_unsigned);
                }
                else if (j.m_value.number_unsigned <= std::numeric_limits<uint8_t>::max())
                {
                    
                    v.push_back(0xcc);
                    add_to_vector(v, 1, j.m_value.number_unsigned);
                }
                else if (j.m_value.number_unsigned <= std::numeric_limits<uint16_t>::max())
                {
                    
                    v.push_back(0xcd);
                    add_to_vector(v, 2, j.m_value.number_unsigned);
                }
                else if (j.m_value.number_unsigned <= std::numeric_limits<uint32_t>::max())
                {
                    
                    v.push_back(0xce);
                    add_to_vector(v, 4, j.m_value.number_unsigned);
                }
                else if (j.m_value.number_unsigned <= std::numeric_limits<uint64_t>::max())
                {
                    
                    v.push_back(0xcf);
                    add_to_vector(v, 8, j.m_value.number_unsigned);
                }
                break;
            }

            case value_t::number_float:
            {
                
                v.push_back(0xcb);
                const auto* helper = reinterpret_cast<const uint8_t*>(&(j.m_value.number_float));
                for (size_t i = 0; i < 8; ++i)
                {
                    v.push_back(helper[7 - i]);
                }
                break;
            }

            case value_t::string:
            {
                const auto N = j.m_value.string->size();
                if (N <= 31)
                {
                    
                    v.push_back(static_cast<uint8_t>(0xa0 | N));
                }
                else if (N <= 255)
                {
                    
                    v.push_back(0xd9);
                    add_to_vector(v, 1, N);
                }
                else if (N <= 65535)
                {
                    
                    v.push_back(0xda);
                    add_to_vector(v, 2, N);
                }
                else if (N <= 4294967295)
                {
                    
                    v.push_back(0xdb);
                    add_to_vector(v, 4, N);
                }

                
                std::copy(j.m_value.string->begin(), j.m_value.string->end(),
                          std::back_inserter(v));
                break;
            }

            case value_t::array:
            {
                const auto N = j.m_value.array->size();
                if (N <= 15)
                {
                    
                    v.push_back(static_cast<uint8_t>(0x90 | N));
                }
                else if (N <= 0xffff)
                {
                    
                    v.push_back(0xdc);
                    add_to_vector(v, 2, N);
                }
                else if (N <= 0xffffffff)
                {
                    
                    v.push_back(0xdd);
                    add_to_vector(v, 4, N);
                }

                
                for (const auto& el : *j.m_value.array)
                {
                    to_msgpack_internal(el, v);
                }
                break;
            }

            case value_t::object:
            {
                const auto N = j.m_value.object->size();
                if (N <= 15)
                {
                    
                    v.push_back(static_cast<uint8_t>(0x80 | (N & 0xf)));
                }
                else if (N <= 65535)
                {
                    
                    v.push_back(0xde);
                    add_to_vector(v, 2, N);
                }
                else if (N <= 4294967295)
                {
                    
                    v.push_back(0xdf);
                    add_to_vector(v, 4, N);
                }

                
                for (const auto& el : *j.m_value.object)
                {
                    to_msgpack_internal(el.first, v);
                    to_msgpack_internal(el.second, v);
                }
                break;
            }

            default:
            {
                break;
            }
        }
    }

    
    static void to_cbor_internal(const basic_json& j, std::vector<uint8_t>& v)
    {
        switch (j.type())
        {
            case value_t::null:
            {
                v.push_back(0xf6);
                break;
            }

            case value_t::boolean:
            {
                v.push_back(j.m_value.boolean ? 0xf5 : 0xf4);
                break;
            }

            case value_t::number_integer:
            {
                if (j.m_value.number_integer >= 0)
                {
                    
                    
                    
                    if (j.m_value.number_integer <= 0x17)
                    {
                        add_to_vector(v, 1, j.m_value.number_integer);
                    }
                    else if (j.m_value.number_integer <= std::numeric_limits<uint8_t>::max())
                    {
                        v.push_back(0x18);
                        
                        add_to_vector(v, 1, j.m_value.number_integer);
                    }
                    else if (j.m_value.number_integer <= std::numeric_limits<uint16_t>::max())
                    {
                        v.push_back(0x19);
                        
                        add_to_vector(v, 2, j.m_value.number_integer);
                    }
                    else if (j.m_value.number_integer <= std::numeric_limits<uint32_t>::max())
                    {
                        v.push_back(0x1a);
                        
                        add_to_vector(v, 4, j.m_value.number_integer);
                    }
                    else
                    {
                        v.push_back(0x1b);
                        
                        add_to_vector(v, 8, j.m_value.number_integer);
                    }
                }
                else
                {
                    
                    
                    const auto positive_number = -1 - j.m_value.number_integer;
                    if (j.m_value.number_integer >= -24)
                    {
                        v.push_back(static_cast<uint8_t>(0x20 + positive_number));
                    }
                    else if (positive_number <= std::numeric_limits<uint8_t>::max())
                    {
                        
                        v.push_back(0x38);
                        add_to_vector(v, 1, positive_number);
                    }
                    else if (positive_number <= std::numeric_limits<uint16_t>::max())
                    {
                        
                        v.push_back(0x39);
                        add_to_vector(v, 2, positive_number);
                    }
                    else if (positive_number <= std::numeric_limits<uint32_t>::max())
                    {
                        
                        v.push_back(0x3a);
                        add_to_vector(v, 4, positive_number);
                    }
                    else
                    {
                        
                        v.push_back(0x3b);
                        add_to_vector(v, 8, positive_number);
                    }
                }
                break;
            }

            case value_t::number_unsigned:
            {
                if (j.m_value.number_unsigned <= 0x17)
                {
                    v.push_back(static_cast<uint8_t>(j.m_value.number_unsigned));
                }
                else if (j.m_value.number_unsigned <= 0xff)
                {
                    v.push_back(0x18);
                    
                    add_to_vector(v, 1, j.m_value.number_unsigned);
                }
                else if (j.m_value.number_unsigned <= 0xffff)
                {
                    v.push_back(0x19);
                    
                    add_to_vector(v, 2, j.m_value.number_unsigned);
                }
                else if (j.m_value.number_unsigned <= 0xffffffff)
                {
                    v.push_back(0x1a);
                    
                    add_to_vector(v, 4, j.m_value.number_unsigned);
                }
                else if (j.m_value.number_unsigned <= 0xffffffffffffffff)
                {
                    v.push_back(0x1b);
                    
                    add_to_vector(v, 8, j.m_value.number_unsigned);
                }
                break;
            }

            case value_t::number_float:
            {
                
                v.push_back(0xfb);
                const auto* helper = reinterpret_cast<const uint8_t*>(&(j.m_value.number_float));
                for (size_t i = 0; i < 8; ++i)
                {
                    v.push_back(helper[7 - i]);
                }
                break;
            }

            case value_t::string:
            {
                const auto N = j.m_value.string->size();
                if (N <= 0x17)
                {
                    v.push_back(0x60 + static_cast<uint8_t>(N));  
                }
                else if (N <= 0xff)
                {
                    v.push_back(0x78);  
                    add_to_vector(v, 1, N);
                }
                else if (N <= 0xffff)
                {
                    v.push_back(0x79);  
                    add_to_vector(v, 2, N);
                }
                else if (N <= 0xffffffff)
                {
                    v.push_back(0x7a); 
                    add_to_vector(v, 4, N);
                }
                
                else if (N <= 0xffffffffffffffff)
                {
                    v.push_back(0x7b);  
                    add_to_vector(v, 8, N);
                }
                

                
                std::copy(j.m_value.string->begin(), j.m_value.string->end(),
                          std::back_inserter(v));
                break;
            }

            case value_t::array:
            {
                const auto N = j.m_value.array->size();
                if (N <= 0x17)
                {
                    v.push_back(0x80 + static_cast<uint8_t>(N));  
                }
                else if (N <= 0xff)
                {
                    v.push_back(0x98);  
                    add_to_vector(v, 1, N);
                }
                else if (N <= 0xffff)
                {
                    v.push_back(0x99);  
                    add_to_vector(v, 2, N);
                }
                else if (N <= 0xffffffff)
                {
                    v.push_back(0x9a);  
                    add_to_vector(v, 4, N);
                }
                
                else if (N <= 0xffffffffffffffff)
                {
                    v.push_back(0x9b);  
                    add_to_vector(v, 8, N);
                }
                

                
                for (const auto& el : *j.m_value.array)
                {
                    to_cbor_internal(el, v);
                }
                break;
            }

            case value_t::object:
            {
                const auto N = j.m_value.object->size();
                if (N <= 0x17)
                {
                    v.push_back(0xa0 + static_cast<uint8_t>(N));  
                }
                else if (N <= 0xff)
                {
                    v.push_back(0xb8);
                    add_to_vector(v, 1, N);  
                }
                else if (N <= 0xffff)
                {
                    v.push_back(0xb9);
                    add_to_vector(v, 2, N);  
                }
                else if (N <= 0xffffffff)
                {
                    v.push_back(0xba);
                    add_to_vector(v, 4, N);  
                }
                
                else if (N <= 0xffffffffffffffff)
                {
                    v.push_back(0xbb);
                    add_to_vector(v, 8, N);  
                }
                

                
                for (const auto& el : *j.m_value.object)
                {
                    to_cbor_internal(el.first, v);
                    to_cbor_internal(el.second, v);
                }
                break;
            }

            default:
            {
                break;
            }
        }
    }


    
    static void check_length(const size_t size, const size_t len, const size_t offset)
    {
        
        if (len > size or offset > size)
        {
            JSON_THROW(std::out_of_range("len out of range"));
        }

        
        if ((size > (std::numeric_limits<size_t>::max() - offset)))
        {
            JSON_THROW(std::out_of_range("len+offset out of range"));
        }

        
        if (len + offset > size)
        {
            JSON_THROW(std::out_of_range("len+offset out of range"));
        }
    }

    
    static basic_json from_msgpack_internal(const std::vector<uint8_t>& v, size_t& idx)
    {
        
        check_length(v.size(), 1, idx);

        
        const size_t current_idx = idx++;

        if (v[current_idx] <= 0xbf)
        {
            if (v[current_idx] <= 0x7f) 
            {
                return v[current_idx];
            }
            if (v[current_idx] <= 0x8f) 
            {
                basic_json result = value_t::object;
                const size_t len = v[current_idx] & 0x0f;
                for (size_t i = 0; i < len; ++i)
                {
                    std::string key = from_msgpack_internal(v, idx);
                    result[key] = from_msgpack_internal(v, idx);
                }
                return result;
            }
            else if (v[current_idx] <= 0x9f) 
            {
                basic_json result = value_t::array;
                const size_t len = v[current_idx] & 0x0f;
                for (size_t i = 0; i < len; ++i)
                {
                    result.push_back(from_msgpack_internal(v, idx));
                }
                return result;
            }
            else 
            {
                const size_t len = v[current_idx] & 0x1f;
                const size_t offset = current_idx + 1;
                idx += len; 
                check_length(v.size(), len, offset);
                return std::string(reinterpret_cast<const char*>(v.data()) + offset, len);
            }
        }
        else if (v[current_idx] >= 0xe0) 
        {
            return static_cast<int8_t>(v[current_idx]);
        }
        else
        {
            switch (v[current_idx])
            {
                case 0xc0: 
                {
                    return value_t::null;
                }

                case 0xc2: 
                {
                    return false;
                }

                case 0xc3: 
                {
                    return true;
                }

                case 0xca: 
                {
                    
                    float res;
                    for (size_t byte = 0; byte < sizeof(float); ++byte)
                    {
                        reinterpret_cast<uint8_t*>(&res)[sizeof(float) - byte - 1] = v.at(current_idx + 1 + byte);
                    }
                    idx += sizeof(float); 
                    return res;
                }

                case 0xcb: 
                {
                    
                    double res;
                    for (size_t byte = 0; byte < sizeof(double); ++byte)
                    {
                        reinterpret_cast<uint8_t*>(&res)[sizeof(double) - byte - 1] = v.at(current_idx + 1 + byte);
                    }
                    idx += sizeof(double); 
                    return res;
                }

                case 0xcc: 
                {
                    idx += 1; 
                    return get_from_vector<uint8_t>(v, current_idx);
                }

                case 0xcd: 
                {
                    idx += 2; 
                    return get_from_vector<uint16_t>(v, current_idx);
                }

                case 0xce: 
                {
                    idx += 4; 
                    return get_from_vector<uint32_t>(v, current_idx);
                }

                case 0xcf: 
                {
                    idx += 8; 
                    return get_from_vector<uint64_t>(v, current_idx);
                }

                case 0xd0: 
                {
                    idx += 1; 
                    return get_from_vector<int8_t>(v, current_idx);
                }

                case 0xd1: 
                {
                    idx += 2; 
                    return get_from_vector<int16_t>(v, current_idx);
                }

                case 0xd2: 
                {
                    idx += 4; 
                    return get_from_vector<int32_t>(v, current_idx);
                }

                case 0xd3: 
                {
                    idx += 8; 
                    return get_from_vector<int64_t>(v, current_idx);
                }

                case 0xd9: 
                {
                    const auto len = static_cast<size_t>(get_from_vector<uint8_t>(v, current_idx));
                    const size_t offset = current_idx + 2;
                    idx += len + 1; 
                    check_length(v.size(), len, offset);
                    return std::string(reinterpret_cast<const char*>(v.data()) + offset, len);
                }

                case 0xda: 
                {
                    const auto len = static_cast<size_t>(get_from_vector<uint16_t>(v, current_idx));
                    const size_t offset = current_idx + 3;
                    idx += len + 2; 
                    check_length(v.size(), len, offset);
                    return std::string(reinterpret_cast<const char*>(v.data()) + offset, len);
                }

                case 0xdb: 
                {
                    const auto len = static_cast<size_t>(get_from_vector<uint32_t>(v, current_idx));
                    const size_t offset = current_idx + 5;
                    idx += len + 4; 
                    check_length(v.size(), len, offset);
                    return std::string(reinterpret_cast<const char*>(v.data()) + offset, len);
                }

                case 0xdc: 
                {
                    basic_json result = value_t::array;
                    const auto len = static_cast<size_t>(get_from_vector<uint16_t>(v, current_idx));
                    idx += 2; 
                    for (size_t i = 0; i < len; ++i)
                    {
                        result.push_back(from_msgpack_internal(v, idx));
                    }
                    return result;
                }

                case 0xdd: 
                {
                    basic_json result = value_t::array;
                    const auto len = static_cast<size_t>(get_from_vector<uint32_t>(v, current_idx));
                    idx += 4; 
                    for (size_t i = 0; i < len; ++i)
                    {
                        result.push_back(from_msgpack_internal(v, idx));
                    }
                    return result;
                }

                case 0xde: 
                {
                    basic_json result = value_t::object;
                    const auto len = static_cast<size_t>(get_from_vector<uint16_t>(v, current_idx));
                    idx += 2; 
                    for (size_t i = 0; i < len; ++i)
                    {
                        std::string key = from_msgpack_internal(v, idx);
                        result[key] = from_msgpack_internal(v, idx);
                    }
                    return result;
                }

                case 0xdf: 
                {
                    basic_json result = value_t::object;
                    const auto len = static_cast<size_t>(get_from_vector<uint32_t>(v, current_idx));
                    idx += 4; 
                    for (size_t i = 0; i < len; ++i)
                    {
                        std::string key = from_msgpack_internal(v, idx);
                        result[key] = from_msgpack_internal(v, idx);
                    }
                    return result;
                }

                default:
                {
                    JSON_THROW(std::invalid_argument("error parsing a msgpack @ " + std::to_string(current_idx) + ": " + std::to_string(static_cast<int>(v[current_idx]))));
                }
            }
        }
    }

    
    static basic_json from_cbor_internal(const std::vector<uint8_t>& v, size_t& idx)
    {
        
        const size_t current_idx = idx++;

        switch (v.at(current_idx))
        {
            
            case 0x00:
            case 0x01:
            case 0x02:
            case 0x03:
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x07:
            case 0x08:
            case 0x09:
            case 0x0a:
            case 0x0b:
            case 0x0c:
            case 0x0d:
            case 0x0e:
            case 0x0f:
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
            {
                return v[current_idx];
            }

            case 0x18: 
            {
                idx += 1; 
                return get_from_vector<uint8_t>(v, current_idx);
            }

            case 0x19: 
            {
                idx += 2; 
                return get_from_vector<uint16_t>(v, current_idx);
            }

            case 0x1a: 
            {
                idx += 4; 
                return get_from_vector<uint32_t>(v, current_idx);
            }

            case 0x1b: 
            {
                idx += 8; 
                return get_from_vector<uint64_t>(v, current_idx);
            }

            
            case 0x20:
            case 0x21:
            case 0x22:
            case 0x23:
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
            case 0x29:
            case 0x2a:
            case 0x2b:
            case 0x2c:
            case 0x2d:
            case 0x2e:
            case 0x2f:
            case 0x30:
            case 0x31:
            case 0x32:
            case 0x33:
            case 0x34:
            case 0x35:
            case 0x36:
            case 0x37:
            {
                return static_cast<int8_t>(0x20 - 1 - v[current_idx]);
            }

            case 0x38: 
            {
                idx += 1; 
                
                return static_cast<number_integer_t>(-1) - get_from_vector<uint8_t>(v, current_idx);
            }

            case 0x39: 
            {
                idx += 2; 
                return static_cast<number_integer_t>(-1) - get_from_vector<uint16_t>(v, current_idx);
            }

            case 0x3a: 
            {
                idx += 4; 
                return static_cast<number_integer_t>(-1) - get_from_vector<uint32_t>(v, current_idx);
            }

            case 0x3b: 
            {
                idx += 8; 
                return static_cast<number_integer_t>(-1) - static_cast<number_integer_t>(get_from_vector<uint64_t>(v, current_idx));
            }

            
            case 0x60:
            case 0x61:
            case 0x62:
            case 0x63:
            case 0x64:
            case 0x65:
            case 0x66:
            case 0x67:
            case 0x68:
            case 0x69:
            case 0x6a:
            case 0x6b:
            case 0x6c:
            case 0x6d:
            case 0x6e:
            case 0x6f:
            case 0x70:
            case 0x71:
            case 0x72:
            case 0x73:
            case 0x74:
            case 0x75:
            case 0x76:
            case 0x77:
            {
                const auto len = static_cast<size_t>(v[current_idx] - 0x60);
                const size_t offset = current_idx + 1;
                idx += len; 
                check_length(v.size(), len, offset);
                return std::string(reinterpret_cast<const char*>(v.data()) + offset, len);
            }

            case 0x78: 
            {
                const auto len = static_cast<size_t>(get_from_vector<uint8_t>(v, current_idx));
                const size_t offset = current_idx + 2;
                idx += len + 1; 
                check_length(v.size(), len, offset);
                return std::string(reinterpret_cast<const char*>(v.data()) + offset, len);
            }

            case 0x79: 
            {
                const auto len = static_cast<size_t>(get_from_vector<uint16_t>(v, current_idx));
                const size_t offset = current_idx + 3;
                idx += len + 2; 
                check_length(v.size(), len, offset);
                return std::string(reinterpret_cast<const char*>(v.data()) + offset, len);
            }

            case 0x7a: 
            {
                const auto len = static_cast<size_t>(get_from_vector<uint32_t>(v, current_idx));
                const size_t offset = current_idx + 5;
                idx += len + 4; 
                check_length(v.size(), len, offset);
                return std::string(reinterpret_cast<const char*>(v.data()) + offset, len);
            }

            case 0x7b: 
            {
                const auto len = static_cast<size_t>(get_from_vector<uint64_t>(v, current_idx));
                const size_t offset = current_idx + 9;
                idx += len + 8; 
                check_length(v.size(), len, offset);
                return std::string(reinterpret_cast<const char*>(v.data()) + offset, len);
            }

            case 0x7f: 
            {
                std::string result;
                while (v.at(idx) != 0xff)
                {
                    string_t s = from_cbor_internal(v, idx);
                    result += s;
                }
                
                idx += 1;
                return result;
            }

            
            case 0x80:
            case 0x81:
            case 0x82:
            case 0x83:
            case 0x84:
            case 0x85:
            case 0x86:
            case 0x87:
            case 0x88:
            case 0x89:
            case 0x8a:
            case 0x8b:
            case 0x8c:
            case 0x8d:
            case 0x8e:
            case 0x8f:
            case 0x90:
            case 0x91:
            case 0x92:
            case 0x93:
            case 0x94:
            case 0x95:
            case 0x96:
            case 0x97:
            {
                basic_json result = value_t::array;
                const auto len = static_cast<size_t>(v[current_idx] - 0x80);
                for (size_t i = 0; i < len; ++i)
                {
                    result.push_back(from_cbor_internal(v, idx));
                }
                return result;
            }

            case 0x98: 
            {
                basic_json result = value_t::array;
                const auto len = static_cast<size_t>(get_from_vector<uint8_t>(v, current_idx));
                idx += 1; 
                for (size_t i = 0; i < len; ++i)
                {
                    result.push_back(from_cbor_internal(v, idx));
                }
                return result;
            }

            case 0x99: 
            {
                basic_json result = value_t::array;
                const auto len = static_cast<size_t>(get_from_vector<uint16_t>(v, current_idx));
                idx += 2; 
                for (size_t i = 0; i < len; ++i)
                {
                    result.push_back(from_cbor_internal(v, idx));
                }
                return result;
            }

            case 0x9a: 
            {
                basic_json result = value_t::array;
                const auto len = static_cast<size_t>(get_from_vector<uint32_t>(v, current_idx));
                idx += 4; 
                for (size_t i = 0; i < len; ++i)
                {
                    result.push_back(from_cbor_internal(v, idx));
                }
                return result;
            }

            case 0x9b: 
            {
                basic_json result = value_t::array;
                const auto len = static_cast<size_t>(get_from_vector<uint64_t>(v, current_idx));
                idx += 8; 
                for (size_t i = 0; i < len; ++i)
                {
                    result.push_back(from_cbor_internal(v, idx));
                }
                return result;
            }

            case 0x9f: 
            {
                basic_json result = value_t::array;
                while (v.at(idx) != 0xff)
                {
                    result.push_back(from_cbor_internal(v, idx));
                }
                
                idx += 1;
                return result;
            }

            
            case 0xa0:
            case 0xa1:
            case 0xa2:
            case 0xa3:
            case 0xa4:
            case 0xa5:
            case 0xa6:
            case 0xa7:
            case 0xa8:
            case 0xa9:
            case 0xaa:
            case 0xab:
            case 0xac:
            case 0xad:
            case 0xae:
            case 0xaf:
            case 0xb0:
            case 0xb1:
            case 0xb2:
            case 0xb3:
            case 0xb4:
            case 0xb5:
            case 0xb6:
            case 0xb7:
            {
                basic_json result = value_t::object;
                const auto len = static_cast<size_t>(v[current_idx] - 0xa0);
                for (size_t i = 0; i < len; ++i)
                {
                    std::string key = from_cbor_internal(v, idx);
                    result[key] = from_cbor_internal(v, idx);
                }
                return result;
            }

            case 0xb8: 
            {
                basic_json result = value_t::object;
                const auto len = static_cast<size_t>(get_from_vector<uint8_t>(v, current_idx));
                idx += 1; 
                for (size_t i = 0; i < len; ++i)
                {
                    std::string key = from_cbor_internal(v, idx);
                    result[key] = from_cbor_internal(v, idx);
                }
                return result;
            }

            case 0xb9: 
            {
                basic_json result = value_t::object;
                const auto len = static_cast<size_t>(get_from_vector<uint16_t>(v, current_idx));
                idx += 2; 
                for (size_t i = 0; i < len; ++i)
                {
                    std::string key = from_cbor_internal(v, idx);
                    result[key] = from_cbor_internal(v, idx);
                }
                return result;
            }

            case 0xba: 
            {
                basic_json result = value_t::object;
                const auto len = static_cast<size_t>(get_from_vector<uint32_t>(v, current_idx));
                idx += 4; 
                for (size_t i = 0; i < len; ++i)
                {
                    std::string key = from_cbor_internal(v, idx);
                    result[key] = from_cbor_internal(v, idx);
                }
                return result;
            }

            case 0xbb: 
            {
                basic_json result = value_t::object;
                const auto len = static_cast<size_t>(get_from_vector<uint64_t>(v, current_idx));
                idx += 8; 
                for (size_t i = 0; i < len; ++i)
                {
                    std::string key = from_cbor_internal(v, idx);
                    result[key] = from_cbor_internal(v, idx);
                }
                return result;
            }

            case 0xbf: 
            {
                basic_json result = value_t::object;
                while (v.at(idx) != 0xff)
                {
                    std::string key = from_cbor_internal(v, idx);
                    result[key] = from_cbor_internal(v, idx);
                }
                
                idx += 1;
                return result;
            }

            case 0xf4: 
            {
                return false;
            }

            case 0xf5: 
            {
                return true;
            }

            case 0xf6: 
            {
                return value_t::null;
            }

            case 0xf9: 
            {
                idx += 2; 

                
                
                
                
                
                
                
                const int half = (v.at(current_idx + 1) << 8) + v.at(current_idx + 2);
                const int exp = (half >> 10) & 0x1f;
                const int mant = half & 0x3ff;
                double val;
                if (exp == 0)
                {
                    val = std::ldexp(mant, -24);
                }
                else if (exp != 31)
                {
                    val = std::ldexp(mant + 1024, exp - 25);
                }
                else
                {
                    val = mant == 0 ? INFINITY : NAN;
                }
                return (half & 0x8000) != 0 ? -val : val;
            }

            case 0xfa: 
            {
                
                float res;
                for (size_t byte = 0; byte < sizeof(float); ++byte)
                {
                    reinterpret_cast<uint8_t*>(&res)[sizeof(float) - byte - 1] = v.at(current_idx + 1 + byte);
                }
                idx += sizeof(float); 
                return res;
            }

            case 0xfb: 
            {
                
                double res;
                for (size_t byte = 0; byte < sizeof(double); ++byte)
                {
                    reinterpret_cast<uint8_t*>(&res)[sizeof(double) - byte - 1] = v.at(current_idx + 1 + byte);
                }
                idx += sizeof(double); 
                return res;
            }

            default: 
            {
                JSON_THROW(std::invalid_argument("error parsing a CBOR @ " + std::to_string(current_idx) + ": " + std::to_string(static_cast<int>(v[current_idx]))));
            }
        }
    }

  public:
    
    static std::vector<uint8_t> to_msgpack(const basic_json& j)
    {
        std::vector<uint8_t> result;
        to_msgpack_internal(j, result);
        return result;
    }

    
    static basic_json from_msgpack(const std::vector<uint8_t>& v,
                                   const size_t start_index = 0)
    {
        size_t i = start_index;
        return from_msgpack_internal(v, i);
    }

    
    static std::vector<uint8_t> to_cbor(const basic_json& j)
    {
        std::vector<uint8_t> result;
        to_cbor_internal(j, result);
        return result;
    }

    
    static basic_json from_cbor(const std::vector<uint8_t>& v,
                                const size_t start_index = 0)
    {
        size_t i = start_index;
        return from_cbor_internal(v, i);
    }

    

    
    
    

    
    std::string type_name() const
    {
        {
            switch (m_type)
            {
                case value_t::null:
                    return "null";
                case value_t::object:
                    return "object";
                case value_t::array:
                    return "array";
                case value_t::string:
                    return "string";
                case value_t::boolean:
                    return "boolean";
                case value_t::discarded:
                    return "discarded";
                default:
                    return "number";
            }
        }
    }

  private:
    
    static std::size_t extra_space(const string_t& s) noexcept
    {
        return std::accumulate(s.begin(), s.end(), size_t{},
                               [](size_t res, typename string_t::value_type c)
        {
            switch (c)
            {
                case '"':
                case '\\':
                case '\b':
                case '\f':
                case '\n':
                case '\r':
                case '\t':
                {
                    
                    return res + 1;
                }

                default:
                {
                    if (c >= 0x00 and c <= 0x1f)
                    {
                        
                        return res + 5;
                    }

                    return res;
                }
            }
        });
    }

    
    static string_t escape_string(const string_t& s)
    {
        const auto space = extra_space(s);
        if (space == 0)
        {
            return s;
        }

        
        string_t result(s.size() + space, '\\');
        std::size_t pos = 0;

        for (const auto& c : s)
        {
            switch (c)
            {
                
                case '"':
                {
                    result[pos + 1] = '"';
                    pos += 2;
                    break;
                }

                
                case '\\':
                {
                    
                    pos += 2;
                    break;
                }

                
                case '\b':
                {
                    result[pos + 1] = 'b';
                    pos += 2;
                    break;
                }

                
                case '\f':
                {
                    result[pos + 1] = 'f';
                    pos += 2;
                    break;
                }

                
                case '\n':
                {
                    result[pos + 1] = 'n';
                    pos += 2;
                    break;
                }

                
                case '\r':
                {
                    result[pos + 1] = 'r';
                    pos += 2;
                    break;
                }

                
                case '\t':
                {
                    result[pos + 1] = 't';
                    pos += 2;
                    break;
                }

                default:
                {
                    if (c >= 0x00 and c <= 0x1f)
                    {
                        
                        
                        static const char hexify[16] =
                        {
                            '0', '1', '2', '3', '4', '5', '6', '7',
                            '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
                        };

                        
                        for (const char m :
                    { 'u', '0', '0', hexify[c >> 4], hexify[c & 0x0f]
                        })
                        {
                            result[++pos] = m;
                        }

                        ++pos;
                    }
                    else
                    {
                        
                        result[pos++] = c;
                    }
                    break;
                }
            }
        }

        return result;
    }


    
    struct numtostr
    {
      public:
        template<typename NumberType>
        numtostr(NumberType value)
        {
            x_write(value, std::is_integral<NumberType>());
        }

        const char* c_str() const
        {
            return m_buf.data();
        }

      private:
        
        std::array < char, 64 > m_buf{{}};

        template<typename NumberType>
        void x_write(NumberType x, std::true_type)
        {
            
            if (x == 0)
            {
                m_buf[0] = '0';
                return;
            }

            const bool is_negative = x < 0;
            size_t i = 0;

            
            while (x != 0 and i < m_buf.size() - 1)
            {
                const auto digit = std::labs(static_cast<long>(x % 10));
                m_buf[i++] = static_cast<char>('0' + digit);
                x /= 10;
            }

            
            assert(x == 0);

            if (is_negative)
            {
                
                assert(i < m_buf.size() - 2);
                m_buf[i++] = '-';
            }

            std::reverse(m_buf.begin(), m_buf.begin() + i);
        }

        template<typename NumberType>
        void x_write(NumberType x, std::false_type)
        {
            
            if (x == 0)
            {
                size_t i = 0;
                if (std::signbit(x))
                {
                    m_buf[i++] = '-';
                }
                m_buf[i++] = '0';
                m_buf[i++] = '.';
                m_buf[i] = '0';
                return;
            }

            
            static constexpr auto d = std::numeric_limits<NumberType>::digits10;

            
            const auto written_bytes = snprintf(m_buf.data(), m_buf.size(), "%.*g", d, x);

            
            assert(written_bytes > 0);
            
            assert(written_bytes < (int)m_buf.size());

            
            const auto loc = localeconv();
            assert(loc != nullptr);
            const char thousands_sep = !loc->thousands_sep ? '\0'
                                       : loc->thousands_sep[0];

            const char decimal_point = !loc->decimal_point ? '\0'
                                       : loc->decimal_point[0];

            
            if (thousands_sep != '\0')
            {
                const auto end = std::remove(m_buf.begin(), m_buf.begin() + written_bytes, thousands_sep);
                std::fill(end, m_buf.end(), '\0');
            }

            
            if (decimal_point != '\0' and decimal_point != '.')
            {
                for (auto& c : m_buf)
                {
                    if (c == decimal_point)
                    {
                        c = '.';
                        break;
                    }
                }
            }

            
            size_t i = 0;
            bool value_is_int_like = true;
            for (i = 0; i < m_buf.size(); ++i)
            {
                
                if (m_buf[i] == '\0')
                {
                    break;
                }

                
                value_is_int_like = value_is_int_like and m_buf[i] != '.' and
                                    m_buf[i] != 'e' and m_buf[i] != 'E';
            }

            if (value_is_int_like)
            {
                
                assert((i + 2) < m_buf.size());
                
                assert(m_buf[i] == '\0');
                assert(m_buf[i - 1] != '\0');

                
                m_buf[i] = '.';
                m_buf[i + 1] = '0';

                
                assert(m_buf[i + 2] == '\0');
            }
        }
    };


    
    void dump(std::ostream& o,
              const bool pretty_print,
              const unsigned int indent_step,
              const unsigned int current_indent = 0) const
    {
        
        unsigned int new_indent = current_indent;

        switch (m_type)
        {
            case value_t::object:
            {
                if (m_value.object->empty())
                {
                    o << "{}";
                    return;
                }

                o << "{";

                
                if (pretty_print)
                {
                    new_indent += indent_step;
                    o << "\n";
                }

                for (auto i = m_value.object->cbegin(); i != m_value.object->cend(); ++i)
                {
                    if (i != m_value.object->cbegin())
                    {
                        o << (pretty_print ? ",\n" : ",");
                    }
                    o << string_t(new_indent, ' ') << "\""
                      << escape_string(i->first) << "\":"
                      << (pretty_print ? " " : "");
                    i->second.dump(o, pretty_print, indent_step, new_indent);
                }

                
                if (pretty_print)
                {
                    new_indent -= indent_step;
                    o << "\n";
                }

                o << string_t(new_indent, ' ') + "}";
                return;
            }

            case value_t::array:
            {
                if (m_value.array->empty())
                {
                    o << "[]";
                    return;
                }

                o << "[";

                
                if (pretty_print)
                {
                    new_indent += indent_step;
                    o << "\n";
                }

                for (auto i = m_value.array->cbegin(); i != m_value.array->cend(); ++i)
                {
                    if (i != m_value.array->cbegin())
                    {
                        o << (pretty_print ? ",\n" : ",");
                    }
                    o << string_t(new_indent, ' ');
                    i->dump(o, pretty_print, indent_step, new_indent);
                }

                
                if (pretty_print)
                {
                    new_indent -= indent_step;
                    o << "\n";
                }

                o << string_t(new_indent, ' ') << "]";
                return;
            }

            case value_t::string:
            {
                o << string_t("\"") << escape_string(*m_value.string) << "\"";
                return;
            }

            case value_t::boolean:
            {
                o << (m_value.boolean ? "true" : "false");
                return;
            }

            case value_t::number_integer:
            {
                o << numtostr(m_value.number_integer).c_str();
                return;
            }

            case value_t::number_unsigned:
            {
                o << numtostr(m_value.number_unsigned).c_str();
                return;
            }

            case value_t::number_float:
            {
                o << numtostr(m_value.number_float).c_str();
                return;
            }

            case value_t::discarded:
            {
                o << "<discarded>";
                return;
            }

            case value_t::null:
            {
                o << "null";
                return;
            }
        }
    }

  private:
    
    
    

    
    value_t m_type = value_t::null;

    
    json_value m_value = {};


  private:
    
    
    

    
    class primitive_iterator_t
    {
      public:

        difference_type get_value() const noexcept
        {
            return m_it;
        }
        
        void set_begin() noexcept
        {
            m_it = begin_value;
        }

        
        void set_end() noexcept
        {
            m_it = end_value;
        }

        
        constexpr bool is_begin() const noexcept
        {
            return (m_it == begin_value);
        }

        
        constexpr bool is_end() const noexcept
        {
            return (m_it == end_value);
        }

        friend constexpr bool operator==(primitive_iterator_t lhs, primitive_iterator_t rhs) noexcept
        {
            return lhs.m_it == rhs.m_it;
        }

        friend constexpr bool operator!=(primitive_iterator_t lhs, primitive_iterator_t rhs) noexcept
        {
            return !(lhs == rhs);
        }

        friend constexpr bool operator<(primitive_iterator_t lhs, primitive_iterator_t rhs) noexcept
        {
            return lhs.m_it < rhs.m_it;
        }

        friend constexpr bool operator<=(primitive_iterator_t lhs, primitive_iterator_t rhs) noexcept
        {
            return lhs.m_it <= rhs.m_it;
        }

        friend constexpr bool operator>(primitive_iterator_t lhs, primitive_iterator_t rhs) noexcept
        {
            return lhs.m_it > rhs.m_it;
        }

        friend constexpr bool operator>=(primitive_iterator_t lhs, primitive_iterator_t rhs) noexcept
        {
            return lhs.m_it >= rhs.m_it;
        }

        primitive_iterator_t operator+(difference_type i)
        {
            auto result = *this;
            result += i;
            return result;
        }

        friend constexpr difference_type operator-(primitive_iterator_t lhs, primitive_iterator_t rhs) noexcept
        {
            return lhs.m_it - rhs.m_it;
        }

        friend std::ostream& operator<<(std::ostream& os, primitive_iterator_t it)
        {
            return os << it.m_it;
        }

        primitive_iterator_t& operator++()
        {
            ++m_it;
            return *this;
        }

        primitive_iterator_t operator++(int)
        {
            auto result = *this;
            m_it++;
            return result;
        }

        primitive_iterator_t& operator--()
        {
            --m_it;
            return *this;
        }

        primitive_iterator_t operator--(int)
        {
            auto result = *this;
            m_it--;
            return result;
        }

        primitive_iterator_t& operator+=(difference_type n)
        {
            m_it += n;
            return *this;
        }

        primitive_iterator_t& operator-=(difference_type n)
        {
            m_it -= n;
            return *this;
        }

      private:
        static constexpr difference_type begin_value = 0;
        static constexpr difference_type end_value = begin_value + 1;

        
        difference_type m_it = std::numeric_limits<std::ptrdiff_t>::denorm_min();
    };

    
    struct internal_iterator
    {
        
        typename object_t::iterator object_iterator;
        
        typename array_t::iterator array_iterator;
        
        primitive_iterator_t primitive_iterator;

        
        internal_iterator() noexcept
            : object_iterator(), array_iterator(), primitive_iterator()
        {}
    };

    
    template<typename IteratorType>
    class iteration_proxy
    {
      private:
        
        class iteration_proxy_internal
        {
          private:
            
            IteratorType anchor;
            
            size_t array_index = 0;

          public:
            explicit iteration_proxy_internal(IteratorType it) noexcept
                : anchor(it)
            {}

            
            iteration_proxy_internal& operator*()
            {
                return *this;
            }

            
            iteration_proxy_internal& operator++()
            {
                ++anchor;
                ++array_index;

                return *this;
            }

            
            bool operator!= (const iteration_proxy_internal& o) const
            {
                return anchor != o.anchor;
            }

            
            typename basic_json::string_t key() const
            {
                assert(anchor.m_object != nullptr);

                switch (anchor.m_object->type())
                {
                    
                    case value_t::array:
                    {
                        return std::to_string(array_index);
                    }

                    
                    case value_t::object:
                    {
                        return anchor.key();
                    }

                    
                    default:
                    {
                        return "";
                    }
                }
            }

            
            typename IteratorType::reference value() const
            {
                return anchor.value();
            }
        };

        
        typename IteratorType::reference container;

      public:
        
        explicit iteration_proxy(typename IteratorType::reference cont)
            : container(cont)
        {}

        
        iteration_proxy_internal begin() noexcept
        {
            return iteration_proxy_internal(container.begin());
        }

        
        iteration_proxy_internal end() noexcept
        {
            return iteration_proxy_internal(container.end());
        }
    };

  public:
    
    template<typename U>
    class iter_impl : public std::iterator<std::random_access_iterator_tag, U>
    {
        
        friend class basic_json;

        
        static_assert(std::is_same<U, basic_json>::value
                      or std::is_same<U, const basic_json>::value,
                      "iter_impl only accepts (const) basic_json");

      public:
        
        using value_type = typename basic_json::value_type;
        
        using difference_type = typename basic_json::difference_type;
        
        using pointer = typename std::conditional<std::is_const<U>::value,
              typename basic_json::const_pointer,
              typename basic_json::pointer>::type;
        
        using reference = typename std::conditional<std::is_const<U>::value,
              typename basic_json::const_reference,
              typename basic_json::reference>::type;
        
        using iterator_category = std::bidirectional_iterator_tag;

        
        iter_impl() = default;

        
        explicit iter_impl(pointer object) noexcept
            : m_object(object)
        {
            assert(m_object != nullptr);

            switch (m_object->m_type)
            {
                case basic_json::value_t::object:
                {
                    m_it.object_iterator = typename object_t::iterator();
                    break;
                }

                case basic_json::value_t::array:
                {
                    m_it.array_iterator = typename array_t::iterator();
                    break;
                }

                default:
                {
                    m_it.primitive_iterator = primitive_iterator_t();
                    break;
                }
            }
        }

        
        operator const_iterator() const
        {
            const_iterator ret;

            if (m_object)
            {
                ret.m_object = m_object;
                ret.m_it = m_it;
            }

            return ret;
        }

        
        iter_impl(const iter_impl& other) noexcept
            : m_object(other.m_object), m_it(other.m_it)
        {}

        
        iter_impl& operator=(iter_impl other) noexcept(
            std::is_nothrow_move_constructible<pointer>::value and
            std::is_nothrow_move_assignable<pointer>::value and
            std::is_nothrow_move_constructible<internal_iterator>::value and
            std::is_nothrow_move_assignable<internal_iterator>::value
        )
        {
            std::swap(m_object, other.m_object);
            std::swap(m_it, other.m_it);
            return *this;
        }

      private:
        
        void set_begin() noexcept
        {
            assert(m_object != nullptr);

            switch (m_object->m_type)
            {
                case basic_json::value_t::object:
                {
                    m_it.object_iterator = m_object->m_value.object->begin();
                    break;
                }

                case basic_json::value_t::array:
                {
                    m_it.array_iterator = m_object->m_value.array->begin();
                    break;
                }

                case basic_json::value_t::null:
                {
                    
                    m_it.primitive_iterator.set_end();
                    break;
                }

                default:
                {
                    m_it.primitive_iterator.set_begin();
                    break;
                }
            }
        }

        
        void set_end() noexcept
        {
            assert(m_object != nullptr);

            switch (m_object->m_type)
            {
                case basic_json::value_t::object:
                {
                    m_it.object_iterator = m_object->m_value.object->end();
                    break;
                }

                case basic_json::value_t::array:
                {
                    m_it.array_iterator = m_object->m_value.array->end();
                    break;
                }

                default:
                {
                    m_it.primitive_iterator.set_end();
                    break;
                }
            }
        }

      public:
        
        reference operator*() const
        {
            assert(m_object != nullptr);

            switch (m_object->m_type)
            {
                case basic_json::value_t::object:
                {
                    assert(m_it.object_iterator != m_object->m_value.object->end());
                    return m_it.object_iterator->second;
                }

                case basic_json::value_t::array:
                {
                    assert(m_it.array_iterator != m_object->m_value.array->end());
                    return *m_it.array_iterator;
                }

                case basic_json::value_t::null:
                {
                    JSON_THROW(std::out_of_range("cannot get value"));
                }

                default:
                {
                    if (m_it.primitive_iterator.is_begin())
                    {
                        return *m_object;
                    }

                    JSON_THROW(std::out_of_range("cannot get value"));
                }
            }
        }

        
        pointer operator->() const
        {
            assert(m_object != nullptr);

            switch (m_object->m_type)
            {
                case basic_json::value_t::object:
                {
                    assert(m_it.object_iterator != m_object->m_value.object->end());
                    return &(m_it.object_iterator->second);
                }

                case basic_json::value_t::array:
                {
                    assert(m_it.array_iterator != m_object->m_value.array->end());
                    return &*m_it.array_iterator;
                }

                default:
                {
                    if (m_it.primitive_iterator.is_begin())
                    {
                        return m_object;
                    }

                    JSON_THROW(std::out_of_range("cannot get value"));
                }
            }
        }

        
        iter_impl operator++(int)
        {
            auto result = *this;
            ++(*this);
            return result;
        }

        
        iter_impl& operator++()
        {
            assert(m_object != nullptr);

            switch (m_object->m_type)
            {
                case basic_json::value_t::object:
                {
                    std::advance(m_it.object_iterator, 1);
                    break;
                }

                case basic_json::value_t::array:
                {
                    std::advance(m_it.array_iterator, 1);
                    break;
                }

                default:
                {
                    ++m_it.primitive_iterator;
                    break;
                }
            }

            return *this;
        }

        
        iter_impl operator--(int)
        {
            auto result = *this;
            --(*this);
            return result;
        }

        
        iter_impl& operator--()
        {
            assert(m_object != nullptr);

            switch (m_object->m_type)
            {
                case basic_json::value_t::object:
                {
                    std::advance(m_it.object_iterator, -1);
                    break;
                }

                case basic_json::value_t::array:
                {
                    std::advance(m_it.array_iterator, -1);
                    break;
                }

                default:
                {
                    --m_it.primitive_iterator;
                    break;
                }
            }

            return *this;
        }

        
        bool operator==(const iter_impl& other) const
        {
            
            if (m_object != other.m_object)
            {
                JSON_THROW(std::domain_error("cannot compare iterators of different containers"));
            }

            assert(m_object != nullptr);

            switch (m_object->m_type)
            {
                case basic_json::value_t::object:
                {
                    return (m_it.object_iterator == other.m_it.object_iterator);
                }

                case basic_json::value_t::array:
                {
                    return (m_it.array_iterator == other.m_it.array_iterator);
                }

                default:
                {
                    return (m_it.primitive_iterator == other.m_it.primitive_iterator);
                }
            }
        }

        
        bool operator!=(const iter_impl& other) const
        {
            return not operator==(other);
        }

        
        bool operator<(const iter_impl& other) const
        {
            
            if (m_object != other.m_object)
            {
                JSON_THROW(std::domain_error("cannot compare iterators of different containers"));
            }

            assert(m_object != nullptr);

            switch (m_object->m_type)
            {
                case basic_json::value_t::object:
                {
                    JSON_THROW(std::domain_error("cannot compare order of object iterators"));
                }

                case basic_json::value_t::array:
                {
                    return (m_it.array_iterator < other.m_it.array_iterator);
                }

                default:
                {
                    return (m_it.primitive_iterator < other.m_it.primitive_iterator);
                }
            }
        }

        
        bool operator<=(const iter_impl& other) const
        {
            return not other.operator < (*this);
        }

        
        bool operator>(const iter_impl& other) const
        {
            return not operator<=(other);
        }

        
        bool operator>=(const iter_impl& other) const
        {
            return not operator<(other);
        }

        
        iter_impl& operator+=(difference_type i)
        {
            assert(m_object != nullptr);

            switch (m_object->m_type)
            {
                case basic_json::value_t::object:
                {
                    JSON_THROW(std::domain_error("cannot use offsets with object iterators"));
                }

                case basic_json::value_t::array:
                {
                    std::advance(m_it.array_iterator, i);
                    break;
                }

                default:
                {
                    m_it.primitive_iterator += i;
                    break;
                }
            }

            return *this;
        }

        
        iter_impl& operator-=(difference_type i)
        {
            return operator+=(-i);
        }

        
        iter_impl operator+(difference_type i)
        {
            auto result = *this;
            result += i;
            return result;
        }

        
        iter_impl operator-(difference_type i)
        {
            auto result = *this;
            result -= i;
            return result;
        }

        
        difference_type operator-(const iter_impl& other) const
        {
            assert(m_object != nullptr);

            switch (m_object->m_type)
            {
                case basic_json::value_t::object:
                {
                    JSON_THROW(std::domain_error("cannot use offsets with object iterators"));
                }

                case basic_json::value_t::array:
                {
                    return m_it.array_iterator - other.m_it.array_iterator;
                }

                default:
                {
                    return m_it.primitive_iterator - other.m_it.primitive_iterator;
                }
            }
        }

        
        reference operator[](difference_type n) const
        {
            assert(m_object != nullptr);

            switch (m_object->m_type)
            {
                case basic_json::value_t::object:
                {
                    JSON_THROW(std::domain_error("cannot use operator[] for object iterators"));
                }

                case basic_json::value_t::array:
                {
                    return *std::next(m_it.array_iterator, n);
                }

                case basic_json::value_t::null:
                {
                    JSON_THROW(std::out_of_range("cannot get value"));
                }

                default:
                {
                    if (m_it.primitive_iterator.get_value() == -n)
                    {
                        return *m_object;
                    }

                    JSON_THROW(std::out_of_range("cannot get value"));
                }
            }
        }

        
        typename object_t::key_type key() const
        {
            assert(m_object != nullptr);

            if (m_object->is_object())
            {
                return m_it.object_iterator->first;
            }

            JSON_THROW(std::domain_error("cannot use key() for non-object iterators"));
        }

        
        reference value() const
        {
            return operator*();
        }

      private:
        
        pointer m_object = nullptr;
        
        internal_iterator m_it = internal_iterator();
    };

    
    template<typename Base>
    class json_reverse_iterator : public std::reverse_iterator<Base>
    {
      public:
        
        using base_iterator = std::reverse_iterator<Base>;
        
        using reference = typename Base::reference;

        
        json_reverse_iterator(const typename base_iterator::iterator_type& it) noexcept
            : base_iterator(it)
        {}

        
        json_reverse_iterator(const base_iterator& it) noexcept
            : base_iterator(it)
        {}

        
        json_reverse_iterator operator++(int)
        {
            return base_iterator::operator++(1);
        }

        
        json_reverse_iterator& operator++()
        {
            base_iterator::operator++();
            return *this;
        }

        
        json_reverse_iterator operator--(int)
        {
            return base_iterator::operator--(1);
        }

        
        json_reverse_iterator& operator--()
        {
            base_iterator::operator--();
            return *this;
        }

        
        json_reverse_iterator& operator+=(difference_type i)
        {
            base_iterator::operator+=(i);
            return *this;
        }

        
        json_reverse_iterator operator+(difference_type i) const
        {
            auto result = *this;
            result += i;
            return result;
        }

        
        json_reverse_iterator operator-(difference_type i) const
        {
            auto result = *this;
            result -= i;
            return result;
        }

        
        difference_type operator-(const json_reverse_iterator& other) const
        {
            return this->base() - other.base();
        }

        
        reference operator[](difference_type n) const
        {
            return *(this->operator+(n));
        }

        
        typename object_t::key_type key() const
        {
            auto it = --this->base();
            return it.key();
        }

        
        reference value() const
        {
            auto it = --this->base();
            return it.operator * ();
        }
    };


  private:
    
    
    

    
    class lexer
    {
      public:
        
        enum class token_type
        {
            uninitialized,   
            literal_true,    
            literal_false,   
            literal_null,    
            value_string,    
            value_unsigned,  
            value_integer,   
            value_float,     
            begin_array,     
            begin_object,    
            end_array,       
            end_object,      
            name_separator,  
            value_separator, 
            parse_error,     
            end_of_input     
        };

        
        using lexer_char_t = unsigned char;

        
        lexer(const lexer_char_t* buff, const size_t len) noexcept
            : m_content(buff)
        {
            assert(m_content != nullptr);
            m_start = m_cursor = m_content;
            m_limit = m_content + len;
        }

        
        explicit lexer(std::istream& s)
            : m_stream(&s), m_line_buffer()
        {
            
            if (s.fail())
            {
                JSON_THROW(std::invalid_argument("stream error"));
            }

            
            fill_line_buffer();

            
            if (m_line_buffer.size() >= 3 and m_line_buffer.substr(0, 3) == "\xEF\xBB\xBF")
            {
                m_line_buffer[0] = ' ';
                m_line_buffer[1] = ' ';
                m_line_buffer[2] = ' ';
            }
        }

        
        lexer() = delete;
        lexer(const lexer&) = delete;
        lexer operator=(const lexer&) = delete;

        
        static string_t to_unicode(const std::size_t codepoint1,
                                   const std::size_t codepoint2 = 0)
        {
            
            std::size_t codepoint = codepoint1;

            
            if (codepoint1 >= 0xD800 and codepoint1 <= 0xDBFF)
            {
                
                if (codepoint2 >= 0xDC00 and codepoint2 <= 0xDFFF)
                {
                    codepoint =
                        
                        (codepoint1 << 10)
                        
                        + codepoint2
                        
                        
                        
                        - 0x35FDC00;
                }
                else
                {
                    JSON_THROW(std::invalid_argument("missing or wrong low surrogate"));
                }
            }

            string_t result;

            if (codepoint < 0x80)
            {
                
                result.append(1, static_cast<typename string_t::value_type>(codepoint));
            }
            else if (codepoint <= 0x7ff)
            {
                
                result.append(1, static_cast<typename string_t::value_type>(0xC0 | ((codepoint >> 6) & 0x1F)));
                result.append(1, static_cast<typename string_t::value_type>(0x80 | (codepoint & 0x3F)));
            }
            else if (codepoint <= 0xffff)
            {
                
                result.append(1, static_cast<typename string_t::value_type>(0xE0 | ((codepoint >> 12) & 0x0F)));
                result.append(1, static_cast<typename string_t::value_type>(0x80 | ((codepoint >> 6) & 0x3F)));
                result.append(1, static_cast<typename string_t::value_type>(0x80 | (codepoint & 0x3F)));
            }
            else if (codepoint <= 0x10ffff)
            {
                
                result.append(1, static_cast<typename string_t::value_type>(0xF0 | ((codepoint >> 18) & 0x07)));
                result.append(1, static_cast<typename string_t::value_type>(0x80 | ((codepoint >> 12) & 0x3F)));
                result.append(1, static_cast<typename string_t::value_type>(0x80 | ((codepoint >> 6) & 0x3F)));
                result.append(1, static_cast<typename string_t::value_type>(0x80 | (codepoint & 0x3F)));
            }
            else
            {
                JSON_THROW(std::out_of_range("code points above 0x10FFFF are invalid"));
            }

            return result;
        }

        
        static std::string token_type_name(const token_type t)
        {
            switch (t)
            {
                case token_type::uninitialized:
                    return "<uninitialized>";
                case token_type::literal_true:
                    return "true literal";
                case token_type::literal_false:
                    return "false literal";
                case token_type::literal_null:
                    return "null literal";
                case token_type::value_string:
                    return "string literal";
                case lexer::token_type::value_unsigned:
                case lexer::token_type::value_integer:
                case lexer::token_type::value_float:
                    return "number literal";
                case token_type::begin_array:
                    return "'['";
                case token_type::begin_object:
                    return "'{'";
                case token_type::end_array:
                    return "']'";
                case token_type::end_object:
                    return "'}'";
                case token_type::name_separator:
                    return "':'";
                case token_type::value_separator:
                    return "','";
                case token_type::parse_error:
                    return "<parse error>";
                case token_type::end_of_input:
                    return "end of input";
                default:
                {
                    
                    return "unknown token"; 
                }
            }
        }

        
        token_type scan()
        {
            while (true)
            {
                
                m_marker = nullptr;

                
                m_start = m_cursor;
                assert(m_start != nullptr);


                {
                    lexer_char_t yych;
                    unsigned int yyaccept = 0;
                    static const unsigned char yybm[] =
                    {
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,  32,  32,   0,   0,  32,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        160, 128,   0, 128, 128, 128, 128, 128,
                        128, 128, 128, 128, 128, 128, 128, 128,
                        192, 192, 192, 192, 192, 192, 192, 192,
                        192, 192, 128, 128, 128, 128, 128, 128,
                        128, 128, 128, 128, 128, 128, 128, 128,
                        128, 128, 128, 128, 128, 128, 128, 128,
                        128, 128, 128, 128, 128, 128, 128, 128,
                        128, 128, 128, 128,   0, 128, 128, 128,
                        128, 128, 128, 128, 128, 128, 128, 128,
                        128, 128, 128, 128, 128, 128, 128, 128,
                        128, 128, 128, 128, 128, 128, 128, 128,
                        128, 128, 128, 128, 128, 128, 128, 128,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                        0,   0,   0,   0,   0,   0,   0,   0,
                    };
                    if ((m_limit - m_cursor) < 5)
                    {
                        fill_line_buffer(5);    
                    }
                    yych = *m_cursor;
                    if (yybm[0 + yych] & 32)
                    {
                        goto basic_json_parser_6;
                    }
                    if (yych <= '[')
                    {
                        if (yych <= '-')
                        {
                            if (yych <= '"')
                            {
                                if (yych <= 0x00)
                                {
                                    goto basic_json_parser_2;
                                }
                                if (yych <= '!')
                                {
                                    goto basic_json_parser_4;
                                }
                                goto basic_json_parser_9;
                            }
                            else
                            {
                                if (yych <= '+')
                                {
                                    goto basic_json_parser_4;
                                }
                                if (yych <= ',')
                                {
                                    goto basic_json_parser_10;
                                }
                                goto basic_json_parser_12;
                            }
                        }
                        else
                        {
                            if (yych <= '9')
                            {
                                if (yych <= '/')
                                {
                                    goto basic_json_parser_4;
                                }
                                if (yych <= '0')
                                {
                                    goto basic_json_parser_13;
                                }
                                goto basic_json_parser_15;
                            }
                            else
                            {
                                if (yych <= ':')
                                {
                                    goto basic_json_parser_17;
                                }
                                if (yych <= 'Z')
                                {
                                    goto basic_json_parser_4;
                                }
                                goto basic_json_parser_19;
                            }
                        }
                    }
                    else
                    {
                        if (yych <= 'n')
                        {
                            if (yych <= 'e')
                            {
                                if (yych == ']')
                                {
                                    goto basic_json_parser_21;
                                }
                                goto basic_json_parser_4;
                            }
                            else
                            {
                                if (yych <= 'f')
                                {
                                    goto basic_json_parser_23;
                                }
                                if (yych <= 'm')
                                {
                                    goto basic_json_parser_4;
                                }
                                goto basic_json_parser_24;
                            }
                        }
                        else
                        {
                            if (yych <= 'z')
                            {
                                if (yych == 't')
                                {
                                    goto basic_json_parser_25;
                                }
                                goto basic_json_parser_4;
                            }
                            else
                            {
                                if (yych <= '{')
                                {
                                    goto basic_json_parser_26;
                                }
                                if (yych == '}')
                                {
                                    goto basic_json_parser_28;
                                }
                                goto basic_json_parser_4;
                            }
                        }
                    }
basic_json_parser_2:
                    ++m_cursor;
                    {
                        last_token_type = token_type::end_of_input;
                        break;
                    }
basic_json_parser_4:
                    ++m_cursor;
basic_json_parser_5:
                    {
                        last_token_type = token_type::parse_error;
                        break;
                    }
basic_json_parser_6:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
                    if (yybm[0 + yych] & 32)
                    {
                        goto basic_json_parser_6;
                    }
                    {
                        continue;
                    }
basic_json_parser_9:
                    yyaccept = 0;
                    yych = *(m_marker = ++m_cursor);
                    if (yych <= 0x1F)
                    {
                        goto basic_json_parser_5;
                    }
                    if (yych <= 0x7F)
                    {
                        goto basic_json_parser_31;
                    }
                    if (yych <= 0xC1)
                    {
                        goto basic_json_parser_5;
                    }
                    if (yych <= 0xF4)
                    {
                        goto basic_json_parser_31;
                    }
                    goto basic_json_parser_5;
basic_json_parser_10:
                    ++m_cursor;
                    {
                        last_token_type = token_type::value_separator;
                        break;
                    }
basic_json_parser_12:
                    yych = *++m_cursor;
                    if (yych <= '/')
                    {
                        goto basic_json_parser_5;
                    }
                    if (yych <= '0')
                    {
                        goto basic_json_parser_43;
                    }
                    if (yych <= '9')
                    {
                        goto basic_json_parser_45;
                    }
                    goto basic_json_parser_5;
basic_json_parser_13:
                    yyaccept = 1;
                    yych = *(m_marker = ++m_cursor);
                    if (yych <= '9')
                    {
                        if (yych == '.')
                        {
                            goto basic_json_parser_47;
                        }
                        if (yych >= '0')
                        {
                            goto basic_json_parser_48;
                        }
                    }
                    else
                    {
                        if (yych <= 'E')
                        {
                            if (yych >= 'E')
                            {
                                goto basic_json_parser_51;
                            }
                        }
                        else
                        {
                            if (yych == 'e')
                            {
                                goto basic_json_parser_51;
                            }
                        }
                    }
basic_json_parser_14:
                    {
                        last_token_type = token_type::value_unsigned;
                        break;
                    }
basic_json_parser_15:
                    yyaccept = 1;
                    m_marker = ++m_cursor;
                    if ((m_limit - m_cursor) < 3)
                    {
                        fill_line_buffer(3);    
                    }
                    yych = *m_cursor;
                    if (yybm[0 + yych] & 64)
                    {
                        goto basic_json_parser_15;
                    }
                    if (yych <= 'D')
                    {
                        if (yych == '.')
                        {
                            goto basic_json_parser_47;
                        }
                        goto basic_json_parser_14;
                    }
                    else
                    {
                        if (yych <= 'E')
                        {
                            goto basic_json_parser_51;
                        }
                        if (yych == 'e')
                        {
                            goto basic_json_parser_51;
                        }
                        goto basic_json_parser_14;
                    }
basic_json_parser_17:
                    ++m_cursor;
                    {
                        last_token_type = token_type::name_separator;
                        break;
                    }
basic_json_parser_19:
                    ++m_cursor;
                    {
                        last_token_type = token_type::begin_array;
                        break;
                    }
basic_json_parser_21:
                    ++m_cursor;
                    {
                        last_token_type = token_type::end_array;
                        break;
                    }
basic_json_parser_23:
                    yyaccept = 0;
                    yych = *(m_marker = ++m_cursor);
                    if (yych == 'a')
                    {
                        goto basic_json_parser_52;
                    }
                    goto basic_json_parser_5;
basic_json_parser_24:
                    yyaccept = 0;
                    yych = *(m_marker = ++m_cursor);
                    if (yych == 'u')
                    {
                        goto basic_json_parser_53;
                    }
                    goto basic_json_parser_5;
basic_json_parser_25:
                    yyaccept = 0;
                    yych = *(m_marker = ++m_cursor);
                    if (yych == 'r')
                    {
                        goto basic_json_parser_54;
                    }
                    goto basic_json_parser_5;
basic_json_parser_26:
                    ++m_cursor;
                    {
                        last_token_type = token_type::begin_object;
                        break;
                    }
basic_json_parser_28:
                    ++m_cursor;
                    {
                        last_token_type = token_type::end_object;
                        break;
                    }
basic_json_parser_30:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
basic_json_parser_31:
                    if (yybm[0 + yych] & 128)
                    {
                        goto basic_json_parser_30;
                    }
                    if (yych <= 0xE0)
                    {
                        if (yych <= '\\')
                        {
                            if (yych <= 0x1F)
                            {
                                goto basic_json_parser_32;
                            }
                            if (yych <= '"')
                            {
                                goto basic_json_parser_33;
                            }
                            goto basic_json_parser_35;
                        }
                        else
                        {
                            if (yych <= 0xC1)
                            {
                                goto basic_json_parser_32;
                            }
                            if (yych <= 0xDF)
                            {
                                goto basic_json_parser_36;
                            }
                            goto basic_json_parser_37;
                        }
                    }
                    else
                    {
                        if (yych <= 0xEF)
                        {
                            if (yych == 0xED)
                            {
                                goto basic_json_parser_39;
                            }
                            goto basic_json_parser_38;
                        }
                        else
                        {
                            if (yych <= 0xF0)
                            {
                                goto basic_json_parser_40;
                            }
                            if (yych <= 0xF3)
                            {
                                goto basic_json_parser_41;
                            }
                            if (yych <= 0xF4)
                            {
                                goto basic_json_parser_42;
                            }
                        }
                    }
basic_json_parser_32:
                    m_cursor = m_marker;
                    if (yyaccept <= 1)
                    {
                        if (yyaccept == 0)
                        {
                            goto basic_json_parser_5;
                        }
                        else
                        {
                            goto basic_json_parser_14;
                        }
                    }
                    else
                    {
                        if (yyaccept == 2)
                        {
                            goto basic_json_parser_44;
                        }
                        else
                        {
                            goto basic_json_parser_58;
                        }
                    }
basic_json_parser_33:
                    ++m_cursor;
                    {
                        last_token_type = token_type::value_string;
                        break;
                    }
basic_json_parser_35:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
                    if (yych <= 'e')
                    {
                        if (yych <= '/')
                        {
                            if (yych == '"')
                            {
                                goto basic_json_parser_30;
                            }
                            if (yych <= '.')
                            {
                                goto basic_json_parser_32;
                            }
                            goto basic_json_parser_30;
                        }
                        else
                        {
                            if (yych <= '\\')
                            {
                                if (yych <= '[')
                                {
                                    goto basic_json_parser_32;
                                }
                                goto basic_json_parser_30;
                            }
                            else
                            {
                                if (yych == 'b')
                                {
                                    goto basic_json_parser_30;
                                }
                                goto basic_json_parser_32;
                            }
                        }
                    }
                    else
                    {
                        if (yych <= 'q')
                        {
                            if (yych <= 'f')
                            {
                                goto basic_json_parser_30;
                            }
                            if (yych == 'n')
                            {
                                goto basic_json_parser_30;
                            }
                            goto basic_json_parser_32;
                        }
                        else
                        {
                            if (yych <= 's')
                            {
                                if (yych <= 'r')
                                {
                                    goto basic_json_parser_30;
                                }
                                goto basic_json_parser_32;
                            }
                            else
                            {
                                if (yych <= 't')
                                {
                                    goto basic_json_parser_30;
                                }
                                if (yych <= 'u')
                                {
                                    goto basic_json_parser_55;
                                }
                                goto basic_json_parser_32;
                            }
                        }
                    }
basic_json_parser_36:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
                    if (yych <= 0x7F)
                    {
                        goto basic_json_parser_32;
                    }
                    if (yych <= 0xBF)
                    {
                        goto basic_json_parser_30;
                    }
                    goto basic_json_parser_32;
basic_json_parser_37:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
                    if (yych <= 0x9F)
                    {
                        goto basic_json_parser_32;
                    }
                    if (yych <= 0xBF)
                    {
                        goto basic_json_parser_36;
                    }
                    goto basic_json_parser_32;
basic_json_parser_38:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
                    if (yych <= 0x7F)
                    {
                        goto basic_json_parser_32;
                    }
                    if (yych <= 0xBF)
                    {
                        goto basic_json_parser_36;
                    }
                    goto basic_json_parser_32;
basic_json_parser_39:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
                    if (yych <= 0x7F)
                    {
                        goto basic_json_parser_32;
                    }
                    if (yych <= 0x9F)
                    {
                        goto basic_json_parser_36;
                    }
                    goto basic_json_parser_32;
basic_json_parser_40:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
                    if (yych <= 0x8F)
                    {
                        goto basic_json_parser_32;
                    }
                    if (yych <= 0xBF)
                    {
                        goto basic_json_parser_38;
                    }
                    goto basic_json_parser_32;
basic_json_parser_41:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
                    if (yych <= 0x7F)
                    {
                        goto basic_json_parser_32;
                    }
                    if (yych <= 0xBF)
                    {
                        goto basic_json_parser_38;
                    }
                    goto basic_json_parser_32;
basic_json_parser_42:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
                    if (yych <= 0x7F)
                    {
                        goto basic_json_parser_32;
                    }
                    if (yych <= 0x8F)
                    {
                        goto basic_json_parser_38;
                    }
                    goto basic_json_parser_32;
basic_json_parser_43:
                    yyaccept = 2;
                    yych = *(m_marker = ++m_cursor);
                    if (yych <= '9')
                    {
                        if (yych == '.')
                        {
                            goto basic_json_parser_47;
                        }
                        if (yych >= '0')
                        {
                            goto basic_json_parser_48;
                        }
                    }
                    else
                    {
                        if (yych <= 'E')
                        {
                            if (yych >= 'E')
                            {
                                goto basic_json_parser_51;
                            }
                        }
                        else
                        {
                            if (yych == 'e')
                            {
                                goto basic_json_parser_51;
                            }
                        }
                    }
basic_json_parser_44:
                    {
                        last_token_type = token_type::value_integer;
                        break;
                    }
basic_json_parser_45:
                    yyaccept = 2;
                    m_marker = ++m_cursor;
                    if ((m_limit - m_cursor) < 3)
                    {
                        fill_line_buffer(3);    
                    }
                    yych = *m_cursor;
                    if (yych <= '9')
                    {
                        if (yych == '.')
                        {
                            goto basic_json_parser_47;
                        }
                        if (yych <= '/')
                        {
                            goto basic_json_parser_44;
                        }
                        goto basic_json_parser_45;
                    }
                    else
                    {
                        if (yych <= 'E')
                        {
                            if (yych <= 'D')
                            {
                                goto basic_json_parser_44;
                            }
                            goto basic_json_parser_51;
                        }
                        else
                        {
                            if (yych == 'e')
                            {
                                goto basic_json_parser_51;
                            }
                            goto basic_json_parser_44;
                        }
                    }
basic_json_parser_47:
                    yych = *++m_cursor;
                    if (yych <= '/')
                    {
                        goto basic_json_parser_32;
                    }
                    if (yych <= '9')
                    {
                        goto basic_json_parser_56;
                    }
                    goto basic_json_parser_32;
basic_json_parser_48:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
                    if (yych <= '/')
                    {
                        goto basic_json_parser_50;
                    }
                    if (yych <= '9')
                    {
                        goto basic_json_parser_48;
                    }
basic_json_parser_50:
                    {
                        last_token_type = token_type::parse_error;
                        break;
                    }
basic_json_parser_51:
                    yych = *++m_cursor;
                    if (yych <= ',')
                    {
                        if (yych == '+')
                        {
                            goto basic_json_parser_59;
                        }
                        goto basic_json_parser_32;
                    }
                    else
                    {
                        if (yych <= '-')
                        {
                            goto basic_json_parser_59;
                        }
                        if (yych <= '/')
                        {
                            goto basic_json_parser_32;
                        }
                        if (yych <= '9')
                        {
                            goto basic_json_parser_60;
                        }
                        goto basic_json_parser_32;
                    }
basic_json_parser_52:
                    yych = *++m_cursor;
                    if (yych == 'l')
                    {
                        goto basic_json_parser_62;
                    }
                    goto basic_json_parser_32;
basic_json_parser_53:
                    yych = *++m_cursor;
                    if (yych == 'l')
                    {
                        goto basic_json_parser_63;
                    }
                    goto basic_json_parser_32;
basic_json_parser_54:
                    yych = *++m_cursor;
                    if (yych == 'u')
                    {
                        goto basic_json_parser_64;
                    }
                    goto basic_json_parser_32;
basic_json_parser_55:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
                    if (yych <= '@')
                    {
                        if (yych <= '/')
                        {
                            goto basic_json_parser_32;
                        }
                        if (yych <= '9')
                        {
                            goto basic_json_parser_65;
                        }
                        goto basic_json_parser_32;
                    }
                    else
                    {
                        if (yych <= 'F')
                        {
                            goto basic_json_parser_65;
                        }
                        if (yych <= '`')
                        {
                            goto basic_json_parser_32;
                        }
                        if (yych <= 'f')
                        {
                            goto basic_json_parser_65;
                        }
                        goto basic_json_parser_32;
                    }
basic_json_parser_56:
                    yyaccept = 3;
                    m_marker = ++m_cursor;
                    if ((m_limit - m_cursor) < 3)
                    {
                        fill_line_buffer(3);    
                    }
                    yych = *m_cursor;
                    if (yych <= 'D')
                    {
                        if (yych <= '/')
                        {
                            goto basic_json_parser_58;
                        }
                        if (yych <= '9')
                        {
                            goto basic_json_parser_56;
                        }
                    }
                    else
                    {
                        if (yych <= 'E')
                        {
                            goto basic_json_parser_51;
                        }
                        if (yych == 'e')
                        {
                            goto basic_json_parser_51;
                        }
                    }
basic_json_parser_58:
                    {
                        last_token_type = token_type::value_float;
                        break;
                    }
basic_json_parser_59:
                    yych = *++m_cursor;
                    if (yych <= '/')
                    {
                        goto basic_json_parser_32;
                    }
                    if (yych >= ':')
                    {
                        goto basic_json_parser_32;
                    }
basic_json_parser_60:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
                    if (yych <= '/')
                    {
                        goto basic_json_parser_58;
                    }
                    if (yych <= '9')
                    {
                        goto basic_json_parser_60;
                    }
                    goto basic_json_parser_58;
basic_json_parser_62:
                    yych = *++m_cursor;
                    if (yych == 's')
                    {
                        goto basic_json_parser_66;
                    }
                    goto basic_json_parser_32;
basic_json_parser_63:
                    yych = *++m_cursor;
                    if (yych == 'l')
                    {
                        goto basic_json_parser_67;
                    }
                    goto basic_json_parser_32;
basic_json_parser_64:
                    yych = *++m_cursor;
                    if (yych == 'e')
                    {
                        goto basic_json_parser_69;
                    }
                    goto basic_json_parser_32;
basic_json_parser_65:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
                    if (yych <= '@')
                    {
                        if (yych <= '/')
                        {
                            goto basic_json_parser_32;
                        }
                        if (yych <= '9')
                        {
                            goto basic_json_parser_71;
                        }
                        goto basic_json_parser_32;
                    }
                    else
                    {
                        if (yych <= 'F')
                        {
                            goto basic_json_parser_71;
                        }
                        if (yych <= '`')
                        {
                            goto basic_json_parser_32;
                        }
                        if (yych <= 'f')
                        {
                            goto basic_json_parser_71;
                        }
                        goto basic_json_parser_32;
                    }
basic_json_parser_66:
                    yych = *++m_cursor;
                    if (yych == 'e')
                    {
                        goto basic_json_parser_72;
                    }
                    goto basic_json_parser_32;
basic_json_parser_67:
                    ++m_cursor;
                    {
                        last_token_type = token_type::literal_null;
                        break;
                    }
basic_json_parser_69:
                    ++m_cursor;
                    {
                        last_token_type = token_type::literal_true;
                        break;
                    }
basic_json_parser_71:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
                    if (yych <= '@')
                    {
                        if (yych <= '/')
                        {
                            goto basic_json_parser_32;
                        }
                        if (yych <= '9')
                        {
                            goto basic_json_parser_74;
                        }
                        goto basic_json_parser_32;
                    }
                    else
                    {
                        if (yych <= 'F')
                        {
                            goto basic_json_parser_74;
                        }
                        if (yych <= '`')
                        {
                            goto basic_json_parser_32;
                        }
                        if (yych <= 'f')
                        {
                            goto basic_json_parser_74;
                        }
                        goto basic_json_parser_32;
                    }
basic_json_parser_72:
                    ++m_cursor;
                    {
                        last_token_type = token_type::literal_false;
                        break;
                    }
basic_json_parser_74:
                    ++m_cursor;
                    if (m_limit <= m_cursor)
                    {
                        fill_line_buffer(1);    
                    }
                    yych = *m_cursor;
                    if (yych <= '@')
                    {
                        if (yych <= '/')
                        {
                            goto basic_json_parser_32;
                        }
                        if (yych <= '9')
                        {
                            goto basic_json_parser_30;
                        }
                        goto basic_json_parser_32;
                    }
                    else
                    {
                        if (yych <= 'F')
                        {
                            goto basic_json_parser_30;
                        }
                        if (yych <= '`')
                        {
                            goto basic_json_parser_32;
                        }
                        if (yych <= 'f')
                        {
                            goto basic_json_parser_30;
                        }
                        goto basic_json_parser_32;
                    }
                }

            }

            return last_token_type;
        }

        
        void fill_line_buffer(size_t n = 0)
        {
            
            assert(m_line_buffer.empty()
                   or m_content == reinterpret_cast<const lexer_char_t*>(m_line_buffer.data()));

            
            assert(m_line_buffer.empty()
                   or m_limit == m_content + m_line_buffer.size());

            
            assert(m_content <= m_start);
            assert(m_start <= m_cursor);
            assert(m_cursor <= m_limit);
            assert(m_marker == nullptr or m_marker  <= m_limit);

            
            const auto num_processed_chars = static_cast<size_t>(m_start - m_content);
            
            const auto offset_marker = (m_marker == nullptr) ? 0 : m_marker - m_start;
            
            const auto offset_cursor = m_cursor - m_start;

            
            if (m_stream == nullptr or m_stream->eof())
            {
                
                
                
                m_line_buffer.assign(m_start, m_limit);

                
                
                m_line_buffer.append(1, '\x00');
                if (n > 0)
                {
                    m_line_buffer.append(n - 1, '\x01');
                }
            }
            else
            {
                
                m_line_buffer.erase(0, num_processed_chars);
                
                m_line_buffer_tmp.clear();
                std::getline(*m_stream, m_line_buffer_tmp, '\n');

                
                m_line_buffer += m_line_buffer_tmp;
                m_line_buffer.push_back('\n');
            }

            
            m_content = reinterpret_cast<const lexer_char_t*>(m_line_buffer.data());
            assert(m_content != nullptr);
            m_start  = m_content;
            m_marker = m_start + offset_marker;
            m_cursor = m_start + offset_cursor;
            m_limit  = m_start + m_line_buffer.size();
        }

        
        string_t get_token_string() const
        {
            assert(m_start != nullptr);
            return string_t(reinterpret_cast<typename string_t::const_pointer>(m_start),
                            static_cast<size_t>(m_cursor - m_start));
        }

        
        string_t get_string() const
        {
            assert(m_cursor - m_start >= 2);

            string_t result;
            result.reserve(static_cast<size_t>(m_cursor - m_start - 2));

            
            for (const lexer_char_t* i = m_start + 1; i < m_cursor - 1; ++i)
            {
                
                auto e = std::find(i, m_cursor - 1, '\\');
                if (e != i)
                {
                    
                    for (auto k = i; k < e; k++)
                    {
                        result.push_back(static_cast<typename string_t::value_type>(*k));
                    }
                    i = e - 1; 
                }
                else
                {
                    
                    
                    ++i;

                    switch (*i)
                    {
                        
                        case 't':
                        {
                            result += "\t";
                            break;
                        }
                        case 'b':
                        {
                            result += "\b";
                            break;
                        }
                        case 'f':
                        {
                            result += "\f";
                            break;
                        }
                        case 'n':
                        {
                            result += "\n";
                            break;
                        }
                        case 'r':
                        {
                            result += "\r";
                            break;
                        }
                        case '\\':
                        {
                            result += "\\";
                            break;
                        }
                        case '/':
                        {
                            result += "/";
                            break;
                        }
                        case '"':
                        {
                            result += "\"";
                            break;
                        }

                        
                        case 'u':
                        {
                            
                            auto codepoint = std::strtoul(std::string(reinterpret_cast<typename string_t::const_pointer>(i + 1),
                                                          4).c_str(), nullptr, 16);

                            
                            if (codepoint >= 0xD800 and codepoint <= 0xDBFF)
                            {
                                
                                if ((i + 6 >= m_limit) or * (i + 5) != '\\' or * (i + 6) != 'u')
                                {
                                    JSON_THROW(std::invalid_argument("missing low surrogate"));
                                }

                                
                                auto codepoint2 = std::strtoul(std::string(reinterpret_cast<typename string_t::const_pointer>
                                                               (i + 7), 4).c_str(), nullptr, 16);
                                result += to_unicode(codepoint, codepoint2);
                                
                                i += 10;
                            }
                            else if (codepoint >= 0xDC00 and codepoint <= 0xDFFF)
                            {
                                
                                JSON_THROW(std::invalid_argument("missing high surrogate"));
                            }
                            else
                            {
                                
                                result += to_unicode(codepoint);
                                
                                i += 4;
                            }
                            break;
                        }
                    }
                }
            }

            return result;
        }


        
        struct strtonum
        {
          public:
            strtonum(const char* start, const char* end)
                : m_start(start), m_end(end)
            {}

            
            template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
            bool to(T& val) const
            {
                return parse(val, std::is_integral<T>());
            }

          private:
            const char* const m_start = nullptr;
            const char* const m_end = nullptr;

            

            
            
            static void strtof(float& f, const char* str, char** endptr)
            {
                f = std::strtof(str, endptr);
            }

            static void strtof(double& f, const char* str, char** endptr)
            {
                f = std::strtod(str, endptr);
            }

            static void strtof(long double& f, const char* str, char** endptr)
            {
                f = std::strtold(str, endptr);
            }

            template<typename T>
            bool parse(T& value, std::false_type) const
            {
                
                
                
                std::string tempstr;
                std::array<char, 64> buf;
                const size_t len = static_cast<size_t>(m_end - m_start);

                
                assert(len > 0);

                
                
                
                const auto loc = localeconv();
                assert(loc != nullptr);
                const char decimal_point_char = (loc->decimal_point == nullptr) ? '.' : loc->decimal_point[0];

                const char* data = m_start;

                if (decimal_point_char != '.')
                {
                    const size_t ds_pos = static_cast<size_t>(std::find(m_start, m_end, '.') - m_start);

                    if (ds_pos != len)
                    {
                        
                        
                        
                        if ((len + 1) < buf.size())
                        {
                            std::copy(m_start, m_end, buf.begin());
                            buf[len] = 0;
                            buf[ds_pos] = decimal_point_char;
                            data = buf.data();
                        }
                        else
                        {
                            tempstr.assign(m_start, m_end);
                            tempstr[ds_pos] = decimal_point_char;
                            data = tempstr.c_str();
                        }
                    }
                }

                char* endptr = nullptr;
                value = 0;
                
                strtof(value, data, &endptr);

                
                
                const bool ok = (endptr == (data + len));

                if (ok and (value == 0.0) and (*data == '-'))
                {
                    
                    value = -0.0;
                }

                return ok;
            }

            

            signed long long parse_integral(char** endptr, std::true_type) const
            {
                return std::strtoll(m_start, endptr, 10);
            }

            unsigned long long parse_integral(char** endptr, std::false_type) const
            {
                return std::strtoull(m_start, endptr, 10);
            }

            template<typename T>
            bool parse(T& value, std::true_type) const
            {
                char* endptr = nullptr;
                errno = 0; 
                const auto x = parse_integral(&endptr, std::is_signed<T>());

                
                static_assert(std::is_signed<T>() == std::is_signed<decltype(x)>(), "");

                value = static_cast<T>(x);

                return (x == static_cast<decltype(x)>(value)) 
                       and (x < 0) == (value < 0)             
                       
                       and (errno == 0)                       
                       and (m_start < m_end)                  
                       and (endptr == m_end);                 
            }
        };

        
        bool get_number(basic_json& result, const token_type token) const
        {
            assert(m_start != nullptr);
            assert(m_start < m_cursor);
            assert((token == token_type::value_unsigned) or
                   (token == token_type::value_integer) or
                   (token == token_type::value_float));

            strtonum num_converter(reinterpret_cast<const char*>(m_start),
                                   reinterpret_cast<const char*>(m_cursor));

            switch (token)
            {
                case lexer::token_type::value_unsigned:
                {
                    number_unsigned_t val;
                    if (num_converter.to(val))
                    {
                        
                        result.m_type = value_t::number_unsigned;
                        result.m_value = val;
                        return true;
                    }
                    break;
                }

                case lexer::token_type::value_integer:
                {
                    number_integer_t val;
                    if (num_converter.to(val))
                    {
                        
                        result.m_type = value_t::number_integer;
                        result.m_value = val;
                        return true;
                    }
                    break;
                }

                default:
                {
                    break;
                }
            }

            
            
            number_float_t val;
            if (num_converter.to(val))
            {
                
                result.m_type = value_t::number_float;
                result.m_value = val;

                
                if (not std::isfinite(result.m_value.number_float))
                {
                    result.m_type  = value_t::null;
                    result.m_value = basic_json::json_value();
                }

                return true;
            }

            
            return false;
        }

      private:
        
        std::istream* m_stream = nullptr;
        
        string_t m_line_buffer {};
        
        string_t m_line_buffer_tmp {};
        
        const lexer_char_t* m_content = nullptr;
        
        const lexer_char_t* m_start = nullptr;
        
        const lexer_char_t* m_marker = nullptr;
        
        const lexer_char_t* m_cursor = nullptr;
        
        const lexer_char_t* m_limit = nullptr;
        
        token_type last_token_type = token_type::end_of_input;
    };

    
    class parser
    {
      public:
        
        parser(const char* buff, const parser_callback_t cb = nullptr)
            : callback(cb),
              m_lexer(reinterpret_cast<const typename lexer::lexer_char_t*>(buff), std::strlen(buff))
        {}

        
        parser(std::istream& is, const parser_callback_t cb = nullptr)
            : callback(cb), m_lexer(is)
        {}

        
        template<class IteratorType, typename std::enable_if<
                     std::is_same<typename std::iterator_traits<IteratorType>::iterator_category, std::random_access_iterator_tag>::value
                     , int>::type
                 = 0>
        parser(IteratorType first, IteratorType last, const parser_callback_t cb = nullptr)
            : callback(cb),
              m_lexer(reinterpret_cast<const typename lexer::lexer_char_t*>(&(*first)),
                      static_cast<size_t>(std::distance(first, last)))
        {}

        
        basic_json parse()
        {
            
            get_token();

            basic_json result = parse_internal(true);
            result.assert_invariant();

            expect(lexer::token_type::end_of_input);

            
            
            return result.is_discarded() ? basic_json() : std::move(result);
        }

      private:
        
        basic_json parse_internal(bool keep)
        {
            auto result = basic_json(value_t::discarded);

            switch (last_token)
            {
                case lexer::token_type::begin_object:
                {
                    if (keep and (not callback
                                  or ((keep = callback(depth++, parse_event_t::object_start, result)) != 0)))
                    {
                        
                        result.m_type = value_t::object;
                        result.m_value = value_t::object;
                    }

                    
                    get_token();

                    
                    if (last_token == lexer::token_type::end_object)
                    {
                        get_token();
                        if (keep and callback and not callback(--depth, parse_event_t::object_end, result))
                        {
                            result = basic_json(value_t::discarded);
                        }
                        return result;
                    }

                    
                    unexpect(lexer::token_type::value_separator);

                    
                    do
                    {
                        
                        if (last_token == lexer::token_type::value_separator)
                        {
                            get_token();
                        }

                        
                        expect(lexer::token_type::value_string);
                        const auto key = m_lexer.get_string();

                        bool keep_tag = false;
                        if (keep)
                        {
                            if (callback)
                            {
                                basic_json k(key);
                                keep_tag = callback(depth, parse_event_t::key, k);
                            }
                            else
                            {
                                keep_tag = true;
                            }
                        }

                        
                        get_token();
                        expect(lexer::token_type::name_separator);

                        
                        get_token();
                        auto value = parse_internal(keep);
                        if (keep and keep_tag and not value.is_discarded())
                        {
                            result[key] = std::move(value);
                        }
                    }
                    while (last_token == lexer::token_type::value_separator);

                    
                    expect(lexer::token_type::end_object);
                    get_token();
                    if (keep and callback and not callback(--depth, parse_event_t::object_end, result))
                    {
                        result = basic_json(value_t::discarded);
                    }

                    return result;
                }

                case lexer::token_type::begin_array:
                {
                    if (keep and (not callback
                                  or ((keep = callback(depth++, parse_event_t::array_start, result)) != 0)))
                    {
                        
                        result.m_type = value_t::array;
                        result.m_value = value_t::array;
                    }

                    
                    get_token();

                    
                    if (last_token == lexer::token_type::end_array)
                    {
                        get_token();
                        if (callback and not callback(--depth, parse_event_t::array_end, result))
                        {
                            result = basic_json(value_t::discarded);
                        }
                        return result;
                    }

                    
                    unexpect(lexer::token_type::value_separator);

                    
                    do
                    {
                        
                        if (last_token == lexer::token_type::value_separator)
                        {
                            get_token();
                        }

                        
                        auto value = parse_internal(keep);
                        if (keep and not value.is_discarded())
                        {
                            result.push_back(std::move(value));
                        }
                    }
                    while (last_token == lexer::token_type::value_separator);

                    
                    expect(lexer::token_type::end_array);
                    get_token();
                    if (keep and callback and not callback(--depth, parse_event_t::array_end, result))
                    {
                        result = basic_json(value_t::discarded);
                    }

                    return result;
                }

                case lexer::token_type::literal_null:
                {
                    get_token();
                    result.m_type = value_t::null;
                    break;
                }

                case lexer::token_type::value_string:
                {
                    const auto s = m_lexer.get_string();
                    get_token();
                    result = basic_json(s);
                    break;
                }

                case lexer::token_type::literal_true:
                {
                    get_token();
                    result.m_type = value_t::boolean;
                    result.m_value = true;
                    break;
                }

                case lexer::token_type::literal_false:
                {
                    get_token();
                    result.m_type = value_t::boolean;
                    result.m_value = false;
                    break;
                }

                case lexer::token_type::value_unsigned:
                case lexer::token_type::value_integer:
                case lexer::token_type::value_float:
                {
                    m_lexer.get_number(result, last_token);
                    get_token();
                    break;
                }

                default:
                {
                    
                    unexpect(last_token);
                }
            }

            if (keep and callback and not callback(depth, parse_event_t::value, result))
            {
                result = basic_json(value_t::discarded);
            }
            return result;
        }

        
        typename lexer::token_type get_token()
        {
            last_token = m_lexer.scan();
            return last_token;
        }

        void expect(typename lexer::token_type t) const
        {
            if (t != last_token)
            {
                std::string error_msg = "parse error - unexpected ";
                error_msg += (last_token == lexer::token_type::parse_error ? ("'" +  m_lexer.get_token_string() +
                              "'") :
                              lexer::token_type_name(last_token));
                error_msg += "; expected " + lexer::token_type_name(t);
                JSON_THROW(std::invalid_argument(error_msg));
            }
        }

        void unexpect(typename lexer::token_type t) const
        {
            if (t == last_token)
            {
                std::string error_msg = "parse error - unexpected ";
                error_msg += (last_token == lexer::token_type::parse_error ? ("'" +  m_lexer.get_token_string() +
                              "'") :
                              lexer::token_type_name(last_token));
                JSON_THROW(std::invalid_argument(error_msg));
            }
        }

      private:
        
        int depth = 0;
        
        const parser_callback_t callback = nullptr;
        
        typename lexer::token_type last_token = lexer::token_type::uninitialized;
        
        lexer m_lexer;
    };

  public:
    
    class json_pointer
    {
        
        friend class basic_json;

      public:
        
        explicit json_pointer(const std::string& s = "")
            : reference_tokens(split(s))
        {}

        
        std::string to_string() const noexcept
        {
            return std::accumulate(reference_tokens.begin(),
                                   reference_tokens.end(), std::string{},
                                   [](const std::string & a, const std::string & b)
            {
                return a + "/" + escape(b);
            });
        }

        
        operator std::string() const
        {
            return to_string();
        }

      private:
        
        std::string pop_back()
        {
            if (is_root())
            {
                JSON_THROW(std::domain_error("JSON pointer has no parent"));
            }

            auto last = reference_tokens.back();
            reference_tokens.pop_back();
            return last;
        }

        
        bool is_root() const
        {
            return reference_tokens.empty();
        }

        json_pointer top() const
        {
            if (is_root())
            {
                JSON_THROW(std::domain_error("JSON pointer has no parent"));
            }

            json_pointer result = *this;
            result.reference_tokens = {reference_tokens[0]};
            return result;
        }

        
        reference get_and_create(reference j) const
        {
            pointer result = &j;

            
            
            for (const auto& reference_token : reference_tokens)
            {
                switch (result->m_type)
                {
                    case value_t::null:
                    {
                        if (reference_token == "0")
                        {
                            
                            result = &result->operator[](0);
                        }
                        else
                        {
                            
                            result = &result->operator[](reference_token);
                        }
                        break;
                    }

                    case value_t::object:
                    {
                        
                        result = &result->operator[](reference_token);
                        break;
                    }

                    case value_t::array:
                    {
                        
                        result = &result->operator[](static_cast<size_type>(std::stoi(reference_token)));
                        break;
                    }

                    
                    default:
                    {
                        JSON_THROW(std::domain_error("invalid value to unflatten"));
                    }
                }
            }

            return *result;
        }

        
        reference get_unchecked(pointer ptr) const
        {
            for (const auto& reference_token : reference_tokens)
            {
                
                if (ptr->m_type == value_t::null)
                {
                    
                    const bool nums = std::all_of(reference_token.begin(),
                                                  reference_token.end(),
                                                  [](const char x)
                    {
                        return std::isdigit(x);
                    });

                    
                    
                    if (nums or reference_token == "-")
                    {
                        *ptr = value_t::array;
                    }
                    else
                    {
                        *ptr = value_t::object;
                    }
                }

                switch (ptr->m_type)
                {
                    case value_t::object:
                    {
                        
                        ptr = &ptr->operator[](reference_token);
                        break;
                    }

                    case value_t::array:
                    {
                        
                        if (reference_token.size() > 1 and reference_token[0] == '0')
                        {
                            JSON_THROW(std::domain_error("array index must not begin with '0'"));
                        }

                        if (reference_token == "-")
                        {
                            
                            ptr = &ptr->operator[](ptr->m_value.array->size());
                        }
                        else
                        {
                            
                            ptr = &ptr->operator[](static_cast<size_type>(std::stoi(reference_token)));
                        }
                        break;
                    }

                    default:
                    {
                        JSON_THROW(std::out_of_range("unresolved reference token '" + reference_token + "'"));
                    }
                }
            }

            return *ptr;
        }

        reference get_checked(pointer ptr) const
        {
            for (const auto& reference_token : reference_tokens)
            {
                switch (ptr->m_type)
                {
                    case value_t::object:
                    {
                        
                        ptr = &ptr->at(reference_token);
                        break;
                    }

                    case value_t::array:
                    {
                        if (reference_token == "-")
                        {
                            
                            JSON_THROW(std::out_of_range("array index '-' (" +
                                                         std::to_string(ptr->m_value.array->size()) +
                                                         ") is out of range"));
                        }

                        
                        if (reference_token.size() > 1 and reference_token[0] == '0')
                        {
                            JSON_THROW(std::domain_error("array index must not begin with '0'"));
                        }

                        
                        ptr = &ptr->at(static_cast<size_type>(std::stoi(reference_token)));
                        break;
                    }

                    default:
                    {
                        JSON_THROW(std::out_of_range("unresolved reference token '" + reference_token + "'"));
                    }
                }
            }

            return *ptr;
        }

        
        const_reference get_unchecked(const_pointer ptr) const
        {
            for (const auto& reference_token : reference_tokens)
            {
                switch (ptr->m_type)
                {
                    case value_t::object:
                    {
                        
                        ptr = &ptr->operator[](reference_token);
                        break;
                    }

                    case value_t::array:
                    {
                        if (reference_token == "-")
                        {
                            
                            JSON_THROW(std::out_of_range("array index '-' (" +
                                                         std::to_string(ptr->m_value.array->size()) +
                                                         ") is out of range"));
                        }

                        
                        if (reference_token.size() > 1 and reference_token[0] == '0')
                        {
                            JSON_THROW(std::domain_error("array index must not begin with '0'"));
                        }

                        
                        ptr = &ptr->operator[](static_cast<size_type>(std::stoi(reference_token)));
                        break;
                    }

                    default:
                    {
                        JSON_THROW(std::out_of_range("unresolved reference token '" + reference_token + "'"));
                    }
                }
            }

            return *ptr;
        }

        const_reference get_checked(const_pointer ptr) const
        {
            for (const auto& reference_token : reference_tokens)
            {
                switch (ptr->m_type)
                {
                    case value_t::object:
                    {
                        
                        ptr = &ptr->at(reference_token);
                        break;
                    }

                    case value_t::array:
                    {
                        if (reference_token == "-")
                        {
                            
                            JSON_THROW(std::out_of_range("array index '-' (" +
                                                         std::to_string(ptr->m_value.array->size()) +
                                                         ") is out of range"));
                        }

                        
                        if (reference_token.size() > 1 and reference_token[0] == '0')
                        {
                            JSON_THROW(std::domain_error("array index must not begin with '0'"));
                        }

                        
                        ptr = &ptr->at(static_cast<size_type>(std::stoi(reference_token)));
                        break;
                    }

                    default:
                    {
                        JSON_THROW(std::out_of_range("unresolved reference token '" + reference_token + "'"));
                    }
                }
            }

            return *ptr;
        }

        
        static std::vector<std::string> split(const std::string& reference_string)
        {
            std::vector<std::string> result;

            
            if (reference_string.empty())
            {
                return result;
            }

            
            if (reference_string[0] != '/')
            {
                JSON_THROW(std::domain_error("JSON pointer must be empty or begin with '/'"));
            }

            
            
            
            for (
                
                size_t slash = reference_string.find_first_of('/', 1),
                
                start = 1;
                
                start != 0;
                
                
                start = slash + 1,
                
                slash = reference_string.find_first_of('/', start))
            {
                
                
                auto reference_token = reference_string.substr(start, slash - start);

                
                for (size_t pos = reference_token.find_first_of('~');
                        pos != std::string::npos;
                        pos = reference_token.find_first_of('~', pos + 1))
                {
                    assert(reference_token[pos] == '~');

                    
                    if (pos == reference_token.size() - 1 or
                            (reference_token[pos + 1] != '0' and
                             reference_token[pos + 1] != '1'))
                    {
                        JSON_THROW(std::domain_error("escape error: '~' must be followed with '0' or '1'"));
                    }
                }

                
                unescape(reference_token);
                result.push_back(reference_token);
            }

            return result;
        }

      private:
        
        static void replace_substring(std::string& s,
                                      const std::string& f,
                                      const std::string& t)
        {
            assert(not f.empty());

            for (
                size_t pos = s.find(f);         
                pos != std::string::npos;       
                s.replace(pos, f.size(), t),    
                pos = s.find(f, pos + t.size()) 
            );
        }

        
        static std::string escape(std::string s)
        {
            
            replace_substring(s, "~", "~0");
            replace_substring(s, "/", "~1");
            return s;
        }

        
        static void unescape(std::string& s)
        {
            
            replace_substring(s, "~1", "/");
            
            replace_substring(s, "~0", "~");
        }

        
        static void flatten(const std::string& reference_string,
                            const basic_json& value,
                            basic_json& result)
        {
            switch (value.m_type)
            {
                case value_t::array:
                {
                    if (value.m_value.array->empty())
                    {
                        
                        result[reference_string] = nullptr;
                    }
                    else
                    {
                        
                        for (size_t i = 0; i < value.m_value.array->size(); ++i)
                        {
                            flatten(reference_string + "/" + std::to_string(i),
                                    value.m_value.array->operator[](i), result);
                        }
                    }
                    break;
                }

                case value_t::object:
                {
                    if (value.m_value.object->empty())
                    {
                        
                        result[reference_string] = nullptr;
                    }
                    else
                    {
                        
                        for (const auto& element : *value.m_value.object)
                        {
                            flatten(reference_string + "/" + escape(element.first),
                                    element.second, result);
                        }
                    }
                    break;
                }

                default:
                {
                    
                    result[reference_string] = value;
                    break;
                }
            }
        }

        
        static basic_json unflatten(const basic_json& value)
        {
            if (not value.is_object())
            {
                JSON_THROW(std::domain_error("only objects can be unflattened"));
            }

            basic_json result;

            
            for (const auto& element : *value.m_value.object)
            {
                if (not element.second.is_primitive())
                {
                    JSON_THROW(std::domain_error("values in object must be primitive"));
                }

                
                
                
                
                
                json_pointer(element.first).get_and_create(result) = element.second;
            }

            return result;
        }

      private:
        friend bool operator==(json_pointer const& lhs,
                               json_pointer const& rhs) noexcept
        {
            return lhs.reference_tokens == rhs.reference_tokens;
        }

        friend bool operator!=(json_pointer const& lhs,
                               json_pointer const& rhs) noexcept
        {
            return !(lhs == rhs);
        }

        
        std::vector<std::string> reference_tokens {};
    };

    
    
    

    
    

    
    reference operator[](const json_pointer& ptr)
    {
        return ptr.get_unchecked(this);
    }

    
    const_reference operator[](const json_pointer& ptr) const
    {
        return ptr.get_unchecked(this);
    }

    
    reference at(const json_pointer& ptr)
    {
        return ptr.get_checked(this);
    }

    
    const_reference at(const json_pointer& ptr) const
    {
        return ptr.get_checked(this);
    }

    
    basic_json flatten() const
    {
        basic_json result(value_t::object);
        json_pointer::flatten("", *this, result);
        return result;
    }

    
    basic_json unflatten() const
    {
        return json_pointer::unflatten(*this);
    }

    

    
    
    

    
    

    
    basic_json patch(const basic_json& json_patch) const
    {
        
        basic_json result = *this;

        
        enum class patch_operations {add, remove, replace, move, copy, test, invalid};

        const auto get_op = [](const std::string op)
        {
            if (op == "add")
            {
                return patch_operations::add;
            }
            if (op == "remove")
            {
                return patch_operations::remove;
            }
            if (op == "replace")
            {
                return patch_operations::replace;
            }
            if (op == "move")
            {
                return patch_operations::move;
            }
            if (op == "copy")
            {
                return patch_operations::copy;
            }
            if (op == "test")
            {
                return patch_operations::test;
            }

            return patch_operations::invalid;
        };

        
        const auto operation_add = [&result](json_pointer & ptr, basic_json val)
        {
            
            if (ptr.is_root())
            {
                result = val;
            }
            else
            {
                
                json_pointer top_pointer = ptr.top();
                if (top_pointer != ptr)
                {
                    result.at(top_pointer);
                }

                
                const auto last_path = ptr.pop_back();
                basic_json& parent = result[ptr];

                switch (parent.m_type)
                {
                    case value_t::null:
                    case value_t::object:
                    {
                        
                        parent[last_path] = val;
                        break;
                    }

                    case value_t::array:
                    {
                        if (last_path == "-")
                        {
                            
                            parent.push_back(val);
                        }
                        else
                        {
                            const auto idx = std::stoi(last_path);
                            if (static_cast<size_type>(idx) > parent.size())
                            {
                                
                                JSON_THROW(std::out_of_range("array index " + std::to_string(idx) + " is out of range"));
                            }
                            else
                            {
                                
                                parent.insert(parent.begin() + static_cast<difference_type>(idx), val);
                            }
                        }
                        break;
                    }

                    default:
                    {
                        
                        assert(false);  
                    }
                }
            }
        };

        
        const auto operation_remove = [&result](json_pointer & ptr)
        {
            
            const auto last_path = ptr.pop_back();
            basic_json& parent = result.at(ptr);

            
            if (parent.is_object())
            {
                
                auto it = parent.find(last_path);
                if (it != parent.end())
                {
                    parent.erase(it);
                }
                else
                {
                    JSON_THROW(std::out_of_range("key '" + last_path + "' not found"));
                }
            }
            else if (parent.is_array())
            {
                
                parent.erase(static_cast<size_type>(std::stoi(last_path)));
            }
        };

        
        if (not json_patch.is_array())
        {
            
            JSON_THROW(std::invalid_argument("JSON patch must be an array of objects"));
        }

        
        for (const auto& val : json_patch)
        {
            
            const auto get_value = [&val](const std::string & op,
                                          const std::string & member,
                                          bool string_type) -> basic_json&
            {
                
                auto it = val.m_value.object->find(member);

                
                const auto error_msg = (op == "op") ? "operation" : "operation '" + op + "'";

                
                if (it == val.m_value.object->end())
                {
                    JSON_THROW(std::invalid_argument(error_msg + " must have member '" + member + "'"));
                }

                
                if (string_type and not it->second.is_string())
                {
                    JSON_THROW(std::invalid_argument(error_msg + " must have string member '" + member + "'"));
                }

                
                return it->second;
            };

            
            if (not val.is_object())
            {
                JSON_THROW(std::invalid_argument("JSON patch must be an array of objects"));
            }

            
            const std::string op = get_value("op", "op", true);
            const std::string path = get_value(op, "path", true);
            json_pointer ptr(path);

            switch (get_op(op))
            {
                case patch_operations::add:
                {
                    operation_add(ptr, get_value("add", "value", false));
                    break;
                }

                case patch_operations::remove:
                {
                    operation_remove(ptr);
                    break;
                }

                case patch_operations::replace:
                {
                    
                    result.at(ptr) = get_value("replace", "value", false);
                    break;
                }

                case patch_operations::move:
                {
                    const std::string from_path = get_value("move", "from", true);
                    json_pointer from_ptr(from_path);

                    
                    basic_json v = result.at(from_ptr);

                    
                    
                    
                    
                    operation_remove(from_ptr);
                    operation_add(ptr, v);
                    break;
                }

                case patch_operations::copy:
                {
                    const std::string from_path = get_value("copy", "from", true);;
                    const json_pointer from_ptr(from_path);

                    
                    result[ptr] = result.at(from_ptr);
                    break;
                }

                case patch_operations::test:
                {
                    bool success = false;
                    JSON_TRY
                    {
                        
                        
                        success = (result.at(ptr) == get_value("test", "value", false));
                    }
                    JSON_CATCH (std::out_of_range&)
                    {
                        
                    }

                    
                    if (not success)
                    {
                        JSON_THROW(std::domain_error("unsuccessful: " + val.dump()));
                    }

                    break;
                }

                case patch_operations::invalid:
                {
                    
                    
                    JSON_THROW(std::invalid_argument("operation value '" + op + "' is invalid"));
                }
            }
        }

        return result;
    }

    
    static basic_json diff(const basic_json& source,
                           const basic_json& target,
                           const std::string& path = "")
    {
        
        basic_json result(value_t::array);

        
        if (source == target)
        {
            return result;
        }

        if (source.type() != target.type())
        {
            
            result.push_back(
            {
                {"op", "replace"},
                {"path", path},
                {"value", target}
            });
        }
        else
        {
            switch (source.type())
            {
                case value_t::array:
                {
                    
                    size_t i = 0;
                    while (i < source.size() and i < target.size())
                    {
                        
                        auto temp_diff = diff(source[i], target[i], path + "/" + std::to_string(i));
                        result.insert(result.end(), temp_diff.begin(), temp_diff.end());
                        ++i;
                    }

                    
                    

                    
                    const auto end_index = static_cast<difference_type>(result.size());
                    while (i < source.size())
                    {
                        
                        
                        result.insert(result.begin() + end_index, object(
                        {
                            {"op", "remove"},
                            {"path", path + "/" + std::to_string(i)}
                        }));
                        ++i;
                    }

                    
                    while (i < target.size())
                    {
                        result.push_back(
                        {
                            {"op", "add"},
                            {"path", path + "/" + std::to_string(i)},
                            {"value", target[i]}
                        });
                        ++i;
                    }

                    break;
                }

                case value_t::object:
                {
                    
                    for (auto it = source.begin(); it != source.end(); ++it)
                    {
                        
                        const auto key = json_pointer::escape(it.key());

                        if (target.find(it.key()) != target.end())
                        {
                            
                            auto temp_diff = diff(it.value(), target[it.key()], path + "/" + key);
                            result.insert(result.end(), temp_diff.begin(), temp_diff.end());
                        }
                        else
                        {
                            
                            result.push_back(object(
                            {
                                {"op", "remove"},
                                {"path", path + "/" + key}
                            }));
                        }
                    }

                    
                    for (auto it = target.begin(); it != target.end(); ++it)
                    {
                        if (source.find(it.key()) == source.end())
                        {
                            
                            const auto key = json_pointer::escape(it.key());
                            result.push_back(
                            {
                                {"op", "add"},
                                {"path", path + "/" + key},
                                {"value", it.value()}
                            });
                        }
                    }

                    break;
                }

                default:
                {
                    
                    result.push_back(
                    {
                        {"op", "replace"},
                        {"path", path},
                        {"value", target}
                    });
                    break;
                }
            }
        }

        return result;
    }

    
};






using json = basic_json<>;
} 







namespace std
{

template<>
inline void swap(nlohmann::json& j1,
                 nlohmann::json& j2) noexcept(
                     is_nothrow_move_constructible<nlohmann::json>::value and
                     is_nothrow_move_assignable<nlohmann::json>::value
                 )
{
    j1.swap(j2);
}


template<>
struct hash<nlohmann::json>
{
    
    std::size_t operator()(const nlohmann::json& j) const
    {
        
        const auto& h = hash<nlohmann::json::string_t>();
        return h(j.dump());
    }
};
} 


inline nlohmann::json operator "" _json(const char* s, std::size_t n)
{
    return nlohmann::json::parse(s, s + n);
}


inline nlohmann::json::json_pointer operator "" _json_pointer(const char* s, std::size_t n)
{
    return nlohmann::json::json_pointer(std::string(s, n));
}


#if defined(__clang__) || defined(__GNUC__) || defined(__GNUG__)
    #pragma GCC diagnostic pop
#endif


#undef JSON_CATCH
#undef JSON_DEPRECATED
#undef JSON_THROW
#undef JSON_TRY

#endif




// Copyright (C) 2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.