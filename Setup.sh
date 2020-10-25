echo "Generating Run File..."
rm -f Run.sh
rm -f Run.txt

n=1
last=100
while [ $n -lt $last ]
do
 echo "./TigerC <<< \042 tconnect 127.0.0.1 user pass
 tget down${n}.txt
 tput upload${n}.txt
 exit
\042

" >> Run.txt

 n=$(($n+1))

done






mv Run.txt Run.sh
chmod +x Run.sh
echo "Generating Data Files..."
./RemoveDataFiles.sh > /dev/null 2>&1
sleep 2s
./CreateDataFiles.sh > /dev/null 2>&1
echo "Making Code..."
make clean > /dev/null 2>&1
make all > /dev/null 2>&1
echo "Complete, closing..."
sleep 2s