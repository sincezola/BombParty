rm ./bin/bombparty
./scripts/mkall_linux.sh
ls ./bin/bombparty
RSL=$?
if [ $RSL -eq 0 ]; then
./bin/bombparty
fi