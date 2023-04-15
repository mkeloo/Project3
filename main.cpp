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
            auto iter = keys.begin();
            while (iter != keys.end() && insertMe.channelID > iter->channelID)
            {
                iter++;
            }
            keys.insert(iter, insertMe);

            if (keys.size() > Lkeys)
            {
                splitLeafNode();
            }
        }
        else // If an internal node
        {
            auto iter = keys.begin();
            int childIndex = 0;
            while (iter != keys.end() && insertMe.channelID > iter->channelID)
            {
                iter++;
                childIndex++;
            }

            children[childIndex]->insert(insertMe);

            if (children[childIndex]->keys.size() > Lkeys)
            {
                splitInternalNode(childIndex);
            }
        }
    }

    void splitLeafNode()
    {
        Node* newLeaf = new Node;
        newLeaf->leaf = true;

        for (int i = 1; i < this->keys.size(); i++)
        {
            newLeaf->keys.push_back(this->keys.at(i));
        }

        while (keys.size() > 1)
        {
            keys.erase(next(keys.begin()));
        }

        forwardLeaf = newLeaf;
        newLeaf->previousLeaf = this;

        if (parent == nullptr)
        {
            Node* newParent = new Node;
            newParent->leaf = false;
            parent = newParent;
            this->parent = newParent;
            newLeaf->parent = newParent;
            parent->keys.push_back(newLeaf->keys.at(0));
            parent->children.push_back(this);
            parent->children.push_back(newLeaf);
        }
        else
        {
            newLeaf->parent = this->parent;
            parent->insert(newLeaf->keys.at(0));
        }
    }

    void splitInternalNode(int childIndex)
    {
        Node* splittedNode = new Node;
        splittedNode->leaf = false;

        for (int i = 1; i < this->keys.size(); i++)
        {
            splittedNode->keys.push_back(this->keys.at(i));
        }

        while (keys.size() > 1)
        {
            keys.erase(next(keys.begin()));
        }

        splittedNode->children.push_back(children.at(childIndex + 1));
        splittedNode->children.push_back(children.at(childIndex + 2));
        children.erase(children.begin() + childIndex + 1);
        children.erase(children.begin() + childIndex + 1);

        splittedNode->parent = this->parent;

        if (parent == nullptr)
        {
            Node* newParent = new Node;
            newParent->leaf = false;
            parent = newParent;
            this->parent = newParent;
            splittedNode->parent = newParent;
            parent->keys.push_back(splittedNode->keys.at(0));
            parent->children.push_back(this);
            parent->children.push_back(splittedNode);
        }
        else
        {
            parent->insert(splittedNode->keys.at(0));
        }
    }
};


struct bPlusTree {
    // 2-3 B+ Tree
    Node* root = nullptr;

    // insert function
    void insert(string id, vector<string> attr) {
        Key keyToInsert(id, attr);
        if (root == nullptr) {
            root = new Node;
            root->leaf = true;
        }
        root->insert(keyToInsert);
    }



    // search function
    vector<string> search(string id) {
        if (root == nullptr) {
            return vector<string>();
        }

        Node* curr = root;

        // travrese internal nodes until reaching a leaf node
        while (!curr->leaf) {
            auto iter = curr->keys.begin();
            size_t childIndx = 0;
            for (; iter != curr->keys.end(); ++iter, ++childIndx) {
                if (id < iter->channelID) {
                    break;
                }
            }
            curr = curr->children[childIndx];
        }

        // At this point, 'current' is a leaf node, so search for the key
        for (auto& key : curr->keys) {
            if (key.channelID == id) {
                // key found, return attributes
                return key.attributes;
            }
        }

        // key !found in the leaf node
        return vector<string>();
    }

    // function to delete nodes
    void deleteNodes(Node* node) {
        if (node == nullptr) {
            return;
        }
        if (!node->leaf) {
            for (auto& child : node->children) {
                deleteNodes(child);
            }
        }
        delete node;
    }


    // destruct
    ~bPlusTree() {
        deleteNodes(root);
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

            //******** NEW ADDITION: Inserts key into B+ tree
            ytBplusTree.insert(currentChannelID, row);
        }
    }
    else
        cout<<"Could not open the file\n";

    cout << content[8][2] << endl;

    // ******** NEW ADDITION: Searches for a key in the B+ tree and prints its attributes
    vector<string> keyAttributes = ytBplusTree.search("example_channel_id");
    if (keyAttributes.empty()) {
        cout << "Key not found in B+ tree" << endl;
    } else {
        cout << "Key attributes: ";
        for (auto& attr : keyAttributes) {
            cout << attr << " ";
        }
        cout << endl;
    }

    return 0;
}
