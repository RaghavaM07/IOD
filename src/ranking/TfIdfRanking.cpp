#include <vector>
#include <algorithm>
#include "TfIdfRanking.h"
#include "Token.h"

namespace IOD
{
    namespace Ranking
    {
        std::vector<Hit> TfIdfRanking::query(std::vector<Tokenisation::Token>& queryTok) {
            std::vector<Hit> found;

            for(const Tokenisation::Token& tok: queryTok) {
                if(index.getIndex().find(tok.value) == index.getIndex().end()) continue;

                SerDe::Posting posting = index.getIndex().at(tok.value);

                double idfVal = idf(tok.value);

                for(std::pair<long, std::vector<size_t>> it: posting) {
                    const long docId = it.first;
                    const std::vector<size_t> &posVec = it.second;

                    double score = tf(tok.value, docId) * idfVal;

                    for(const size_t& pos: posVec) {
                        found.push_back(Hit{.docId = docId, .pos = pos, .score = score});
                    }
                }
            }
            std::sort(found.begin(), found.end(), IOD::Ranking::stdScoreComparator);
            return found;
        }
    } // namespace Ranking
} // namespace IOD

