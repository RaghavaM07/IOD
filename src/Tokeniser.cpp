#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include "Tokeniser.h"
#include "Token.h"

namespace fs = std::filesystem;

namespace IOD
{
    namespace Tokenisation
    {
        std::vector<Token> Tokeniser::tokenise() {
            std::ifstream file(filePath, std::ios::binary | std::ios::ate);

            const std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            std::string content(size, '\0');
            file.read(content.data(), size);

            return tokenise(content);
        }

        std::vector<Token> Tokeniser::tokenise(std::string_view content) {

            std::vector<Token> retVal;

            std::string currToken = "";
            for (size_t i = 0; i < content.size(); ++i) {
                if(content[i] == ' ' || content[i] == '\n') {
                    int removed = 0;
                    while (trimChars.find(currToken.back()) != trimChars.end()) {
                        removed++;
                        currToken.pop_back();
                    }
                    if (currToken.size() == 0) {
                        currToken.clear();
                        continue;
                    }

                    int begEraseCnt = 0;
                    while (trimChars.find(currToken[begEraseCnt]) != trimChars.end())
                        removed++, begEraseCnt++;
                    currToken.erase(0, begEraseCnt);

                    if(currToken.size() != 0) {
                        transform(currToken.begin(), currToken.end(), currToken.begin(), ::tolower);
                        Token tok = {
                            .pos    = i - currToken.length() - removed,
                            .value  = currToken
                        };
                        retVal.push_back(tok);
                    }

                    currToken.clear();
                    continue;
                }

                currToken+=content[i];
            }
            size_t removed = 0;
            while (currToken.size() > 0 && trimChars.find(currToken.back()) != trimChars.end()) {
                removed++;
                currToken.pop_back();
            }
            if (currToken.size() != 0) {
                int begEraseCnt = 0;
                while (begEraseCnt < currToken.size() && trimChars.find(currToken[begEraseCnt]) != trimChars.end()) removed++, begEraseCnt++;
                currToken.erase(0, begEraseCnt);

                if (currToken.size() != 0) {
                    Token tok = {
                        .pos = content.length() - currToken.length() - removed,
                        .value = currToken
                    };
                    retVal.push_back(tok);
                }
            }

            return retVal;
        }
    } // namespace Tokenisation
} // namespace IOD