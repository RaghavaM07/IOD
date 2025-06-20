#include <iostream>
#include "SearchCommand.h"
#include "InvertedIndex.h"
#include "Tokeniser.h"

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
            std::cout << queryString << std::endl;
        }
    } // namespace Commands
} // namespace IOD