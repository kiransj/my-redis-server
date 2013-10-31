count=0
while [ $count -le 132140 ]
do
    count=$[$count+1]
    echo zadd myset:$count "$count"
done

#count=1
#while [ $count -le 112345 ]
#do
#    echo set myset:$count "$count"
#    count=$[$count+1]
#done
