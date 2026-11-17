#include <sodium.h>
#include <string>
#include <array>

using Hash = std::array<unsigned char, crypto_generichash_BYTES>;

class Hasher{
    public:
        static std::array<unsigned char, crypto_generichash_BYTES>* hash(std::string &password);
        static bool compare(Hash &hash1, Hash &hash2);
        static void test();
};
