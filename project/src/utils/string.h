/*******************************************************************************
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : string management toolset
*******************************************************************************/
#pragma once

#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include "char.h"

/// @namespace utils
/// General utilities
namespace utils
{
    using ustring = std::basic_string<utils::unicode_char_t>; ///< Unicode (UCS) string
    
    /// @class String
    /// @brief String management toolset
    class String
    {
    public:
        // -- Get size --
    
        /// @brief Check if a string is null or empty
        /// @param[in] data  String value
        /// @returns Empty/null (true) or not
        template <typename T>
        static inline bool isEmpty(const T* data) { return (data == nullptr || *data == 0); }
        /// @brief Check if a string is empty
        /// @param[in] data  String value
        /// @returns Empty (true) or not
        template <typename T>
        static inline bool isEmpty(const std::basic_string<T>& data) { return data.empty(); }
        
        /// @brief Get the length of a string
        /// @param[in] data  String value
        /// @returns Length of the string value
        template <typename T>
        static inline size_t getLength(const T* data) 
        { 
            size_t len = 0u;
            if (data != nullptr)
            {
                while (*data != 0)
                {
                    ++data;
                    ++len;
                }
            }
            return len;
        }
        template <> static inline size_t getLength<char>(const char* data) { return (data != nullptr) ? strlen(data) : 0u; }
        template <> static inline size_t getLength<wchar_t>(const wchar_t* data) { return (data != nullptr) ? wcslen(data) : 0u; }
        /// @brief Get the length of a string
        /// @param[in] data  String value
        /// @returns Length of the string value
        template <typename T>
        static inline size_t getLength(const std::basic_string<T>& data) { return data.size(); }
        
        
        // -- Change size --
        
        /// @brief Truncate beginning of a string
        /// @param[in] data        String value
        /// @param[in] desiredLen  Desired length after truncation
        /// @returns Truncated string
        template <typename T>
        static inline std::basic_string<T> ltruncate(const std::basic_string<T>& data, const size_t desiredLen) 
        { 
            return (desiredLen < data.size()) ? data.substr(data.size() - desiredLen, desiredLen) : data;
        }
        /// @brief Truncate beginning of a string
        /// @param[in|out] data    String value to truncate
        /// @param[in] desiredLen  Desired length after truncation
        template <typename T>
        static inline void ltruncateSelf(std::basic_string<T>& data, const size_t desiredLen) 
        { 
            if (desiredLen < data.size()) 
                data = data.substr(data.size() - desiredLen, desiredLen);
        }
        
        /// @brief Truncate end of a string
        /// @param[in] data        String value
        /// @param[in] desiredLen  Desired length after truncation
        /// @returns Truncated string
        template <typename T>
        static inline std::basic_string<T> rtruncate(const std::basic_string<T>& data, const size_t desiredLen) 
        { 
            return (desiredLen <= data.size()) ? data.substr(0, desiredLen) : data;
        }
        /// @brief Truncate end of a string
        /// @param[in|out] data    String value to truncate
        /// @param[in] desiredLen  Desired length after truncation
        template <typename T>
        static inline void rtruncateSelf(std::basic_string<T>& data, const size_t desiredLen) 
        { 
            if (desiredLen <= data.size()) 
                data = data.substr(0, desiredLen);
        }

        
        /// @brief Add padding at the beginning of a string
        /// @param[in] data        String value
        /// @param[in] desiredLen  Desired length with padding
        /// @param[in] padChar     Padding character (default: space)
        /// @returns Padded string
        template <typename T>
        static inline std::basic_string<T> lpad(const std::basic_string<T>& data, const size_t desiredLen, const T padChar = T { 0x20 }) 
        { 
            if (desiredLen > data.size())
            {
                std::basic_string<T> out;
                out.reserve(desiredLen + 1);
                for (size_t padding = desiredLen - data.size(); padding > 0u; --padding)
                    out += padChar;
                out += data;
            }
            else
                return data;
        }
        /// @brief Add padding at the beginning of a string
        /// @param[in|out] data    String value to pad
        /// @param[in] desiredLen  Desired length with padding
        /// @param[in] padChar     Padding character (default: space)
        template <typename T>
        static inline void lpadSelf(std::basic_string<T>& data, const size_t desiredLen, const T padChar = T { 0x20 }) 
        { 
            if (desiredLen > data.size())
            {
                std::basic_string<T> pad;
                pad.reserve(desiredLen + 1);
                for (size_t padding = desiredLen - data.size(); padding > 0u; --padding)
                    pad += padChar;
                pad += data;
                data = std::move(pad);
            }
        }
        
        /// @brief Add padding at the end of a string
        /// @param[in] data        String value
        /// @param[in] desiredLen  Desired length with padding
        /// @param[in] padChar     Padding character (default: space)
        /// @returns Padded string
        template <typename T>
        static inline std::basic_string<T> rpad(const std::basic_string<T>& data, const size_t desiredLen, const T padChar = T { 0x20 }) 
        { 
            if (desiredLen > data.size())
            {
                std::basic_string<T> out;
                out.reserve(desiredLen + 1);
                out = data;
                for (size_t padding = desiredLen - data.size(); padding > 0u; --padding)
                    out += padChar;
            }
            else
                return data;
        }
        /// @brief Add padding at the end of a string
        /// @param[in|out] data    String value to pad
        /// @param[in] desiredLen  Desired length with padding
        /// @param[in] padChar     Padding character (default: space)
        template <typename T>
        static inline void rpadSelf(std::basic_string<T>& data, const size_t desiredLen, const T padChar = T { 0x20 }) 
        { 
            if (desiredLen > data.size())
            {
                data.reserve(desiredLen + 1);
                for (size_t padding = desiredLen - data.size(); padding > 0u; --padding)
                    data += padChar;
            }
        }
        
        
        /// @brief Change the length of a string - truncate beginning or add padding at the beginning, depending on the current length
        /// @param[in] data        String value
        /// @param[in] desiredLen  Desired length
        /// @param[in] padChar     Padding character (default: space)
        template <typename T>
        static inline std::basic_string<T> ltruncateOrPad(const std::basic_string<T>& data, const size_t desiredLen, const T padChar = T { 0x20 }) 
        { 
            if (desiredLen <= data.size())
                return ltruncate(data, desiredLen);
            else
                return lpad(data, desiredLen);
        }
        /// @brief Change the length of a string - truncate beginning or add padding at the beginning, depending on the current length
        /// @param[in] data        String value
        /// @param[in] desiredLen  Desired length
        /// @param[in] padChar     Padding character (default: space)
        template <typename T>
        static inline void ltruncateOrPadSelf(std::basic_string<T>& data, const size_t desiredLen, const T padChar = T { 0x20 }) 
        { 
            if (desiredLen <= data.size())
                ltruncateSelf(data, desiredLen);
            else
                lpadSelf(data, desiredLen);
        }
        
        /// @brief Change the length of a string - truncate end or add padding at the end, depending on the current length
        /// @param[in] data        String value
        /// @param[in] desiredLen  Desired length
        /// @param[in] padChar     Padding character (default: space)
        template <typename T>
        static inline std::basic_string<T> rtruncateOrPad(const std::basic_string<T>& data, const size_t desiredLen, const T padChar = T { 0x20 }) 
        { 
            if (desiredLen <= data.size())
                return rtruncate(data, desiredLen);
            else
                return rpad(data, desiredLen);
        }
        /// @brief Change the length of a string - truncate end or add padding at the end, depending on the current length
        /// @param[in] data        String value
        /// @param[in] desiredLen  Desired length
        /// @param[in] padChar     Padding character (default: space)
        template <typename T>
        static inline void rtruncateOrPadSelf(std::basic_string<T>& data, const size_t desiredLen, const T padChar = T { 0x20 }) 
        { 
            if (desiredLen <= data.size())
                rtruncateSelf(data, desiredLen);
            else
                rpadSelf(data, desiredLen);
        }
        
        

        // -- Compare --
        
        /// @brief Compare two strings
        /// @param[in] lhs  First string value
        /// @param[in] rhs  Second string value
        /// @returns Comparison result : lhs==rhs (0), lhs<rhs (-1), lhs>rhs (1)
        template <typename T>
        static int32_t compare(const T* lhs, const T* rhs);
        /// @brief Compare two strings
        /// @param[in] lhs  First string value
        /// @param[in] rhs  Second string value
        /// @returns Comparison result : lhs==rhs (0), lhs<rhs (-1), lhs>rhs (1)
        template <typename T>
        static inline int32_t compare(const std::basic_string<T>& lhs, const std::basic_string<T>& rhs) { return lhs.compare(rhs); }
        
        
        /// @brief Check equality of two strings (case-sensitive)
        /// @param[in] lhs  First string value
        /// @param[in] rhs  Second string value
        /// @returns Equal (true) or not
        template <typename T>
        static inline bool equals(const T* lhs, const T* rhs) 
        { 
            if (lhs != nullptr && rhs != nullptr)
            {
                const T* lhsIt = lhs;
                const T* rhsIt = rhs;
                while (*lhsIt != 0 && *lhsIt == *rhsIt)
                {
                    ++lhsIt;
                    ++rhsIt;
                }
                return (*lhsIt == *rhsIt); // both '\0'
            }
            return false;
        }
        /// @brief Check equality of two strings (case-sensitive)
        /// @param[in] lhs  First string value
        /// @param[in] rhs  Second string value
        /// @returns Equal (true) or not
        template <typename T>
        static inline bool equals(const std::basic_string<T>& lhs, const std::basic_string<T>& rhs) { return (lhs.compare(rhs) == 0); }

        
        /// @brief Check equality of two strings (case-insensitive for ASCII alphabetic characters)
        /// @param[in] lhs  First string value
        /// @param[in] rhs  Second string value
        /// @returns Equal (true) or not
        template <typename T>
        static inline bool iequalsAscii(const T* lhs, const T* rhs) 
        { 
            if (lhs != nullptr && rhs != nullptr)
            {
                const T* lhsIt = lhs;
                const T* rhsIt = rhs;
                while (*lhsIt != 0 && utils::Char::iequalsAscii(*lhsIt, *rhsIt))
                {
                    ++lhsIt;
                    ++rhsIt;
                }
                return (*lhsIt == *rhsIt); // both '\0'
            }
            return false;
        }
        /// @brief Check equality of two strings (case-insensitive for ASCII alphabetic characters)
        /// @param[in] lhs  First string value
        /// @param[in] rhs  Second string value
        /// @returns Equal (true) or not
        template <typename T>
        static inline bool iequalsAscii(const std::basic_string<T>& lhs, const std::basic_string<T>& rhs) 
        { 
            const T* lhsIt = lhs.c_str();
            const T* rhsIt = rhs.c_str();
            while (*lhsIt != 0 && utils::Char::iequalsAscii(*lhsIt, *rhsIt))
            {
                ++lhsIt;
                ++rhsIt;
            }
            return (*lhsIt == *rhsIt); // both '\0'
        }
        
        
        /// @brief Check equality of two strings (case-insensitive for unicode alphabetic characters)
        /// @param[in] lhs  First string value
        /// @param[in] rhs  Second string value
        /// @returns Equal (true) or not
        template <typename T>
        static inline bool iequalsUnicode(const T* lhs, const T* rhs) 
        { 
            if (lhs != nullptr && rhs != nullptr)
            {
                const T* lhsIt = lhs;
                const T* rhsIt = rhs;
                while (*lhsIt != 0 && utils::Char::iequalsUnicode(*lhsIt, *rhsIt))
                {
                    ++lhsIt;
                    ++rhsIt;
                }
                return (*lhsIt == *rhsIt); // both '\0'
            }
            return false;
        }
        /// @brief Check equality of two strings (case-insensitive for unicode alphabetic characters)
        /// @param[in] lhs  First string value
        /// @param[in] rhs  Second string value
        /// @returns Equal (true) or not
        template <typename T>
        static inline bool iequalsUnicode(const std::basic_string<T>& lhs, const std::basic_string<T>& rhs) 
        { 
            const T* lhsIt = lhs.c_str();
            const T* rhsIt = rhs.c_str();
            while (*lhsIt != 0 && utils::Char::iequalsUnicode(*lhsIt, *rhsIt))
            {
                ++lhsIt;
                ++rhsIt;
            }
            return (*lhsIt == *rhsIt); // both '\0'
        }
        
        
        
        // -- Upper/lower-case converters --
        
        /// @brief Convert string to another upper-case string (ASCII alphabetic characters only)
        /// @param[in] data  String value
        /// @param[out] out  Output value
        template <typename T>
        static inline void toUpperAscii(const T* data, T* out) 
        { 
            if (data != nullptr && out != nullptr)
            {
                while (*data != 0)
                {
                    *out = utils::Char::toUpperAscii(*data);
                    ++data;
                    ++out;
                }
                *out = 0;
            }
        }
        /// @brief Convert string to another upper-case string (ASCII alphabetic characters only)
        /// @param[in] data  String value
        /// @returns Output value
        template <typename T>
        static inline std::basic_string<T> toUpperAscii(const std::basic_string<T>& data) 
        { 
            std::basic_string<T> out;
            out.reserve(data.size() + 1);
            for (uint32_t i = 0; i < data.size(); ++i)
                out += utils::Char::toUpperAscii(data[i]);
            return out;
        }
        /// @brief Convert string to upper-case string (ASCII alphabetic characters only)
        /// @param[in|out] data  String value to convert
        template <typename T>
        static inline void toUpperAsciiSelf(T* data) 
        { 
            if (data != nullptr)
            {
                while (*data != 0)
                {
                    *data = utils::Char::toUpperAscii(*data);
                    ++data;
                }
            }
        }
        /// @brief Convert string to upper-case string (ASCII alphabetic characters only)
        /// @param[in|out] data  String value to convert
        template <typename T>
        static inline void toUpperAsciiSelf(std::basic_string<T>& data) 
        { 
            for (int32_t i = data.size() - 1; i >= 0; --i)
                data[i] = utils::Char::toUpperAscii(data[i]);
        }
        
        
        /// @brief Convert string to another upper-case string (unicode alphabetic characters)
        /// @param[in] data  String value
        /// @param[out] out  Output value
        template <typename T>
        static inline void toUpperUnicode(const T* data, T* out) 
        { 
            if (data != nullptr && out != nullptr)
            {
                while (*data != 0)
                {
                    *out = utils::Char::toUpperUnicode(*data);
                    ++data;
                    ++out;
                }
                *out = 0;
            }
        }
        /// @brief Convert string to another upper-case string (unicode alphabetic characters)
        /// @param[in] data  String value
        /// @returns Output value
        template <typename T>
        static inline std::basic_string<T> toUpperUnicode(const std::basic_string<T>& data) 
        { 
            std::basic_string<T> out;
            out.reserve(data.size() + 1);
            for (uint32_t i = 0; i < data.size(); ++i)
                out += utils::Char::toUpperUnicode(data[i]);
            return out;
        }
        /// @brief Convert string to upper-case string (unicode alphabetic characters)
        /// @param[in|out] data  String value to convert
        template <typename T>
        static inline void toUpperUnicodeSelf(T* data) 
        { 
            if (data != nullptr)
            {
                while (*data != 0)
                {
                    *data = utils::Char::toUpperUnicode(*data);
                    ++data;
                }
            }
        }
        /// @brief Convert string to upper-case string (unicode alphabetic characters)
        /// @param[in|out] data  String value to convert
        template <typename T>
        static inline void toUpperUnicodeSelf(std::basic_string<T>& data) 
        { 
            for (int32_t i = data.size() - 1; i >= 0; --i)
                data[i] = utils::Char::toUpperUnicode(data[i]);
        }
        
        
        /// @brief Convert string to another lower-case string (ASCII alphabetic characters only)
        /// @param[in] data  String value
        /// @param[out] out  Output value
        template <typename T>
        static inline void toLowerAscii(const T* data, T* out) 
        { 
            if (data != nullptr && out != nullptr)
            {
                while (*data != 0)
                {
                    *out = utils::Char::toLowerAscii(*data);
                    ++data;
                    ++out;
                }
                *out = 0;
            }
        }
        /// @brief Convert string to another lower-case string (ASCII alphabetic characters only)
        /// @param[in] data  String value
        /// @returns Output value
        template <typename T>
        static inline std::basic_string<T> toLowerAscii(const std::basic_string<T>& data) 
        { 
            std::basic_string<T> out;
            out.reserve(data.size() + 1);
            for (uint32_t i = 0; i < data.size(); ++i)
                out += utils::Char::toLowerAscii(data[i]);
            return out;
        }
        /// @brief Convert string to lower-case string (ASCII alphabetic characters only)
        /// @param[in|out] data  String value to convert
        template <typename T>
        static inline void toLowerAsciiSelf(T* data) 
        { 
            if (data != nullptr)
            {
                while (*data != 0)
                {
                    *data = utils::Char::toLowerAscii(*data);
                    ++data;
                }
            }
        }
        /// @brief Convert string to lower-case string (ASCII alphabetic characters only)
        /// @param[in|out] data  String value to convert
        template <typename T>
        static inline void toLowerAsciiSelf(std::basic_string<T>& data) 
        { 
            for (int32_t i = data.size() - 1; i >= 0; --i)
                data[i] = utils::Char::toLowerAscii(data[i]);
        }
        
        
        /// @brief Convert string to another lower-case string (unicode alphabetic characters)
        /// @param[in] data  String value
        /// @param[out] out  Output value
        template <typename T>
        static inline void toLowerUnicode(const T* data, T* out) 
        { 
            if (data != nullptr && out != nullptr)
            {
                while (*data != 0)
                {
                    *out = utils::Char::toLowerUnicode(*data);
                    ++data;
                    ++out;
                }
                *out = 0;
            }
        }
        /// @brief Convert string to another lower-case string (unicode alphabetic characters)
        /// @param[in] data  String value
        /// @returns Output value
        template <typename T>
        static inline std::basic_string<T> toLowerUnicode(const std::basic_string<T>& data) 
        { 
            std::basic_string<T> out;
            out.reserve(data.size() + 1);
            for (uint32_t i = 0; i < data.size(); ++i)
                out += utils::Char::toLowerUnicode(data[i]);
            return out;
        }
        /// @brief Convert string to lower-case string (unicode alphabetic characters)
        /// @param[in|out] data  String value to convert
        template <typename T>
        static inline void toLowerUnicodeSelf(T* data) 
        { 
            if (data != nullptr)
            {
                while (*data != 0)
                {
                    *data = utils::Char::toLowerUnicode(*data);
                    ++data;
                }
            }
        }
        /// @brief Convert string to lower-case string (unicode alphabetic characters)
        /// @param[in|out] data  String value to convert
        template <typename T>
        static inline void toLowerUnicodeSelf(std::basic_string<T>& data) 
        { 
            for (int32_t i = data.size() - 1; i >= 0; --i)
                data[i] = utils::Char::toLowerUnicode(data[i]);
        }
        

        
        // -- Find first --
        
        static constexpr size_t notFound = std::string::npos; ///< Needle not found (find/rfind methods)
        
        /// @brief Find first occurrence of a character in a string
        /// @param[in] data        String value
        /// @param[in] needleChar  Character to find (needle)
        /// @returns Needle position
        template <typename T>
        static inline size_t find(const T* data, const T needleChar)
        {
            size_t needlePos = utils::String::notFound;
            size_t curPos = 0u;
            if (data != nullptr)
            {
                for (const T* it = data; *it != 0; ++it)
                {
                    if (*it == needleChar)
                    {
                        needlePos = curPos;
                        break;
                    }
                    ++curPos;
                }
            }
            return needlePos;
        }
        /// @brief Find first occurrence of a character in a string
        /// @param[in] data        String value
        /// @param[in] needleChar  Character to find (needle)
        /// @returns Needle position
        template <typename T>
        static inline size_t find(const std::basic_string<T> data, const T needleChar)
        {
            return data.find(needleChar);
        }
        
        /// @brief Find first occurrence of a pattern in a string
        /// @param[in] data     String value
        /// @param[in] pattern  String to find
        /// @returns Pattern position
        template <typename T>
        static size_t find(const T* data, const T* pattern);
        /// @brief Find first occurrence of a pattern in a string
        /// @param[in] data     String value
        /// @param[in] pattern  String to find
        /// @returns Pattern position
        template <typename T>
        static inline size_t find(const std::basic_string<T> data, const std::basic_string<T> pattern)
        {
            return data.find(pattern);
        }
        
        
        // -- Find last --
        
        /// @brief Find last occurrence of a character in a string
        /// @param[in] data        String value
        /// @param[in] needleChar  Character to find (needle)
        /// @returns Needle position
        /// @warning Very inefficient. Prefer rfind(const T* data, const size_t len, const T needleChar).
        template <typename T>
        static inline size_t rfind(const T* data, const T needleChar)
        {
            size_t needlePos = utils::String::notFound;
            size_t curPos = 0u;
            if (data != nullptr)
            {
                for (const T* it = data; *it != 0; ++it)
                {
                    if (*it == needleChar)
                        needlePos = curPos;
                    ++curPos;
                }
            }
            return needlePos;
        }
        /// @brief Find last occurrence of a character in a string
        /// @param[in] data        String value
        /// @param[in] needleChar  Character to find (needle)
        /// @returns Needle position
        template <typename T>
        static inline size_t rfind(const T* data, const size_t len, const T needleChar)
        {
            size_t needlePos = utils::String::notFound;
            size_t curPos = len;
            if (data != nullptr)
            {
                for (const T* it = data[len - 1]; curPos > 0; --it)
                {
                    if (*it == needleChar)
                    {
                        needlePos = curPos - 1u;
                        break;
                    }
                    --curPos;
                }
            }
            return needlePos;
        }
        /// @brief Find last occurrence of a character in a string
        /// @param[in] data        String value
        /// @param[in] needleChar  Character to find (needle)
        /// @returns Needle position
        template <typename T>
        static inline size_t rfind(const std::basic_string<T> data, const T needleChar)
        {
            return data.rfind(needleChar);
        }
        
        /// @brief Find last occurrence of a pattern in a string
        /// @param[in] data     String value
        /// @param[in] pattern  String to find
        /// @returns Pattern position
        /// @warning Very inefficient. Prefer rfind(const T* data, const size_t len, const T* pattern).
        template <typename T>
        static size_t rfind(const T* data, const T* pattern);
        /// @brief Find last occurrence of a pattern in a string
        /// @param[in] data     String value
        /// @param[in] pattern  String to find
        /// @returns Pattern position
        template <typename T>
        static size_t rfind(const T* data, const size_t len, const T* pattern);
        /// @brief Find last occurrence of a pattern in a string
        /// @param[in] data     String value
        /// @param[in] pattern  String to find
        /// @returns Pattern position
        template <typename T>
        static inline size_t rfind(const std::basic_string<T> data, const std::basic_string<T> pattern)
        {
            return data.rfind(pattern);
        }
        
        
        // -- Find all --
        
        /// @brief Find all occurrences of a character in a string
        /// @param[in] data        String value
        /// @param[in] needleChar  Character to find (needle)
        /// @returns Vector with found positions
        template <typename T>
        static inline std::vector<size_t> findAll(const T* data, const T needleChar)
        {
            std::vector<size_t> posArray;
            posArray.reserve(8);
            size_t curPos = 0u;
            if (data != nullptr)
            {
                for (const T* it = data; *it != 0; ++it)
                {
                    if (*it == needleChar)
                        posArray.push_back(curPos);
                    ++curPos;
                }
            }
            return posArray;
        }
        /// @brief Find all occurrences of a character in a string
        /// @param[in] data        String value
        /// @param[in] needleChar  Character to find (needle)
        /// @returns Vector with found positions
        template <typename T>
        static inline std::vector<size_t> findAll(const std::basic_string<T> data, const T needleChar)
        {
            std::vector<size_t> posArray;
            posArray.reserve(8);
            size_t curPos = 0u;
            if (data.empty() == false)
            {
                for (const T* it = data.c_str(); *it != 0; ++it)
                {
                    if (*it == needleChar)
                        posArray.push_back(curPos);
                    ++curPos;
                }
            }
            return posArray;
        }
        
        /// @brief Find all occurrences of a pattern in a string (occurrences may overlap)
        /// @param[in] data     String value
        /// @param[in] pattern  String to find
        /// @returns Vector with found positions
        template <typename T>
        static std::vector<size_t> findAll(const T* data, const T* pattern);
        /// @brief Find all occurrences of a pattern in a string (occurrences may overlap)
        /// @param[in] data     String value
        /// @param[in] pattern  String to find
        /// @returns Vector with found positions
        template <typename T>
        static std::vector<size_t> findAll(const std::basic_string<T> data, const std::basic_string<T> pattern);
        
        
        // -- Count all --

        /// @brief Count all occurrences of a character in a string
        /// @param[in] data        String value
        /// @param[in] needleChar  Character to find (needle)
        /// @returns Number of occurrences
        template <typename T>
        static inline uint32_t countOccurrences(const T* data, const T needleChar)
        {
            uint32_t count = 0u;
            size_t curPos = 0u;
            if (data != nullptr)
            {
                for (const T* it = data; *it != 0; ++it)
                {
                    if (*it == needleChar)
                        ++count;
                    ++curPos;
                }
            }
            return count;
        }
        /// @brief Count all occurrences of a character in a string
        /// @param[in] data        String value
        /// @param[in] needleChar  Character to find (needle)
        /// @returns Number of occurrences
        template <typename T>
        static inline uint32_t countOccurrences(const std::basic_string<T> data, const T needleChar)
        {
            uint32_t count = 0u;
            size_t curPos = 0u;
            if (data.empty() == false)
            {
                for (const T* it = data.c_str(); *it != 0; ++it)
                {
                    if (*it == needleChar)
                        ++count;
                    ++curPos;
                }
            }
            return count;
        }
        
        /// @brief Count all occurrences of a pattern in a string (occurrences may overlap)
        /// @param[in] data     String value
        /// @param[in] pattern  String to find
        /// @returns Number of occurrences
        template <typename T>
        static uint32_t countOccurrences(const T* data, const T* pattern);
        /// @brief Count all occurrences of a pattern in a string (occurrences may overlap)
        /// @param[in] data     String value
        /// @param[in] pattern  String to find
        /// @returns Number of occurrences
        template <typename T>
        static uint32_t countOccurrences(const std::basic_string<T> data, const std::basic_string<T> pattern);
        
        
        // -- Other finding utilities --
        
        /// @brief Check whether a string contains a character or not
        /// @param[in] data        String value
        /// @param[in] needleChar  Character to find
        /// @returns Occurrence(s) found (true) or not
        template <typename T>
        static inline bool contains(const T* data, const T needleChar) { return (find(data, needleChar) != utils::String::notFound); }
        /// @brief Check whether a string contains a character or not
        /// @param[in] data        String value
        /// @param[in] needleChar  Character to find
        /// @returns Occurrence(s) found (true) or not
        template <typename T>
        static inline bool contains(const std::basic_string<T> data, const T needleChar) { return (data.find(needleChar) != std::basic_string<T>::npos); }
        /// @brief Check whether a string contains a pattern or not
        /// @param[in] data     String value
        /// @param[in] pattern  Pattern to find
        /// @returns Occurrence(s) found (true) or not
        template <typename T>
        static inline bool contains(const T* data, const T* pattern) { return (find(data, pattern) != utils::String::notFound); }
        /// @brief Check whether a string contains a pattern or not
        /// @param[in] data     String value
        /// @param[in] pattern  Pattern to find
        /// @returns Occurrence(s) found (true) or not
        template <typename T>
        static inline bool contains(const std::basic_string<T> data, const std::basic_string<T> pattern) { return (data.find(pattern) != std::basic_string<T>::npos); }
        
        /// @brief Check whether a string starts with a character or not
        /// @param[in] data        String value
        /// @param[in] needleChar  Character to find
        /// @returns Occurrence(s) found at the beginning (true) or not
        template <typename T>
        static inline bool startsWith(const T* data, const T needleChar) { return (data != nullptr && *data == needleChar); }
        /// @brief Check whether a string starts with a character or not
        /// @param[in] data        String value
        /// @param[in] needleChar  Character to find
        /// @returns Occurrence(s) found at the beginning (true) or not
        template <typename T>
        static inline bool startsWith(const std::basic_string<T> data, const T needleChar) { return (data.empty() == false && data[0] == needleChar); }
        /// @brief Check whether a string starts with a pattern or not
        /// @param[in] data     String value
        /// @param[in] pattern  Pattern to find
        /// @returns Occurrence(s) found at the beginning (true) or not
        template <typename T>
        static inline bool startsWith(const T* data, const T* pattern)
        {
            if (data != nullptr && pattern != nullptr)
            {
                while (*data != 0 && *data == *pattern)
                {
                    ++data;
                    ++pattern;
                }
                return (*pattern == 0);
            }
            return false;
        }
        /// @brief Check whether a string starts with a pattern or not
        /// @param[in] data     String value
        /// @param[in] pattern  Pattern to find
        /// @returns Occurrence(s) found at the beginning (true) or not
        template <typename T>
        static inline bool startsWith(const std::basic_string<T> data, const std::basic_string<T> pattern)
        {
            const T* it = data.c_str();
            const T* patternIt = pattern.c_str();
            while (*it != 0 && *it == *patternIt)
            {
                ++it;
                ++patternIt;
            }
            return (*patternIt == 0);
        }
        
        /// @brief Check whether a string ends with a character or not
        /// @param[in] data        String value
        /// @param[in] needleChar  Character to find
        /// @returns Occurrence(s) found at the end (true) or not
        template <typename T>
        static inline bool endsWith(const T* data, const size_t len, const T needleChar) { return (data != nullptr && data[len - 1u] == needleChar); }
        /// @brief Check whether a string ends with a character or not
        /// @param[in] data        String value
        /// @param[in] needleChar  Character to find
        /// @returns Occurrence(s) found at the end (true) or not
        template <typename T>
        static inline bool endsWith(const std::basic_string<T> data, const T needleChar) { return (data.empty() == false && data[data.size() - 1u] == needleChar); }
        /// @brief Check whether a string ends with a pattern or not
        /// @param[in] data     String value
        /// @param[in] pattern  Pattern to find
        /// @returns Occurrence(s) found at the end (true) or not
        template <typename T>
        static inline bool endsWith(const T* data, const size_t len, const T* pattern) { return (rfind(data, len, pattern) == len - getLength(pattern)); }
        {
            size_t patternLen = getLength(pattern);
            if (data != nullptr && patternLen > 0u && patternLen <= len)
            {
                const T* it = &data[len - patternLen];
                while (*it != 0 && *it == *pattern)
                {
                    ++it;
                    ++pattern;
                }
                return (*pattern == 0);
            }
            return false;
        }
        /// @brief Check whether a string ends with a pattern or not
        /// @param[in] data     String value
        /// @param[in] pattern  Pattern to find
        /// @returns Occurrence(s) found at the end (true) or not
        template <typename T>
        static inline bool endsWith(const std::basic_string<T> data, const std::basic_string<T> pattern) { return (data.rfind(pattern) == data.size() - pattern.size()); }
        {
            size_t patternLen = pattern.size();
            if (patternLen > 0u && patternLen <= len)
            {
                const T* it = &data.c_str[data.size() - patternLen];
                const T* patternIt = pattern.c_str();
                while (*it != 0 && *it == *patternIt)
                {
                    ++it;
                    ++patternIt;
                }
                return (*patternIt == 0);
            }
            return false;
        }
        
        
        
        // -- Split --

        /// @brief Split a string at the specified position (character at position is included in second part)
        /// @param[in] data  String value
        /// @param[in] pos   Split position (included in second part)
        /// @returns Vector with sub-strings
        template <typename T>
        static inline std::vector<std::basic_string<T>> splitAt(const std::basic_string<T>& data, const uint32_t pos)
        {
            if (data.size() > pos)
            {
                std::vector<std::basic_string<T>> parts { data.substr(0, pos), data.substr(pos) };
                return parts;
            }
            else
                return std::vector<std::basic_string<T>> { data };
        }
        /// @brief Split a string at the specified position (character at position is excluded)
        /// @param[in] data  String value
        /// @param[in] pos   Split position (excluded)
        /// @returns Vector with sub-strings
        template <typename T>
        static inline std::vector<std::basic_string<T>> splitAtExcluded(const std::basic_string<T>& data, const uint32_t pos)
        {
            if (data.size() > pos + 1)
            {
                std::vector<std::basic_string<T>> parts { data.substr(0, pos), data.substr(pos + 1u) };
                return parts;
            }
            else if (data.size() > pos)
                return std::vector<std::basic_string<T>> { data.substr(0, pos) };
            else
                return std::vector<std::basic_string<T>> { data };
        }
    
        
        /// @brief Split a string every time an occurrence of a character is found
        /// @param[in] data                 String value
        /// @param[in] sep                  Separator (character)
        /// @param[in] isKeepingEmptyParts  If two separators are contiguous, add an empty sub-string (default: false)
        /// @returns Vector with sub-strings
        template <typename T>
        static std::vector<std::basic_string<T>> split(const std::basic_string<T>& data, const T sep, const bool isKeepingEmptyParts = false);
        
        /// @brief Split a string every time an occurrence of a pattern is found (no overlap)
        /// @param[in] data                 String value
        /// @param[in] sepString            Separator (pattern)
        /// @param[in] isKeepingEmptyParts  If two separators are contiguous, add an empty sub-string (default: false)
        /// @returns Vector with sub-strings
        template <typename T>
        static std::vector<std::basic_string<T>> split(const std::basic_string<T>& data, const std::basic_string<T>& sepString, const bool isKeepingEmptyParts = false);
        
        
        
        // -- Trim --
        
        /// @brief Remove all white-space characters at the beginning of a string
        /// @param[in] data  String value
        /// @returns Left-trimmed string
        template <typename T>
        static inline std::basic_string<T> ltrim(const std::basic_string<T>& data)
        {
            if (data.empty() == false)
            {
                // find first non-white-space character
                size_t pos = 0u;
                const T* it = data.c_str();
                while (utils::Char::isWhiteSpace(*it))
                {
                    ++it;
                    ++pos;
                }
                // trim
                return (pos == 0u) ? data : data.substr(pos);
            }
        }
        
        /// @brief Remove all white-space characters at the end of a string
        /// @param[in] data  String value
        /// @returns Right-trimmed string
        template <typename T>
        static inline std::basic_string<T> rtrim(const std::basic_string<T>& data)
        {
            if (data.empty() == false)
            {
                // find last non-white-space character
                size_t pos = data.size() - 1u;
                const T* it = data.c_str();
                it += pos;
                while (pos >= 0u && utils::Char::isWhiteSpace(*it))
                {
                    --it;
                    --pos;
                }
                // trim
                return (pos == data.size() - 1u) ? data : data.substr(0, pos + 1u);
            }
        }
        
        /// @brief Remove all white-space characters at the beginning and the end of a string
        /// @param[in] data  String value
        /// @returns Trimmed string
        template <typename T>
        static std::basic_string<T> trim(const std::basic_string<T>& data);
    };
}
