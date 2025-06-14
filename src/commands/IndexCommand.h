#pragma once

#include <string>
#include <filesystem>
#include "ICommand.h"

namespace fs = std::filesystem;

namespace IOD
{
    namespace Commands
    {
        class IndexCommand : public IOD::ICommand {
        public:
            IndexCommand(const char* folderPath) : folderPath(folderPath) {}

            void execute();

        private:
            std::string folderPath;

            bool isSupported(const fs::path& file);
        };

    } // namespace Commands
} // namespace IOD
