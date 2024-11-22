#include <iostream>
#include <string>
#include <vector>
#include <list>

#define HT_SIZE 26
#define HT_BASE 11

using namespace std;

typedef unsigned long long ull;
typedef char letter;
typedef string symbol;

class HashTable {
private:
    list<pair<symbol, letter>> array[HT_SIZE];

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

    ull hash(string value) {
        ull hash = 0;
        ull keySize = value.size();
        for (ull i = 0; i < keySize; i++)
            hash = (hash + value[i] * (fastExp(HT_BASE, keySize - i - 1, HT_SIZE))) % HT_SIZE;
        return hash % HT_SIZE;
    }

public:
    void insert(const string& symbol, char letter) {
        int index = hash(symbol);
        array[index].push_back({symbol, letter});
    }

    char getLetterBySymbol(const string& symbol) {
        int index = hash(symbol);
        for (auto& pair : array[index]) {
            if (pair.first == symbol) {
                return pair.second;
            }
        }
        return '\0';
    }

    void removeBySymbol(const string& symbol) {
        int index = hash(symbol);
        for (auto it = array[index].begin(); it != array[index].end(); ++it) {
            if (it->first == symbol) {
                array[index].erase(it);
                return;
            }
        }
    }

    void show() {
        cout << "[==== HASH TABLE ====]" << endl;
        for (int i = 0; i < HT_SIZE; ++i) {
            cout << i << "| ";
            for (auto& pair : array[i]) {
                cout << "(" << pair.first << ", " << pair.second << ") ";
            }
            if (array[i].empty())
                cout << "(NULL)";
            cout << endl;
        }
        cout << "[=====================]" << endl;
    }
};

int main() {
  // create dict
  HashTable ht;
  vector<pair<symbol, letter>> dict = {
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
  string symbols_text, encoded_text;
  while (getline(cin, symbols_text))
  {
    for (int i = 0; i < symbols_text.size(); i += 3) {
      string symbol = symbols_text.substr(i, 3);
      encoded_text += ht.getLetterBySymbol(symbol);
    }
    if (encoded_text.substr(encoded_text.size() - 1) == ".")
      break;
  }
  // store target words
  vector<string> target_words;
  string word;
  while (getline(cin, word) && word != "fim")
    target_words.push_back(word);

  cout << encoded_text << endl;
  for (auto &word : target_words)
    cout << word << endl;
}