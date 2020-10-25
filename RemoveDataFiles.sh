n=1
last=100
while [ $n -lt $last ]
do
 rm -f "clientFiles/upload${n}.txt"
 rm -f "serverFiles/upload${n}.txt"
 rm -f "clientFiles/down${n}.txt"
 rm -f "serverFiles/down${n}.txt"
 n=$(($n+1))
done