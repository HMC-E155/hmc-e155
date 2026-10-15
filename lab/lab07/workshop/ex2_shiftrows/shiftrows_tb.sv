`timescale 10ns/1ns

/////////////////////////////////////////////
// shiftrows_tb
//   Self-checking testbench for shiftrows
//   On a mismatch it prints the state as a 4x4 grid, laid out like the
//   figures in FIPS-197, and marks each wrong byte with a *. The shape of
//   the error (one byte? a whole row? a whole column?) tells you where to look.
/////////////////////////////////////////////

module shiftrows_tb();
    logic [127:0] a, y;
    integer       tests, errors;

    // device under test
    shiftrows dut(.a(a), .y(y));

    // byte at row r, column c of a packed state (see the packing in shiftrows.sv)
    function logic [7:0] state_byte(input logic [127:0] s, input integer r, input integer c);
        state_byte = s[127 - 8*(r + 4*c) -: 8];
    endfunction

    // print got and expected side by side as 4x4 grids, starring the bytes that differ
    task print_grids(input logic [127:0] got, input logic [127:0] expected);
        integer r, c;
        $display("         got                  expected");
        for (r = 0; r < 4; r = r + 1) begin
            $write("  ");
            for (c = 0; c < 4; c = c + 1)
                $write("%h%s", state_byte(got, r, c),
                       (state_byte(got, r, c) === state_byte(expected, r, c)) ? "  " : "* ");
            $write("    ");
            for (c = 0; c < 4; c = c + 1)
                $write("%h  ", state_byte(expected, r, c));
            $display("");
        end
    endtask

    // apply one test vector and compare against the expected output
    task check(input string name, input logic [127:0] in, input logic [127:0] expected);
        tests = tests + 1;
        if ($isunknown(in) || $isunknown(expected)) begin
            $error("%s: test vector still contains x. Fill in the TODO.", name);
            errors = errors + 1;
        end else begin
            a = in;
            #1;
            if (y === expected) $display("PASS  %s", name);
            else begin
                $error("%s\n  in       = %h\n  got      = %h\n  expected = %h", name, in, y, expected);
                print_grids(y, expected);
                errors = errors + 1;
            end
        end
    endtask

    initial begin
        tests = 0; errors = 0;

        // Test 1: every byte holds its own position, S[r][c] = 4c + r, so a wrong
        // answer shows you exactly which byte ended up where.
        // TODO: work out the expected output by hand before you run anything.
        check("numbered bytes", 128'h000102030405060708090a0b0c0d0e0f, 128'hx);

        // Test 2: FIPS-197 Appendix B, Round 1, "After SubBytes" -> "After ShiftRows".
        // Appendix B prints the state as 4x4 grids. Read each grid column by column.
        // TODO: fill in both values.
        check("Appendix B round 1", 128'hx, 128'hx);

        // Test 3: FIPS-197 Appendix B, Round 2, "After SubBytes" -> "After ShiftRows".
        // TODO: fill in both values.
        check("Appendix B round 2", 128'hx, 128'hx);

        if (errors == 0) $display("PASSED: all %0d tests", tests);
        else             $display("FAILED: %0d of %0d tests", errors, tests);
        $finish;
    end
endmodule
