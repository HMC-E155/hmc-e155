# Lab 7 Workshop: Debugging AES with Testbenches

Starter files for the in-class AES workshop. The instructions are on the course website:
[Lab 7 Workshop handout](https://hmc-e155.github.io/lab/lab7/workshop.html).

Each folder is a self-contained project. Open it in [Jacquard FPGA Studio](https://jacquardfpga.com) with
the link in the table, or download the folder and add the files to a QuestaSim project.

| Folder | What it is | Open in Jacquard |
|---|---|---|
| `ex1_state_array/` | Exercise 1: write `input_to_state` and `state_to_output`, and finish their self-checking testbenches | [open](https://jacquardfpga.com/?gh=HMC-E155/hmc-e155/tree/main/lab/lab07/workshop/ex1_state_array) |
| `ex2_shiftrows/` | Exercise 2: write `shiftrows`, on the 128-bit vector or the state array, and finish its self-checking testbench | [open](https://jacquardfpga.com/?gh=HMC-E155/hmc-e155/tree/main/lab/lab07/workshop/ex2_shiftrows) |
| `ex3_mixcolumns_a/` | Exercise 3: find the bug in `mixcolumns` (version A) | [open](https://jacquardfpga.com/?gh=HMC-E155/hmc-e155/tree/main/lab/lab07/workshop/ex3_mixcolumns_a) |
| `ex3_mixcolumns_b/` | Exercise 3: find the bug in `mixcolumns` (version B) | [open](https://jacquardfpga.com/?gh=HMC-E155/hmc-e155/tree/main/lab/lab07/workshop/ex3_mixcolumns_b) |
| `ex3_mixcolumns_c/` | Exercise 3: find the bug in `mixcolumns` (version C) | [open](https://jacquardfpga.com/?gh=HMC-E155/hmc-e155/tree/main/lab/lab07/workshop/ex3_mixcolumns_c) |
| `stretch_round_trace/` | Stretch: a round-by-round tracer for your own `aes_core` | Add `aes_core_trace_tb.sv` to your Lab 7 project |

Each `ex3` folder has one bug, and the three bugs are different. The `mixcolumns` in the Lab 7 starter
code is correct, so you could find the bug by comparing against it. Please don't: the point is to practice
finding a bug when there is no answer key, which is the situation you'll be in for the rest of Lab 7.
