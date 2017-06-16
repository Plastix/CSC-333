# BruteHull

A simple brute force algorithm for finding the convex hull of a set of points in the cartesian plane. Parallelized with OpenMP.

### Compiling and Running:
To compile:
```
> make main
```

To run:
```
> ./main 1000 1000 100 2 hull.ppm
```
This will generate 100 random points within a circle, calculate the convex hull of them using 2 threads then render them on a 1000 x 1000 color PPM image called `hull.ppm`.


### Resources:

 * [PPM-lib](https://github.com/Cheekyshroom/PPM-lib) for rendering color PPM images
 * [Algorithms and Data Structures Fall 2007](https://www.cs.princeton.edu/~rs/AlgsDS07/16Geometric.pdf) for information on planar convex hull algorithms
 * [An Introduction to Parallel Programming](https://www.elsevier.com/books/an-introduction-to-parallel-programming/pacheco/978-0-12-374260-5) for OpenMP reference
