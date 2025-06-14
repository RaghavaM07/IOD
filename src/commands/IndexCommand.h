#pragma once

#include <string>

#include "ICommand.h"

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
        };

    } // namespace Commands
} // namespace IOD
