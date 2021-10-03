#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <queue>

using std::map;
using std::vector;
using std::set;
using std::queue;
using std::string;

struct ConstructionPool {
    struct Node {
        Node *child[27];
        set<int> idsLeft;

        [[nodiscard]] inline bool isTerminal() const {
            return child.empty();
        }
    };

    struct DNA {
        const string inputString;
        int pos = 0;
        Node *node = nullptr;

        DNA(ConstructionPool::Node *node, string inputString) :
                node(node), inputString(std::move(inputString)) {}
    };

    vector<DNA> addedDNA;
    Node *root;

    ConstructionPool() : root(new Node) {}

    vector<int> insertDNA(const string &inputString) {
        const int id = (int) addedDNA.size();
        root->idsLeft.insert(id);
        addedDNA.push_back({root, inputString});
        if (addDNA(id))
            return {id};
        return {};
    }

    vector<int> NewGene(const string &inputString) {
        queue<int> toUpdate;

        Node *curNode = root;
        for (const auto c : inputString) {
            if (!curNode->child.count(c)) {
                curNode->child.insert({c, new Node()});
                for (const auto &id : curNode->idsLeft) {
                    const DNA &dna = addedDNA[id];
                    if (dna.inputString[dna.pos] == c)
                        toUpdate.push(id);
                }
            }
            curNode = curNode->child.at(c);
        }

        vector<int> ans;
        while (!toUpdate.empty()) {
            auto id = toUpdate.front();
            toUpdate.pop();
            if (addDNA(id))
                ans.push_back(id);
        }
        return ans;
    }

    ~ConstructionPool() {
        delete root;
    }

    bool addDNA(int id) {
        DNA &dna = addedDNA[id];x
        Node *curNode = dna.node;
        curNode->idsLeft.erase(id);

        while (dna.pos < dna.inputString.length()) {
            char c = dna.inputString[dna.pos];
            if (curNode->child.count(c)) {
                curNode = curNode->child[c];
                dna.pos++;
            } else if (curNode->isTerminal() && curNode != root)
                curNode = root;
            else
                break;
        }

        if (dna.pos == dna.inputString.length() && curNode->isTerminal())
            return true;

        if (dna.pos < dna.inputString.length()) {
            curNode->idsLeft.insert(id);
            dna.node = curNode;
        }
        return false;
    }

};

int main() {
    int n = 0, k = 0;
    std::cin >> n;
    ConstructionPool pool;
    char op = 0;
    string inputString;
    for (int i = 0; i < n; ++i) {
        std::cin >> op >> inputString;
        k = k % (int)inputString.length();

        if (k)
            inputString = inputString.substr(k,
                                             inputString.length() - k) + inputString.substr(0, k);

        vector<int> ans = (op == '+') ?
                                     pool.NewGene(inputString) :
                                     pool.insertDNA(std::move(inputString));

        std::cout << ans.size() << ' ';
        for (const auto &id : ans)
            std::cout << (id + 1) << ' ';

        std::cout << std::endl;
        k = (int)ans.size();
    }
}
