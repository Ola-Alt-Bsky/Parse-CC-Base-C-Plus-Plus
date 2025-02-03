#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <direct.h>
#include "./json.hpp"  // External Header-only file provided by https://json.nlohmann.me/

using namespace std;
using json = nlohmann::json;

string trim_all_space(const string& str) {
    size_t new_start = str.find_first_not_of(" \t\n\r\f\v");
    size_t new_end = str.find_last_not_of(" \t\n\r\f\v");

    return (new_start == string::npos) ? "" : str.substr(new_start, new_end + 1);
}

string delete_char(const string& str, char ch) {
    string new_str = "";
    for (char c : str) {
        if (c != ch) {
            new_str += c;
        }
    }
    return new_str;
}

string trim_leading_space(const string& str) {
    // Find first non-space character
    size_t new_start = str.find_first_not_of(" \t\n\r\f\v");

    return (new_start == string::npos) ? "" : str.substr(new_start);
}

string remove_bom(const string& str) {
    if (str.size() >= 3 &&
        (unsigned char)str[0] == 0xEF &&
        (unsigned char)str[1] == 0xBB &&
        (unsigned char)str[2] == 0xBF) {
        return str.substr(3); // Remove BOM
    }
    return str;
}

json parse_to_json(vector<string>& file_lines) {
    json info;

    string last_season;
    string last_episode;
    string last_attribute;
    string last_content;
    string last_specific;

    for (string line : file_lines) {
        bool starts_with_star = line[0] == '*';
        bool starts_with_space = line[0] == ' ';
        int amount_leading_space = line.length() - trim_leading_space(line).length();

        if (!(starts_with_star || starts_with_space)) {  // Season
            line = remove_bom(line);

            info[line] = json::object();
            last_season = line;
        }
        else if (starts_with_star) { // Episode
            line = trim_all_space(delete_char(line, '*'));
            info[last_season][line] = json::object();
            last_episode = line;
        }
        else if (starts_with_space && amount_leading_space == 3) { // Attributes
            line = trim_all_space(delete_char(line, '*'));

            if (line == "Songs") {
                info[last_season][last_episode][line] = json::object();
            }
            else {
                info[last_season][last_episode][line] = json::array();
            }

            last_attribute = line;
        }
        else if (starts_with_space && amount_leading_space == 6) { // Content
            line = trim_all_space(delete_char(line, '*'));

            if (last_attribute == "Songs") {
                info[last_season][last_episode][last_attribute][line] = json::object();
            }
            else {
                info[last_season][last_episode][last_attribute].push_back(line);
            }

            last_content = line;
        }
        else if (starts_with_space && amount_leading_space == 9) { // Specific
            line = trim_all_space(delete_char(line, '*'));

            if (last_content == "Scene Specific") {
                info[last_season][last_episode][last_attribute][last_content][line] = json::object();
            }
            else {
                info[last_season][last_episode][last_attribute][last_content] = line;
            }

            last_specific = line;
        }
        else if (starts_with_space && amount_leading_space == 12) {
            line = trim_all_space(delete_char(line, '*'));
            info[last_season][last_episode][last_attribute][last_content][last_specific] = line;
        }
    }

    info.erase("Chapter Template");
    info.erase("Extra Songs");

    return info;
}

set<string> get_items(json& info, int item) {
    // 1 for characters, 2 for locations, 3 for songs

    set<string> item_list;
    for (auto& season : info.items()) {
        for (auto& episode : season.value().items()) {
            if (item == 1) {
                auto& ep_items = episode.value()["Characters"];
                for (auto item_i : ep_items) {
                    item_list.insert(item_i);
                }
            }
            else if (item == 2) {
                auto & ep_items = episode.value()["Locations"];
                for (auto item_i : ep_items) {
                    item_list.insert(item_i);
                }
            }
            else if (item == 3) {
                auto& songs = episode.value()["Songs"];
                for (auto& item : songs.items()) {
                    if (item.value().is_object()) {
                        for (auto& scene : item.value().items()) {
                            item_list.insert(scene.value());
                        }
                    }
                    else {
                        item_list.insert(item.value());
                    }
                }
            }
        }
    }

    return item_list;
}

int main() {
    // Read input from a .txt file
    string file_path;

    cout << "Welcome! You will need to enter in the location of your file." << endl;
    cout << "Enter in the ABSOLUTE file path of the base txt file: ";

    getline(cin, file_path);

    file_path.erase(
        remove(file_path.begin(), file_path.end(), '\"'), 
        file_path.end()
    );

    // Try to retrieve the text from the file
    ifstream file(file_path);

    if (!file) {
        cerr << "Error opening file: " << file_path << endl;
        return 1;
    }

    vector<string> file_lines;
    string cur_read_line;

    while (getline(file, cur_read_line)) {
        file_lines.push_back(cur_read_line);
    }

    file.close();

    // Parse and convert to JSON
    json parsed_json = parse_to_json(file_lines);

    // Retrieve a list of characters, locations, and song
    set<string> characters = get_items(parsed_json, 1);
    set<string> locations = get_items(parsed_json, 2);
    set<string> songs = get_items(parsed_json, 3);

    // Save the parsed JSON information to a folder
    string parent_dir = file_path.substr(0, file_path.find_last_of("/\\"));
    string output_dir = parent_dir + "\\Output";

    if (_mkdir(output_dir.c_str()) == 0) {
        cout << "Directory created: " << output_dir << endl;
    }
    else {
        perror("Could not create Output Directory");
    }

    ofstream writer;
    writer.open(output_dir + "\\Casual Roleplay.json");
    writer << parsed_json.dump(4);
    writer.close();
    cout << "Parsed JSON has been saved to Casual Roleplay.json." << endl;

    writer.open(output_dir + "\\Casual Roleplay Characters.txt");
    for (string item : characters) {
        writer << item << "\n";
    }
    writer.close();
    cout << "Parsed JSON has been saved to Casual Roleplay Characters.txt." << endl;

    writer.open(output_dir + "\\Casual Roleplay Locations.txt");
    for (string item : locations) {
        writer << item << "\n";
    }
    writer.close();
    cout << "Parsed JSON has been saved to Casual Roleplay Locations.txt." << endl;

    writer.open(output_dir + "\\Casual Roleplay Songs.txt");
    for (string item : songs) {
        writer << item << "\n";
    }
    writer.close();
    cout << "Parsed JSON has been saved to Casual Roleplay Songs.txt." << endl;

    return 0;
}
