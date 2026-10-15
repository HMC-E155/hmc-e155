`timescale 10ns/1ns

/////////////////////////////////////////////
// input_to_state_tb
//   Self-checking testbench for input_to_state
//   Each expected value is a state array written row by row, so it reads
//   like the grids in FIPS-197. On a mismatch it prints what you got next to
//   what was expected, as grids, and marks each wrong byte with a *.
/////////////////////////////////////////////

module input_to_state_tb();
    logic [127:0]         a;
    logic [0:3][0:3][7:0] y;
    integer               tests, errors;

    // device under test
    input_to_state dut(.a(a), .y(y));

    // byte at row r, column c of a state array. The array is packed, so it is
    // also a 128-bit vector, laid out row by row: [r][c] is bits
    // [127 - 8*(4*r + c) -: 8]. Compare that with the aes_core packing.
    function logic [7:0] grid_byte(input logic [127:0] s, input integer r, input integer c);
        grid_byte = s[127 - 8*(4*r + c) -: 8];
    endfunction

    // print got and expected side by side as 4x4 grids, starring the bytes that differ
    task print_grids(input logic [0:3][0:3][7:0] got, input logic [0:3][0:3][7:0] expected);
        integer r, c;
        $display("         got                  expected");
        for (r = 0; r < 4; r = r + 1) begin
            $write("  ");
            for (c = 0; c < 4; c = c + 1)
                $write("%h%s", grid_byte(got, r, c), (grid_byte(got, r, c) === grid_byte(expected, r, c)) ? "  " : "* ");
            $write("    ");
            for (c = 0; c < 4; c = c + 1)
                $write("%h  ", grid_byte(expected, r, c));
            $display("");
        end
    endtask

    // apply one test vector and compare against the expected state
    task check(input string name, input logic [127:0] in, input logic [0:3][0:3][7:0] expected);
        tests = tests + 1;
        if ($isunknown(in) || $isunknown(expected)) begin
            $error("%s: test vector still contains x. Fill in the TODO.", name);
            errors = errors + 1;
        end else begin
            a = in;
            #1;
            if (y === expected) $display("PASS  %s", name);
            else begin
                $error("%s\n  in = %h", name, in);
                print_grids(y, expected);
                errors = errors + 1;
            end
        end
    endtask

    initial begin
        tests = 0; errors = 0;

        // Test 1: FIPS-197 Appendix B, "input". This is the worked example
        // from the Background section of the handout.
        check("Appendix B input", 128'h3243f6a8885a308d313198a2e0370734,
              {8'h32, 8'h88, 8'h31, 8'he0,
               8'h43, 8'h5a, 8'h31, 8'h37,
               8'hf6, 8'h30, 8'h98, 8'h07,
               8'ha8, 8'h8d, 8'ha2, 8'h34});

        // Test 2: every byte holds its own position in the 128-bit value, so a
        // wrong answer shows exactly which byte ended up where.
        // TODO: fill in the expected grid, row by row.
        check("numbered bytes", 128'h000102030405060708090a0b0c0d0e0f, 'x);

        // Test 3: the Appendix B cipher key. You filled in this grid by hand in Part A.
        // TODO: fill in the expected grid, row by row.
        check("Appendix B cipher key", 128'h2b7e151628aed2a6abf7158809cf4f3c, 'x);

        if (errors == 0) $display("PASSED: all %0d tests", tests);
        else             $display("FAILED: %0d of %0d tests", errors, tests);
        $finish;
    end
endmodule
