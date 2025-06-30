# Multifit

Implementation of the MULTIFIT ALGORITHM originally developed for the identical-machines scheduling problem.
https://en.wikipedia.org/wiki/Multifit_algorithm

Identical-machines scheduling is an optimization problem. 
We are given n jobs of varying processing times, which need to be scheduled on m identical machines, 
such that a certain objective function is optimized, for example, the makespan is minimized.

The input to the multifit algorithm is a set S of numbers, and a parameter n.
The required output is a partition of S into n subsets, such that the largest subset sum (also called the makespan) is as small as possible.

The algorithm uses as a subroutine, an algorithm called first-fit-decreasing bin packing (FFD)
Multifit runs FFD multiple times, each time with a different capacity C, until it finds some C such that FFD with capacity C packs S into at most n bins.

To find it, it uses binary search as follows.
1. Let L := max ( sum(S) / n, max(S) )
2. Let U := max ( 2 sum(S) / n, max(S) )
3. Iterate k times (where k is a precision parameter)
	- Let C := (L+U)/2. Run FFD on S with capacity C
	- If FFD needs at most n bins, then decrease U by letting U := C
	- If FFD needs more than n bins, then increase L by letting L := C
4. Run FFD with capacity U. It is guaranteed to use at most n bins

The scheduling returned by Multifit has makespan at most 13/11 + (1/2)^k times the optimal makespan.
