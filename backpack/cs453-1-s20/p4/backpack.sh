#!/bin/bash
# Tests
./booga_load
cat /proc/driver/booga

./test-booga 0 100 read
./test-booga 1 100 read
./test-booga 2 100 read
./test-booga 3 100 read
./test-booga 0 200 read
./test-booga 1 200 read
./test-booga 2 200 read
./test-booga 3 200 read
./test-booga 0 100 write
./test-booga 1 100 write
./test-booga 2 100 write
./test-booga 3 100 write
./test-booga 0 1000 write
./test-booga 1 1000 write
./test-booga 2 1000 write
./test-booga 3 1000 write

cat /proc/driver/booga
./booga_unload