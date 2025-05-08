@echo off
PATH=.;%PATH%
sh.exe -exec test/tester.exe 1> output 2> output2
