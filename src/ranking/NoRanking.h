#pragma once

#include "IRankingStrategy.h"
#include "InvertedIndex.h"

namespace IOD
{
    namespace Ranking
    {
        class NoRanking: public IOD::IRankingStrategy {
        private:
            SerDe::InvertedIndex index;

        public:
            NoRanking(SerDe::InvertedIndex index): index(index) {}

            std::vector<Hit> query(std::vector<Tokenisation::Token>& queryTokens);

            std::string name() {
                return "No-Op";
            }
        };
    } // namespace Ranking    
} // namespace IOD

