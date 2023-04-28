#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

enum Color { RED, BLACK };
vector<string> names(20);

// 2-3 B-tree Implement
int childrenL = 3;
int keysN = 2;
int count12 = 0;

// 2-3B Tree
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
            keys.insert(keys.begin(), children.at(splitIndex)->keys.at(median));
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
        children.insert(next(children.begin() + splitIndex), greaterSplitNode);
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
            keys.insert(keys.begin(), children.at(splitIndex)->keys.at(median));
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
        children.insert(next(children.begin() + splitIndex), greaterSplitNode);
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

vector<vector<string>> tubers(20);
vector<string> names(20);
vector<int> subCt(20);


// RedBlack Tree
// YouTube youTubeData struct
struct YouTubeData {
    int videoCommentCount;
    int channelCommentCount;
    string channelId;
    string videoId;
    string videoPublished;
    long long  channelViewCount;
    int channelElapsedTime;
    double commentsSubscriber;
    double commentsViews;
    double dislikesSubscriber;
    double dislikesViews;
    int elapsedTime;
    int likesDislikes;
    double likesSubscriber;
    double likesViews;
    int subscriberCount;
    double totalViewsChannelElapsedTime;
    double totVideosVideoCount;
    double totViewsTotSubs;
    int videoCategoryId;
    int videoCount;
    int videoDislikeCount;
    int videoLikeCount;
    int videoViewCount;
    double viewsElapsedTime;
    double viewsSubscribers;
};

vector<YouTubeData> tubers(20);

// Red-Black Tree Node
struct Node {
    string nodeKey;
    Color color;
    Node *leftPtr, *rightPtr, *parentPtr;
    YouTubeData youTubeData;

    Node(const string &nodeKey, const YouTubeData& youTubedata, Color color, Node *parent = nullptr) {
        this->nodeKey = nodeKey;
        this->youTubeData = youTubedata;
        this->color = color;
        this->leftPtr = nullptr;
        this->rightPtr = nullptr;
        this->parentPtr = parent;
    }
};

// Red-Black Tree
class RedBlackTree {

public:
    RedBlackTree() {
        root = nullptr;
    }

    // inserts a new node with a given key and YouTube youTubeData into the red-black tree
    void insert(const string &nodeKey, const YouTubeData& youTubeData) {
        // if the tree is empty, create a new BLACK root node
        if (!root) {
            root = new Node(nodeKey, youTubeData, BLACK);
        } else {
            // traverse the tree to find the correct position for the new rootNodeCheck
            Node *rootNodeCheck = root;
            Node *parentNode = nullptr;
            while (rootNodeCheck) {
                parentNode = rootNodeCheck;
                rootNodeCheck = (nodeKey < rootNodeCheck->nodeKey) ? rootNodeCheck->leftPtr : rootNodeCheck->rightPtr;
            }

            // create the new RED rootNodeCheck and connect it to its parentPtr
            Node *insertedNode = new Node(nodeKey, youTubeData, RED, parentNode);
            (nodeKey < parentNode->nodeKey) ? parentNode->leftPtr = insertedNode : parentNode->rightPtr = insertedNode;

            // change the red-black tree proprties after insertion
            while (insertedNode != root && insertedNode->parentPtr->color == RED) {
                Node *grandParentNode = insertedNode->parentPtr->parentPtr;
                bool isLeft = (insertedNode->parentPtr == grandParentNode->leftPtr);
                Node *uncleNode = isLeft ? grandParentNode->rightPtr : grandParentNode->leftPtr;

                // case 1: Uncle rootNodeCheck is RED
                if (uncleNode && uncleNode->color == RED) {
                    grandParentNode->color = RED;
                    insertedNode->parentPtr->color = BLACK;
                    uncleNode->color = BLACK;
                    insertedNode = grandParentNode;
                } else {
                    // case 2: Node is an inner child
                    if (isLeft && insertedNode == insertedNode->parentPtr->rightPtr) {
                        insertedNode = insertedNode->parentPtr;
                        leftRotation(insertedNode);
                    } else if (!isLeft && insertedNode == insertedNode->parentPtr->leftPtr) {
                        insertedNode = insertedNode->parentPtr;
                        rightRotation(insertedNode);
                    }

                    // case 3: Node is an outer child
                    isLeft ? rightRotation(grandParentNode) : leftRotation(grandParentNode);
                    swap(insertedNode->parentPtr->color, grandParentNode->color);
                    insertedNode = insertedNode->parentPtr;
                }
            }

            // make sure the root is always BLACK
            root->color = BLACK;
        }
    }

    // searches in the red-black tree for a node with the specified key
    Node *search(const string &nodeKey) {
        Node *node = root;
        // traverse the tree to find the node with the specified key
        while (node && node->nodeKey != nodeKey) {
            node = (nodeKey < node->nodeKey) ? node->leftPtr : node->rightPtr;
        }

        return node;
    }


private:
    Node *root;

    // performs a leftPtr rotation on the given rotateNode
    Node* leftRotation(Node *rotateNode) {
        // save the leftPtr child of the rightPtr child (grandChild) and the rightPtr child (new parentPtr)
        Node *grandChild = rotateNode->rightPtr->leftPtr;
        Node *newParent = rotateNode->rightPtr;

        // connect the new parentPtr's leftPtr child to the rotateNode and the rotateNode's rightPtr child to the grandChild
        newParent->leftPtr = rotateNode;
        rotateNode->rightPtr = grandChild;

        // update the grandChild's parentPtr pntr if it exists
        if (grandChild) {
            grandChild->parentPtr = rotateNode;
        }

        // update parentPtr pntrs for the rotateNode and new parentPtr
        newParent->parentPtr = rotateNode->parentPtr;
        rotateNode->parentPtr = newParent;

        // update the root or the parentPtr's child pntr as needed
        if (!newParent->parentPtr) {
            root = newParent;
        } else if (newParent->parentPtr->leftPtr == rotateNode) {
            newParent->parentPtr->leftPtr = newParent;
        } else {
            newParent->parentPtr->rightPtr = newParent;
        }

        return newParent;
    }

    // performs a rightPtr rotation on the given rotateNode
    Node* rightRotation(Node *rotateNode) {
        // save the rightPtr child of the leftPtr child (grandChild) and the leftPtr child (new parentPtr)
        Node *grandChild = rotateNode->leftPtr->rightPtr;
        Node *newParent = rotateNode->leftPtr;

        // connect the new parentPtr's rightPtr child to the rotateNode and the rotateNode's leftPtr child to the grandChild
        newParent->rightPtr = rotateNode;
        rotateNode->leftPtr = grandChild;

        // update the grandChild's parentPtr pntr if it exists
        if (grandChild) {
            grandChild->parentPtr = rotateNode;
        }

        // update parentPtr pntrs for the rotateNode and new parentPtr
        newParent->parentPtr = rotateNode->parentPtr;
        rotateNode->parentPtr = newParent;

        // update the root or the parentPtr's child pntr as needed
        if (!newParent->parentPtr) {
            root = newParent;
        } else if (newParent->parentPtr->leftPtr == rotateNode) {
            newParent->parentPtr->leftPtr = newParent;
        } else {
            newParent->parentPtr->rightPtr = newParent;
        }

        return newParent;
    }


};


// Used for parsing the YouTube youTubeData csv file
vector<string> separateYoutubeData(const string &strng, char separator) {
    vector<string> dataTkns;
    stringstream ss(strng);
    string word;
    while (getline(ss, word, separator)) {
        dataTkns.push_back(word);
    }
    return dataTkns;
}

// function takes a string as input and returns true if the string is an integer, and false otherwise.
bool checkInt(const string &strng) {
    // check if the string is empty
    if (strng.empty()) {
        return false;
    }
    // iterate through each character in the string
    for (char c : strng) {
        // if the curr char is not a digit, return false
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

// A function that reads a CSV file and returns its data as a vector of maps.
vector<map<string, string>> read_csv_file(const string& file_path) {
    ifstream file_stream(file_path);
    string line;
    vector<string> headers;
    vector<map<string, string>> rows;

    // Check if the file was successfully opened
    if (!file_stream.is_open()) {
        cerr << "Error: Unable to open the file '" << file_path << "'." << endl;
        return rows;
    }

    // Read the headers of the CSV file
    if (getline(file_stream, line)) {
        istringstream line_stream(line);
        string header;
        while (getline(line_stream, header, ',')) {
            headers.push_back(header);
        }
    }

    // Read the rows of the CSV file
    while (getline(file_stream, line)) {
        istringstream line_stream(line);
        string cell;
        map<string, string> row;
        int col = 0;

        while (getline(line_stream, cell, ',')) {
            row[headers[col]] = cell;
            col++;
        }

        rows.push_back(row);
    }

    // Return the data as a vector of maps
    return rows;
}

void implement_rbtree_algorithm() {
    cout << "Implementing Red Black Tree algorithm..." << endl;
     ifstream file("data.csv");
    string line;
    RedBlackTree redBlackTree;

    // Read headers of the csv file first
    getline(file, line);
    
    string searchKey;
    cout << "Enter channelId to search for: ";
    cin >> searchKey;

    cout << "Searching for " << searchKey << "... \n"<< endl;

    // start timer
    auto start = high_resolution_clock::now();

// read youTubeData from the csv file
    while (getline(file, line)) {
        // remove double quotes
        line.erase(remove(line.begin(), line.end(), '\"'), line.end());

        // separateYoutubeData the line into fields
        vector<string> fields = separateYoutubeData(line, ',');

        // create a YouTubeData object and populate its fields
        YouTubeData data;
        data.videoCommentCount = stoi(fields[0]);
        data.channelCommentCount = stoi(fields[1]);
        string channelId = fields[2];



        // making sure if field[3] has the correct youTubeData type before converting
        if (!checkInt(fields[3])) {
            cout << "Invalid integer value for channelViewCount in line: " << line << endl;
            cout << "Value: " << fields[3] << endl;
            continue;
        }

        try {
            data.channelViewCount = stoll(fields[3]);
        } catch (const out_of_range& e) {
            cout << "Error converting channelViewCount in line: " << line << endl;
            cout << "Value: " << fields[3] << endl;
            continue;
        }

        // Additional fields
        data.channelElapsedTime = stoi(fields[4]);
        data.commentsSubscriber = stod(fields[5]);
        data.commentsViews = stod(fields[6]);
        data.dislikesSubscriber = stod(fields[7]);
        data.dislikesViews = stod(fields[8]);
        data.elapsedTime = stoi(fields[9]);
        data.likesDislikes = stoi(fields[10]);
        data.likesSubscriber = stod(fields[11]);
        data.likesViews = stod(fields[12]);
        data.subscriberCount = stoi(fields[13]);
        data.totalViewsChannelElapsedTime = stod(fields[14]);
        data.totVideosVideoCount = stod(fields[15]);
        data.totViewsTotSubs = stod(fields[16]);
        data.videoCategoryId = stoi(fields[17]);
        data.videoCount = stoi(fields[18]);
        data.videoDislikeCount = stoi(fields[19]);
        data.videoId = fields[20];
        data.videoLikeCount = stoi(fields[21]);
        data.videoPublished = fields[22];
        data.videoViewCount = stoi(fields[23]);
        data.viewsElapsedTime = stod(fields[24]);
        data.viewsSubscribers = stod(fields[25]);


        // store the youTubeData in the red-black tree

        if (find(names.begin(), names.end(), channelId) == names.end()) {
            for (int i = 0; i < 20; i++) {
                if (data.subscriberCount > tubers.at(19).subscriberCount && data.subscriberCount > tubers.at(i).subscriberCount) {
                    tubers.pop_back();
                    tubers.insert(tubers.begin() + i, data);
                    tubers.at(i).channelId = channelId;
                    names.pop_back();
                    names.insert(names.begin() + i, channelId);
                    break;
                }
            }
        }
    }

    cout << endl;
    for (int i = 0; i < 20; i++) {
        cout << tubers.at(i).channelId << ": " << tubers.at(i).subscriberCount << endl;
    }
    cout << endl;
    // search for channelId and print the youTubeData
    Node *result = redBlackTree.search(searchKey);

    // if the search was successful, print the youTubeData
    if (result) {
        YouTubeData data = result->youTubeData;
        cout << "Found channelId: " << searchKey << endl;
        cout << "VideoCommentCount: " << data.videoCommentCount << endl;
        cout << "ChannelCommentCount: " << data.channelCommentCount << endl;

        // Extra Fields
        cout << "ChannelViewCount: " << data.channelViewCount << endl;
        cout << "ChannelElapsedTime: " << data.channelElapsedTime << endl;
        cout << "CommentsSubscriber: " << data.commentsSubscriber << endl;
        cout << "CommentsViews: " << data.commentsViews << endl;
        cout << "DislikesSubscriber: " << data.dislikesSubscriber << endl;
        cout << "DislikesViews: " << data.dislikesViews << endl;
        cout << "ElapsedTime: " << data.elapsedTime << endl;
        cout << "LikesDislikes: " << data.likesDislikes << endl;
        cout << "LikesSubscriber: " << data.likesSubscriber << endl;
        cout << "LikesViews: " << data.likesViews << endl;
        cout << "SubscriberCount: " << data.subscriberCount << endl;
        cout << "TotalViewsChannelElapsedTime: " << data.totalViewsChannelElapsedTime << endl;
        cout << "TotVideosVideoCount: " << data.totVideosVideoCount << endl;
        cout << "TotViewsTotSubs: " << data.totViewsTotSubs << endl;
        cout << "VideoCategoryId: " << data.videoCategoryId << endl;
        cout << "VideoCount: " << data.videoCount << endl;
        cout << "VideoDislikeCount: " << data.videoDislikeCount << endl;
        cout << "VideoId: " << data.videoId << endl;
        cout << "VideoLikeCount: " << data.videoLikeCount << endl;
        cout << "VideoPublished: " << data.videoPublished << endl;
        cout << "VideoViewCount: " << data.videoViewCount << endl;
        cout << "ViewsElapsedTime: " << data.viewsElapsedTime << endl;
        cout << "ViewsSubscribers: " << data.viewsSubscribers << endl;


    } else {
        cout << "ChannelId not found." << endl;
    }

    // stop timer and calculate duration in seconds then print duration
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end - start);
    cout << "Parsing time: " << duration.count() << " seconds" << endl;

}

void implement_btree_algorithm() {
    cout << "Implementing hash map algorithm..." << endl;
    // File name to import data here
    string fname = "item_exported.csv";

    // Initialize variables to collect data from import file
    vector<vector<string>> content;
    vector<string> row;
    string line, word;
    string currentChannelID;

    // Initialize B+ tree data structure
    bTree bTreeYoutube;

    auto start = high_resolution_clock::now();
    cout << "Starting Count: " << endl;

    fstream file(fname, ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            //cout << "AAA" << endl;
            // i variable to get channel ID on third iteration to insert as key string
            int i = 0;
            // Clears row
            row.clear();

            stringstream str(line);

            // Collects all attributes and places into row vector
            while (getline(str, word, ',')) {
                if (i == 2)
                {
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

            //cout << temp.first << "  " << currentChannelID << endl;

            string sub = row.at(13).substr(1, (row.at(13).size() - 2));
            int subs = stoi(sub);

            if (find(names.begin(), names.end(), row.at(2)) == names.end()) {
                for (int i = 0; i < 20; i++) {
                    if (subs > subCt.at(19) && subs > subCt.at(i)) {
                        subCt.pop_back();
                        names.pop_back();
                        tubers.pop_back();

                        names.insert(names.begin() + i, row.at(2));
                        subCt.insert(subCt.begin() + i, subs);
                        tubers.insert(tubers.begin() + i, row);
                        break;
                    }
                }
            }
            //count12++;

           
            //cout << subs << endl;
            //cout << row.at(13) << endl;
            //cout << endl;
        }
    }
    else
        cout << "Could not open the file\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end - start);
    cout << "Parsing time: " << duration.count() << " seconds" << endl << endl;

    for (int i = 0; i < 20; i++) {
        cout << tubers.at(i).at(2) << ": " << tubers.at(i).at(13) << endl;
    }
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


    string searchForThis = "\"UC_wIGmvdyAQLtl-U2nHV9rg\"";

    pair<string, vector<string>>* channelPointer = bTreeYoutube.search(searchForThis);

    if (channelPointer != nullptr)
    {
        cout << "Found channel: " << searchForThis << endl;
    }
    else
    {
        cout << "oh brother this guy stinks" << endl;
    }

}

// A function to print the data of a vector of maps
void print_csv_data(const vector<map<string, string>>& rows) {
    for (size_t i = 0; i < rows.size(); i++) {
        cout << "Row " << i + 1 << ":\n";
        for (const auto& [key, value] : rows[i]) {
            cout << "  " << key << ": " << value << "\n";
        }
    }
}



int main() {
    string file_path = "/Users/mokshkeloo/CDA/data2.csv";
    auto rows = read_csv_file(file_path);

    int choice;
    bool exit = false;

    while (!exit) {
        cout << "Menu:\n";
        cout << "  1. Print CSV data\n";
        cout << "  2. Implement Red-Black algorithm\n";
        cout << "  3. Implement 2-3 B Tree algorithm\n";
        cout << "  4. Exit\n";
        cout << "Enter your choice (1-4): ";
        cin >> choice;

        switch (choice) {
            case 1:
                print_csv_data(rows);
                break;
            case 2:
                implement_rbtree_algorithm();
                break;
            case 3:
                implement_btree_map_algorithm();
                break;
            case 4:
                exit = true;
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
        }
    }

    return 0;
}
