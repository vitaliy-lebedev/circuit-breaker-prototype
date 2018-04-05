#include "circuit_breaker.h"
#include <list>
#include <map>
#include <cstddef>

static std::list<std::pair<int, int> > incidents_time;
static std::map<int, int> accounts_to_skip;

long int CircuitBreaker::GetCurrentTimestamp() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000; 
    return ms;
}

void CircuitBreaker::AddIncident(int account_id) {
    ClearSkips();
    ClearIncidents();

    incidents_time.push_back(std::make_pair(account_id, GetCurrentTimestamp()));
    if (GetIncidentsAmount(account_id) >= _incident_threshold) {
        AddAccountToSkips(account_id);
    }
}

bool CircuitBreaker::AllowAccountId(int account_id) {
    ClearSkips();
    ClearIncidents();

    std::map<int, int>::iterator it;
    for (it = accounts_to_skip.begin(); it != accounts_to_skip.end(); it++) {
        if (it->first == account_id) {
            return false;
        }
    }
    return true;
}

void CircuitBreaker::ClearIncidents() {
    long int current_time = GetCurrentTimestamp();
    std::list<std::pair<int, int> >::iterator it = incidents_time.begin();
    while (it != incidents_time.end()) {
        if (current_time - it->second > _incident_timeout) {
            it = incidents_time.erase(it);
        } else {
            it++;
        }
    }
}

int CircuitBreaker::GetIncidentsAmount(int account_id) {
    int incidents_amount = 0;
    std::list<std::pair<int, int> >::iterator it;
    for (it = incidents_time.begin(); it != incidents_time.end(); it++) {
        if (it->first == account_id) {
            incidents_amount++;
        }
    }

    return incidents_amount;
}

void CircuitBreaker::ClearSkips() {
    long int current_time = GetCurrentTimestamp();
    std::map<int, int>::iterator it = accounts_to_skip.begin();
     while (it != accounts_to_skip.end()) {
        if (current_time - it->second > _skip_timeout) {
            accounts_to_skip.erase(it++);
        } else {
            it++;
        }
    }
}

void CircuitBreaker::AddAccountToSkips(int account_id) {
    accounts_to_skip[account_id] = GetCurrentTimestamp();
}