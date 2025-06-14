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
            std::ifstream file(filePath, std::ifstream::in);
            std::ostringstream oss;
            oss << file.rdbuf();
            std::string content = oss.str();

            return this->tokenise(content);
        }

        std::vector<Token> Tokeniser::tokenise(const std::string& content) {

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