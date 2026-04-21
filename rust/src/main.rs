use std::time::Instant;

fn main() {
    const N: usize = 10000;
    const BLOCK_SIZE: usize = 256;

    // Create a large array
    let data: Vec<f64> = (0..N).map(|x| x as f64).collect();

    println!("Array size: {} elements", N);
    println!("Block size: {}", BLOCK_SIZE);

    // Non-blocked sum (sequential access)
    let start = Instant::now();
    let mut sum1 = 0.0;
    for i in 0..N {
        sum1 += data[i];
    }
    let duration1 = start.elapsed();
    println!("Non-blocked sum: {:.0}, time: {:?}", sum1, duration1);

    // Blocked sum
    let start = Instant::now();
    let mut sum2 = 0.0;
    for block_start in (0..N).step_by(BLOCK_SIZE) {
        let block_end = (block_start + BLOCK_SIZE).min(N);
        for i in block_start..block_end {
            sum2 += data[i];
        }
    }
    let duration2 = start.elapsed();
    println!("Blocked sum: {:.0}, time: {:?}", sum2, duration2);

    println!("Speedup: {:.2}x", duration1.as_nanos() as f64 / duration2.as_nanos() as f64);
}
