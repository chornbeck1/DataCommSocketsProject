n=1
last=100
while [ $n -lt $last ]
do
 rm "down${n}.txt"
 n=$(($n+1))
done