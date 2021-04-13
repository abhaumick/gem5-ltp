# Project Readme - Last Touch Prediction

## Decisions

- [ ] Make MSI with LTP-Invalidate
- [ ] Decide L1 and L2 behavior
- [ ] 

## Roadmap

- [ ] Setup a Distributed Shared Memory architecture in Ruby 
	- For system emulation and full-system simulation using gem5

- [ ] Modify gem5 to add Last-Touch Predictor to L2 cache 
	- (Add as a layer between the processor and directory)
	- Current Trace Generation logic per cache-block
	- Pag OR Pap Last Touch History Table

- [ ] Modify cache controller (Ruby) to allow self-invalidation.
	- Use cache replace logic.
	- Queue self-invalidation to directory.

- [ ] Write Microbenchmarks to verify LTP correctness using SE mode.

- [ ] Benchmark suite
	- Barnes, ocean, raytrace from the SPLASH-2 benchmark suite 
		- (evaluated in paper) – can be used for comparison.
	- PARSEC 2.0 benchmarks [5] – all Pthreads based benchmarks.
		- More up to date

## Deliverables

- [ ] Characterize the performance improvement in memory request service latency for applications using gem5 + Ruby ProtocolTrace.

- [ ] Evaluate for different cache coherence schemes – MSI, MESI etc
	- Observe whether gains diminish or persist.

- [ ] Evaluate the effect of prediction accuracy.
	- Different (simple) branch predictor schemes 1-bit, 2-bit, 3-bit

- [ ] Evaluate the effect of hashing schemes and different hash sizes used for trace correlation.

- [ ] Add logic to verify self-invalidation correctness.

- [ ] Detect incorrect “last-touch” due to low confidence predictions.


