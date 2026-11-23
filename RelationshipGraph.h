#ifndef RELATIONSHIPGRAPH_H
#define RELATIONSHIPGRAPH_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

struct Relationship {
    string target;
    string nature;
    int strength;

    Relationship() : target(""), nature(""), strength(0) {}
    Relationship(const string& target, const string& nature, int strength)
        : target(target), nature(nature), strength(strength) {}
};

class RelationshipGraph {
private:
    unordered_map<string, vector<Relationship>> adjList;

public:
    void addPerson(const string& name) {
        if (adjList.find(name) == adjList.end()) {
            adjList[name] = vector<Relationship>();
        }
    }

    void addRelationship(const string& personA, const string& personB,
                         const string& nature, int strength) {
        addPerson(personA);
        addPerson(personB);
        adjList[personA].push_back(Relationship(personB, nature, strength));
        adjList[personB].push_back(Relationship(personA, nature, strength));
    }

    bool hasPerson(const string& name) const {
        return adjList.find(name) != adjList.end();
    }

    vector<Relationship> getRelationships(const string& person) const {
        auto it = adjList.find(person);
        if (it == adjList.end()) return {};
        return it->second;
    }

    bool hasDirectConnection(const string& a, const string& b) const {
        if (adjList.find(a) == adjList.end()) return false;
        for (const Relationship& rel : adjList.at(a)) {
            if (rel.target == b) return true;
        }
        return false;
    }

    void displayPerson(const string& person) const {
        if (adjList.find(person) == adjList.end()) {
            cout << "  No person named '" << person << "' in relationship network." << endl;
            return;
        }
        cout << "  Relationships of " << person << ":" << endl;
        for (const Relationship& rel : adjList.at(person)) {
            cout << "    -> " << rel.target
                 << "  (Nature: " << rel.nature
                 << ", Strength: " << rel.strength << ")" << endl;
        }
        if (adjList.at(person).empty()) {
            cout << "    (No known relationships)" << endl;
        }
    }

    vector<string> bfsShortestPath(const string& start, const string& target) const {
        vector<string> empty;
        if (adjList.find(start) == adjList.end() ||
            adjList.find(target) == adjList.end()) {
            return empty;
        }
        if (start == target) return {start};

        unordered_map<string, bool> visited;
        unordered_map<string, string> parent;
        queue<string> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            string current = q.front();
            q.pop();

            for (const Relationship& rel : adjList.at(current)) {
                if (!visited[rel.target]) {
                    visited[rel.target] = true;
                    parent[rel.target] = current;
                    q.push(rel.target);

                    if (rel.target == target) {
                        vector<string> path;
                        string node = target;
                        while (!node.empty()) {
                            path.push_back(node);
                            node = parent[node];
                        }
                        reverse(path.begin(), path.end());
                        return path;
                    }
                }
            }
        }
        return empty;
    }

    unordered_set<string> findConspirators(const string& central) const {
        unordered_set<string> conspirators;
        vector<string> suspects;
        suspects.push_back(central);

        for (const auto& pair : adjList) {
            if (hasDirectConnection(central, pair.first)) {
                suspects.push_back(pair.first);
            }
        }

        unordered_map<string, unordered_set<string>> clusters;
        for (size_t i = 0; i < suspects.size(); i++) {
            for (size_t j = i + 1; j < suspects.size(); j++) {
                if (hasDirectConnection(suspects[i], suspects[j])) {
                    clusters[suspects[i]].insert(suspects[j]);
                    clusters[suspects[j]].insert(suspects[i]);
                }
            }
        }

        for (const string& person : suspects) {
            if (clusters[person].size() >= 2) {
                conspirators.insert(person);
            }
        }
        return conspirators;
    }

    void displayNetwork() const {
        cout << "  Relationship Network:" << endl;
        cout << "  " << string(50, '-') << endl;
        for (const auto& pair : adjList) {
            cout << "  " << pair.first << " (" << pair.second.size()
                 << " connections)" << endl;
        }
        cout << "  " << string(50, '-') << endl;
    }
};

#endif