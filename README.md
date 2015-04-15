A sample implementation of redis server which supports more than 100 concurrent clients using epoll.

Only a subset of commands are supported now. List of supported commands are 
1. ZADD 
2. ZRANGE 
3. ZCARD 
4. ZCOUNT 
5. SET 
6. GET 
7. SETBIT 
8. GETBIT 
9. Save

Data structure used are 
1. Variation of avl-tree
2. Doubly-linked-lis
3. Skip-list.
4. BitArray

All the above are implemented in the same node will host the data 
for the commands ZADD, ZRANGE, ZCARD, ZCOUNT. BitArray is used to 
host the data for SET,GET,SETBIT, GETBIT 

