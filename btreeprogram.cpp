#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// 2-3 B-tree Implement
int childrenL = 3;
int keysN = 2;

struct Node
{
    vector<pair<string, vector<string>>> keys;
    vector<Node*> children;
    bool isLeaf;

    Node()
    {
        isLeaf = false;
    }

    void insertKeyNodeNotFull(pair<string, vector<string>> insertMe)
    {
        // If no keys in node, simply push_back the key InsertMe
        if (keys.empty())
        {
            keys.push_back(insertMe);
        }
        // Else find appropriate place to insert new key in ascending order
        else
        {
            // 1 (not full) keys present

            // In the case where there is 1 key in the node already (node not full)
            if (keys.size() == 1)
            {
                // If less than the key, place as the 1st key
                if (insertMe.first < keys.at(0).first)
                {
                    keys.insert(keys.begin(), insertMe);
                }
                // If greater than, place as 2nd key
                else
                {
                    keys.push_back(insertMe);
                }
            }
        }
    }


    void insertKeyNodeFull(pair<string, vector<string>> insertMe)
    {
        // In the case where there are 2 keys in the node already (node IS full)
        if (keys.size() == 2) {
            // Insert where key where it needs to be
            // If less than 1st key
            if (insertMe.first < keys.at(0).first)
            {
                keys.insert(keys.begin(), insertMe);
            }
            // If greater than 2nd key
            else if (insertMe.first > keys.at(1).first)
            {
                keys.push_back(insertMe);
            }
            // If in between the 1st and second key
            else
            {
                keys.insert(next(keys.begin()), insertMe);
            }
        }
    }

    void splitChildNewParent()
    {
        // Find median element of child which will be 1 in 2-3 tree (3 keys will be in child when here)
        int median = 1;

        // Place median key into keys (move up)
        keys.push_back(children.at(0)->keys.at(median));

        // Split child into two
        // Create new node to house >= median keys
        Node* greaterSplitNode = new Node;
        // If child is leaf, make another leaf child, and same for if its not a leaf
        if (children.at(0)->isLeaf)
        {
            greaterSplitNode->isLeaf = true;
        }
        else
        {
            greaterSplitNode->isLeaf = false;
            // Allocate children amongst split nodes
            // Copy children 2 and 3 from original root into greater split to split up children
            greaterSplitNode->children.push_back(children.at(0)->children.at(2));
            greaterSplitNode->children.push_back(children.at(0)->children.at(3));
            // Erase children 2 and 3 from original root
            children.at(0)->children.erase(children.at(0)->children.begin() + 2);
            children.at(0)->children.erase(children.at(0)->children.begin() + 2);
        }
        // Place key greater median in right child and push to children vector
        greaterSplitNode->keys.push_back(children.at(0)->keys.at(2));
        children.push_back(greaterSplitNode);
        // Delete those median and greater than key from the left child, leaving only 1st key in left child
        children.at(0)->keys.erase(next(children.at(0)->keys.begin()));
        children.at(0)->keys.erase(next(children.at(0)->keys.begin()));

    }

    void splitChildOldParent(int splitIndex)
    {
        // Find median element of child which will be 1 in 2-3 tree (3 keys will be in child when here)
        int median = 1;

        // SplitIndex will be the child you want to split
        // Place median key into keys (move up)
        if (splitIndex == 1) // If splitting bigger than median keys, place new median onto end
        {
            keys.push_back(children.at(splitIndex)->keys.at(median));
        }
        else // If splitting child smaller than the median key, place new median at the front
        {
            keys.insert(keys.begin() ,children.at(splitIndex)->keys.at(median));
        }


        // Split child into two
        // Create new node to house >= median keys
        Node* greaterSplitNode = new Node;
        // If child is leaf, make another leaf child, and same for if its not a leaf
        if (children.at(splitIndex)->isLeaf)
        {
            greaterSplitNode->isLeaf = true;
        }
        else
        {
            greaterSplitNode->isLeaf = false;
            // Allocate children amongst split nodes
            // Copy children 2 and 3 from original root into greater split to split up children
            greaterSplitNode->children.push_back(children.at(splitIndex)->children.at(2));
            greaterSplitNode->children.push_back(children.at(splitIndex)->children.at(3));
            // Erase children 2 and 3 from original root
            children.at(splitIndex)->children.erase(children.at(splitIndex)->children.begin() + 2);
            children.at(splitIndex)->children.erase(children.at(splitIndex)->children.begin() + 2);
        }
        // Place key greater median in right child and push to children vector
        greaterSplitNode->keys.push_back(children.at(splitIndex)->keys.at(2));
        // Insert at greatersplitNode at correct position
        children.insert(next(children.begin() + splitIndex) ,greaterSplitNode);
        // Delete those median and greater than key from the left child, leaving only 1st key in left child
        children.at(splitIndex)->keys.erase(next(children.at(splitIndex)->keys.begin()));
        children.at(splitIndex)->keys.erase(next(children.at(splitIndex)->keys.begin()));

    }

    void splitChildVeryOldParent(int splitIndex)
    {
        // Find median element of child which will be 1 in 2-3 tree (3 keys will be in child when here)
        int median = 1;

        // SplitIndex will be the child you want to split
        // Place median key into keys (move up)
        if (splitIndex == 2) // If splitting bigger than median keys, place new median onto end
        {
            keys.push_back(children.at(splitIndex)->keys.at(median));
        }
        else if (splitIndex == 1) // If splitting between median keys, place new median into middle
        {
            keys.insert(next(keys.begin()), children.at(splitIndex)->keys.at(median));
        }
        else // If splitting child smaller than the median key, place new median at the front
        {
            keys.insert(keys.begin() ,children.at(splitIndex)->keys.at(median));
        }


        // Split child into two
        // Create new node to house >= median keys
        Node* greaterSplitNode = new Node;
        // If child is leaf, make another leaf child, and same for if its not a leaf
        if (children.at(splitIndex)->isLeaf)
        {
            greaterSplitNode->isLeaf = true;
        }
        else
        {
            greaterSplitNode->isLeaf = false;
            // Allocate children amongst split nodes
            // Copy children 2 and 3 from original root into greater split to split up children
            greaterSplitNode->children.push_back(children.at(splitIndex)->children.at(2));
            greaterSplitNode->children.push_back(children.at(splitIndex)->children.at(3));
            // Erase children 2 and 3 from original root
            children.at(splitIndex)->children.erase(children.at(splitIndex)->children.begin() + 2);
            children.at(splitIndex)->children.erase(children.at(splitIndex)->children.begin() + 2);
        }
        // Place key greater median in right child and push to children vector
        greaterSplitNode->keys.push_back(children.at(splitIndex)->keys.at(2));
        // Insert at correct position
        children.insert(next(children.begin() + splitIndex) ,greaterSplitNode);
        // Delete those median and greater than key from the left child, leaving only 1st key in left child
        children.at(splitIndex)->keys.erase(next(children.at(splitIndex)->keys.begin()));
        children.at(splitIndex)->keys.erase(next(children.at(splitIndex)->keys.begin()));

    }

    // Search function returns pointer to key value pair if found, else nullptr
    void findLeafToInsert(Node* root, string wantedKey, pair<string, vector<string>> insertMe)
    {

        // If root is not the leaf, pick correct path to recursively call
        if (!root->isLeaf)
        {
            // In the case where there is only one key in the node
            // There is 1 key moved up to the current root,
            // and the num of children is 2 since it split only once
            if (root->keys.size() == 1)
            {
                // If wanted key is less than the only key of the root
                // go to first child of the root (keys less than the first child)
                if (wantedKey < root->keys.at(0).first)
                {
                    findLeafToInsert(root->children.at(0), wantedKey, insertMe);
                }
                // If wanted key greater than only key of the root,
                // go to second child where keys > are located
                else if (wantedKey > root->keys.at(0).first)
                {
                    findLeafToInsert(root->children.at(1), wantedKey, insertMe);
                }
                // If wanted key is equal to the only key return
                else
                {
                    return;
                }

                // Check if there are too many keys in a child and split if needed
                if (root->children.at(0)->keys.size() > keysN)
                {
                    root->splitChildOldParent(0);
                }
                else if (root->children.at(1)->keys.size() > keysN)
                {
                    root->splitChildOldParent(1);
                }
            }
            // Else there are two keys in the root, and there are 3 children
            else
            {
                // Check if wanted key is equal to any of the keys in the root
                if (wantedKey == root->keys.at(0).first)
                {
                    return;
                }
                if (wantedKey == root->keys.at(1).first)
                {
                    return;
                }

                // If not any of the keys in the node go to children
                // If key less than first key, go to first child (keys < 1st key)
                if (wantedKey < root->keys.at(0).first)
                {
                    findLeafToInsert(root->children.at(0), wantedKey, insertMe);
                }
                // If key is greater than second key, go to third child (keys > 2nd key)
                else if (wantedKey > root->keys.at(1).first)
                {
                    findLeafToInsert(root->children.at(2), wantedKey, insertMe);
                }
                // If key is between the first and second key go to middle child
                else
                {
                    findLeafToInsert(root->children.at(1), wantedKey, insertMe);
                }

                // Check if any children have too many keys (check three)
                if (root->children.at(0)->keys.size() > keysN)
                {
                    root->splitChildVeryOldParent(0);
                }
                else if (root->children.at(1)->keys.size() > keysN)
                {
                    root->splitChildVeryOldParent(1);
                }
                else if (root->children.at(2)->keys.size() > keysN)
                {
                    root->splitChildVeryOldParent(2);
                }
            }
        }


        // If root is leaf return it
        else if (root->isLeaf)
        {
            // Look through all the keys to see if already present
            for (int i = 0; i < root->keys.size(); i++)
            {
                if (root->keys.at(i).first == wantedKey)
                {
                    return;
                }
            }

            // If not present insert
            if (root->keys.size() < keysN)
            {
                root->insertKeyNodeNotFull(insertMe);
            }
            else
            {
                root->insertKeyNodeFull(insertMe);
            }
        }

    }

    // Search function returns pointer to key value pair if found, else nullptr
    pair<string, vector<string>>* search(Node* root, string wantedKey)
    {
        pair<string, vector<string>>* returnKey = nullptr;

        // If root is not the leaf, pick correct path to recursively call
        if (!root->isLeaf)
        {
            // In the case where there is only one key in the node
            // There is 1 key moved up to the current root,
            // and the num of children is 2 since it split only once
            if (root->keys.size() == 1)
            {
                // If wanted key is less than the only key of the root
                // go to first child of the root (keys less than the first child)
                if (wantedKey < root->keys.at(0).first)
                {

                    returnKey = search(root->children.at(0), wantedKey);
                }
                // If wanted key greater than only key of the root,
                // go to second child where keys > are located
                else if (wantedKey > root->keys.at(0).first)
                {
                    returnKey = search(root->children.at(1), wantedKey);
                }
                // If wanted key is equal to the only key return it
                else
                {
                    return &root->keys.at(0);
                }
            }
            // Else there are two keys in the root, and there are 3 children
            else
            {
                // Check if wanted key is equal to any of the keys in the root
                if (wantedKey == root->keys.at(0).first)
                {
                    return &root->keys.at(0);
                }
                if (wantedKey == root->keys.at(1).first)
                {
                    return &root->keys.at(1);
                }

                // If not any of the keys in the node go to children
                // If key less than first key, go to first child (keys < 1st key)
                if (wantedKey < root->keys.at(0).first)
                {
                    returnKey = search(root->children.at(0), wantedKey);
                }
                // If key is greater than second key, go to third child (keys > 2nd key)
                else if (wantedKey > root->keys.at(1).first)
                {
                    returnKey = search(root->children.at(2), wantedKey);
                }
                // If key is between the first and second key go to middle child
                else
                {
                    returnKey = search(root->children.at(1), wantedKey);
                }
            }
        }


        // If root is leaf check keys to look for match
        else if (root->isLeaf)
        {
            cout << "leaf node reached" << endl;
            // Look through all the keys to see if they match to wanted key
            for (int i = 0; i < root->keys.size(); i++)
            {
                if (root->keys.at(i).first == wantedKey)
                {
                    return &root->keys.at(i);
                }
            }

            // Return nullptr if key is not found
            return nullptr;
        }

        return returnKey;
    }

};

struct bTree
{
    // Starting point to access tree
    Node* root;

    // Initialize bTree and set root as nullptr initially
    bTree()
    {
        root = nullptr;
    }

    // Search Function
    pair<string, vector<string>>* search(string wantedstr)
    {
        return root->search(root, wantedstr);
    }

    // Insert Function
    void insert(pair<string, vector<string>> insertMe)
    {
        // If nothing is in the tree (root is null) create a node and point root to it as the start
        // Also places the key into the newly made root
        // Starting root is a leaf
        if (root == nullptr)
        {
            Node* newRoot = new Node;
            newRoot->isLeaf = true;
            newRoot->insertKeyNodeNotFull(insertMe);
            root = newRoot;
        }
        else
        {
            // If the root has the maximum number of keys,
            // insert and split the node, moving the median key up

            // If root is a leaf node and has maxmimum amount of keys N
            if (root->keys.size() == keysN && root->isLeaf)
            {
                // Insert key into root
                root->insertKeyNodeFull(insertMe);
                // Make new root
                Node* newRoot = new Node;
                newRoot->isLeaf = false;
                // Make child of new root the old root
                newRoot->children.push_back(root);
                // Perform a split operation on that child
                newRoot->splitChildNewParent();

                // Point root of B-Tree to newly created root
                root = newRoot;
            }
            // If root is leaf node and is not full
            else if (root->isLeaf)
            {
                root->insertKeyNodeNotFull(insertMe);
            }
            // If root is not a leaf anymore
            else if (!root->isLeaf)
            {
                root->findLeafToInsert(root, insertMe.first, insertMe);

                // Check if root has too many keys and split if it does
                if (root->keys.size() > keysN)
                {
                    // Make new root
                    Node* newRoot = new Node;
                    newRoot->isLeaf = false;
                    // Make child of new root the old root
                    newRoot->children.push_back(root);
                    // Perform a split operation on that child
                    newRoot->splitChildNewParent();

                    // Make newroot the root
                    root = newRoot;
                }

            }
        }
    }
};

int main() {
    // File name to import data here
    string fname = "item_exported.csv";

    // Initialize variables to collect data from import file
    vector<vector<string>> content;
    vector<string> row;
    string line, word;
    string currentChannelID;

    // Initialize B+ tree data structure
    bTree bTreeYoutube;

    fstream file(fname, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            // i variable to get channel ID on third iteration to insert as key string
            int i = 0;
            // Clears row
            row.clear();

            stringstream str(line);

            // Collects all attributes and places into row vector
            while (getline(str, word, ',')) {
                if (i == 2) {
                    currentChannelID = word;
                }
                row.push_back(word);
                i++;
            }

            // Row vector now has all the attributes
            content.push_back(row);
            // INSERT ROW and use CURRENTCHANNELID as KEY into B TREE STRUCTURE HERE
            pair<string, vector<string>> temp;
            temp.first = currentChannelID;
            temp.second = row;

            //******** NEW ADDITION: Inserts key into B+ tree
            bTreeYoutube.insert(temp);
        }
    } else
        cout << "Could not open the file\n";

//   DEBUG!
//    cout << content[8][2] << endl;
//
//    cout << bTreeYoutube.root->keys.size() << endl;
//
//    cout << "Root first key " << bTreeYoutube.root->keys.at(0).first << endl;
//
//    cout << bTreeYoutube.root->children.at(0)->keys.at(0).first << endl;
//
//    cout << bTreeYoutube.root->children.at(1)->keys.at(0).first << endl;

//    string bruh = "\"UCXjtAvK5P3wXBGh0vbGylzg\"";
//    string bruh2 = bTreeYoutube.root->keys.at(0).first;
//    bool isSmaller = bruh == bTreeYoutube.root->keys.at(0).first;
//
//    cout << isSmaller << endl;
//    cout << bruh << endl;
//    cout << bruh2 << endl;

    string choice;
    cout << "Input choice: 1-10";
    cin >> choice;

    if (choice == "1")
    {
        string searchForThis = "\"UC_wIGmvdyAQLtl-U2nHV9rg\"";
        pair<string, vector<string>>* channelPointer = bTreeYoutube.search(searchForThis);

        if (channelPointer != nullptr)
        {
            cout << "Found channel: " << searchForThis << endl;
            for (int i = 0; i < channelPointer->second.size(); i++)
            {
                cout << channelPointer->second.at(i) << endl;
            }
        }
        else
        {
            cout << "oh brother this guy stinks" << endl;
        }
    }
    else if (choice == "2")
    {
        string searchForThis = "\"UCaBmj7srjEN8hpcq2lcYQJA\"";
        pair<string, vector<string>>* channelPointer = bTreeYoutube.search(searchForThis);

        if (channelPointer != nullptr)
        {
            cout << "Found channel: " << searchForThis << endl;
            for (int i = 0; i < channelPointer->second.size(); i++)
            {
                cout << channelPointer->second.at(i) << endl;
            }
        }
        else
        {
            cout << "oh brother this guy stinks" << endl;
        }
    }
    else if (choice == "3")
    {
        string searchForThis = "\"UCVjfxCDKw_1bfH9GzhfmuMQ\"";
        pair<string, vector<string>>* channelPointer = bTreeYoutube.search(searchForThis);

        if (channelPointer != nullptr)
        {
            cout << "Found channel: " << searchForThis << endl;
            for (int i = 0; i < channelPointer->second.size(); i++)
            {
                cout << channelPointer->second.at(i) << endl;
            }
        }
        else
        {
            cout << "oh brother this guy stinks" << endl;
        }
    }
    else if (choice == "4")
    {
        string searchForThis = "\"UCpJ-kuYqO3-FmecU4hViajw\"";
        pair<string, vector<string>>* channelPointer = bTreeYoutube.search(searchForThis);

        if (channelPointer != nullptr)
        {
            cout << "Found channel: " << searchForThis << endl;
            for (int i = 0; i < channelPointer->second.size(); i++)
            {
                cout << channelPointer->second.at(i) << endl;
            }
        }
        else
        {
            cout << "oh brother this guy stinks" << endl;
        }
    }
    else if (choice == "5")
    {
        string searchForThis = "\"UC_6j2h3N5jNjx-IVWhIeXhQ\"";
        pair<string, vector<string>>* channelPointer = bTreeYoutube.search(searchForThis);

        if (channelPointer != nullptr)
        {
            cout << "Found channel: " << searchForThis << endl;
            for (int i = 0; i < channelPointer->second.size(); i++)
            {
                cout << channelPointer->second.at(i) << endl;
            }
        }
        else
        {
            cout << "oh brother this guy stinks" << endl;
        }
    }
    else if (choice == "6")
    {
        string searchForThis = "\"UCtH1zjiEXG7pK_n30LXGpSQ\"";
        pair<string, vector<string>>* channelPointer = bTreeYoutube.search(searchForThis);

        if (channelPointer != nullptr)
        {
            cout << "Found channel: " << searchForThis << endl;
            for (int i = 0; i < channelPointer->second.size(); i++)
            {
                cout << channelPointer->second.at(i) << endl;
            }
        }
        else
        {
            cout << "oh brother this guy stinks" << endl;
        }
    }
    else if (choice == "7")
    {
        string searchForThis = "\"UC1lBNgO-iUWIzJdelqXKAvw\"";
        pair<string, vector<string>>* channelPointer = bTreeYoutube.search(searchForThis);

        if (channelPointer != nullptr)
        {
            cout << "Found channel: " << searchForThis << endl;
            for (int i = 0; i < channelPointer->second.size(); i++)
            {
                cout << channelPointer->second.at(i) << endl;
            }
        }
        else
        {
            cout << "oh brother this guy stinks" << endl;
        }
    }
    else if (choice == "8")
    {
        string searchForThis = "\"UCrNdw9pjfStYU_10jkU9Fwg\"";
        pair<string, vector<string>>* channelPointer = bTreeYoutube.search(searchForThis);

        if (channelPointer != nullptr)
        {
            cout << "Found channel: " << searchForThis << endl;
            for (int i = 0; i < channelPointer->second.size(); i++)
            {
                cout << channelPointer->second.at(i) << endl;
            }
        }
        else
        {
            cout << "oh brother this guy stinks" << endl;
        }
    }
    else if (choice == "9")
    {
        string searchForThis = "\"UCK_k08qLykhW03fPP5P1kWA\"";
        pair<string, vector<string>>* channelPointer = bTreeYoutube.search(searchForThis);

        if (channelPointer != nullptr)
        {
            cout << "Found channel: " << searchForThis << endl;
            for (int i = 0; i < channelPointer->second.size(); i++)
            {
                cout << channelPointer->second.at(i) << endl;
            }
        }
        else
        {
            cout << "oh brother this guy stinks" << endl;
        }
    }
    else if (choice == "10")
    {
        string searchForThis = "\"UCrfnNPLMIHZI-wDupFVPcBA\"";
        pair<string, vector<string>>* channelPointer = bTreeYoutube.search(searchForThis);

        if (channelPointer != nullptr)
        {
            cout << "Found channel: " << searchForThis << endl;
            for (int i = 0; i < channelPointer->second.size(); i++)
            {
                cout << channelPointer->second.at(i) << endl;
            }
        }
        else
        {
            cout << "oh brother this guy stinks" << endl;
        }
    }




    return 0;
}
