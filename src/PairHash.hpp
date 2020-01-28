#ifndef PAIRHASH_HPP
#define PAIRHASH_HPP

#include <utility>

struct PairHash {
    template <class T, class U>
    size_t operator() (const std::pair<T, U>& pair) const {
        // Magic number & algorithm from boost library
        constexpr auto magic = 0x9e3779b9;

        size_t seed = 0;
        seed ^= std::hash<T>()(pair.first) + magic;
        seed ^= std::hash<U>()(pair.second) + magic + (seed << 6) + (seed >> 2);
        return seed;
    }
};

#endif // PAIRHASH_HPP
