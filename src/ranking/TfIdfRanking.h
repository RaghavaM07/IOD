#pragma once

#include "IRankingStrategy.h"
#include "InvertedIndex.h"
#include <cmath>

namespace IOD
{
    namespace Ranking
    {
        class TfIdfRanking : public IRankingStrategy {
        private:
            SerDe::InvertedIndex index;
        
        public:
            TfIdfRanking(SerDe::InvertedIndex index): index(index) {}

            std::vector<Hit> query(std::vector<Tokenisation::Token>& queryTokens);

            std::string name() {
                return "Tf-Idf";
            }

            // tf(t, d) = freq_of_t_in_d/terms_in_d
            double tf(std::string term, long docId) {
                size_t f_t_d = index.termFreq(docId, term);
                size_t terms_in_d = index.docLength(docId);
                return 1.0 * f_t_d / terms_in_d;
            }

            // idf(t, D) = log((1+total_docs)/(1+total_docs_with_t))
            double idf(std::string term) {
                size_t total_docs = index.totalDocs();
                size_t docs_with_t = index.docFreq(term);
                return std::log10(1.0 * (1+total_docs) / (1+docs_with_t));
            }
        };
    } // namespace Ranking
} // namespace IOD