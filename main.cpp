#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cctype>

#define HT_SIZE 26
#define HT_BASE 11

typedef unsigned long long ull;
typedef char letter;
typedef std::string symbol;

class HashTable {
private:
    std::list<std::pair<symbol, letter>> array[HT_SIZE];

    ull fastExp(ull base, ull expoent, ull mod) {
        ull ans = 1;
        while (expoent) {
            if (expoent & 1)
                ans = ((ans % mod) * (base % mod)) % mod;
            base = ((base % mod) * (base % mod)) % mod;
            expoent >>= 1;
        }
        return ans % mod;
    }

    ull hash(std::string value) {
        ull hash = 0;
        ull keySize = value.size();
        for (ull i = 0; i < keySize; i++)
            hash = (hash + value[i] * (fastExp(HT_BASE, keySize - i - 1, HT_SIZE))) % HT_SIZE;
        return hash % HT_SIZE;
    }

public:
    void insert(const std::string &symbol, char letter) {
        int index = hash(symbol);
        array[index].push_back({symbol, letter});
    }

    char getLetterBySymbol(const std::string &symbol) {
        int index = hash(symbol);
        for (auto &pair : array[index]) {
            if (pair.first == symbol) {
                return pair.second;
            }
        }
        return '\0';
    }

    void removeBySymbol(const std::string &symbol) {
        int index = hash(symbol);
        for (auto it = array[index].begin(); it != array[index].end(); ++it) {
            if (it->first == symbol) {
                array[index].erase(it);
                return;
            }
        }
    }

    void show() {
        std::cout << "[==== HASH TABLE ====]" << std::endl;
        for (int i = 0; i < HT_SIZE; ++i) {
            std::cout << i << "| ";
            for (auto &pair : array[i]) {
                std::cout << "(" << pair.first << ", " << pair.second << ") ";
            }
            if (array[i].empty())
                std::cout << "(NULL)";
            std::cout << std::endl;
        }
        std::cout << "[=====================]" << std::endl;
    }
};

class BoyerMoore {
private:
    std::vector<int> R;
    std::string P;

public:
    BoyerMoore(std::string p) : R(256), P(p) {
        int M = P.length();
        for (int i = 0; i < 256; i++)
            R[i] = -1;
        for (int j = 0; j < M; j++)
            R[P[j]] = j;
    }

    std::string search(std::string &T) {
        std::string segmentation;
        int N = T.size(), M = P.size(), skip = 0;
        for (int i = 0; i <= N - M; i += skip) {
            skip = 0;
            for (int j = M - 1; j >= 0; j--) {
                if (P[j] != T[i + j]) {
                    skip = j - R[T[i + j]];
                    if (skip < 1)
                        skip = 1;
                    break;
                }
            }
            segmentation += std::to_string(skip);
            segmentation.push_back(' ');
            if (skip == 0) {
                segmentation.push_back('(');
                segmentation += std::to_string(i);
                segmentation.push_back(')');
                segmentation.push_back(' ');
                skip = 1;
            }
        }
        return segmentation;
    }
};

std::string decoder(std::string &T, int k) {
    std::string decoded_text;
    for (char c : T) {
        if (c != ' ' && c != '.')
            decoded_text += char((c - 'A' - k + 26) % 26 + 'A');
        else
            decoded_text += c;
    }
    return decoded_text;
}

std::list<std::string> cesarDecoder(std::string &T, std::list<std::string> &words) {
    std::string decoded_text, segmentation;
    std::list<std::string> decoded_words;
    bool flag = false;
    for (int i = 0; i < 26; ++i) {
        decoded_text = decoder(T, i);
        for (std::string &P : words) {
            BoyerMoore bm(P);
            segmentation = bm.search(decoded_text);
            decoded_words.push_back(segmentation);
            for (auto w : segmentation) {
                if (w == '(') {
                    flag = true;
                    break;
                }
            }
        }
        if (flag)
            return decoded_words;
        else
            decoded_words.clear();
    }
    decoded_words.push_back("-1");
    return decoded_words;
}

int main() {
    // create dict
    HashTable ht;
    std::vector<std::pair<symbol, letter>> dict = {
        {":::", 'A'}, {".::", 'B'}, {":.:", 'C'}, {"::.", 'D'},
        {":..", 'E'}, {".:.", 'F'}, {"..:", 'G'}, {"...", 'H'},
        {"|::", 'I'}, {":|:", 'J'}, {"::|", 'K'}, {"|.:", 'L'},
        {".|:", 'M'}, {".:|", 'N'}, {"|:.", 'O'}, {":|.", 'P'},
        {":.|", 'Q'}, {"|..", 'R'}, {".|.", 'S'}, {"..|", 'T'},
        {".||", 'U'}, {"|.|", 'V'}, {"||.", 'W'}, {"-.-", 'X'},
        {".--", 'Y'}, {"--.", 'Z'}, {"---", ' '}, {"~~~", '.'}
    };
    for (auto &pair : dict)
        ht.insert(pair.first, pair.second);

    // store decoded text
    std::string symbols_text, translated_text;
    while (std::getline(std::cin, symbols_text)) {
        for (int i = 0; i < symbols_text.size(); i += 3) {
            std::string symbol = symbols_text.substr(i, 3);
            translated_text += ht.getLetterBySymbol(symbol);
        }
        if (translated_text.substr(translated_text.size() - 1) == ".")
            break;
    }

    // store target words
    std::list<std::string> target_words;
    std::string word;
    while (std::getline(std::cin, word)) {
        std::string upper;
        if (word[0] >= 'A' && word[0] <= 'Z')
            for (char p : word)
                upper.push_back(p);
        else
            for (char p : word)
                upper.push_back(p - ('a' - 'A'));
        if (upper == "FIM" || word == "FIM")
            break;
        target_words.push_back(upper);
    }

    std::list<std::string> decoded_text = cesarDecoder(translated_text, target_words);
    auto i = target_words.begin();
    auto j = decoded_text.begin();

    while (i != target_words.end() && j != decoded_text.end()) {
        std::cout << *i << ": ";
        std::cout << *j << std::endl;
        i++;
        j++;
    }
    return 0;
}
