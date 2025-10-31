#ifndef TIME_POINT_H
#define TIME_POINT_H

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

namespace DataStorm
{
    /// DataStorm::Encoder specialization to encode std::chrono::system_clock::time_point values.
    template<> struct Encoder<std::chrono::system_clock::time_point>
    {
        /// Encodes the given std::chrono::system_clock::time_point value into a byte sequence. The value is encoded
        /// as the number of seconds since epoch in little-endian format using a variable number of bytes.
        ///
        /// @param time The time_point value to encode.
        /// @return The resulting byte sequence.
        static Ice::ByteSeq encode(const std::chrono::system_clock::time_point& time)
        {
            Ice::ByteSeq data;
            auto value = std::chrono::time_point_cast<std::chrono::seconds>(time).time_since_epoch().count();
            while (value)
            {
                data.push_back(static_cast<std::byte>(value & 0xFF));
                value >>= 8;
            }
            return data;
        }
    };

    /// DataStorm::Decoder specialization to decode std::chrono::system_clock::time_point values.
    template<> struct Decoder<std::chrono::system_clock::time_point>
    {
        /// Decodes the given byte sequence into a std::chrono::system_clock::time_point value. The value is decoded
        /// from the number of seconds since epoch in little-endian format using a variable number of bytes.
        ///
        /// @param data The byte sequence to decode.
        /// @return The resulting time_point value.
        static std::chrono::system_clock::time_point decode(const Ice::ByteSeq& data)
        {
            int64_t value = 0;
            for (auto p = data.rbegin(); p != data.rend(); ++p)
            {
                value = (value << 8) | std::to_integer<int64_t>(*p);
            }
            return std::chrono::time_point<std::chrono::system_clock>(std::chrono::seconds(value));
        }
    };
}

#endif
