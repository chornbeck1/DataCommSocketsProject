n=1
last=100
while [ $n -lt $last ]
do
 rm "clientFiles/upload${n}.txt"
 rm "serverFiles/upload${n}.txt"
 rm "clientFiles/down${n}.txt"
 rm "serverFiles/down${n}.txt"
 n=$(($n+1))
done