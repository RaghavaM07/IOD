#pragma once

#include <string>

#include "ICommand.h"

namespace IOD
{
    namespace Commands
    {
        class SearchCommand : public IOD::ICommand {
        public:
            SearchCommand(const char* indexFile, std::string queryString) : indexFile(indexFile), queryString(queryString) {}

            void execute();

        private:
            std::string indexFile;
            std::string queryString;
        };

    } // namespace Commands
} // namespace IOD

