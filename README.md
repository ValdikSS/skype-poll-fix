Skype poll fix
==============
This library is made to reduce Skype CPU consumption by increasing poll() call timeouts. It lowers Skype idle CPU load from 4.4% down to 0.9% and improves battery life on my laptop, while making Skype only a bit slower.

How to use
----------
* Compile the source code with make
* Run Skype with LD_PRELOAD command: ```LD_PRELOAD=/path/to/skype-poll-fix.so skype```

Configuration
-------------
You can configure SET_POLL and MIN_POLL environmental variable. SET_POLL is a poll() timeout value which would be set if Skype tries to call poll() with timeout less than MIN_POLL. Both variables are default to 300 ms.

#####Example
```LD_PRELOAD=/path/to/skype-poll-fix.so MIN_POLL=100 SET_POLL=250 skype```

All poll() timeouts less than 100 ms would be set to 250 ms.

Based on [Valery Yundin](http://habrahabr.ru/users/Vayun/) idea.
