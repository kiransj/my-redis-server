##count=0
##while [ $count -le 13214 ]
##do
##    count=$[$count+1]
##    echo zadd myset:$count "$count"
##done
##
count=1
while [ $count -le 10123 ]
do
    echo set mykey:$count "$count"
    count=$[$count+1]
done

#count=1
#while [ $count -le 10123 ]
#do
#    echo get mykey:$count
#    count=$[$count+1]
#done
