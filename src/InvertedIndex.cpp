#include <unordered_map>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "InvertedIndex.h"
#include "Document.h"
#include "Token.h"
#include "ExitCodes.h"
#include "Constants.h"

namespace fs = std::filesystem;

namespace IOD
{
    namespace SerDe
    {
        void InvertedIndex::addDocToIndex(Document doc) {
            if(documentSet.find(doc.getId()) != documentSet.end()) //already added
                return;

            long docId = doc.getId();
            for(const Tokenisation::Token& tok: doc.getTokens()) {
                invIndex[tok.value][docId].push_back(tok.pos);
            }

            documentSet[docId] = DocInfo{.docId = docId, .docPath = fs::absolute(doc.getPath()), .tokCount = doc.getTokens().size()};
        }

        /*
            SIGNATURE: ".IIX"
            DOC_COUNT: size_t
            [
                DOC_ID: long
                PATH_SIZE: size_t
                PATH_STR: string[PATH_SIZE]     // absolute path
                DOC_TOK_CNT: size_t
            ]
            INDEX_SIZE: size_t
            [
                TOKEN_SIZE: size_t
                TOKEN_STR: string[TOKEN_SIZE]
                REF_LIST_SIZE: size_t
                [
                    DOC_ID: long
                    POS_CNT: size_t
                    [
                        POS: size_t
                    ]
                ]
            ]
        */

        void InvertedIndex::serialise(std::string outFileName) {

            if(outFileName.size() <= 4 || outFileName.compare(outFileName.size()-4, 4, ".iix")!=0)
                outFileName += ".iix";

            std::ofstream file;
            file.open(outFileName, std::ios::out | std::ios::binary);
            if(!file) {
                std::cerr << "Could not open " << outFileName << " for writing!" << std::endl;
                exit(EXIT_CANT_OPEN_FILE);
            }

            // SIGNATURE
            file.write(IOD::SIGNATURE, sizeof(IOD::SIGNATURE)-1);

            // DOC_COUNT
            size_t docCount = documentSet.size();
            file.write(reinterpret_cast<char *>(&docCount), sizeof(docCount));

            // DOC_LIST
            for (const auto& it: documentSet) {
                long docId = it.first;
                std::string pathString = it.second.docPath.string();
                size_t pathSize = pathString.size();
                size_t docTokCnt = it.second.tokCount;

                file.write(reinterpret_cast<char *>(&docId), sizeof(docId));
                file.write(reinterpret_cast<char *>(&pathSize), sizeof(pathSize));
                file.write(pathString.c_str(), pathSize);
                file.write(reinterpret_cast<char *>(&docTokCnt), sizeof(docTokCnt));
            }

            // INDEX_SIZE
            size_t indexSize = invIndex.size();
            file.write(reinterpret_cast<char *>(&indexSize), sizeof(indexSize));

            // INDEX
            for(const auto& indexIter: invIndex) {
                std::string tokenString = indexIter.first;
                size_t tokenSize = tokenString.size();
                
                file.write(reinterpret_cast<char *>(&tokenSize), sizeof(tokenSize));
                file.write(tokenString.c_str(), tokenSize);

                // REF_LIST
                size_t refListSize = indexIter.second.size();
                file.write(reinterpret_cast<char *>(&refListSize), sizeof(refListSize));

                for(const auto& postingsIter: indexIter.second) {
                    // DOC_ID
                    long docId = postingsIter.first;
                    file.write(reinterpret_cast<char *>(&docId), sizeof(docId));

                    // POS_CNT
                    size_t posCnt = postingsIter.second.size();
                    file.write(reinterpret_cast<char *>(&posCnt), sizeof(posCnt));

                    // POS
                    file.write(reinterpret_cast<const char*>(postingsIter.second.data()), posCnt * sizeof(size_t));
                }
            }

            file.close();
        }

        InvertedIndex InvertedIndex::deserialise(std::string inFileName) {
            std::ifstream file;
            file.open(inFileName, std::ios::in | std::ios::binary);
            if(!file) {
                std::cerr << "Could not open " << inFileName << " for reading!" << std::endl;
                exit(EXIT_CANT_OPEN_FILE);
            }

            InvertedIndex newIndex;

            // SIGNATURE
            char signature[5] = {0};
            file.read(signature, sizeof(IOD::SIGNATURE)-1);
            if(0 != std::string(signature).compare(IOD::SIGNATURE)) {
                std::cerr << inFileName << " has invalid signature " << signature << std::endl;
                exit(EXIT_BAD_FILE_SIGNATURE);
            }

            // DOC_COUNT
            size_t docCount = 0;
            file.read(reinterpret_cast<char *>(&docCount), sizeof(docCount));

            // DOC_LIST
            for (int i = 0; i < docCount; ++i) {
                long docId = 0;
                size_t pathSize = 0;
                size_t docTokCnt = 0;

                file.read(reinterpret_cast<char *>(&docId), sizeof(docId));
                file.read(reinterpret_cast<char *>(&pathSize), sizeof(pathSize));

                char pathString[pathSize + 1] = {0};
                file.read(pathString, pathSize);
                file.read(reinterpret_cast<char *>(&docTokCnt), sizeof(docTokCnt));

                newIndex.documentSet[docId] = DocInfo{.docId = docId, .docPath = fs::path(std::string(pathString)), .tokCount = docTokCnt};
            }

            // INDEX_SIZE
            size_t indexSize = 0;
            file.read(reinterpret_cast<char *>(&indexSize), sizeof(indexSize));

            // INDEX
            for (int i = 0; i < indexSize; ++i) {
                // TOKEN_SIZE
                size_t tokenSize = 0;
                file.read(reinterpret_cast<char *>(&tokenSize), sizeof(tokenSize));

                // TOKEN_STR
                char tokenStr[tokenSize + 1] = {0};
                file.read(tokenStr, tokenSize);

                // REF_LIST_SIZE
                size_t refListSize = 0;
                file.read(reinterpret_cast<char *>(&refListSize), sizeof(refListSize));

                // REF_LIST
                for (int j = 0; j < refListSize; ++j) {
                    long docId = 0;
                    size_t posCnt = 0;

                    file.read(reinterpret_cast<char *>(&docId), sizeof(docId));
                    file.read(reinterpret_cast<char *>(&posCnt), sizeof(posCnt));

                    newIndex.invIndex[tokenStr][docId].resize(posCnt);
                    file.read(reinterpret_cast<char*>(newIndex.invIndex[tokenStr][docId].data()), posCnt * sizeof(size_t));
                }
            }

            return newIndex;
        }
    } // namespace SerDe
} // namespace IOD

