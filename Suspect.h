#ifndef SUSPECT_H
#define SUSPECT_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class Suspect {
private:
    string name;
    string description;
    string alibi;
    string motive;
    string means;
    int suspicionLevel;
    bool interrogated;
    bool isGuilty;
    vector<string> knownAssociates;

public:
    Suspect()
        : name(""), description(""), alibi(""), motive(""), means(""),
          suspicionLevel(0), interrogated(false), isGuilty(false) {}

    Suspect(const string& name, const string& description,
            const string& alibi, const string& motive,
            const string& means, bool isGuilty = false)
        : name(name), description(description), alibi(alibi),
          motive(motive), means(means), suspicionLevel(0),
          interrogated(false), isGuilty(isGuilty) {}

    string getName() const { return name; }
    string getDescription() const { return description; }
    string getAlibi() const { return alibi; }
    string getMotive() const { return motive; }
    string getMeans() const { return means; }
    int getSuspicionLevel() const { return suspicionLevel; }
    bool isInterrogated() const { return interrogated; }
    bool getIsGuilty() const { return isGuilty; }
    vector<string> getKnownAssociates() const { return knownAssociates; }

    void setAlibi(const string& a) { alibi = a; }
    void setMotive(const string& m) { motive = m; }
    void setMeans(const string& ms) { means = ms; }
    void setSuspicionLevel(int level) { suspicionLevel = max(0, min(10, level)); }
    void setGuilty(bool g) { isGuilty = g; }
    void interrogate() { interrogated = true; }

    void addSuspicion(int amount) {
        suspicionLevel = max(0, min(10, suspicionLevel + amount));
    }

    void addAssociate(const string& associate) {
        if (find(knownAssociates.begin(), knownAssociates.end(), associate)
            == knownAssociates.end()) {
            knownAssociates.push_back(associate);
        }
    }

    void displayInfo() const {
        cout << "========================================" << endl;
        cout << "  SUSPECT: " << name << endl;
        cout << "========================================" << endl;
        cout << "  Description : " << description << endl;
        cout << "  Alibi       : " << alibi << endl;
        cout << "  Motive      : " << motive << endl;
        cout << "  Means       : " << means << endl;
        cout << "  Suspicion   : [";
        for (int i = 0; i < 10; i++) {
            cout << (i < suspicionLevel ? "#" : "-");
        }
        cout << "] " << suspicionLevel << "/10" << endl;
        cout << "  Interrogated: " << (interrogated ? "Yes" : "No") << endl;
        if (!knownAssociates.empty()) {
            cout << "  Associates  : ";
            for (size_t i = 0; i < knownAssociates.size(); i++) {
                cout << knownAssociates[i];
                if (i < knownAssociates.size() - 1) cout << ", ";
            }
            cout << endl;
        }
        cout << "========================================" << endl;
    }

    bool operator<(const Suspect& other) const {
        return suspicionLevel > other.suspicionLevel;
    }
};

#endif