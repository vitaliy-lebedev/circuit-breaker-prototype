#include "circuit_breaker.h"
#include <iostream>
#include <unistd.h>

int main() {
    CircuitBreaker circuit_breaker(3, 5000, 10000); //if we have 3 errors in 5 seconds, next 10 seconds we will skip this id.
    
    int evil_account = 101342357;

    //We have new timeout for account id 101342357
    std::cout << "First timeout error occurs." << std::endl;
    circuit_breaker.AddIncident(evil_account);

    //Check if account is not locked
    if (circuit_breaker.AllowAccountId(evil_account)) {
        std::cout << "We are trying to lock account and get timeout." << std::endl;
        //Add one more timeout incident
         circuit_breaker.AddIncident(evil_account);
    }
    
     //Check if account is still not locked
     if (circuit_breaker.AllowAccountId(evil_account)) {
        std::cout << "We are trying to lock account and get timeout." << std::endl;
        //Add one more timeout incident
         circuit_breaker.AddIncident(evil_account);
    }

    //Now we will have "false"
     if (circuit_breaker.AllowAccountId(evil_account)) {
          std::cout << "you can't be here." << std::endl;
     } else {
         std::cout << "Account " << evil_account << " is locked for next 10 seconds" << std::endl;
     }

    //Let's wait for 5 second
     sleep(5);

     //This account is still unavailable, because skipping time is 10 seconds
     if (circuit_breaker.AllowAccountId(evil_account)) {
          std::cout << "you can't be here.";
     } else {
         std::cout << "Account " << evil_account << " is locked for next 5 seconds" << std::endl;
     }

    //Let's wait for 6 more second
     sleep(6);

    //account was unlocked because 10 seconds finished.
    if (circuit_breaker.AllowAccountId(evil_account)) {
          std::cout << "hey! Account " << evil_account << " is unblocked! " << std::endl;
    }

     std::cout << "Press any key to exit . . ." << std::endl;
}