#include "hash.h"

#include <fstream>
#include <sstream>

#include <openssl/evp.h>
#include <openssl/md5.h>

namespace util::hash {

std::string calc_file_md5(std::string_view path) {
    std::ifstream file(path.data(), std::ios::binary);
    if (!file) return {};

    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_md5(), nullptr);

    char buffer[1024 * 16];
    while (file.good()) {
        file.read(buffer, sizeof(buffer));
        EVP_DigestUpdate(mdctx, buffer, file.gcount());
    }

    const unsigned int md5_digest_length = MD5_DIGEST_LENGTH;
    unsigned char      md5_digest[md5_digest_length];
    EVP_DigestFinal_ex(mdctx, md5_digest, &const_cast<unsigned int&>(md5_digest_length));
    EVP_MD_CTX_free(mdctx);

    std::stringstream stream;
    stream << std::hex << std::setw(2) << std::setfill('0');
    for (unsigned int idx = 0; idx < md5_digest_length; idx++) {
        stream << static_cast<int>(md5_digest[idx]);
    }

    return stream.str();
}

} // namespace util::hash
