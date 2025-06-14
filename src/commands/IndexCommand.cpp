#include <iostream>

#include "IndexCommand.h"

namespace IOD
{
    namespace Commands
    {
        void IndexCommand::execute() {
            // TODO: implement
            /*
            // crawl folder for all text files and populate in a list
            // for each eligible file, create a custom TextFile object with file metadata and tokens' info (value, position)
            // combine data of all these tokens and file locations into an inverse map
            // persist index in current directory and output index location
            */
            std::cout << folderPath << std::endl;
        }
    } // namespace Commands
} // namespace IOD
