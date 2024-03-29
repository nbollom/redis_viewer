//
// zip.h
// Created by TheFatNinja 
// 05-11-2019
//

#ifndef REDIS_VIEWER_GZIP_H
#define REDIS_VIEWER_GZIP_H

#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <zlib.h>

namespace gzip {

    const unsigned int buffer_size = 32768;
    const unsigned int windowBits = 15;
    const unsigned int gzip_encoding = 16;

    std::string compress_string(const std::string &str, int compression_level = Z_BEST_COMPRESSION) {
        z_stream zs;                        // z_stream is zlib's control structure
        memset(&zs, 0, sizeof(zs));

        if (deflateInit2(&zs, compression_level, Z_DEFLATED, windowBits | gzip_encoding, 8, Z_DEFAULT_STRATEGY)) {
            throw (std::runtime_error("deflateInit failed while compressing."));
        }

        zs.next_in = (Bytef *) str.data();
        zs.avail_in = str.size();           // set the z_stream's input

        int ret;
        char out_buffer[buffer_size];
        std::string out_string;

        // retrieve the compressed bytes blockwise
        do {
            zs.next_out = reinterpret_cast<Bytef *>(out_buffer);
            zs.avail_out = sizeof(out_buffer);

            ret = deflate(&zs, Z_FINISH);

            if (out_string.size() < zs.total_out) {
                // append the block to the output string
                out_string.append(out_buffer, zs.total_out - out_string.size());
            }
        }
        while (ret == Z_OK);

        deflateEnd(&zs);

        if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
            std::ostringstream oss;
            oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
            throw (std::runtime_error(oss.str()));
        }

        return out_string;
    }

/** Decompress an STL string using zlib and return the original data. */
    std::string decompress_string(const std::string &str) {
        z_stream zs;                        // z_stream is zlib's control structure
        memset(&zs, 0, sizeof(zs));

        if (inflateInit2(&zs, windowBits | gzip_encoding) != Z_OK) {
            throw (std::runtime_error("inflateInit failed while decompressing."));
        }

        zs.next_in = (Bytef *) str.data();
        zs.avail_in = str.size();

        int ret;
        char out_buffer[buffer_size];
        std::string out_string;

        // get the decompressed bytes blockwise using repeated calls to inflate
        do {
            zs.next_out = reinterpret_cast<Bytef *>(out_buffer);
            zs.avail_out = sizeof(out_buffer);

            ret = inflate(&zs, 0);

            if (out_string.size() < zs.total_out) {
                out_string.append(out_buffer, zs.total_out - out_string.size());
            }

        }
        while (ret == Z_OK);

        inflateEnd(&zs);

        if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
            std::ostringstream oss;
            oss << "Exception during zlib decompression: (" << ret << ") " << zs.msg;
            throw (std::runtime_error(oss.str()));
        }

        return out_string;
    }
}

#endif //REDIS_VIEWER_GZIP_H
