#ifndef TIME_POINT_H
#define TIME_POINT_H

#include "../../common/Time.h"

#include <DataStorm/DataStorm.h>
#include <Ice/Ice.h>

#include <cstring>
#include <stdexcept>

namespace DataStorm
{
    /// DataStorm::Encoder specialization to encode std::chrono::system_clock::time_point values.
    template<> struct Encoder<std::chrono::system_clock::time_point>
    {
        /// Encodes the given std::chrono::system_clock::time_point value into a byte sequence. The value is first
        /// converted to a time stamp (see common/Time.h), then encoded as 8 bytes in little-endian order.
        ///
        /// @param time The time_point value to encode.
        /// @return The resulting byte sequence.
        static Ice::ByteSeq encode(const std::chrono::system_clock::time_point& time)
        {
            std::int64_t value = Time::toTimeStamp(time);
            Ice::ByteSeq data(sizeof(value));
            std::memcpy(data.data(), &value, sizeof(value)); // we assume a little-endian system
            return data;
        }
    };

    /// DataStorm::Decoder specialization to decode std::chrono::system_clock::time_point values.
    template<> struct Decoder<std::chrono::system_clock::time_point>
    {
        /// Decodes the given byte sequence into a std::chrono::system_clock::time_point value. The value is decoded
        /// as an 8-byte little-endian time stamp (see common/Time.h), then converted to a time point.
        ///
        /// @param data The byte sequence to decode.
        /// @return The resulting time_point value.
        /// @throws std::invalid_argument Thrown when the byte sequence is not 8 bytes long.
        static std::chrono::system_clock::time_point decode(const Ice::ByteSeq& data)
        {
            if (data.size() != sizeof(std::int64_t))
            {
                throw std::invalid_argument("cannot decode time point: expected an 8-byte encoded value");
            }
            std::int64_t value;
            std::memcpy(&value, data.data(), sizeof(value)); // we assume a little-endian system
            return Time::toTimePoint(value);
        }
    };
}

#endif
