#include <iostream>
#include <string>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>

#include "ExitCodes.h"
#include "ICommand.h"
#include "IndexCommand.h"
#include "SearchCommand.h"
#include "InteractiveSearchCommand.h"

void printUsage(const char* progName) {
    std::cerr << "Usage: " << progName << " [";
    std::cerr << "index <folder path> | ";
    std::cerr << "search <index file> [search string] | ";
    std::cerr << "help]" << std::endl;
}

int main(int argc, char const *argv[]) {

    if(argc < 2) {
        printUsage(argv[0]);
        exit(EXIT_WRONG_USAGE);
    }

    std::string cmdStr(argv[1]);
    transform(cmdStr.begin(), cmdStr.end(), cmdStr.begin(), ::tolower);

    IOD::ICommand* command = NULL;

    if(cmdStr == "index" && argc == 3) {
        // verify folder exists and is accessible
        struct stat info;
        if(stat(argv[2], &info) != 0)
            exit(EXIT_FOLDER_NOT_ACCESSIBLE);
#ifdef S_IFDIR
        else if((info.st_mode & S_IFDIR) || (info.st_mode & S_IFREG))
#else
        else if((info.st_mode & S_ISDIR) || (info.st_mode & S_ISREG))
#endif
            command = new IOD::Commands::IndexCommand(argv[2]);
        else
            exit(EXIT_NOT_A_FOLDER);
    }
    else if(cmdStr == "search" && argc == 3) {
        const char *indexFile = argv[2];
        command = new IOD::Commands::InteractiveSearchCommand(indexFile);
    }
    else if(cmdStr == "search" && argc > 3) {
        const char *indexFile = argv[2];
        std::string queryString = "";
        for (int i = 3; i<argc; ++i) {
            queryString += argv[i];
            queryString += " ";
        }
        command = new IOD::Commands::SearchCommand(indexFile, queryString);
    }
    else if(cmdStr == "help") {
        printUsage(argv[0]);
        exit(EXIT_SUCCESS);
    }
    else {
        printUsage(argv[0]);
        exit(EXIT_WRONG_USAGE);
    }

    command->execute();
    delete command;

    return EXIT_SUCCESS;
}

