#include <iostream>
#include <filesystem>
#include <unordered_set>
#include <vector>
#include <chrono>
#include "IndexCommand.h"
#include "Document.h"
#include "InvertedIndex.h"

namespace fs = std::filesystem;

namespace IOD
{
    namespace Commands
    {
        bool IndexCommand::isSupported(const fs::path& file) {
            static const std::unordered_set<std::string> formats({ ".txt" });   // TODO: implement hueristic

            if(formats.find(file.extension().string()) == formats.end())
                return false;

            return true;
        }

        void IndexCommand::execute() {

            // crawl folder for all text files and populate in a list
            std::vector<fs::path> fileList;
            if(fs::is_regular_file(folderPath)) {
                fileList.push_back(folderPath);
            }
            else {
                try {
                    for (const auto &entry : fs::recursive_directory_iterator(folderPath)) {
                        if (fs::is_regular_file(entry)) {
                            fs::path filePath = entry.path();
                            if (isSupported(filePath)) fileList.push_back(filePath);
                        }
                    }
                }
                catch (const fs::filesystem_error &e) {
                    std::cerr << "Filesystem error: " << e.what() << std::endl;
                }
            }


            // for each eligible file, create a custom Document object with file metadata and tokens' info (value, position)
            // combine data of all these tokens and file locations into an inverse map
            SerDe::InvertedIndex index;
            for (const fs::path& filePath: fileList) {
                Document doc(filePath);
                int tokCnt = doc.tokenise();
                std::cout << "Tokens in " << filePath.filename().string() << ": " << tokCnt << std::endl;

                index.addDocToIndex(doc);
            }

            // persist index in current directory and output index location
            auto now = std::chrono::system_clock::now();
            std::time_t epoch = std::chrono::system_clock::to_time_t(now);

            std::string serialisedFileName = "index-" + std::to_string(epoch) + ".iix";
            index.serialise(serialisedFileName);

            std::cout << "Use index file: ./" << serialisedFileName << std::endl;
        }
    } // namespace Commands
} // namespace IOD
