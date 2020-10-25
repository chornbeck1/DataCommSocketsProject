n=1
last=100
while [ $n -lt $last ]
do
 rm "upload${n}.txt"
 n=$(($n+1))
done