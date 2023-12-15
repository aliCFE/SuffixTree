#include <iostream>
#include <cstring>

using namespace std;

// Node representing a suffix tree
struct SuffixTreeNode{
    SuffixTreeNode *child;
    SuffixTreeNode *sibling;
    int leafId;
    int startIndex;
    int length;

    SuffixTreeNode(int startIndex = -1, int length = -1, int leafId = -1)
        : child(nullptr), sibling(nullptr), leafId(leafId), startIndex(startIndex), length(length) {}

    ~SuffixTreeNode(){
        delete child;
        delete sibling;
    }
};

// Suffix tree class
class SuffixTree{
public:
    SuffixTreeNode *root;
    const char *str;
    // banana
public:
    SuffixTree(const char *text)
        : str(text), root(new SuffixTreeNode()){
        int length = static_cast<int>(strlen(str));
        for (int i = length - 1; i >= 0; --i){
            insert(i, length - i, root, i);
        }
    }
    // Insert a new edge into the suffix tree
    void insert(int startIndex, int length, SuffixTreeNode *n, int leafID){
        SuffixTreeNode *prev = nullptr; // prev pointer to keep track of prev node
        SuffixTreeNode *currentNode = n;

        // if current node has no children then add new child and return
        if (currentNode->child == nullptr){
            currentNode->child = new SuffixTreeNode(startIndex, length, leafID);
            return;
        }
        currentNode = n->child;
        bool foundMatch = false;

        // Iterate over all nodes in this linked list to find match
        while (currentNode != nullptr){

            // checks if there is a match in the children
            foundMatch = str[currentNode->startIndex] == str[startIndex];

            if (foundMatch){
                int i;
                // Count how many characters are common between these two nodes
                for (i = 0; i < currentNode->length; i++){
                    if (str[currentNode->startIndex + i] != str[startIndex + i])
                        break;
                }

                if (i < currentNode->length){
                    int oldLength = currentNode->length;
                    int oldID = currentNode->leafId;

                    // set the current node to the common characters length
                    currentNode->length = i;
                    // set the current node ID to -1 as it is not a leaf anymore
                    currentNode->leafId = -1;
                    // create a new node that carries the children of the current node and stores the different characters from the inserted strings
                    SuffixTreeNode *carryingChild = new SuffixTreeNode(currentNode->startIndex + i, oldLength - i, oldID);
                    carryingChild->child = currentNode->child;
                    currentNode->child = carryingChild;
                }

                // Recursive call to insert the rest of the characters
                insert(startIndex + i, length - i, currentNode, leafID);

                // ends the loop to avoid further insertions
                return;
            }
            else{
                prev = currentNode;
                currentNode = currentNode->sibling;
            }
        }
        // If you did not find a match, add new node to the linked list
        prev->sibling = new SuffixTreeNode(startIndex, length, leafID);
    }

    // Get leaf nodes and print their leaf IDs
    void getLeafNodes(SuffixTreeNode *root){
        if (root != nullptr && root->leafId != -1){
            cout << root->leafId << " ";
        }

        SuffixTreeNode *child = ((root != nullptr) ? root->child : nullptr);
        while (child != nullptr){
            getLeafNodes(child);
            child = child->sibling;
        }
    }

    const void search(SuffixTreeNode *n, const char *text, int foundCharacters){
        SuffixTreeNode *child = n->child;

        while (child != nullptr){
            if (str[child->startIndex] == text[foundCharacters])
                break;
            child = child->sibling;
        }

        // checks if a child for the corresponding character exists
        if (child == nullptr){
            return;
        }

        // for loop that iterates over all the characters in the node
        // if all characters were found we exit this loop
        for (int i = 0; (i < child->length) && (foundCharacters < strlen(text)); i++){
            // checks if the each character is equal to the corresponding search text positon.

            // if true we increment the position by one and check for the next character until we find all characters
            // else if we find a character that doesn't match our string's corresponding charcter we return and empty string
            char testChar1 = text[foundCharacters];
            char testChar2 = str[child->startIndex + i];
            int index = child->startIndex + i;
            if (str[child->startIndex + i] == text[foundCharacters])
                foundCharacters++;
            else
                return;
        }

        // if all corresponding characters in the node were found but we didn't reach the end of the search string
        // we search the child with the current foundCharacters position until foundCharacters is equal to the string length or a character mismatch.

        // else if all corresponding characters in the node were found and we reached the end of the search string.
        // we get all leaf nodes for the child and return them in string format uisng joinIntArray.
        if (foundCharacters < strlen(text)){
            search(child, text, foundCharacters);
        }
        else if (foundCharacters == strlen(text)){
            getLeafNodes(child);
        }
    }

    void search(const char *text){
        search(root, text, 0);
    }

    ~SuffixTree(){
        delete root;
    }
};

int main(){
    // Construct a suffix tree containing all suffixes of "bananabanaba$"
    //            0123456789012
    SuffixTree t("bananabanaba$");

    t.search("ana"); // Prints: 7 3 1
    cout << endl;

    t.search("naba"); // Prints: 8 4
    cout << endl;

    t.search("a"); // print: 5 9 11 3 7 1
    cout << endl;

    t.search("n"); // print: 4 8 2
    cout << endl;

    t.search("b"); // print: 0 6 10
    cout << endl;

    t.search("anan"); // print: 1
    cout << endl;

    t.search("banana"); // Prints: 0
    cout << endl;

    t.search("bananaban"); // Prints: 0
    cout << endl;

    t.search("anana"); // Prints: 1
    cout << endl;

    t.search("nana"); // Prints: 2
    cout << endl;

    t.search("ban"); // Prints: 6 0
    cout << endl;

    t.search("nabana"); // Prints: 4
    cout << endl;

    t.search("bana"); // Prints: 6 0
    cout << endl;

    t.search("abanaba"); // Prints: 5
    cout << endl;

    t.search("aba"); // Prints: 9 5
    cout << endl;

    t.search("aban"); // Prints: 5
    cout << endl;

    t.search("ali"); // Prints nothing because it's not a suffix
    cout << endl;

    t.search("an"); // Prints: 7 3 1
    cout << endl;

    return 0;
}