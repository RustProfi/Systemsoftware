## What does the 2>&1 do in the given command? Why is it needed?
2 is the file descriptor stderr and 1 stdout. The & ensures that 1 is also a descriptor and not a file. > is to redirect the output from stderr to stdin.
Without it the output from less is messed up.
