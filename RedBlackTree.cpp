#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;


enum Color { RED, BLACK };

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


int main() {
    ifstream file("/Users/mokshkeloo/CDA/data.csv");
    string line;
    RedBlackTree redBlackTree;

    // Read headers of the csv file first
    getline(file, line);

    // search for channelId and print the dataf
    // string searchKey = "UC0UnhAG47DRyVZGVcbhAXhQ"; replace with different channelId
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
        redBlackTree.insert(channelId, data);
    }

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


    return 0;
}
