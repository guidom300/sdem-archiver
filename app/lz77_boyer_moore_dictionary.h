//
//  lz77_boyer_moore_dictionary.h
//  
//
//  Created by Guido on 03/12/14.
//
//

#ifndef _lz77_boyer_moore_dictionary_h
#define _lz77_boyer_moore_dictionary_h
#include <algorithm>
#include "lz77_naive_dictionary.h"
#include "utils.h"

template <size_t max_size, typename T>
class LZ77BoyerMooreDictionary {
public:
    LZ77BoyerMooreDictionary():bc_table(1 << (8*sizeof(T))){}
    
    template <typename InputIterator>
    LZ77BoyerMooreDictionary(InputIterator first, InputIterator last)
    : _dictionary(first, last), bc_table(1 << (8*sizeof(T))) {}
    
    typedef typename LZ77NaiveDictionary<max_size, T>::match_type match_type;
    
    template <typename Container>
    match_type find_match(const Container&);
    
    void clear();
    
    template <typename InputIterator>
    void slide_window(InputIterator begin, InputIterator end);
    
private:
    std::deque<T> _dictionary;
    
    std::vector<int> bc_table;
    
    template <typename Container>
    void preprocess_bad_character_rule(const Container&);
    
    template <typename Container>
    void preprocess_good_suffix_rule(const Container&, std::vector<int>& gs_table);
};


template <size_t max_size, typename T>
template <typename Container>
typename LZ77BoyerMooreDictionary<max_size, T>::match_type
LZ77BoyerMooreDictionary<max_size, T>::find_match(const Container& search){
    std::vector<T> pattern(search.rbegin(), search.rend());
    std::vector<int> gs_table(pattern.size() + 1, pattern.size());
    
    preprocess_bad_character_rule(pattern);
    preprocess_good_suffix_rule(pattern, gs_table);
    
    int i;
    match_type match;
    int size_diff = _dictionary.size();
    size_diff -= pattern.size();
    for(int j = 0; j <  size_diff;){
        for(i = pattern.size() - 1; i >= 0 && pattern[i] == _dictionary[i + j]; --i);
        if(i < 0){
        // The whole pattern has been found
            match.position = _dictionary.size() - j;
            match.length = pattern.size();
            return match;
        }
        else{
            decltype(match.length) current_length = pattern.size() - i - 1;
            if((current_length > 0) && (match.length <= current_length)){
                match.position = _dictionary.size() - j;
                match.length = current_length;
            }
            j += std::max(static_cast<size_t>(gs_table[i]), bc_table[_dictionary[i + j]] - pattern.size() + 1 + i);
        }
    }
    return match;
}

template <size_t max_size, typename T>
template <typename Container>
inline void LZ77BoyerMooreDictionary<max_size, T>::preprocess_bad_character_rule(const Container& pattern){
    bc_table.assign(bc_table.size(), pattern.size());
    auto pattern_length = pattern.size();
    auto end = pattern.end() - 1;
    for(auto it = pattern.begin(); it != end; ++it){
        bc_table[*it] = pattern_length - (it - pattern.begin()) - 1;
    }

}

template <size_t max_size, typename T>
template <typename Container>
void LZ77BoyerMooreDictionary<max_size, T>::preprocess_good_suffix_rule(const Container& pattern, std::vector<int>& gs_table){
    
    // Suffixes
    std::vector<int> suffix(pattern.size());
    suffix[pattern.size() - 1] = pattern.size();
    int g = pattern.size() - 1, f = 0;
    for(int i = pattern.size() - 2; i >= 0; --i){
        if(i > g && suffix[i + pattern.size() - 1 - f] < (i - g)){
            suffix[i] = suffix[i + pattern.size() - 1 - f];
        }
        else{
            if(i < g){
                g = i;
            }
            f = i;
            while(g >= 0 && pattern[g] == pattern[g - pattern.size() -1 - f]){
                --g;
            }
            suffix[i] = f - g;
        }
    }
    
    //
    size_t j = 0;
    for(int i = pattern.size() - 1; i >= 0; --i){
        if(suffix[i] == i + 1){
            for(; j < pattern.size() - 1 - i; ++j){
                if(static_cast<size_t>(gs_table[j]) == pattern.size()){
                    gs_table[j] = static_cast<int>(pattern.size() - 1 - i);
                }
            }
        }
    }
    
    for(size_t i = 0; i <= pattern.size() - 2; ++i){
        gs_table[pattern.size() - 1 - suffix[i]] = static_cast<int>(pattern.size() - 1 - i);
    }
}

template <size_t max_size, typename T>
template <typename InputIterator>
inline void LZ77BoyerMooreDictionary<max_size, T>::slide_window(InputIterator begin,
                                                           InputIterator end) {
    for(; begin != end; ++begin){
        _dictionary.push_front(*begin);
    }
    
    auto size = _dictionary.size();
    if (size > max_size) {
        auto dend = _dictionary.end();
        _dictionary.erase(dend - (size - max_size), dend);
    }
}



#endif
