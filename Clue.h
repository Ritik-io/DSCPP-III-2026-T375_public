#ifndef CLUE_H
#define CLUE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

enum class ClueType {
    PHYSICAL,
    TESTIMONIAL,
    DIGITAL,
    CIRCUMSTANTIAL
};

string clueTypeToString(ClueType type) {
    switch (type) {
        case ClueType::PHYSICAL:       return "Physical";
        case ClueType::TESTIMONIAL:    return "Testimonial";
        case ClueType::DIGITAL:        return "Digital";
        case ClueType::CIRCUMSTANTIAL: return "Circumstantial";
        default:                       return "Unknown";
    }
}

class Clue {
private:
    int id;
    string description;
    string location;
    string timestamp;
    ClueType type;
    vector<string> linkedSuspects;
    bool discovered;
    bool verified;

public:
    Clue()
        : id(0), description(""), location(""), timestamp(""),
          type(ClueType::CIRCUMSTANTIAL), discovered(false), verified(false) {}

    Clue(int id, const string& description, const string& location,
         const string& timestamp, ClueType type)
        : id(id), description(description), location(location),
          timestamp(timestamp), type(type), discovered(false), verified(false) {}

    int getId() const { return id; }
    string getDescription() const { return description; }
    string getLocation() const { return location; }
    string getTimestamp() const { return timestamp; }
    ClueType getType() const { return type; }
    vector<string> getLinkedSuspects() const { return linkedSuspects; }
    bool isDiscovered() const { return discovered; }
    bool isVerified() const { return verified; }

    void discover() { discovered = true; }
    void verify() { verified = true; }

    void linkSuspect(const string& suspect) {
        for (const string& s : linkedSuspects) {
            if (s == suspect) return;
        }
        linkedSuspects.push_back(suspect);
    }

    void displayClue() const {
        cout << "  [Clue #" << id << "] "
             << (discovered ? "[DISCOVERED]" : "[UNDISCOVERED]")
             << (verified ? " [VERIFIED]" : "") << endl;
        cout << "    Type       : " << clueTypeToString(type) << endl;
        cout << "    Description: " << description << endl;
        cout << "    Location   : " << location << endl;
        cout << "    Time       : " << timestamp << endl;
        if (!linkedSuspects.empty()) {
            cout << "    Linked to  : ";
            for (size_t i = 0; i < linkedSuspects.size(); i++) {
                cout << linkedSuspects[i];
                if (i < linkedSuspects.size() - 1) cout << ", ";
            }
            cout << endl;
        }
    }
};

#endif