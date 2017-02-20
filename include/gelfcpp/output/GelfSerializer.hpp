#pragma once

#include <gelfcpp/GelfMessage.hpp>
#include <gelfcpp/detail/DocumentAccessor.hpp>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#ifndef GELFCPP_NO_COMPRESSION
#   include <boost/iostreams/device/array.hpp>
#   include <boost/iostreams/filter/gzip.hpp>
#   include <boost/iostreams/copy.hpp>
#   include <boost/iostreams/filtering_streambuf.hpp>
#endif
#include <chrono>
#include <random>
#include <sstream>

namespace gelfcpp
{

/**
 * \brief Handles chunking and compression
 *
 * \todo Merge this into GelfUDPOutput, for convenience and performance.
 *
 * \see GelfUDPOutput
 */
class GelfSerializer
{
    static constexpr std::size_t DEFAULT_CHUNK_SIZE = 2096;
    static constexpr std::size_t DEFAULT_COMPRESS_THRESHOLD = 0;

public:
    GelfSerializer() :
            random_(std::random_device{}()),
            chunk_size_(DEFAULT_CHUNK_SIZE),
            compress_threshold_(DEFAULT_COMPRESS_THRESHOLD) {}

    std::vector<std::string> Serialize(const GelfMessage& message)
    {
        std::vector<std::string> out;

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        detail::DocumentAccessor::get(message).Accept(writer);

#ifndef GELFCPP_NO_COMPRESSION
        if (buffer.GetSize() >= compress_threshold_)
        {
            std::string compressed = Compress(buffer.GetString(), buffer.GetSize());
            if (compressed.size() > chunk_size_)
                ToChunks(compressed.c_str(), compressed.size(), out);
            else
                out.emplace_back(std::move(compressed));
        }
        else
#endif
        {
            if (buffer.GetSize() > chunk_size_)
                ToChunks(buffer.GetString(), buffer.GetSize(), out);
            else
                out.emplace_back(buffer.GetString(), buffer.GetSize());
        }

        return out;
    }

private:
#ifndef GELFCPP_NO_COMPRESSION
    std::string Compress(const char* message, std::size_t size)
    {
        boost::iostreams::array_source input(message, size);
        boost::iostreams::filtering_istreambuf compressor;
        compressor.push(boost::iostreams::gzip_compressor());
        compressor.push(input);
        std::ostringstream out;
        boost::iostreams::copy(compressor, out);
        return out.str();
    }
#endif

    uint64_t GenerateMessageID()
    {
        uint64_t random = random_();
        uint64_t time = static_cast<uint64_t>(std::chrono::steady_clock::now().time_since_epoch().count());
        return random ^ time;
    }

    void ToChunks(const char* message, size_t size, std::vector<std::string>& out)
    {
        static constexpr char CHUNKED_GELF_MAGIC[2] = { 0x1e, 0x0f };
        static constexpr std::size_t HEADER_SIZE = 2 + 8 + 1 + 1;

        std::size_t count = size / chunk_size_ + 1;
        const char* message_ptr = message;

        uint64_t message_id = GenerateMessageID();

        for (std::size_t i = 0; i < count; ++i, message_ptr += chunk_size_)
        {
            std::string chunk;
            chunk.reserve(count + HEADER_SIZE);

            chunk.append(CHUNKED_GELF_MAGIC, 2);
            chunk.append(reinterpret_cast<char*>(&message_id), 8);
            chunk.push_back(static_cast<char>(i & 0x7F));
            chunk.push_back(static_cast<char>(count & 0x7F));

            chunk.append(message_ptr, chunk_size_);

            out.emplace_back(std::move(chunk));
        }
    }

private:
    std::mt19937_64 random_;
    std::size_t chunk_size_;
    std::size_t compress_threshold_;
};

}
