#!/bin/bash

for i in `seq 1 5`;
do
	./bin/tester "test/cases/r.txt" "test/cases/s.txt" 
done
