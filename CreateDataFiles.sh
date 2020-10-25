blockSize=1024
blockCount=1024

n=1
last=100
while [ $n -lt $last ]
do
 dd if=/dev/zero of="clientFiles/upload${n}.txt" bs=$blockSize count=$blockCount
 dd if=/dev/zero of="serverFiles/down${n}.txt" bs=$blockSize count=$blockCount
 n=$(($n+1))
done