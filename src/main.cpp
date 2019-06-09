//
//  main.cpp
//  Uzduotis 5
//
//  Created by Emilis Baliukonis on 09/06/2019.
//  Copyright © 2019 Emilis Baliukonis. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cwctype>
#include <algorithm>
#include <regex>
#include <locale>

#define LOCALE_TYPE "lt_LT.UTF-8"
const std::locale LOCALE(LOCALE_TYPE);

bool isNotAlpha(const unsigned wchar_t &c) {
    return std::iswalpha(c) == 0;
}

bool isUrl(const std::wstring &word) {
    return std::regex_match(word, std::wregex(
                                              L"^(http[s]?:\\/\\/(www\\.)?|ftp:\\/\\/(www\\.)?|www\\.){1}([0-9A-Za-z-\\.@:%_\+~#=]+)+((\\.[a-zA-Z]{2,3})+)(/(.)*)?(\\?(.)*)?"
                                              ));
}

std::wstring toLower(std::wstring word) {
    std::wstring result;
    for (auto &c : word) {
        result += std::tolower(c, LOCALE);
    }
    
    return result;
}

int main()
{
    setlocale(LC_ALL, LOCALE_TYPE);
    
    std::multimap<std::wstring, unsigned int> words;
    std::vector<std::wstring> urls;
    
    std::wifstream input("data/text.txt", std::ios::binary);
    input.imbue(LOCALE);
    
    std::wstring line;
    unsigned int lineNum = 1;
    
    while (std::getline(input, line)) {
        std::wstring word;
        std::wstringstream lineStream(line);
        
        while (lineStream >> word) {
            if (isUrl(word)) {
                urls.push_back(word);
            } else {
                word.erase(std::remove_if(word.begin(), word.end(), isNotAlpha), word.end());
                if (!word.empty()) {
                    words.insert({ toLower(word), lineNum });
                }
            }
        }
        
        lineNum++;
    }
    
    input.close();
    
    std::wofstream output("data/output.txt");
    output.imbue(LOCALE);
    
    output << "Links:" << std::endl;
    for (const auto &url : urls) {
        output << url << std::endl;
    }
    output << std::endl;
    
    for (auto it = words.begin(), end = words.end(); it != end; it = words.upper_bound(it->first)) {
        const auto word = it->first;
        const auto count = words.count(word);
        
        if (count > 1) {
            output << word << " - used " << count << " times in lines:";
            auto wordRange = words.equal_range(word);
            for (auto lines_it = wordRange.first; lines_it != wordRange.second; ++lines_it) {
                output << ' ' << lines_it->second;
            }
            output << std::endl;
        }
    }
    
    output.close();
    
    std::cout << "Output written to data/output.txt" << std::endl;
    return 0;
}
