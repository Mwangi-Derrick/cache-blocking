import time

N = 10000
BLOCK_SIZE = 256

# Create a large array
data = [float(i) for i in range(N)]

print(f"Array size: {N} elements")
print(f"Block size: {BLOCK_SIZE}")

# Non-blocked sum
start1 = time.perf_counter_ns()
sum1 = 0.0
for i in range(N):
    sum1 += data[i]
duration1 = time.perf_counter_ns() - start1
print(f"Non-blocked sum: {sum1:.0f}, time: {duration1} ns")

# Blocked sum
start2 = time.perf_counter_ns()
sum2 = 0.0
for block_start in range(0, N, BLOCK_SIZE):
    block_end = min(block_start + BLOCK_SIZE, N)
    for i in range(block_start, block_end):
        sum2 += data[i]
duration2 = time.perf_counter_ns() - start2
print(f"Blocked sum: {sum2:.0f}, time: {duration2} ns")

print(f"Speedup: {duration1 / duration2:.2f}x")
