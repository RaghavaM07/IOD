#pragma once

#include <vector>
#include "Token.h"

namespace IOD
{
    typedef struct Hit {
        long docId;
        size_t pos;
        double score;
    } Hit;

    class IRankingStrategy {
    public:
        virtual std::vector<Hit> query(std::vector<Tokenisation::Token>& queryTokens) = 0;

        virtual std::string name() = 0;
    };

    namespace Ranking
    {
        static bool stdScoreComparator(const Hit &a, const Hit &b) { return a.score > b.score; }
    } // namespace Ranking
    
} // namespace IOD

