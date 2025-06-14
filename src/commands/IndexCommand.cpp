#include <iostream>
#include <filesystem>
#include <unordered_set>
#include <vector>
#include "IndexCommand.h"
#include "Document.h"

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
            // TODO: implement

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
            std::vector<Document> docList;
            for (const fs::path& filePath: fileList) {
                Document doc(filePath);
                int tokCnt = doc.tokenise();
                std::cout << "Tokens in " << filePath.filename().string() << ": " << tokCnt << std::endl;

                
            }
            
            // persist index in current directory and output index location

            std::cout << folderPath << std::endl;
        }
    } // namespace Commands
} // namespace IOD
