#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <algorithm>
#include <cctype>
#include <sstream>
using namespace std;

// ---------- Entities (OOP) ----------
class Suspect {
public:
    string name, alibi, motive;
    int suspicion = 0;
    bool guilty;
    Suspect(string n, string a, string m, bool g = false)
        : name(n), alibi(a), motive(m), guilty(g) {}
    void show(bool showScore) const {
        cout << "  -----------------------------------------\n"
             << "  SUSPECT: " << name << "\n"
             << "  Alibi     : " << alibi << "\n"
             << "  Motive    : " << motive << "\n";
        if (showScore)
            cout << "  Lead score: [" << string(suspicion, '#')
                 << string(10 - suspicion, '-') << "] " << suspicion << "/10\n";
    }
};

class Clue {
public:
    int id; string desc, place, linked;
    Clue(int i, string d, string p, string l) : id(i), desc(d), place(p), linked(l) {}
    void show() const {
        cout << "  [Clue #" << id << "] " << desc << "\n"
             << "    Location : " << place << "\n"
             << "    Linked to: " << linked << "\n\n";
    }
};

class Evidence {
public:
    int id, strength; string desc, against; bool physical;
    Evidence(int i, string d, string a, int s, bool p)
        : id(i), strength(s), desc(d), against(a), physical(p) {}
    bool operator<(const Evidence& o) const {
        return strength == o.strength ? id > o.id : strength < o.strength;
    }
};

struct Event { int hour; string desc; };

// ---------- Investigation engine (DSA) ----------
class Investigation {
    int level, decisiveEvidence;
    string title, difficulty, briefing, hint, solution;
    vector<Suspect> suspects;                       // vector
    vector<Clue> clues;
    priority_queue<Evidence> evidence;              // priority queue (strongest first)
    vector<Event> timeline;                         // sorted vector
    unordered_map<string, int> index;               // hash map name -> index
    unordered_map<string, vector<string>> graph;    // graph (adjacency list)
    stack<string> history;                          // stack (history)
    queue<string> tasks;                            // queue (pending tasks)

    string lower(string s) {
        for (char& c : s) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        return s;
    }

public:
    Investigation(int l, string t, string d, string b, string h, string s, int e)
        : level(l), decisiveEvidence(e), title(t), difficulty(d), briefing(b),
          hint(h), solution(s) {}
    void showLevel() const {
        cout << "\n  LEVEL " << level << "/3 - " << difficulty << "\n"
             << "  " << title << "\n"
             << "  Solved: " << level - 1 << "/3\n";
        if (level >= 3)
            cout << "  To solve: name the culprit AND cite the decisive evidence ID.\n";
    }
    void showCase() {
        cout << "\n=============================================\n"
             << "  CASETRACE - CASE FILE\n"
             << "=============================================\n"
             << "  Title  : " << title << "\n"
             << "  Brief  : " << briefing << "\n"
             << "  Suspects: " << suspects.size() << " | Clues: " << clues.size()
             << " | Evidence: " << evidence.size() << "\n";
        showTimeline();
        showHistory();
    }
    void addSuspect(Suspect s) {
        index[lower(s.name)] = suspects.size();
        suspects.push_back(s);
        history.push("Added suspect " + s.name);
    }
    Suspect* find(const string& n) {
        auto it = index.find(lower(n));
        return it == index.end() ? nullptr : &suspects[it->second];
    }
    void addClue(string d, string p, string linked) {
        clues.push_back(Clue(clues.size() + 1, d, p, linked));
        if (Suspect* s = find(linked)) s->suspicion = min(10, s->suspicion + 2);
        tasks.push("Verify clue " + to_string(clues.size()));
        history.push("Added clue: " + d);
    }
    void addEvidence(string d, string against, int strength, bool physical) {
        evidence.push(Evidence(evidence.size() + 1, d, against, strength, physical));
        if (Suspect* s = find(against)) s->suspicion = min(10, s->suspicion + strength / 3);
        history.push("Added evidence against " + against);
    }
    void addEvent(int h, string d) {
        timeline.push_back({h, d});
        sort(timeline.begin(), timeline.end(),
             [](const Event& a, const Event& b) { return a.hour < b.hour; });
    }
    void relate(string a, string b) { graph[a].push_back(b); graph[b].push_back(a); }

    void showSuspects() {
        vector<Suspect> v = suspects;
        if (level <= 2)
            stable_sort(v.begin(), v.end(), [](const Suspect& a, const Suspect& b) {
                return a.suspicion > b.suspicion; });
        cout << "\nSUSPECTS:\n";
        cout << (level <= 2 ? "  Lead scores are hints, not proof of guilt.\n"
                           : "  No automatic ranking: compare statements with the evidence.\n");
        for (auto& s : v) s.show(level <= 2);
    }
    void showClues() {
        cout << "\nCLUES:\n";
        for (auto& c : clues) c.show();
    }
    void showEvidence() {
        cout << "\nEVIDENCE (strongest first):\n";
        priority_queue<Evidence> pq = evidence;
        while (!pq.empty()) {
            Evidence e = pq.top(); pq.pop();
            cout << "  [Evidence #" << e.id << "] " << e.desc << "\n"
                 << "    Related to: " << e.against << "\n"
                 << "    Type      : " << (e.physical ? "Physical" : "Record / testimony") << "\n"
                 << "    Reliability: " << e.strength << "/10 (not a guilt score)\n\n";
        }
    }
    void showTimeline() {
        cout << "\nTIMELINE:\n";
        for (auto& e : timeline)
            cout << "  " << (e.hour % 24 < 10 ? "0" : "") << e.hour % 24
                 << ":00" << (e.hour >= 24 ? " (next day)" : "") << "  " << e.desc << "\n";
    }
    // BFS shortest path in relationship graph
    void path(string a, string b) {
        Suspect* from = find(a);
        Suspect* to = find(b);
        if (!from || !to) { cout << "  Unknown suspect.\n"; return; }
        a = from->name;
        b = to->name;
        unordered_map<string, string> parent; queue<string> q; q.push(a); parent[a] = "";
        while (!q.empty()) {
            string cur = q.front(); q.pop();
            if (cur == b) break;
            for (auto& nb : graph[cur])
                if (!parent.count(nb)) { parent[nb] = cur; q.push(nb); }
        }
        if (!parent.count(b)) { cout << "  No connection.\n"; return; }
        vector<string> p;
        for (string n = b; n != ""; n = parent[n]) p.push_back(n);
        reverse(p.begin(), p.end());
        cout << "  Path: ";
        for (size_t i = 0; i < p.size(); i++) cout << p[i] << (i + 1 < p.size() ? " -> " : "\n");
    }
    void analyze() {
        // A physical trace alone does not contradict an alibi; hints use case facts.
        cout << "\nCASE ANALYSIS:\n  " << hint << "\n"
             << "  A connection or fingerprint alone does not prove guilt.\n"
             << "\nCLUE REVIEW CHECKLIST:\n";
        queue<string> review = tasks;
        while (!review.empty()) { cout << "  - " << review.front() << "\n"; review.pop(); }
        history.push("Reviewed case analysis");
    }
    void showHistory() {
        cout << "\nHISTORY (latest first):\n";
        stack<string> h = history;
        while (!h.empty()) { cout << "  " << h.top() << "\n"; h.pop(); }
    }
    bool accuse(string n, int evidenceId) {
        Suspect* s = find(n);
        if (!s) { cout << "  Unknown suspect.\n"; return false; }
        cout << "\n=============================================\n"
             << "  FINAL CONCLUSION\n"
             << "=============================================\n"
             << "  You accuse: " << s->name << "\n";
        history.push("Accused " + s->name);
        if (!s->guilty || (level >= 3 && evidenceId != decisiveEvidence)) {
            cout << "  Not solved. Review your suspect"
                 << (level >= 3 ? " and supporting evidence" : "")
                 << ". This level stays open.\n";
            return false;
        }
        cout << "  CORRECT! Level " << level << " solved.\n"
             << "  Explanation: " << solution << "\n";
        return true;
    }
};

// Each level gets a fresh investigation, so evidence IDs and history never leak across cases.
Investigation load(int level) {
    if (level == 1) {
        Investigation inv(1, "The Missing Medal", "Easy",
            "A medal vanished from the school display at 10:00. Only Ava and Ben had access.",
            "Compare Ava's claim with the clear camera recording. Ben's alibi is independently confirmed.",
            "The camera shows Ava taking the medal while Ben was in class.", 1);
        inv.addSuspect(Suspect("Ava", "Never entered the display room", "Wanted the medal", true));
        inv.addSuspect(Suspect("Ben", "In class with the teacher", "Lost the competition"));
        inv.relate("Ava", "Ben");
        inv.addEvent(9, "Teacher checks the medal in the display");
        inv.addEvent(10, "Camera records the theft");
        inv.addEvent(11, "Empty display reported");
        inv.addClue("Ava's scarf beside the open display", "Display room", "Ava");
        inv.addEvidence("Clear camera recording shows Ava pocketing the medal at 10:00", "Ava", 10, false);
        inv.addEvidence("Class video and teacher place Ben in class throughout the theft", "Ben", 9, false);
        return inv;
    }
    if (level == 2) {
        Investigation inv(2, "The Gallery Switch", "Medium",
            "A painting was swapped at 18:00. The gallery has one exit, continuously filmed.",
            "A fingerprint may be old. Check who carried the only package out after the swap.",
            "Noah carried out the stolen painting. Mira's prints came from an earlier authorized inspection.", 3);
        inv.addSuspect(Suspect("Mira", "Inspected the frame at 16:00; left at 17:00", "Insurance dispute"));
        inv.addSuspect(Suspect("Noah", "Left at 18:00 with an empty delivery tube", "Unpaid delivery fees", true));
        inv.addSuspect(Suspect("Owen", "On camera at reception from 17:00 to 19:00", "Wanted a promotion"));
        inv.relate("Mira", "Noah"); inv.relate("Noah", "Owen");
        inv.addEvent(16, "Mira performs an authorized frame inspection");
        inv.addEvent(17, "Camera shows Mira leave; painting still present");
        inv.addEvent(18, "Painting swapped; Noah leaves with the only outgoing package");
        inv.addEvent(19, "Police seal Noah's tube without a break in camera coverage");
        inv.addClue("Mira's fingerprints on the frame", "Gallery", "Mira");
        inv.addClue("Insurance complaint signed by Mira", "Office", "Mira");
        inv.addClue("Heavy delivery tube", "Exit", "Noah");
        inv.addEvidence("Mira's fingerprints match the frame she inspected at 16:00", "Mira", 10, true);
        inv.addEvidence("Continuous reception video confirms Owen's alibi", "Owen", 9, false);
        inv.addEvidence("Sealed tube carried by Noah contains the original painting, not packing material", "Noah", 9, true);
        return inv;
    }
    Investigation inv(3, "The Borrowed Badge", "Hard",
        "A laboratory prototype vanished at 23:00. A badge identifies its owner, not necessarily its user.",
        "Trace the borrowed badge through the relationships, then compare the carrier's hand with the exit image.",
        "Rhea borrowed Kian's badge via Sol. Her documented left-thumb cast matches the filmed thief; Kian was at the clinic.", 4);
    inv.addSuspect(Suspect("Kian", "At the clinic from 22:00 to midnight", "Denied a bonus"));
    inv.addSuspect(Suspect("Sol", "At the security desk throughout the theft", "Owed money"));
    inv.addSuspect(Suspect("Tess", "At a recorded conference from 22:00 to midnight", "Rival research"));
    inv.addSuspect(Suspect("Rhea", "At home all evening; left thumb in a rigid cast", "Wanted to sell the design", true));
    inv.addSuspect(Suspect("Uma", "Working in the monitored server room", "Promotion dispute"));
    inv.relate("Kian", "Sol"); inv.relate("Sol", "Rhea");
    inv.relate("Sol", "Tess"); inv.relate("Tess", "Uma");
    inv.addEvent(21, "Sol signs out Kian's spare badge for Rhea's scheduled visit");
    inv.addEvent(22, "Kian checks into the clinic; all five suspects' hands documented earlier");
    inv.addEvent(23, "Kian's badge opens the lab; a masked person carries the prototype out");
    inv.addClue("Signed badge handover: Kian -> Sol -> Rhea; no return before midnight", "Security desk", "Badge chain");
    inv.addClue("Medical record and arrival images: only Rhea has a rigid left-thumb cast", "Visitor records", "Hand identification");
    inv.addEvidence("Access log records Kian's badge opening the lab", "Kian's badge", 10, false);
    inv.addEvidence("Clinic footage continuously shows Kian from 22:00 to midnight", "Kian", 10, false);
    inv.addEvidence("Independent recordings confirm Sol, Tess and Uma's alibis", "Sol / Tess / Uma", 9, false);
    inv.addEvidence("Exit video shows the prototype carrier with a rigid left-thumb cast", "Unidentified carrier", 9, false);
    return inv;
}

int main() {
    int level = 1;
    Investigation inv = load(level);
    cout << "\n=============================================\n"
         << "                 CASETRACE\n"
         << "       Crime Investigation & Mystery\n"
         << "=============================================\n"
         << "  Welcome, Detective.\n"
         << "  Solve three cases in order: Easy, Medium, Hard.\n"
         << "  Wrong answers keep the current case open.\n"
         << "  Progress is kept for this session only.\n";
    inv.showLevel();
    inv.showCase();
    string input, a, b;
    while (true) {
        inv.showLevel();
        cout << "\n------------- INVESTIGATION MENU -------------\n"
             << "  1. Case file (timeline & history)\n"
             << "  2. Examine suspects\n"
             << "  3. View clues & evidence\n"
             << "  4. Find a connection\n"
             << "  5. Analyze the case\n"
             << "  6. REACH FINAL CONCLUSION\n"
             << "  0. Exit case\n"
             << "---------------------------------------------\n"
             << "  Choice: ";
        if (!getline(cin, input)) break;
        istringstream selection(input);
        int c;
        char extra;
        if (!(selection >> c) || (selection >> extra) || c < 0 || c > 6) {
            cout << "  Enter a number from 0 to 6.\n";
            continue;
        }
        bool finished = false;
        switch (c) {
            case 1: inv.showCase(); break;
            case 2: inv.showSuspects(); break;
            case 3: inv.showClues(); inv.showEvidence(); break;
            case 4:
                cout << "  From (suspect name): ";
                if (!getline(cin, a)) return 0;
                cout << "  To (suspect name): ";
                if (!getline(cin, b)) return 0;
                inv.path(a, b); break;
            case 5: inv.analyze(); break;
            case 6:
                cout << "  Who do you accuse? ";
                if (!getline(cin, a)) return 0;
                {
                    int evidenceId = 0;
                    if (level >= 3) {
                        cout << "  Decisive evidence ID (number from option 3): ";
                        if (!getline(cin, input)) return 0;
                        istringstream answer(input);
                        if (!(answer >> evidenceId) || (answer >> extra) || evidenceId <= 0) {
                            cout << "  Enter a positive evidence number. Case stays open.\n";
                            break;
                        }
                    }
                    finished = inv.accuse(a, evidenceId);
                }
                break;
            case 0: cout << "\n  Case file closed. Goodbye, Detective.\n"; return 0;
        }
        if (finished && level == 3) {
            cout << "\n=============================================\n"
                 << "  ALL 3 LEVELS COMPLETE - MASTER DETECTIVE!\n"
                 << "=============================================\n"
                 << "  Press Enter to exit...";
            getline(cin, input);
            break;
        }
        cout << "\n  Press Enter to " << (finished ? "unlock the next level..." : "continue...");
        if (!getline(cin, input)) break;
        if (finished) {
            inv = load(++level);
            inv.showLevel();
            inv.showCase();
        }
    }
}
