import timeit
import math

magic_number = 2147483647
N = 1000000

print(timeit.timeit(lambda: math.sqrt(magic_number), number = N))

print(timeit.timeit(lambda: magic_number ** 0.5, number = N))
