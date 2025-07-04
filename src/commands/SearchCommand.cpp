#include <iostream>
#include "SearchCommand.h"
#include "InvertedIndex.h"
#include "Tokeniser.h"
#include "NoRanking.h"

namespace IOD
{
    namespace Commands
    {
        void SearchCommand::execute() {
            // TODO: implement
            /*
            // for each token in queryString, calculate bm25 for all files
            // add scores for all tokens file-wise and generate result list in sorted order based on combined score
            */

            // deserialise index file
            std::cout << "Trying to read " << indexFile << std::endl;
            SerDe::InvertedIndex index = SerDe::InvertedIndex::deserialise(indexFile);

            // tokenise queryString
            Tokenisation::Tokeniser tokeniser;
            std::vector<Tokenisation::Token> qToks = tokeniser.tokenise(queryString);

            IRankingStrategy *ranker = new Ranking::NoRanking(index);
            std::vector<Hit> searchRes = ranker->query(qToks);

            for(const Hit& hit: searchRes) {
                std::cout <<    index.getDocById(hit.docId).docPath.string()    << "\t\t\t" << \
                                "at = " << hit.pos                              << "\t\t\t" << \
                                "score = " << hit.score                         <<std::endl;
            }
        }
    } // namespace Commands
} // namespace IOD