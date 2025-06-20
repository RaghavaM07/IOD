#pragma once

#include <unordered_map>
#include <vector>
#include <filesystem>
#include <string>
#include "Document.h"

namespace fs = std::filesystem;

namespace IOD
{
    namespace SerDe
    {
        typedef std::unordered_map<long, std::vector<size_t>> Posting;

        typedef struct DocInfo {
            long docId;
            fs::path docPath;
            size_t tokCount;
        } DocInfo;

        class InvertedIndex
        {
        public:
            InvertedIndex() { }

            void addDocToIndex(Document doc);
            void serialise(std::string outFileName);
            static InvertedIndex deserialise(std::string inFileName);

        private:
            std::unordered_map<long, DocInfo> documentSet;
            std::unordered_map<std::string, Posting> invIndex;
        };
    } // namespace SerDe
} // namespace IOD

