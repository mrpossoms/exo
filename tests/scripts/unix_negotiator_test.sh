#!/bin/bash

for i in $(seq 1); do
    ./tests/bin/negotiator_test --sender | ./tests/bin/negotiator_test
done
