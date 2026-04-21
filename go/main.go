package main

import (
	"fmt"
	"time"
)

func main() {
	const N = 1000000  // Large enough to measure
	const BLOCK_SIZE = 256
	const ITERATIONS = 100  // Multiple iterations for stable results

	data := make([]float64, N)
	for i := 0; i < N; i++ {
		data[i] = float64(i)
	}

	fmt.Printf("Array size: %d elements\n", N)
	fmt.Printf("Block size: %d\n", BLOCK_SIZE)

	// Non-blocked sum
	start1 := time.Now()
	for iter := 0; iter < ITERATIONS; iter++ {
		sum1 := 0.0
		for i := 0; i < N; i++ {
			sum1 += data[i]
		}
		_ = sum1
	}
	duration1 := time.Since(start1)

	// Blocked sum
	start2 := time.Now()
	for iter := 0; iter < ITERATIONS; iter++ {
		sum2 := 0.0
		for block_start := 0; block_start < N; block_start += BLOCK_SIZE {
			block_end := block_start + BLOCK_SIZE
			if block_end > N {
				block_end = N
			}
			for i := block_start; i < block_end; i++ {
				sum2 += data[i]
			}
		}
		_ = sum2
	}
	duration2 := time.Since(start2)

	// Report speedup (not absolute times)
	speedup := float64(duration1) / float64(duration2)
	fmt.Printf("Non-blocked time: %v\n", duration1/ITERATIONS)
	fmt.Printf("Blocked time: %v\n", duration2/ITERATIONS)
	fmt.Printf("Speedup: %.2fx\n", speedup)
}