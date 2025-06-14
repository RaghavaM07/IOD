#include <iostream>

#include "SearchCommand.h"

namespace IOD
{
    namespace Commands
    {
        void SearchCommand::execute() {
            // TODO: implement
            /*
            // tokenise queryString
            // deserialise index file
            // for each token in queryString, calculate bm25 for all files
            // add scores for all tokens file-wise and generate result list in sorted order based on combined score
            */
            std::cout << indexFile << std::endl;
            std::cout << queryString << std::endl;
        }
    } // namespace Commands
} // namespace IOD