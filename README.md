[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-c66648af7eb3fe8bc4f294546bfd86ef473780cde1dea487d3c4ff354943c9ae.svg)](https://classroom.github.com/online_ide?assignment_repo_id=9314938&assignment_repo_type=AssignmentRepo)
# Burrows-Wheeler transform

The tasks for this exercise are quite simple. Write a function that will do the Burrows-Wheeler transform on a string, and another function that will reverse it. The simplicity is deceptive, though, because quite a bit of work needs to be done in both of them. You know what the work is, however, and so you should be able to do it.

The exact interfaces to the functions depend on the programming language, but they have the rough form:

```
    bwt(x) -> the BWT of x
    rbwt(y) -> x if y is the BWT of x
```

Both functions should run in `O(len(x))`. If you do not have a linear-time suffix array construction implementation, however, it is okay if the complexity is bounded by the time it takes to build a suffix array with the code you have. The rest, however, should run in linear time.

The `x` strings in the tests will contain a sentinel, the zero-byte, so you do not need to add it yourself (if you were thinking of doing that).

