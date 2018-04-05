#ifndef CIRCUIT_BREAKER_H
#define CIRCUIT_BREAKER_H

#include <sys/time.h>

class CircuitBreaker        
{
public:
    CircuitBreaker(int threshold, int timeout, int skip) {
       _incident_threshold = threshold;
       _incident_timeout = timeout;
       _skip_timeout = skip;
    }
    void AddIncident(int account_id);
    bool AllowAccountId(int account_id);

private:
    int _incident_threshold;
    int _incident_timeout;
    int _skip_timeout;

    long int GetCurrentTimestamp();
    void ClearIncidents();
    void ClearSkips();    
    int GetIncidentsAmount(int account_id);
    void AddAccountToSkips(int account_id);    
};


#endif