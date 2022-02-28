#include <vector>
#include <string>
#include <memory>

using std::vector;
using std::string;
using std::shared_ptr;
using std::make_shared;

namespace algorithm::tree{
    // simplify trie only character a-z
    struct trieNode{
        bool isLeaf = true;
        bool isEnd = false;

        shared_ptr<trieNode> parent;
        vector<shared_ptr<trieNode>> children;

        string val;

        trieNode(shared_ptr<trieNode> parent = nullptr): parent(parent), children(26, nullptr){}
    };

    class trie{
    private:
        shared_ptr<trieNode> root;
    public:
        inline trie(){
            root = make_shared<trieNode>();
            root->isEnd = true;
        }

        bool insert(const string& word);
        bool remove(const string& word);

        bool find(const string& word);
    };

    bool trie::insert(const string& word){
        if(word.empty())return false;
        auto p  = this->root;

        for(auto&& c : word){
            if(!p->children[c-'a']){
                p->children[c-'a'] = make_shared<trieNode>(p);
                p->isLeaf = false;
            }
            p = p->children[c-'a'];
        }
        if(p->isEnd){
            return false;
        }
        p->isEnd = true;
        p->val = word;

        return true;
    }

    bool trie::find(const string& word){
        auto p  = this->root;

        for(auto&& c : word){
            if(!p->children[c-'a']){
                return false;
            }
            p = p->children[c-'a'];
        }
        return p->isEnd;
    }

    bool trie::remove(const string& word){
        if(word.empty())return false;
        auto p  = this->root;

        for(auto&& c : word){
            if(!p->children[c-'a']){
                return false;
            }
            p = p->children[c-'a'];
        }

        if(p->isEnd && p->isLeaf){
            p = p->parent;
            
            for(auto iter = word.rbegin(); iter != word.rend(); iter++){
                p->children[*iter - 'a'] = nullptr;
                p = p->parent;
            }

            if(p)p->isLeaf = true;

            return true;
        } else if(p->isEnd){
            p->isEnd = false;
            return true;
        } else {
            return false;
        }
    }
}