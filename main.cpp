#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

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

void implement_graph_algorithm() {
    cout << "Implementing graph algorithm..." << endl;
    // Add your graph algorithm implementation here
}

void implement_hash_map_algorithm() {
    cout << "Implementing hash map algorithm..." << endl;
    // Add your hash map algorithm implementation here
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
        cout << "  2. Implement Graph algorithm\n";
        cout << "  3. Implement Hash Map algorithm\n";
        cout << "  4. Exit\n";
        cout << "Enter your choice (1-4): ";
        cin >> choice;

        switch (choice) {
            case 1:
                print_csv_data(rows);
                break;
            case 2:
                implement_graph_algorithm();
                break;
            case 3:
                implement_hash_map_algorithm();
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