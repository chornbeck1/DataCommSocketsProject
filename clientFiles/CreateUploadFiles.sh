n=1
last=100
while [ $n -lt $last ]
do
 echo "$n" > "upload${n}.txt"
 n=$(($n+1))
done