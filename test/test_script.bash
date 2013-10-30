count=1
while [ $count -le 32140 ]
do
    r=$RANDOM
    count=$[$count+1]
    echo zadd myset $r "$r"
done
echo zcard myset
