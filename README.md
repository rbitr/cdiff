# Mileena
Automatic differentiation experiments in C

Still very basic as I get the different parts figured out. Please get in touch if this looks interesting to you. (I'm Andrew Marble)

The goal is jax in C with no significant dependencies (probably BLAS). I have a roadmap in my head, though right now I'm just setting it up and thinking about how I'll do it. Basic parts are the jax / autodiff style tracing and pullback framework, linear algebra, and the vector-jacobian products.

`mileena.c` is a POC for tracing function execition the way autograd (https://github.com/HIPS/autograd) does

`symbolic.c` does basic symbolic differentation, without handling edge cases (see tests for example)


it compiles with clang with default arguments and I think should work anywhere

`ex2.c` trains a linear binary classifier. See `/data` for the data. Compile with 

```
clang ex2.c -I/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/Accelerate.framework/Versions/Current/Frameworks/vecLib.framework/Headers/ -framework Accelerate -o ex2
``` 

but some of the paths may need updating.


`include/mil_structures.h` is work on the matrix data structures and linear algebra
`tests/matrix_test.c` is tests for`the data structures.

`blas/layers2.ipynb` is a python example of a first simple AD program to be ported to C

more to come...

