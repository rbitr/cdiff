# cdiff
Automatic differentiation experiments in C

The goal is jax in C with no significant dependencies (probably BLAS). I have a roadmap in my head, though right now I'm just setting it up and thinking about how I'll do it.

`symbolic.c` does basic symbolic differentation, without handling edge cases (see tests for example)

it compiles with clang with default arguments and I think should work anywhere

more to come...

