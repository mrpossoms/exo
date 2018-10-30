#!/usr/bin/python

import os
import subprocess

def getTestList():
	tests = []
	for file in os.listdir("./tests/scripts"):
            if file.endswith('.sh'):
	        tests.append(os.path.join("./tests/scripts", file))
	return tests

testNames = getTestList()
testsPassed = 0
testsRan = 0

def runTest(file):
        args = [file, str(testsRan), str(len(testNames))]
	return subprocess.call(args)

print("\n")

for file in testNames:
	errored = runTest(file)

	if not errored:
		testsPassed += 1

	testsRan += 1

status = "\033[0;32m"

# check o see if any failed
if testsPassed != len(testNames):
	status = "\033[1;31m"

print(str(testsRan) + " of " + str(len(testNames)) + " ran")
print(status + str(testsPassed) + " of " + str(len(testNames)) + " passed\033[0m")
