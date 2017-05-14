/*******************************************************************************
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : advanced file reader
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
        /// @class FileReader
        /// @brief Advanced file reader - with encoder specialization
        /// @see FileIO
        template <FileIO::file_encoder_t Encoder>
        class FileReader : public FileIO
        {
        public:
            /// @enum file_mode_t
            /// @brief File opening mode
            enum class file_mode_t : uint32_t
            {
                start = 0u,
                atEnd = 1u
            };
            /// @enum flag_t
            /// @brief File reader flags - can be combined with |
            enum flag_t : uint32_t
            {
                FileReader_toUppercase = 0x20,  ///< Convert strings to uppercase (only in text mode)
                FileReader_toLowercase = 0x21,  ///< Convert strings to lowercase (only in text mode)
                FileReader_trimLeft = 0x22,     ///< Remove leading white-space characters for each operation (only in text mode)
                FileReader_trimRight = 0x24     ///< Remove trailing white-space characters for each operation (only in text mode)
            };
            
            
            /// @brief Create file reader instance (specialized)
            FileReader() noexcept;
            /// @brief Copy file reader instance
            /// @param[in] other  Other instance
            FileReader(const FileReader& other) noexcept;
            /// @brief Move file reader instance
            /// @param[in] other  Other instance
            FileReader(FileReader&& other) noexcept;
            /// @brief Destroy file reader instance
            ~FileReader()
            {
                close();
            }
            
            /// @brief Assign copy of file reader instance
            /// @param[in] other  Other instance
            /// @returns Copied instance
            FileReader& operator=(const FileReader& other) noexcept;
            /// @brief Assign moved file reader instance
            /// @param[in] other  Other instance
            /// @returns Moved instance
            FileReader& operator=(FileReader&& other) noexcept;
            /// @brief Swap file reader instances
            /// @param[in] other  Other instance
            void swap(FileReader& other) noexcept;
        
        
            // -- File management --
            
            /// @brief Open output file
            /// @param[in] filePath     Source file path
            /// @param[in] openMode     File opening mode (optional)
            /// @param[in] formatFlags  Formatting flags, only for text mode (optional, see enum flag_t)
            /// @returns Success (use getLastErrorMessage() if false)
            bool open(const std::wstring filePath, const file_mode_t openMode = file_mode_t::start, const flag_t formatFlags = 0) noexcept;
            /// @brief Close input file
            void close();
            
            /// @brief Change reader position in current file
            /// @param[in] whence  Reference position to move from
            /// @param[in] offset  Offset, based on reference (bytes)
            /// @returns Current instance
            inline FileReader& seek(const seek_reference_t whence, const int32_t offset)
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
                return (0);//...m_file ????
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
            /// @brief Get total file length (bytes)
            /// @returns File length
            inline size_t size() const noexcept
            {
                return m_fileLength;
            }
            /// @brief Check if current file is empty
            /// @returns Empty (true) or not
            inline bool isEmpty() const noexcept
            {
                return (size == 0u);
            }
            
            
            // -- Input operations --
            
            FileReader& get(bool val);
            FileReader& operator>>(bool val);
            FileReader& get(char val);
            FileReader& operator>>(char val);
            FileReader& get(uint16_t val);
            FileReader& operator>>(uint16_t val);
            FileReader& get(int16_t val);
            FileReader& operator>>(int16_t val);
            FileReader& get(uint32_t val);
            FileReader& operator>>(uint32_t val);
            FileReader& get(int32_t val);
            FileReader& operator>>(int32_t val);
            FileReader& get(uint64_t val);
            FileReader& operator>>(uint64_t val);
            FileReader& get(int64_t val);
            FileReader& operator>>(int64_t val);
            FileReader& get(float val);
            FileReader& operator>>(float val);
            FileReader& get(double val);
            FileReader& operator>>(double val);
            FileReader& get(long double val);
            FileReader& operator>>(long double val);
            
            FileReader& read(char* stream, size_t size);
            
            FileReader& read(wchar_t* strVal, size_t size);
            FileReader& read(std::string& strVal);
            FileReader& operator>>(std::string& val);
            FileReader& read(std::wstring& strVal);
            FileReader& operator>>(std::wstring& val);
            
            FileReader& readLine(const char* strVal, size_t size);
            FileReader& readLine(wchar_t* strVal, size_t size);
            FileReader& readLine(std::string& strVal);
            FileReader& readLine(std::wstring& strVal);
            
            
            // -- Reader settings --
            
            /// @brief Get configured file encoding
            /// @brief File encoding type
            inline FileIO::file_encoder_t getEncoding() const noexcept
            {
                return Encoder;
            }
            
            /// @brief Get formatting flags
            /// @brief Flag set
            inline flag_t getFormatFlags() const noexcept
            {
                return m_formatFlags.data();
            }
            /// @brief Set formatting flags
            /// @param[in] formatFlags  Flag set
            /// @returns Current instance
            inline FileReader& setFormatFlags(const flag_t formatFlags) noexcept
            {
                m_fileLock.lock();
                setFormatFlags_noLock(formatFlags);
                m_fileLock.unlock();
                return *this;
            }
            
            
        private:
            void decode(char* stream, size_t size);
            void decode(wchar_t* stream, size_t size);
            
            /// @brief Change reader position in current file
            /// @param[in] whence  Reference position to move from
            /// @param[in] offset  Offset, based on reference (bytes)
            void seek_noLock(const seek_reference_t whence, const int32_t offset);
            
            /// @brief Set formatting flags - no lock
            /// @param[in] formatFlags  Flag(s)
            inline void setFormatFlags_noLock(const flag_t formatFlags) noexcept
            {
                m_formatFlags = utils::memory::flag_set<flag_t>(formatFlags);
            }
            
            
        private:
            //handle??? m_file;
            uint32_t m_offset;
            uint32_t m_fileLength;
            
            std::string m_errorBuffer;
            
            utils::memory::flag_set<flag_t> m_formatFlags;
        };
    }
}
