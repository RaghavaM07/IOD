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
            NoRanking() {}
            // NoRanking(SerDe::InvertedIndex index): index(index) {}

            std::vector<Hit> query(std::vector<Tokenisation::Token>& queryTokens);

            std::string name() {
                return "No-Op";
            }

            void setIndex(SerDe::InvertedIndex index) {
                this->index = index;
            }
        };
    } // namespace Ranking    
} // namespace IOD

