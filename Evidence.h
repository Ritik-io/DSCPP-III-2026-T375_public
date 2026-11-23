#ifndef EVIDENCE_H
#define EVIDENCE_H

#include <string>
#include <iostream>

using namespace std;

enum class EvidenceType {
    FINGERPRINT,
    DNA,
    WEAPON,
    FORENSIC,
    DOCUMENT,
    WITNESS
};

string evidenceTypeToString(EvidenceType type) {
    switch (type) {
        case EvidenceType::FINGERPRINT: return "Fingerprint";
        case EvidenceType::DNA:         return "DNA";
        case EvidenceType::WEAPON:      return "Weapon";
        case EvidenceType::FORENSIC:    return "Forensic";
        case EvidenceType::DOCUMENT:    return "Document";
        case EvidenceType::WITNESS:     return "Witness";
        default:                        return "Unknown";
    }
}

class Evidence {
private:
    int id;
    string description;
    string evidenceSuspect;
    int strength;
    EvidenceType type;
    bool admissible;

public:
    Evidence()
        : id(0), description(""), evidenceSuspect(""),
          strength(0), type(EvidenceType::FORENSIC), admissible(false) {}

    Evidence(int id, const string& description, const string& evidenceSuspect,
             int strength, EvidenceType type)
        : id(id), description(description), evidenceSuspect(evidenceSuspect),
          strength(max(1, min(10, strength))), type(type),
          admissible(false) {}

    int getId() const { return id; }
    string getDescription() const { return description; }
    string getSuspect() const { return evidenceSuspect; }
    int getStrength() const { return strength; }
    EvidenceType getType() const { return type; }
    bool isAdmissible() const { return admissible; }

    void setStrength(int s) { strength = max(1, min(10, s)); }
    void makeAdmissible() { admissible = true; }

    void displayEvidence() const {
        cout << "  [Evidence #" << id << "] "
             << (admissible ? "[ADMISSIBLE]" : "[NOT ADMISSIBLE]") << endl;
        cout << "    Type        : " << evidenceTypeToString(type) << endl;
        cout << "    Description : " << description << endl;
        cout << "    Implicates  : " << evidenceSuspect << endl;
        cout << "    Strength    : " << strength << "/10" << endl;
    }
};

#endif