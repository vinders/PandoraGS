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
#include "file_io.hpp"
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
        template <FileIO::file_encoder_t Encoder>
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
                FileWriter_lineFeedWithCR = 0x02,      ///< Add carriage-return (CR) symbols before line-feeds (LF) (only in text mode)
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
            FileWriter() noexcept;
            /// @brief Copy file writer instance
            /// @param[in] other  Other instance
            FileWriter(const FileWriter& other) noexcept;
            /// @brief Move file writer instance
            /// @param[in] other  Other instance
            FileWriter(FileWriter&& other) noexcept;
            /// @brief Destroy file writer instance
            ~FileWriter()
            {
                close();
            }
            
            /// @brief Assign copy of file writer instance
            /// @param[in] other  Other instance
            /// @returns Copied instance
            FileWriter& operator=(const FileWriter& other) noexcept;
            /// @brief Assign moved file writer instance
            /// @param[in] other  Other instance
            /// @returns Moved instance
            FileWriter& operator=(FileWriter&& other) noexcept;
            /// @brief Swap file writer instances
            /// @param[in] other  Other instance
            void swap(FileWriter& other) noexcept;
        
        
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
                m_fileLock.lock();
                flush_noLock();
                m_fileLock.unlock();
                return *this;
            }
            
            /// @brief Change writer position in current file
            /// @param[in] whence  Reference position to move from
            /// @param[in] offset  Offset, based on reference (bytes)
            /// @returns Current instance
            inline FileWriter& seek(const seek_reference_t whence, const int32_t offset)
            {
                m_fileLock.lock();
                seek_noLock(whence, offset);
                m_fileLock.unlock();
                return *this;
            }
            /// @brief Check if a file is open
            /// @returns File open (true) or not
            inline bool isOpen() const noexcept
            {
                return (0); //...m_file ????
            }
            /// @brief Check if current position is at the end of the file
            /// @returns End of file reached (true) or not
            inline bool isEof() const noexcept
            {
                return (m_offset >= m_fileLength);
            }
            
            /// @brief Get most recent error message (if any)
            /// @returns Error message
            inline const std::string const& getLastErrorMessage() const noexcept
            {
                return m_errorBuffer;
            }
            
            /// @brief Get system home directory
            /// @returns Home directory path
            static std::wstring getHomeDirectoryPath() noexcept;
            /// @brief Get working directory
            /// @returns Working directory path
            static std::wstring getWorkingDirectoryPath() noexcept;
            
            
            // -- Output operations --
            
            FileWriter& put(const bool val);
            FileWriter& operator<<(const bool val);
            FileWriter& put(const char val);
            FileWriter& operator<<(const char val);
            FileWriter& put(const uint16_t val);
            FileWriter& operator<<(const uint16_t val);
            FileWriter& put(const int16_t val);
            FileWriter& operator<<(const int16_t val);
            FileWriter& put(const uint32_t val);
            FileWriter& operator<<(const uint32_t val);
            FileWriter& put(const int32_t val);
            FileWriter& operator<<(const int32_t val);
            FileWriter& put(const uint64_t val);
            FileWriter& operator<<(const uint64_t val);
            FileWriter& put(const int64_t val);
            FileWriter& operator<<(const int64_t val);
            FileWriter& put(const float val);
            FileWriter& operator<<(const float val);
            FileWriter& put(const double val);
            FileWriter& operator<<(const double val);
            FileWriter& put(const long double val);
            FileWriter& operator<<(const long double val);
            
            FileWriter& write(const char* stream, size_t size);
            
            FileWriter& write(const char* strVal);
            FileWriter& write(const wchar_t* strVal);
            FileWriter& write(const std::string& strVal);
            FileWriter& operator<<(const std::string& val);
            FileWriter& write(std::string&& strVal);
            FileWriter& operator<<(std::string&& val);
            FileWriter& write(const std::wstring& strVal);
            FileWriter& operator<<(const std::wstring& val);
            FileWriter& write(std::wstring&& strVal);
            FileWriter& operator<<(std::wstring&& val);
            
            FileWriter& writeLine(const char* strVal);
            FileWriter& writeLine(const wchar_t* strVal);
            FileWriter& writeLine(const std::string& strVal);
            FileWriter& writeLine(std::string&& strVal);
            FileWriter& writeLine(const std::wstring& strVal);
            FileWriter& writeLine(std::wstring&& strVal);
            
            
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
                m_fileLock.lock();
                setFormatFlags_noLock(formatFlags);
                m_fileLock.unlock();
                return *this;
            }
            
            /// @brief Get decimal precision for floating point numbers
            /// @returns Decimal precision
            inline uint32_t getFloatDecimalPrecision() const noexcept
            {
                return m_floatDecimalPrecision;
            }
            /// @brief Set decimal precision for floating point numbers
            /// @param[in] floatDecimalPrecision  Decimal precision
            /// @returns Current instance
            inline FileWriter& setFloatDecimalPrecision(const uint32_t floatDecimalPrecision) noexcept
            {
                m_fileLock.lock();
                setFloatDecimalPrecision_noLock(floatDecimalPrecision);
                m_fileLock.unlock();
                return *this;
            }
        
            
        private:
            void encode(const char* stream, size_t size);
            void encode(const wchar_t* stream, size_t size);
            
            /// @brief Flush output buffer
            inline void flush_noLock()
            {
                //...
            }
            /// @brief Change writer position in current file
            /// @param[in] whence  Reference position to move from
            /// @param[in] offset  Offset, based on reference (bytes)
            void seek_noLock(const seek_reference_t whence, const int32_t offset);
            
            /// @brief Set formatting flags - no lock
            /// @param[in] formatFlags  Flag(s)
            inline void setFormatFlags_noLock(const flag_t formatFlags) noexcept
            {
                m_formatFlags = utils::memory::flag_set<flag_t>(formatFlags);
            }
            /// @brief Set decimal precision for floating point numbers - no lock
            /// @param[in] floatDecimalPrecision  Decimal precision
            inline void setFloatDecimalPrecision_noLock(const uint32_t floatDecimalPrecision) noexcept
            {
                m_floatDecimalPrecision = floatDecimalPrecision;
            }
            
            
        private:
            //handle??? m_file;
            uint32_t m_offset;
            uint32_t m_fileLength;
            
            std::string m_errorBuffer;
            
            uint32_t m_floatDecimalPrecision;
            utils::memory::flag_set<flag_t> m_formatFlags;
        };
    }
}
