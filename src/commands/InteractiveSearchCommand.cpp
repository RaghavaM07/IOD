#include <iostream>
#include "InteractiveSearchCommand.h"
#include "InvertedIndex.h"
#include "Tokeniser.h"
#include "TfIdfRanking.h"

namespace IOD
{
    namespace Commands
    {
        void InteractiveSearchCommand::execute() {
            std::cout << "Reading index at " << this->indexFile << " to memory..." << std::endl;
            
            this->index = SerDe::InvertedIndex::deserialise(indexFile);

            int nQueries = loop();
            std::cout << "Ran " << nQueries << " queries" << std::endl;
        }

        int InteractiveSearchCommand::loop() {
            int queryCount = 0;
            
            Tokenisation::Tokeniser tokeniser;
            IRankingStrategy *ranker = new Ranking::TfIdfRanking(index);

            while(true) {
                std::string queryStr;
                std::cout << "Enter query string (or `qq` to quit): ";
                std::getline(std::cin, queryStr);   // to read with spaces

                if(queryStr == "qq") {
                    return queryCount;
                }

                std::vector<Tokenisation::Token> qToks = tokeniser.tokenise(queryStr);

                std::vector<Hit> searchRes = ranker->query(qToks);

                if(!searchRes.empty()) {
                    for(const Hit& hit: searchRes) {
                    std::cout << index.getDocById(hit.docId).docPath.string()    << ":" << \
                                 hit.pos                                         << "\t\t\t" << \
                                 "score = " << hit.score                         <<std::endl;
                    }
                } else {
                    std::cout << "Search for `" << queryStr << "` returned 0 results" << std::endl;
                }
                queryCount++;
            }

            return queryCount;
        }
    } // namespace Commands
} // namespace IOD
