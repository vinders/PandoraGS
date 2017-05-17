/*******************************************************************************
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : advanced file writer
*******************************************************************************/
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <ofstream>
#include "file_io.h"
#include "string_encoder.h"
#include "../memory/flag_set.hpp"

/// @namespace utils
/// General utilities
namespace utils
{
    /// @namespace utils.io
    /// Input/output utilities
    namespace io
    {
        /// @class FileWriter
        /// @brief Advanced file writer - with encoder specialization
        /// @see FileIO
        template <FileIO::file_encoder_t Encoder, bool checkConcurrency = true>
        class FileWriter : public FileIO
        {
        public:
            /// @enum file_mode_t
            /// @brief File opening mode
            enum class file_mode_t : uint32_t
            {
                truncate = 0u,
                append = 1u
            };
            /// @enum flag_t
            /// @brief File writer flags - can be combined with |
            enum flag_t : uint32_t
            {
                FileWriter_flushBufferOnWrite = 0x01,  ///< Flush output buffer content after each write operation
                FileWriter_boolAsAlpha = 0x04,         ///< Write boolean names ("true"/"false") instead of "1"/"0" (only in text mode)
                FileWriter_showPositiveSign = 0x10,    ///< Add '+' sign before positive numbers (only in text mode)
                FileWriter_intToHex = 0x11,            ///< Hexadecimal notation for integers (only in text mode)
                FileWriter_intToOctal = 0x12,          ///< Octal notation for integers (only in text mode)
                FileWriter_floatFixed = 0x14,          ///< Fixed notation (##.##) for floating-point numbers (only in text mode)
                FileWriter_floatScientific = 0x18,     ///< Scientific notation (#.###E#) for floating-point numbers (only in text mode)
                FileWriter_stringToUppercase = 0x20,   ///< Convert strings to uppercase (only in text mode)
                FileWriter_stringToLowercase = 0x21,   ///< Convert strings to lowercase (only in text mode)
                FileWriter_trimLeft = 0x22,            ///< Remove leading white-space characters from strings for each operation (only in text mode)
                FileWriter_trimRight = 0x24            ///< Remove trailing white-space characters from strings for each operation (only in text mode)
            };
            
            
            /// @brief Create file writer instance (specialized)
            FileWriter() noexcept : FileIO(), m_floatDecimalPrecision(0u), m_formatFlags(0u) {}
            /// @brief Move file writer instance
            /// @param[in] other  Other instance
            FileWriter(FileWriter&& other) : 
                FileIO(other), 
                m_fileStream(std::move(other.m_fileStream)), 
                m_floatDecimalPrecision(other.m_floatDecimalPrecision), 
                m_formatFlags(other.m_formatFlags), 
                m_errorBuffer(std::move(other.m_errorBuffer)) {}
            // no copy allowed
            FileWriter(const FileWriter& other) = delete;
            /// @brief Destroy file writer instance
            ~FileWriter()
            {
                close();
            }
            
            /// @brief Assign moved file writer instance
            /// @param[in] other  Other instance
            /// @returns Moved instance
            FileWriter& operator=(FileWriter&& other)
            {
                FileIO::operator=(other);
                m_fileStream = std::move(other.m_fileStream); 
                m_floatDecimalPrecision = other.m_floatDecimalPrecision;
                m_formatFlags = other.m_formatFlags;
                m_errorBuffer = std::move(other.m_errorBuffer);
            }
            // no copy allowed
            FileWriter& operator=(const FileWriter& other) = delete;
            /// @brief Swap file writer instances
            /// @param[in] other  Other instance
            void swap(FileWriter& other) noexcept
            {
                FileIO::swap(other);
                m_fileStream.swap(other.m_fileStream); 
                std::swap(m_floatDecimalPrecision, other.m_floatDecimalPrecision);
                m_formatFlags.swap(other.m_formatFlags);
                m_errorBuffer.swap(other.m_errorBuffer);
            }
        
        
            // -- File management --
        
            /// @brief Open output file
            /// @param[in] filePath     Destination file path
            /// @param[in] openMode     File opening mode (optional)
            /// @param[in] formatFlags  Formatting flags (optional, see enum flag_t)
            /// @returns Success (use getLastErrorMessage() if false)
            bool open(const std::wstring filePath, const file_mode_t openMode = file_mode_t::truncate, const flag_t formatFlags = 0) noexcept;
            /// @brief Close output file and flush buffer
            void close();
            /// @brief Flush output buffer
            /// @returns Current instance
            inline FileWriter& flush()
            {
                if (isOpen)
                {
                    lock<checkConcurrency>();
                    flush_noLock();
                    unlock<checkConcurrency>();
                }
                return *this;
            }
            
            /// @brief Change writer position in current file
            /// @param[in] whence  Reference position to move from
            /// @param[in] offset  Offset, based on reference (bytes)
            /// @returns Current instance
            /// @throws failure  Seek failure
            inline FileWriter& seek(const FileIO::seek_reference_t whence, const int32_t offset)
            {
                if (isOpen)
                {
                    lock<checkConcurrency>();
                    seek_noLock(whence, offset);
                    unlock<checkConcurrency>();
                }
                return *this;
            }
            /// @brief Check if a file is open
            /// @returns File open (true) or not
            inline bool isOpen() const noexcept
            {
                return m_fileStream.is_open();
            }
            /// @brief Check if current position is at the end of the file
            /// @returns End of file reached (true) or not
            inline bool isEof() const noexcept
            {
                return ((isOpen() == false) || m_fileStream.eof());
            }
            
            /// @brief Get most recent error message (if any)
            /// @returns Error message
            inline const std::string const& getLastErrorMessage() const noexcept
            {
                return m_errorBuffer;
            }
            
            
            // -- Writer settings --
            
            /// @brief Get configured file encoding
            /// @brief File encoding type
            inline FileIO::file_encoder_t getEncoding() const noexcept
            {
                return Encoder;
            }
            
            /// @brief Get formatting flags
            /// @returns Flag(s)
            inline flag_t getFormatFlags() const noexcept
            {
                return m_formatFlags.data();
            }
            /// @brief Set formatting flags
            /// @param[in] formatFlags  Flag(s)
            /// @returns Current instance
            inline FileWriter& setFormatFlags(const flag_t formatFlags) noexcept
            {
                if (isOpen())
                {
                    lock<checkConcurrency>();
                    setFormatFlags_noLock(formatFlags);
                    unlock<checkConcurrency>();
                }
                return *this;
            }
            
            /// @brief Get decimal precision for floating point numbers
            /// @returns Decimal precision
            inline uint32_t getFloatDecimalPrecision() const noexcept
            {
                return m_floatDecimalPrecision;
            }
            /// @brief Set decimal precision for floating point numbers
            /// @param[in] floatDecimalPrecision  Decimal precision (0 for maximum precision)
            /// @returns Current instance
            inline FileWriter& setFloatDecimalPrecision(const uint32_t floatDecimalPrecision) noexcept
            {
                lock<checkConcurrency>();
                setFloatDecimalPrecision_noLock(floatDecimalPrecision);
                unlock<checkConcurrency>();
                return *this;
            }
            
            
            // -- Output operations - general --
            
            /// @brief Write stream as is (no encoding) - use in binary mode
            /// @param[in] stream  Byte stream
            /// @param[in] size    Size of the stream (bytes)
            /// @returns Current object
            FileWriter& writeStream(const char* stream, const size_t size)
            {
                m_fileStream.write(stream, size);
                return *this;
            }

            
            /// @brief Write encoded string value - use in text mode (ANSI/UTF-8)
            /// @param[in] strVal  String value
            /// @returns Current object
            template <FileIO::string_encoder_t SrcEncoder>
            FileWriter& write(const char* strVal);
            /// @brief Write encoded wide-string value - use in text mode (UTF-16, UCS-2)
            /// @param[in] strVal  String value
            /// @returns Current object
            template <FileIO::wstring_encoder_t SrcEncoder = FileIO::wstring_encoder_t::utf16>
            FileWriter& write(const wchar_t* strVal);
            
            /// @brief Write encoded string value and append newline symbol - use in text mode
            /// @param[in] strVal  String value
            /// @returns Current object
            template <FileIO::string_encoder_t SrcEncoder>
            inline FileWriter& writeLine(const char* strVal)
            {
                return write<SrcEncoder>(strVal.c_str()).writeStream(FileIO::endl, 1u);
            }
            /// @brief Write encoded wide-string value and append newline symbol - use in text mode
            /// @param[in] strVal  String value
            /// @returns Current object
            template <FileIO::wstring_encoder_t SrcEncoder = FileIO::wstring_encoder_t::utf16>
            inline FileWriter& writeLine(const wchar_t* strVal);
            {
                return write<SrcEncoder>(strVal.c_str()).put(FileIO::endl);
            }
    
            
            
            // -- Output operations - base types --
            
            /// @brief Write boolean value
            FileWriter& put(const bool val);
            /// @brief Write unsigned character
            FileWriter& put(const unsigned char val)
            {
                put(reinterpret_cast<char>(val));
            }
            /// @brief Write single character
            FileWriter& put(const char val);
            /// @brief Write boolean value
            inline FileWriter& operator<<(const bool val) { return put(val); }
            /// @brief Write unsigned character
            inline FileWriter& operator<<(const unsigned char val) { return put(val); }
            /// @brief Write single character
            inline FileWriter& operator<<(const char val) { return put(val); }
            
            /// @brief Write unsigned short integer value
            FileWriter& put(const uint16_t val);
            /// @brief Write short integer value
            FileWriter& put(const int16_t val);
            /// @brief Write unsigned integer value
            FileWriter& put(const uint32_t val);
            /// @brief Write integer value
            FileWriter& put(const int32_t val);
            /// @brief Write unsigned 64-bit integer value
            FileWriter& put(const uint64_t val);
            /// @brief Write s64-bit integer value
            FileWriter& put(const int64_t val);
            /// @brief Write unsigned short integer value
            inline FileWriter& operator<<(const uint16_t val) { return put(val); }
            /// @brief Write short integer value
            inline FileWriter& operator<<(const int16_t val) { return put(val); }
            /// @brief Write unsigned integer value
            inline FileWriter& operator<<(const uint32_t val) { return put(val); }
            /// @brief Write integer value
            inline FileWriter& operator<<(const int32_t val) { return put(val); }
            /// @brief Write unsigned 64-bit integer value
            inline FileWriter& operator<<(const uint64_t val) { return put(val); }
            /// @brief Write 64-bit integer value
            inline FileWriter& operator<<(const int64_t val) { return put(val); }
            
            /// @brief Write floating-point value
            FileWriter& put(const float val);
            /// @brief Write double-precision floating-point value
            FileWriter& put(const double val);
            /// @brief Write long double-precision floating-point value
            FileWriter& put(const long double val);
            /// @brief Write floating-point value
            inline FileWriter& operator<<(const float val) { return put(val); }
            /// @brief Write double-precision floating-point value
            inline FileWriter& operator<<(const double val) { return put(val); }
            /// @brief Write long double-precision floating-point value
            inline FileWriter& operator<<(const long double val) { return put(val); }
            
            
            // -- Output operations - strings --
            
            /// @brief Write string value
            template <FileIO::string_encoder_t SrcEncoder>
            inline FileWriter& write(const std::string& strVal) { return write<SrcEncoder>(strVal.c_str()); }
            /// @brief Write string value (move)
            template <FileIO::string_encoder_t SrcEncoder>
            inline FileWriter& write(std::string&& strVal) { return write<SrcEncoder>(strVal.c_str()); }
            /// @brief Write wide-string value
            template <FileIO::wstring_encoder_t SrcEncoder = FileIO::wstring_encoder_t::utf16>
            inline FileWriter& write(const std::wstring& strVal) { return write<SrcEncoder>(strVal.c_str()); }
            /// @brief Write wide-string value (move)
            template <FileIO::wstring_encoder_t SrcEncoder = FileIO::wstring_encoder_t::utf16>
            inline FileWriter& write(std::wstring&& strVal) { return write<SrcEncoder>(strVal.c_str()); }
            /// @brief Write string value
            inline FileWriter& operator<<(const std::string& strVal) { return write<FileIO_op_sys_string_encoder_t>(strVal.c_str()); }
            /// @brief Write string value (move)
            inline FileWriter& operator<<(std::string&& strVal) { return write<FileIO_op_sys_string_encoder_t>(strVal.c_str()); }
            /// @brief Write wide-string value
            inline FileWriter& operator<<(const std::wstring& strVal) { return write<FileIO::wstring_encoder_t::utf16>(strVal.c_str()); }
            /// @brief Write wide-string value (move)
            inline FileWriter& operator<<(std::wstring&& strVal) { return write<FileIO::wstring_encoder_t::utf16>(strVal.c_str()); }
           
            /// @brief Write string value and append newline symbol
            template <FileIO::string_encoder_t SrcEncoder>
            inline FileWriter& writeLine(const std::string& strVal) { return write<SrcEncoder>(strVal.c_str()); }
            /// @brief Write string value and append newline symbol (move)
            template <FileIO::string_encoder_t SrcEncoder>
            inline FileWriter& writeLine(std::string&& strVal) { return write<SrcEncoder>(strVal.c_str()); }
            /// @brief Write wide-string value and append newline symbol
            template <FileIO::wstring_encoder_t SrcEncoder = FileIO::wstring_encoder_t::utf16>
            inline FileWriter& writeLine(const std::wstring& strVal) { return write<SrcEncoder>(strVal.c_str()); }
            /// @brief Write wide-string value and append newline symbol (move)
            template <FileIO::wstring_encoder_t SrcEncoder = FileIO::wstring_encoder_t::utf16>
            inline FileWriter& writeLine(std::wstring&& strVal) { return write<SrcEncoder>(strVal.c_str()); }
            
        
            
        private:
            /// @brief Flush output buffer
            inline void flush_noLock()
            {
                m_fileStream.flush();
            }
            
            /// @brief Write wide-string text data
            /// @param[in] val   Text data
            /// @returns Current object
            inline FileWriter& writeWideString(const wchar_t* val)
            {
                if (val == nullptr)
                    return *this;
                if (Encoder == FileIO::file_encoder_t::utf16_be)
                {
                    std::string data = StringEncoder::wideStringToBigEndianBytes(val);
                    return writeStream(data.c_str(), data.size());
                }
                else
                {
                    std::string data = StringEncoder::wideStringToLittleEndianBytes(val);
                    return writeStream(data.c_str(), data.size());
                }
            }
            
            /// @brief Change writer position in current file
            /// @param[in] whence  Reference position to move from
            /// @param[in] offset  Offset, based on reference (bytes)
            /// @throws failure  Seek failure
            inline void seek_noLock(const FileIO::seek_reference_t whence, const int32_t offset)
            {
                switch (whence)
                {
                    case FileIO::seek_reference_t::begin:
                        m_fileStream.seekp((Encoder != FileIO::file_encoder_t::utf8_bom) ? offset : offset + 3, ios_base::beg); 
                        break;
                    case FileIO::seek_reference_t::end:
                        m_fileStream.seekp(offset, ios_base::end); 
                        break;
                    case FileIO::seek_reference_t::cur
                    default: 
                        m_fileStream.seekp(offset, ios_base::cur); 
                        break;
                }
            }
            
            /// @brief Set formatting flags - no lock
            /// @param[in] formatFlags  Flag(s)
            void setFormatFlags_noLock(const flag_t formatFlags) noexcept;
            /// @brief Set decimal precision for floating point numbers - no lock
            /// @param[in] floatDecimalPrecision  Decimal precision
            inline void setFloatDecimalPrecision_noLock(const uint32_t floatDecimalPrecision) noexcept
            {
                m_floatDecimalPrecision = floatDecimalPrecision;
                if (isOpen())
                {
                    if (floatDecimalPrecision != 0u)
                    {
                        m_fileStream.precision(floatDecimalPrecision);
                    }
                    else
                    {
                        m_floatDecimalPrecision = m_defaultPrecision;
                        m_fileStream.precision(m_defaultPrecision);
                    }
                }
            }
            
            
        private:
            std::ofstream m_fileStream; ///< File output stream
            std::string m_errorBuffer;  ///< Last opening error message
            
            uint32_t m_floatDecimalPrecision;   ///< Decimal precision for floating-point numbers (0 to use maximum)
            std::streamsize m_defaultPrecision; ///< Default decimal precision value
            utils::memory::flag_set<flag_t> m_formatFlags; ///< Formatting flags (text mode) + flush flag
        };
    }
}
