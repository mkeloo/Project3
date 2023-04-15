#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


struct Key
{
    string channelID;
    vector<string> attributes;

    Key(string channel, vector<string> attr)
    {
        channelID = channel;
        attributes = attr;
    }
};

struct Node
{
    int Lkeys = 2;
    int Nchildren = 3;

    bool leaf;
    vector<Key> keys;
    Node* parent = nullptr;
    vector<Node*> children;
    Node* forwardLeaf = nullptr;
    Node* previousLeaf = nullptr;


    void insert(Key insertMe)
    {
        // If node is a leaf node
        if (leaf)
        {
            // Insert element at appropriate position
            auto iter = keys.begin();
            while (insertMe.channelID < iter->channelID)
            {
                iter++;
            }
            keys.insert(iter, insertMe);

            // If too many keys in the node, make new leaf nad move keys
            if (keys.size() > Lkeys)
            {
                // Creates new leaf node to move extra keys to and links the leaves together
                Node newLeaf;
                newLeaf.leaf = true;
                // Push keys 1 and 2 to new leaf
                for (int i = 1; i < this->keys.size(); i++)
                {
                    newLeaf.keys.push_back(this->keys.at(i));
                }
                // Erase keys 1 and 2, keep key 0 in leaf
                while (keys.size() > 1)
                {
                    keys.erase(next(keys.begin()));
                }
                // Link leafs to finish splitting
                forwardLeaf = &newLeaf;
                newLeaf.previousLeaf = this;

                // Moves a copy of a leaf key (first key of new leaf node) to parent node
                // create parent node if none exists yet
                if (parent == nullptr)
                {
                    Node newParent;
                    newParent.leaf = false;
                    // Insert first element into parent node
                    parent->keys.push_back(newLeaf.keys.at(0));
                    // add child to parent's children vector
                    parent->children.push_back(this);
                }
                else
                {
                    parent->insert(newLeaf.keys.at(0));
                }

            }
        }

        else // If an internal node
        {
            // Insert element at appropriate position
            auto iter = keys.begin();
            while (insertMe.channelID < iter->channelID)
            {
                iter++;
            }
            keys.insert(iter, insertMe);

            // If amount of keys goes over maximum amount allowed, move up
            if (keys.size() > (Nchildren - 1))
            {
                // If parent does not exist create one and place middle key inside
                if (parent == nullptr)
                {
                    Node newParent;
                    newParent.leaf = false;
                    // Insert middle element into parent node
                    parent->keys.push_back(keys.at(1));
                    // Erase middle element from current internal node since it moved up
                    keys.erase(next(keys.begin()));
                    // Place in children vector
                    parent->children.push_back(this);
                }
                else
                {
                    // If parent exists, insert middle key
                    parent->insert(keys.at(1));
                    // Erases Middle Element from current internal node
                    keys.erase(next(keys.begin()));
                }
            }

            // If number of children over maximum amount split node
            if (children.size() > Nchildren)
            {

            }
        }
    }



};

struct bPlusTree
{
    // 2-3 B+ Tree
    Node* root;
    Node* firstLeaf;

    // Functions
    void insert(string id, vector<string> attr)
    {
        Key keyToInsert(id, attr);
        Node nodeToInsert;
    }

    void search(string)
    {

    }
};

int main()
{
    // File name to import data here
    string fname = "item_exported.csv";

    // Initialize variables to collect data from import file
    vector<vector<string>> content;
    vector<string> row;
    string line, word;
    string currentChannelID;

    // Initialize B+ tree data structure
    bPlusTree ytBplusTree;

    fstream file (fname, ios::in);
    if(file.is_open())
    {
        while(getline(file, line))
        {
            // i variable to get channel ID on third iteration to insert as key string
            int i = 0;
            // Clears row
            row.clear();

            stringstream str(line);

            // Collects all attributes and places into row vector
            while(getline(str, word, ',')) {
                if (i == 2)
                {
                    currentChannelID = word;
                }
                row.push_back(word);
                i++;
            }

            // Row vector now has all the attributes
            content.push_back(row);
            // INSERT ROW and use CURRENTCHANNELID as KEY into B+ TREE STRUCTURE HERE
            // Creates key
            Key insertKey(currentChannelID, row);

        }
    }
    else
        cout<<"Could not open the file\n";


    cout << content[8][2] << endl;


    return 0;
}