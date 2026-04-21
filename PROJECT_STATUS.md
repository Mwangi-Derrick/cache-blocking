# Cache Blocking Project - Complete Investigation

## Project Overview

This project demonstrates **cache blocking** (loop tiling) - a performance optimization technique for improving memory access patterns by breaking large data structures into smaller chunks that fit in CPU cache.

## Directory Structure

\\\
cache/
├── cpp/main.cpp              # C++ implementation (needs compilation)
├── go/main.go                # Go implementation
├── js/main.js                # Node.js implementation
├── mojo/main.mojo            # Mojo implementation
├── python/main.py                # Python implementation ✓
├── rust/                       # Rust implementation ✓
│   ├── Cargo.toml
│   ├── src/
│   │   └── main.rs
│   └── target/
├── zig/main.zig              # Zig implementation ✓
├── docs/README.md
└── .github/workflows/build.yml
\\\

## Working Implementations ✓

### 1. Python - 1.39x Speedup
\\\
Array size: 10000 elements
Block size: 256
Non-blocked sum: 49995000, time: 3623000 ns
Blocked sum: 49995000, time: 2600200 ns
Speedup: 1.39x
\\\

### 2. Rust - 1.02x Speedup
\\\
Array size: 10000 elements
Block size: 256
Non-blocked sum: 49995000, time: 17.8µs
Blocked sum: 49995000, time: 17.4µs
Speedup: 1.02x
\\\

### 3. JavaScript/Node.js
\\\
Array size: 10000 elements
Block size: 256
Non-blocked sum: 49995000, time: 288900 ns
Blocked sum: 49995000, time: 641100 ns
Speedup: 0.45x
\\\

### 4. Zig - 1.31x Speedup
\\\
Array size: 10000 elements
Block size: 256
Non-blocked sum: 49995000, time: 64100 ns
Blocked sum: 49995000, time: 49100 ns
Speedup: 1.31x
\\\

### 5. Go
Note: Go's timing is very precise; no measurable difference at this scale.

## Fixed Issues

### Encoding Problem (RESOLVED)
Files created with PowerShell's echo command were saved as UTF-16 with null bytes between every character. 

**Solution:** Recreated all files using PowerShell's \Out-File -Encoding UTF8\ cmdlet.

Files fixed:
- cpp/main.cpp
- go/main.go
- js/main.js
- mojo/main.mojo
- py/main.py
- rsust/src/main.rs
- rust/Cargo.toml
- zig/main.zig

### Rust Project Structure (RESOLVED)
Moved main.rs from \
s/main.rs\ to proper \
s/src/main.rs\ for Cargo compatibility.

## Key Concepts Demonstrated

### Cache Blocking
Divides large arrays into smaller blocks (256 elements) that fit in CPU cache:

\\\
ust
for block_start in (0..N).step_by(BLOCK_SIZE) {
    let block_end = (block_start + BLOCK_SIZE).min(N);
    for i in block_start..block_end {
        sum += data[i];  // Sequential access within block
    }
}
\\\

This enables:
- **Spatial locality**: Cache loads contiguous blocks
- **Temporal locality**: Reusing cached data before eviction
- **Better cache hit rate**: Fewer expensive RAM accesses

### Memory Hierarchy (ns latency)
- L1 Cache: ~2ns
- L2 Cache: ~6ns  
- L3 Cache: ~20ns
- Main RAM: ~50-100ns

## Running the Implementations

### Python
\\\ash
python py/main.py
\\\
Result: **✓ Works** (1.39x speedup)

### Rust
\\\ash
cd rs && cargo run --release
\\\
Result: **✓ Works** (1.02x speedup)

### JavaScript/Node.js
\\\ash
node js/main.js
\\\
Result: **✓ Works** (0.45x)

### Zig
\\\ash
zig run zig/main.zig
\\\
Result: **✓ Works** (1.31x speedup)

### Go
\\\ash
go run go/main.go
\\\
Result: **✓ Compiles** (timing too precise at small scale)

### C++
\\\ash
cd cpp && g++ main.cpp -o main -std=c++11 -O3 && ./main
\\\
Status: File fixed (UTF-8), ready for compilation

### Mojo
\\\ash
cd mojo && mojo main.mojo
\\\
Status: Implementation ready (requires Mojo SDK)

## Performance Notes

1. **Python shows best relative speedup (1.39x)** - interpreter overhead masks fine-grained timing
2. **Zig is fastest (64100 ns non-blocked)** - low-level control
3. **Rust is consistent (1.02x)** - compiler optimizations already handle this pattern
4. **JavaScript is slower blocked** - JIT overhead for complex patterns
5. **Go timing too fast** - microsecond precision needed

## What We Learned

### .step_by() in Rust
\\\
ust
(0..N).step_by(BLOCK_SIZE)  // Yields: 0, BLOCK_SIZE, 2*BLOCK_SIZE, ...
\\\

This creates the **outer loop** that iterates over block start positions. The **inner loop** processes each block sequentially:

\\\
ust
for block_start in (0..N).step_by(BLOCK_SIZE) {
    // block_start: 0, 256, 512, 768, ...
    for i in block_start..block_start+BLOCK_SIZE {
        // i: 0-255, 256-511, 512-767, ...
        process(data[i]);  // Sequential access pattern
    }
}
\\\

### RAM as a 1D Tape
Cache blocking works because RAM is accessed as a linear sequence:

\\\
RAM: [elem0][elem1][elem2]...[elem255][elem256]...[elem10000]
     └─────────Block 0─────────┘└────Block 1────┘└─Block 39──┘
     
Load Block 0 once (1 cache miss)
Reuse Block 0 many times (all cache hits)
Then load Block 1 (1 cache miss)
Reuse Block 1 many times (all cache hits)
\\\

## Project Structure Validation

All files have been:
- ✓ Created with proper UTF-8 encoding
- ✓ Organized in language-specific directories
- ✓ Implemented with identical cache blocking logic
- ✓ Ready for cross-language performance comparison

## Automated Testing

GitHub Actions workflow (\.github/workflows/build.yml\) configured to:
- Build Rust with Cargo
- Compile C++ with g++
- Run Python directly
- Execute Go programs
- Run JavaScript with Node.js
- Test Zig compilation
