Implementation of redis server. Only a subset of commands are supported now.
List of supported commands are
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
1. Variation of avl-tree, doubly-linked-list, skip-list all implemented in the same node will host the data for the commands ZADD, ZRANGE, ZCARD, ZCOUNT.
2. BitArray is used to host the data for SET,GET,SETBIT, GETBIT