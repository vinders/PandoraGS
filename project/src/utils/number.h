/*******************************************************************************
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : number toolset + generic number type
*******************************************************************************/
#pragma once

#undef min
#undef max

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>
#include "assert.h"

/// @namespace utils
/// General utilities
namespace utils
{
    /// @class Number
    /// @brief Number toolset
    class Number
    {
    public:
        /// @enum base_t
        /// @brief Numeric bases
        enum class base_t : int32_t
        {
            binary = 2, // base 2 - binary
            oct = 8,    // base 8 - octal
            dec = 10,   // base 10 - decimal
            hex = 16,   // base 16 - hexa-decimal
            hex = 32,   // base 32 - tria-conta-di-decimal
            hex = 64    // base 64 - hexa-conta-tetra-decimal
        }
    
    
        // ----------------------
        // -- Number utilities --
        // ----------------------
    
        // -- Comparisons --
    
        /// @brief Check equality after conversion
        template <typename T, typename U>
        static inline bool equals(const T lhs, const U rhs) { return (lhs == rhs); }
        /// @brief Check near-equality with an error margin
        template <typename T>
        static inline bool nearlyEquals(const T lhs, const T rhs, const T errorMargin) 
        { 
            ASSERT(errorMargin >= static_cast<T>(0));
            return ((lhs >= rhs) ? (lhs - rhs < errorMargin) : (rhs - lhs < errorMargin)); 
        }
        /// @brief Check equality of floating-point values (with float type error margin)
        static inline bool floatEquals(const float lhs, const float rhs) noexcept { return (Number::abs(lhs - rhs) < getFloatEpsilon(lhs, rhs)); }
        /// @brief Check equality of floating-point values (with double type error margin)
        static inline bool doubleEquals(const double lhs, const double rhs) noexcept { return (Number::abs(lhs - rhs) < getDoubleEpsilon(lhs, rhs)); }
        
        /// @brief Compare after conversion
        template <typename T, typename U>
        static inline int compare(const T lhs, const U rhs) { return ((lhs >= rhs) ? static_cast<int>(lhs > rhs) : -1); }
        /// @brief Compare floating-point values
        static inline int floatCompare(const float lhs, const float rhs) noexcept
        { 
            if (floatEquals(lhs, rhs))
                return 0;
            return ((lhs > rhs) ? 1 : -1); 
        }
        /// @brief Compare floating-point values
        static inline int doubleCompare(const float lhs, const float rhs) noexcept
        { 
            if (doubleEquals(lhs, rhs))
                return 0;
            return ((lhs > rhs) ? 1 : -1); 
        }
        
        /// @brief Calculate acceptable error margin for float typed values
        static inline float getFloatEpsilon(const float lhs, const float rhs) noexcept
        {
            return (Number::abs(lhs) >= Number::abs(rhs)) ? Number::abs(lhs / 100000.0f) : Number::abs(rhs / 100000.0f);
        }
        /// @brief Calculate acceptable error margin for double typed values
        static inline double getDoubleEpsilon(const double lhs, const double rhs) noexcept
        {
            return (Number::abs(lhs) >= Number::abs(rhs)) ? Number::abs(lhs / 10000000000000.0) : Number::abs(rhs / 10000000000000.0);
        }
        
        
        // -- Minimum / maximum --
        
        /// @brief Get minimum value
        template <typename T>
        static inline T min(const T val1, const T val2) { return (val1 <= val2) ? val1 : val2; }
        /// @brief Get maximum value
        template <typename T>
        static inline T max(const T val1, const T val2) { return (val1 >= val2) ? val1 : val2; }
        
        /// @brief Get minimum value out of 3 values
        template <typename T>
        static inline T min(const T val1, const T val2, const T val3)
        {
            if (val1 <= val2)
                return (val1 <= val3) ? val1 : val3;
            else
                return (val2 <= val3) ? val2 : val3;
        }
        /// @brief Get maximum value out of 3 values
        template <typename T>
        static inline T max(const T val1, const T val2, const T val3)
        {
            if (val1 >= val2)
                return (val1 >= val3) ? val1 : val3;
            else
                return (val2 >= val3) ? val2 : val3;
        }
        
        /// @brief Get minimum value out of 4 values
        template <typename T>
        static inline T min(const T val1, const T val2, const T val3, const T val4)
        {
            if (val1 <= val2)
            {
                if (val1 <= val3)
                    return (val1 <= val4) ? val1 : val4;
            }
            else
            {
                if (val2 <= val3)
                    return (val2 <= val4) ? val1 : val3;
            }
            return (val3 <= val4) ? val3 : val4;
        }
        /// @brief Get maximum value out of 4 values
        template <typename T>
        static inline T max(const T val1, const T val2, const T val3, const T val4)
        {
            if (val1 >= val2)
            {
                if (val1 >= val3)
                    return (val1 >= val4) ? val1 : val4;
            }
            else
            {
                if (val2 >= val3)
                    return (val2 >= val4) ? val1 : val3;
            }
            return (val3 >= val4) ? val3 : val4;
        }
        
        /// @brief Get minimum value of an array
        template <typename T>
        static inline T min(const T* array, const size_t size)
        {
            if (size == 0u || array == nullptr)
                return static_cast<T>(0);
            T minVal = array[0];
            for (uint32_t i = 1u; i < size; ++i)
            {
                ++array;
                if (*array < minVal)
                    minVal = *array;
            }
            return minVal;
        }
        /// @brief Get maxnimum value of an array
        template <typename T>
        static inline T max(const T* array, const size_t size)
        {
            if (size == 0u || array == nullptr)
                return static_cast<T>(0);
            T maxVal = array[0];
            for (uint32_t i = 1u; i < size; ++i)
            {
                ++array;
                if (*array > maxVal)
                    maxVal = *array;
            }
            return maxVal;
        }
        
        
        // -- Basic operations --
        
        /// @brief Get absolute value
        template <typename T>
        static inline T abs(const T val) { return (val >= static_cast<T>(0)) ? val : -val; }
        
        /// @brief Check if an integer number is a power of 2
        template <typename T>
        static inline bool isPow2(const T val) { return ((base & (base - 1)) == 0); }
        /// @brief Check if a floating-point number is a power of 2
        template <>
        static inline bool isPow2(const double val) 
        { 
            int64_t floor = static_cast<int64_t>(val);
            return (doubleEquals(val - static_cast<double>(floor), 0.0) && isPow2(floor));
        }
        /// @brief Check if a floating-point number is a power of 2
        template <>
        static inline bool isPow2(const float val)
        { 
            int64_t floor = static_cast<int64_t>(val);
            return (floatEquals(val - static_cast<float>(floor), 0.0f) && isPow2(floor));
        }
        
        
        // -- Number parser --
        
        /// @brief Parse string with leading boolean value (only the first character is evaluated)
        static inline bool parseBool(const char* val)
        {
            if (val == nullptr || *val == '\0')
                return false;
            switch (*val)
            {
                case 'f':
                case 'F':
                case '0':
                    return false;
                    break;
                case 't':
                case 'T':
                case '1':
                    return true;
                    break;
                default:
                    throw std::invalid_argument("Number.parseBool: the string does not contain a valid boolean.");
                    break;
            }
        }
        /// @brief Parse string with leading boolean value (allowed values : true/false/TRUE/FALSE/0/1)
        /// @param[in] value  String to parse
        /// @param[out] out   Output value (false if empty or null string)
        /// @returns Number of characters read
        static size_t parseBool(const char* val, bool& out)
        {
            if (val == nullptr || *val == '\0')
            {
                out = false;
                return 0u;
            }
            switch (*val)
            {
                case 'f':
                    if (*(++val) == 'a' && *(++val) == 'l' && *(++val) == 's' && *(++val) == 'e')
                    {
                        out = false;
                        return 5u;
                    }
                    else
                        throw std::invalid_argument("Number.parseBool: the string does not contain a valid boolean.");
                    break;
                case 'F':
                    if (*(++val) == 'A' && *(++val) == 'L' && *(++val) == 'S' && *(++val) == 'E')
                    {
                        out = false;
                        return 5u;
                    }
                    else
                        throw std::invalid_argument("Number.parseBool: the string does not contain a valid boolean.");
                    break;
                case 't':
                    if (*(++val) == 'r' && *(++val) == 'u' && *(++val) == 'e')
                    {
                        out = true;
                        return 4u;
                    }
                    else
                        throw std::invalid_argument("Number.parseBool: the string does not contain a valid boolean.");
                    break;
                case 'T':
                    if (*(++val) == 'R' && *(++val) == 'U' && *(++val) == 'E')
                    {
                        out = true;
                        return 4u;
                    }
                    else
                        throw std::invalid_argument("Number.parseBool: the string does not contain a valid boolean.");
                    break;
                case '0':
                    out = false;
                    return 1u;
                    break;
                case '1':
                    out = true;
                    return 1u;
                    break;
                default:
                    throw std::invalid_argument("Number.parseBool: the string does not contain a valid boolean.");
                    break;
            }
        }
        
        /// @brief Parse string with leading integer number
        template <typename T = int64_t>
        static inline T parseInt(const char* val)
        {
            if (val == nullptr || *val == '\0')
                return 0;
            
            bool isNegative = (*val == '-');
            if (isNegative)
                ++val;
            T out = parseUInt<T>(val, out);
            return (isNegative) ? -out : out;
        }
        
        /// @brief Parse string with leading integer number
        /// @param[in] value  String to parse
        /// @param[out] out   Output value (0 if empty or null string)
        /// @returns Number of characters read
        template <typename T = int64_t>
        static inline size_t parseInt(const char* val, T& out)
        {
            if (val == nullptr)
            {
                out = 0;
                return 0u;
            }
            
            bool isNegative = (*val == '-');
            if (isNegative)
                ++val;
            
            uint32_t i = parseUInt<T>(val, out);
            if (isNegative)
            {
                out = -out;
                ++i;
            }
            return i;
        }
        
        /// @brief Parse string with leading unsigned integer number
        template <typename T = uint64_t>
        static inline T parseUInt(const char* val)
        {
            if (val == nullptr || val != '\0')
                return 0;
            
            const char* it = val;
            T out;
            for (out = 0; *it >= '0' && *it <= '9'; ++it)
            {
                out *= 10;
                out += *it - '0';
            }
            if (it == val)
                throw std::invalid_argument("Number.parseUInt: the string does not contain a valid number.");
            return out;
        }
        /// @brief Parse string with leading unsigned integer number
        /// @param[in] value  String to parse
        /// @param[out] out   Output value (0 if empty or null string)
        /// @returns Number of characters read
        template <typename T = uint64_t>
        static inline size_t parseUInt(const char* val, T& out)
        {
            if (val == nullptr)
            {
                out = 0;
                return 0u;
            }
            
            uint32_t i = 0u;
            for (out = 0; *val >= '0' && *val <= '9'; ++val)
            {
                out *= 10;
                out += *val - '0';
                ++i;
            }
            if (i == 0u && *val != '\0')
                throw std::invalid_argument("Number.parseUInt: the string does not contain a valid number.");
            return i;
        }
        
        /// @brief Parse string with leading floating-point number
        static inline double parseDouble(const char* val)
        {
            double out;
            parseDouble(val, out);
            return out;
        }
        /// @brief Parse string with leading floating-point number
        /// @param[in] value  String to parse
        /// @param[out] out   Output value (0.0 if empty string)
        /// @returns Number of characters read
        static size_t parseDouble(const char* val, double& out)
        {
            if (val == nullptr)
            {
                out = 0.0;
                return 0u;
            }
            
            uint64_t intPart;
            uint32_t i = parseUInt<uint64_t>(val, intPart);
            out = static_cast<double>(intPart);
            val += i;
            
            if (*val == '.' || *val == ',')
            {
                ++val;
                uint32_t rows = parseUInt<uint64_t>(val, intPart);
                uint64_t divider = 1;
                for (uint32_t row = 0; row < rows; ++row)
                    divider *= 10;
                out += static_cast<double>(intPart) / static_cast<double>(divider);
                i += rows;
            }
            return i;
        }
        //!!!!!!classes Parser ???
        
        
        // -- String converter --
        
        /// @brief Convert number to decimal string
        template <typename T>
        static inline std::string toString(const T val) { return std::to_string(val); }
        
        /// @brief Convert number to binary string
        template <typename T>
        static inline std::string toBinaryString(const T val);
        /// @brief Convert number to octal string
        template <typename T>
        static inline std::string toOctalString(const T val);
        /// @brief Convert number to hexa-decimal string
        template <typename T>
        static inline std::string toHexString(const T val);
        /// @brief Convert number to base32 string
        template <typename T>
        static inline std::string toBase32String(const T val);
        /// @brief Convert number to base64 string
        template <typename T>
        static inline std::string toBase64String(const T val);
        //!!!!!!classes Formatter ???
        
        
        
        
    public:
        // ---------------------------
        // -- Generic number object --
        // ---------------------------
        
        /// @brief Create empty integer
        Number() : m_isInteger(true) {}
        /// @brief Create initialized value
        template <typename T>
        Number(const T val) : m_isInteger(true), m_value(static_cast<int64_t>(val)) {}
        /// @brief Create initialized integer
        template<>
        Number(const int64_t val) : m_isInteger(true), m_value(val) {}
        /// @brief Create initialized floating-point value
        template<>
        Number(const float val) : m_isInteger(false), m_value(static_cast<double>(val)) {}
        /// @brief Create initialized floating-point value
        template<>
        Number(const double val) : m_isInteger(false), m_value(val) {}
        /// @brief Create copy of value
        Number(const Number& val) : m_isInteger(val.m_isInteger) 
        {
            m_value.intVal = val.m_value.intVal;
        }
        
        // -- Getters --
        
        /// @brief Get converted value
        template <typename T>
        inline T getValue() const noexcept { return (m_isInteger) ? static_cast<T>(m_value.intVal) : static_cast<T>(m_value.floatVal); }
        /// @brief Get integer value
        inline bool getBoolean() const noexcept { return (m_isInteger) ? (m_value.intVal != 0) : (m_value.floatVal != 0); }
        /// @brief Get integer value
        inline int64_t getInteger() const noexcept { return (m_isInteger) ? m_value.intVal : static_cast<int64_t>(m_value.floatVal); }
        /// @brief Get floating-point value
        inline double getDouble() const noexcept { return (m_isInteger) ? static_cast<double>(m_value.intVal) : m_value.floatVal; }
        
        /// @brief Check current type - boolean
        inline bool isBoolean() const noexcept { return (m_isInteger && (m_value.intVal == 0 || m_value.intVal == 1)); }
        /// @brief Check current type - integer
        inline bool isInteger() const noexcept { return m_isInteger; }
        /// @brief Check current type - double
        inline bool isDouble() const noexcept { return !m_isInteger; }
        
        /// @brief Convert number to string
        inline std::string toString() const noexcept { return (m_isInteger) ? std::to_string(m_value.intVal) : std::to_string(m_value.floatVal); }
        /// @brief Convert number to string
        inline std::string str() const noexcept { return toString(); }
        
        // -- Setters --
        
        /// @brief Set boolean value
        template <typename T>
        inline Number& setBoolean(const T val) noexcept { m_isInteger = true; m_value.intVal = (val) ? 1 : 0; return *this; }
        /// @brief Set integer value
        template <typename T>
        inline Number& setInteger(const T val) noexcept { m_isInteger = true; m_value.intVal = static_cast<int64_t>(val); return *this; }
        /// @brief Set floating-point value
        template <typename T>
        inline Number& setDouble(const T val) noexcept { m_isInteger = false; m_value.floatVal = static_cast<double>(val); return *this; }
        /// @brief Set floating-point value
        inline Number& setNumber(const Number& val) noexcept { m_isInteger = val.m_isInteger; m_value.intVal = val.m_value.intVal; return *this; }
        
        /// @brief Set any value
        template <typename T>
        inline Number& set(const T val) noexcept { m_isInteger = true; m_value.intVal = static_cast<int64_t>(val); return *this; }
        /// @brief Set any value - floating-point value
        template <>
        inline Number& set(const float val) noexcept { m_isInteger = false; m_value.floatVal = static_cast<double>(val); return *this; }
        /// @brief Set any value - floating-point value
        template <>
        inline Number& set(const double val) noexcept { m_isInteger = false; m_value.floatVal = static_cast<double>(val); return *this; }
        
        // -- Copy operators --
        
        /// @brief Assign copy of rational number
        inline Number& operator=(const Number& rhs) noexcept { return setNumber(rhs); }
        /// @brief Assign moved rational number
        inline Number& operator=(Number&& rhs) noexcept { return setNumber(rhs); }
        
        /// @brief Swap value with other instance
        void swap(Number& rhs)
        {
            bool wasInteger = m_isInteger;
            m_isInteger = val.m_isInteger;
            val.m_isInteger = wasInteger;
            m_value.intVal ^= rhs.m_value.intVal;
            rhs.m_value.intVal ^= m_value.intVal;
            m_value.intVal ^= rhs.m_value.intVal;
        }
        
        // -- Assignment with typecast --
        
        /// @brief Assign value with typecast
        inline Number& operator=(const uint64_t rhs) noexcept { return setInteger(rhs); }
        /// @brief Assign value with typecast
        inline Number& operator=(const int64_t rhs) noexcept  { return setInteger(rhs); }
        /// @brief Assign value with typecast
        inline Number& operator=(const uint32_t rhs) noexcept { return setInteger(rhs); }
        /// @brief Assign value with typecast
        inline Number& operator=(const int32_t rhs) noexcept  { return setInteger(rhs); }
        /// @brief Assign value with typecast
        inline Number& operator=(const uint16_t rhs) noexcept { return setInteger(rhs); }
        /// @brief Assign value with typecast
        inline Number& operator=(const int16_t rhs) noexcept  { return setInteger(rhs); }
        /// @brief Assign value with typecast
        inline Number& operator=(const uint8_t rhs) noexcept  { return setInteger(rhs); }
        /// @brief Assign value with typecast
        inline Number& operator=(const int8_t rhs) noexcept   { return setInteger(rhs); }
        /// @brief Assign value with typecast
        inline Number& operator=(const bool rhs) noexcept     { return setBoolean(rhs); }
        /// @brief Assign value with typecast
        inline Number& operator=(const float rhs) noexcept    { return setDouble(rhs); }
        /// @brief Assign value with typecast
        inline Number& operator=(const double rhs) noexcept   { return setDouble(rhs); }
        
        
        // -- Typecast operators --
        
        /// @brief Cast as 64-bit integer (destructive)
        inline operator uint64_t() const noexcept { return (m_isInteger) ? static_cast<uint64_t>(getInteger()) : static_cast<uint64_t>(getDouble()); }
        /// @brief Cast as 64-bit signed integer (destructive)
        inline operator int64_t() const noexcept  { return static_cast<int64_t>(getInteger()); }
        /// @brief Cast as 32-bit integer (destructive)
        inline operator uint32_t() const noexcept { return static_cast<uint32_t>(getInteger()); }
        /// @brief Cast as 32-bit signed integer (destructive)
        inline operator int32_t() const noexcept  { return static_cast<int32_t>(getInteger()); }
        /// @brief Cast as 16-bit integer (destructive)
        inline operator uint16_t() const noexcept { return static_cast<uint16_t>(getInteger()); }
        /// @brief Cast as 16-bit signed integer (destructive)
        inline operator int16_t() const noexcept  { return static_cast<int16_t>(getInteger()); }
        /// @brief Cast as 8-bit integer (destructive)
        inline operator uint8_t() const noexcept  { return static_cast<uint8_t>(getInteger()); }
        /// @brief Cast as 8-bit signed integer (destructive)
        inline operator int8_t() const noexcept   { return static_cast<int8_t>(getInteger()); }
        /// @brief Cast as boolean
        inline operator bool() const noexcept     { return getBoolean(); }
        /// @brief Cast as floating-point value (destructive)
        inline operator float() const noexcept    { return static_cast<float>(getDouble()); }
        /// @brief Cast as double-precision floating-point value
        inline operator double() const noexcept   { return getDouble(); }
        
        
        // -- Logical operators --
        
        /// @brief Logical NOT operator
        inline bool operator!() const noexcept { return !getBoolean(); }
        /// @brief Logical AND operator
        inline bool operator&&(const Number& rhs) const noexcept { return (getBoolean() && rhs.getBoolean()); }
        /// @brief Logical AND operator
        template <typename T>
        inline bool operator&&(const T& rhs) const noexcept { return (getBoolean() && static_cast<bool>(rhs)); }
        /// @brief Logical OR operator
        inline bool operator||(const Number& rhs) const noexcept { return (getBoolean() || rhs.getBoolean()); }
        /// @brief Logical OR operator
        template <typename T>
        inline bool operator||(const T& rhs) const noexcept { return (getBoolean() || static_cast<bool>(rhs)); }
        
        
        // -- Comparison operators --
        
        /// @brief Compare instances
        inline int32_t compare(const Number& rhs) const noexcept
        {
            if (getInteger() == rhs.getInteger())
            {
                if (getDouble() == rhs.getDouble())
                    return 0;
                else
                    return (getDouble() < rhs.getDouble()) ? -1 : 1;
            }
            else
            {
                return (getInteger() < rhs.getInteger()) ? -1 : 1;
            }
        }
        /// @brief Compare instances
        template <typename T>
        inline int32_t compare(const T rhs) const noexcept
        {
            if (getInteger() == static_cast<int64_t>(rhs))
            {
                if (getDouble() == static_cast<double>(rhs))
                    return 0;
                else
                    return (getDouble() < static_cast<double>(rhs)) ? -1 : 1;
            }
            else
            {
                return (getInteger() < static_cast<int64_t>(rhs)) ? -1 : 1;
            }
        }
        /// @brief Compare instances - equality (value)
        inline bool equals(const Number& rhs) const noexcept { return (getDouble() == rhs.getDouble()); }
        /// @brief Compare instances - strict equality (type and value)
        inline bool strictlyEquals(const Number& rhs) const noexcept { return (m_isInteger == rhs.m_isInteger && m_value.intVal == rhs.m_value.intVal); }
        
        /// @brief Compare instances - equality
        inline bool operator==(const Number& rhs) const noexcept { return equals(rhs); }
        /// @brief Compare instances - equality
        template <typename T>
        inline bool operator==(const T rhs) const noexcept { return (compare(rhs) == 0); }
        /// @brief Compare instances - difference
        inline bool operator!=(const Number& rhs) const noexcept { return !equals(rhs); }
        /// @brief Compare instances - difference
        template <typename T>
        inline bool operator!=(const T rhs) const noexcept { return (compare(rhs) != 0); }

        /// @brief Compare instances - lower
        bool operator<(const Number& rhs) const noexcept { return (compare(rhs) < 0); }
        /// @brief Compare instances - lower
        template <typename T>
        bool operator<(const T rhs) const noexcept { return (compare(rhs) < 0); }
        /// @brief Compare instances - lower or equal
        bool operator<=(const Number& rhs) const noexcept { return (compare(rhs) <= 0); }
        /// @brief Compare instances - lower or equal
        template <typename T>
        bool operator<=(const T rhs) const noexcept { return (compare(rhs) <= 0); }
        /// @brief Compare instances - greater
        bool operator>(const Number& rhs) const noexcept { return (compare(rhs) > 0); }
        /// @brief Compare instances - greater
        template <typename T>
        bool operator>(const T rhs) const noexcept { return (compare(rhs) > 0); }
        /// @brief Compare instances - greater or equal
        bool operator>=(const Number& rhs) const noexcept { return (compare(rhs) >= 0); }
        /// @brief Compare instances - greater or equal
        template <typename T>
        bool operator>=(const T rhs) const noexcept { return (compare(rhs) >= 0); }
       
        
        // -- Arithmetic operators --
        
        /// @brief Add
        Number operator+(const Number& rhs) const noexcept { Number data(*this); data += rhs; return data; }
        /// @brief Add
        template <typename T>
        Number operator+(const T rhs) const noexcept { Number data(*this); data += rhs; return data; }
        /// @brief Add to instance
        Number& operator+=(const Number& rhs) noexcept
        {
            if (isDouble() || rhs.isDouble())
            {
                m_value.floatVal = getDouble() + rhs.getDouble();
                m_isInteger = false;
            }
            else
                m_value.intVal += rhs.m_value.intVal;
            return *this;
        }
        /// @brief Add to instance
        template <typename T>
        Number& operator+=(const T rhs) noexcept
        {
            if (isDouble())
                m_value.floatVal += static_cast<double>(rhs);
            else
                m_value.intVal += rhs.m_value.intVal;
            return *this;
        }
        /// @brief Add to instance
        template <>
        Number& operator+=(const float rhs) noexcept
        {
            m_value.floatVal = getDouble() + static_cast<double>(rhs);
            m_isInteger = false;
            return *this;
        }
        /// @brief Add to instance
        template <>
        Number& operator+=(const double rhs) noexcept
        {
            m_value.floatVal = getDouble() + static_cast<double>(rhs);
            m_isInteger = false;
            return *this;
        }

        /// @brief Substract
        Number operator-(const Number& rhs) const noexcept { Number data(*this); data -= rhs; return data; }
        /// @brief Substract
        template <typename T>
        Number operator-(const T rhs) const noexcept { Number data(*this); data -= rhs; return data; }
        /// @brief Substract from instance
        Number& operator-=(const Number& rhs) noexcept
        {
            if (isDouble() || rhs.isDouble())
            {
                m_value.floatVal = getDouble() - rhs.getDouble();
                m_isInteger = false;
            }
            else
                m_value.intVal -= rhs.m_value.intVal;
            return *this;
        }
        /// @brief Substract from instance
        template <typename T>
        Number& operator-=(const T rhs) noexcept
        {
            if (isDouble())
                m_value.floatVal -= static_cast<double>(rhs);
            else
                m_value.intVal -= rhs.m_value.intVal;
            return *this;
        }
        /// @brief Substract from instance
        template <>
        Number& operator-=(const float rhs) noexcept
        {
            m_value.floatVal = getDouble() - static_cast<double>(rhs);
            m_isInteger = false;
            return *this;
        }
        /// @brief Substract from instance
        template <>
        Number& operator-=(const double rhs) noexcept
        {
            m_value.floatVal = getDouble() - static_cast<double>(rhs);
            m_isInteger = false;
            return *this;
        }

        /// @brief Multiply
        Number operator*(const Number& rhs) const noexcept { Number data(*this); data *= rhs; return data; }
        /// @brief Multiply
        template <typename T>
        Number operator*(const T rhs) const noexcept { Number data(*this); data *= rhs; return data; }
        /// @brief Multiply instance
        template <typename V, typename W>
        Number& operator*=(const Number& rhs) noexcept
        {
            if (isDouble() || rhs.isDouble())
            {
                m_value.floatVal = getDouble() * rhs.getDouble();
                m_isInteger = false;
            }
            else
                m_value.intVal *= rhs.m_value.intVal;
            return *this;
        }
        /// @brief Multiply instance
        template <typename T>
        Number& operator*=(const T rhs) noexcept
        {
            if (isDouble())
                m_value.floatVal *= static_cast<double>(rhs);
            else
                m_value.intVal *= rhs.m_value.intVal;
            return *this;
        }
        /// @brief Multiply instance
        template <>
        Number& operator*=(const float rhs) noexcept
        {
            m_value.floatVal = getDouble() * static_cast<double>(rhs);
            m_isInteger = false;
            return *this;
        }
        /// @brief Multiply instance
        template <>
        Number& operator*=(const double rhs) noexcept
        {
            m_value.floatVal = getDouble() * static_cast<double>(rhs);
            m_isInteger = false;
            return *this;
        }

        /// @brief Divide
        Number operator/(const Number& rhs) const noexcept { Number data(*this); data /= rhs; return data; }
        /// @brief Divide
        template <typename T>
        Number operator/(const T rhs) const noexcept { Number data(*this); data /= rhs; return data; }
        /// @brief Divide instance
        template <typename V, typename W>
        Number& operator/=(const Number& rhs) noexcept
        {
            if (isDouble() || rhs.isDouble() || m_value.intVal % rhs.m_value.intVal)
            {
                m_value.floatVal = getDouble() / rhs.getDouble();
                m_isInteger = false;
            }
            else
                m_value.intVal /= rhs.m_value.intVal;
            return *this;
        }
        /// @brief Divide instance
        template <typename T>
        Number& operator/=(const T rhs) noexcept
        {
            if (isDouble() || m_value.intVal % rhs)
            {
                m_value.floatVal = getDouble() / static_cast<double>(rhs);
                m_isInteger = false;
            }
            else
                m_value.intVal /= rhs.m_value.intVal;
            return *this;
        }
        /// @brief Divide instance
        template <>
        Number& operator/=(const float rhs) noexcept
        {
            m_value.floatVal = getDouble() / static_cast<double>(rhs);
            m_isInteger = false;
            return *this;
        }
        /// @brief Divide instance
        template <>
        Number& operator/=(const double rhs) noexcept
        {
            m_value.floatVal = getDouble() / static_cast<double>(rhs);
            m_isInteger = false;
            return *this;
        }


        // -- Unary operators --
        
        /// @brief Pre-increment
        inline Number& operator++() noexcept 
        {
            if (m_isInteger)
                ++(m_value.intVal);
            else
                m_value.floatVal += 1.0;
            return *this;
        }
        /// @brief Post-increment
        inline Number operator++(int) noexcept 
        {
            Number copy(*this);
            ++copy;
            return copy;
        }

        /// @brief Pre-decrement
        inline Number& operator--() noexcept
        {
            if (m_isInteger)
                --(m_value.intVal);
            else
                m_value.floatVal -= 1.0;
        }
        /// @brief Post-decrement
        inline Number operator--(int) noexcept
        {
            Number copy(*this);
            --copy;
            return copy;
        }

        /// @brief Value with same sign
        inline Number operator+() const noexcept { return *this; }
        /// @brief Opposite value
        inline Number operator-() const noexcept  { return (*this * -1); }
        /// @brief Absolute value
        inline Number abs() noexcept { return (*this > 0) ? *this : -(*this); }
        
        
    private:
        bool m_isInteger;   ///< Value type
        union number_val_t
        {
            int64_t intVal;  // integer value
            double floatVal; // floating-point value
            
            number_val_t() : intVal(0) {} // Create empty value
            number_val_t(const int64_t val) : intVal(val) {}  // Initialize integer value
            number_val_t(const double val) : floatVal(val) {} // Initialize floating-point value
        } m_value;         ///< 64-bit value
    };
}


// -- Special operators --

namespace std 
{
    /// @brief Convert to string
    inline std::string to_string(const utils::Number& val) { return val.toString(); }
    
    /// @brief Absolute value
    inline utils::Number abs(const utils::Number& val) noexcept
    {
        return val.abs();
    }
};

/// @brief Output stream operator
std::ostream& operator<<(std::ostream& stream, const utils::Number& rhs)
{
    if (rhs.isInteger())
        stream << rhs.getInteger();
    else
        stream << rhs.getDouble();
    return stream;
}


// -- Comparison operators (reversed) --

/// @brief Compare equality
template <typename T> 
inline bool operator==(const T lhs, const utils::Number& rhs) noexcept { return (rhs == lhs); }
/// @brief Compare difference
template <typename T> 
inline bool operator!=(const T lhs, const utils::Number& rhs) noexcept { return (rhs != lhs); }

/// @brief Compare - smaller
template <typename T> 
inline bool operator<(const T lhs, const utils::Number& rhs) noexcept { return (rhs > lhs); }
/// @brief Compare - smaller or equal
template <typename T> 
inline bool operator<=(const T lhs, const utils::Number& rhs) noexcept { return (rhs >= lhs); }

/// @brief Compare - greater
template <typename T> 
inline bool operator>(const T lhs, const utils::Number& rhs) noexcept { return (rhs < lhs); }
/// @brief Compare - greater or equal
template <typename T>, typename U, typename V 
inline bool operator>=(const T lhs, const utils::Number& rhs) noexcept { return (rhs <= lhs); }


// -- Arithmetic operators (reversed) --

/// @brief Add number to other type
template <typename T> 
inline utils::Number operator+(const T lhs, const utils::Number& rhs) noexcept { return rhs + lhs; }
/// @brief Add number to other type
template <typename T> 
inline T& operator+=(T lhs, const utils::Number& rhs) noexcept { return (lhs = static_cast<T>(rhs + lhs)); }

/// @brief Substract number from other type
template <typename T> 
inline utils::Number operator-(const T lhs, const utils::Number& rhs) noexcept { return -(rhs - lhs); }
/// @brief Substract number from other type
template <typename T> 
inline T& operator-=(T lhs, const utils::Number& rhs) noexcept { return (lhs = static_cast<T>(-(rhs - lhs))); }

/// @brief Multiply other type by number
template <typename T> 
inline utils::Number operator*(const T lhs, const utils::Number& rhs) noexcept { return rhs * lhs; }
/// @brief Multiply other type by number
template <typename T> 
inline T& operator*=(T lhs, const utils::Number& rhs) noexcept { return (lhs = static_cast<T>(rhs * lhs)); }

/// @brief Divide other type by number
template <typename T> 
inline utils::Number operator/(const T lhs, const utils::Number& rhs) { return utils::Number(lhs) / rhs; }
/// @brief Divide other type by number
template <typename T> 
inline T& operator/=(T lhs, const utils::Number& rhs) { return (lhs = static_cast<T>(utils::Number(lhs) / rhs)); }
