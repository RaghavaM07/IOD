#include <vector>
#include "NoRanking.h"
#include "Token.h"

namespace IOD
{
    namespace Ranking
    {
        std::vector<Hit> NoRanking::query(std::vector<Tokenisation::Token>& queryTok) {
            std::vector<Hit> retVal;

            for(const Tokenisation::Token& tok: queryTok) {
                SerDe::Posting posting = index.getIndex().at(tok.value);

                for(std::pair<long, std::vector<size_t>> it: posting) {
                    const long docId = it.first;
                    const std::vector<size_t> &posVec = it.second;

                    for(const size_t& pos: posVec) {
                        retVal.push_back(Hit{.docId = docId, .pos = pos, .score = 1});
                    }
                }
            }

            return retVal;
        }

        // Factory functions
        extern "C" const char* plugin_interface_id() {
            return "RANKER";
        }

        extern "C" IRankingStrategy* create_instance() {
            return new NoRanking();
        }
    } // namespace Ranking
} // namespace IOD

