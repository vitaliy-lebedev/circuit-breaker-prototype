 # circuit-breaker-prototype

## Problem:
When CRE receives any event, it is trying to lock account id and parent account id in IncoreDatabase. If some other process already locked parent id, it could lead to some degradation of performance, because whole process will wait for 1 second (default timeout) for each sub account request.

## How to solve:
We can't just decrease the timeout for all requests, because it could lead to timeout errors in the highload peaks. But, we can identify accounts that are locked, and decrease timeout for them, or simply reject them during some small time period.

## How it is implemented:
When CRE receives any event that will lock account record in database it will check if there are were timeout errors before. If there were no timeout error last N seconds, it will try to lock current account id. Else if some errors exists for this account, and the number of errors is greater than some threshold, CRE starting rejected all requests for this account. After M seconds, CRE will give another chance to this account.

## Source Code:
``CircuitBreaker circuit_breaker(3, 5000, 10000)``
^ In this case CRE will check if there were 3 or more errors in last 5 seconds, and reject all requests to this account in next 10 seconds (these values could be configured).

If you will run circuit_breaker.exe file in this repo, you will see output like this:
``First timeout error occurs.`` 
``cre will give second chance to this account`` 
``cre will give third chance to this account`` 
``Account 101342357 is locked for next 10 seconds`` 
``Account 101342357 is locked for next 5 seconds``
``hey! Account 101342357 is unblocked!``

I suggest you to compare this output with main.cc file to understand how it's working.


