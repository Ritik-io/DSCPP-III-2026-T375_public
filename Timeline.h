#ifndef TIMELINE_H
#define TIMELINE_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class CaseEvent {
private:
    int timeSlot;
    string description;
    string location;
    vector<string> involvedSuspects;
    bool confirmed;

public:
    CaseEvent()
        : timeSlot(0), description(""), location(""), confirmed(false) {}

    CaseEvent(int timeSlot, const string& description, const string& location)
        : timeSlot(timeSlot), description(description),
          location(location), confirmed(false) {}

    int getTimeSlot() const { return timeSlot; }
    string getDescription() const { return description; }
    string getLocation() const { return location; }
    vector<string> getInvolvedSuspects() const { return involvedSuspects; }
    bool isConfirmed() const { return confirmed; }

    void confirm() { confirmed = true; }
    void reject() { confirmed = false; }

    void addInvolvedSuspect(const string& suspect) {
        for (const string& s : involvedSuspects) {
            if (s == suspect) return;
        }
        involvedSuspects.push_back(suspect);
    }

    void displayEvent() const {
        cout << "  [T+" << timeSlot << "h] "
             << (confirmed ? "[CONFIRMED]" : "[UNCONFIRMED]") << endl;
        cout << "    Event    : " << description << endl;
        cout << "    Location : " << location << endl;
        if (!involvedSuspects.empty()) {
            cout << "    Involved : ";
            for (size_t i = 0; i < involvedSuspects.size(); i++) {
                cout << involvedSuspects[i];
                if (i < involvedSuspects.size() - 1) cout << ", ";
            }
            cout << endl;
        }
    }

    bool operator<(const CaseEvent& other) const {
        return timeSlot < other.timeSlot;
    }
};

class Timeline {
private:
    vector<CaseEvent> events;

public:
    void addEvent(const CaseEvent& event) {
        events.push_back(event);
        sort(events.begin(), events.end());
    }

    vector<CaseEvent> getEvents() const { return events; }

    void displayTimeline() const {
        if (events.empty()) {
            cout << "  (No events recorded yet)" << endl;
            return;
        }
        for (size_t i = 0; i < events.size(); i++) {
            cout << " ----------------------------------------" << endl;
            events[i].displayEvent();
        }
        cout << " ----------------------------------------" << endl;
    }

    void displayConfirmedTimeline() const {
        bool any = false;
        for (const CaseEvent& e : events) {
            if (e.isConfirmed()) {
                e.displayEvent();
                cout << endl;
                any = true;
            }
        }
        if (!any) cout << "  (No confirmed events)" << endl;
    }

    vector<CaseEvent> findGaps() const {
        vector<CaseEvent> gaps;
        if (events.size() < 2) return gaps;

        vector<CaseEvent> sorted = events;
        for (size_t i = 1; i < sorted.size(); i++) {
            int diff = sorted[i].getTimeSlot() - sorted[i - 1].getTimeSlot();
            if (diff > 4) {
                cout << "  [TIMELINE GAP] " << diff << " hours unexplained between "
                     << "T+" << sorted[i - 1].getTimeSlot() << "h and T+"
                     << sorted[i].getTimeSlot() << "h" << endl;
                gaps.push_back(sorted[i - 1]);
            }
        }
        return gaps;
    }
};

#endif