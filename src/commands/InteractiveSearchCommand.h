#pragma once

#include <string>
#include <filesystem>
#include "ICommand.h"
#include "InvertedIndex.h"

namespace fs = std::filesystem;

namespace IOD
{
    namespace Commands
    {
        class InteractiveSearchCommand : public IOD::ICommand {
        public:
            InteractiveSearchCommand(const char* indexFile) : indexFile(indexFile) {}

            void execute();

        private:
            std::string indexFile;

            SerDe::InvertedIndex index;

            int loop();
        };
    } // namespace Commands
} // namespace IOD
