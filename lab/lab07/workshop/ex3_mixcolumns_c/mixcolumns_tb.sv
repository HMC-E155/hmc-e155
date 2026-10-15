`timescale 10ns/1ns

/////////////////////////////////////////////
// mixcolumns_tb
//   Self-checking testbench for mixcolumns
//   On a mismatch it prints the state as a 4x4 grid, laid out like the
//   figures in FIPS-197, and marks each wrong byte with a *. The shape of
//   the error (one byte? a whole row? a whole column?) tells you where to look.
/////////////////////////////////////////////

module mixcolumns_tb();
    logic [127:0] a, y;
    integer       tests, errors;

    // device under test
    mixcolumns dut(.a(a), .y(y));

    // byte at row r, column c of a packed state (see the packing comment on aes_core in aes_starter.sv)
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

        // Tests 1 and 2: from a published table of MixColumns examples.
        check("all 01", 128'h01010101010101010101010101010101, 128'h01010101010101010101010101010101);
        check("all c6", 128'hc6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6, 128'hc6c6c6c6c6c6c6c6c6c6c6c6c6c6c6c6);

        // Test 3: FIPS-197 Appendix B, Round 1, "After ShiftRows" -> "After MixColumns".
        // TODO: fill in both values. Read each grid column by column, as in Exercise 1.
        check("Appendix B round 1", 128'hx, 128'hx);

        // Test 4: FIPS-197 Appendix B, Round 2, "After ShiftRows" -> "After MixColumns".
        // TODO: fill in both values.
        check("Appendix B round 2", 128'hx, 128'hx);

        if (errors == 0) $display("PASSED: all %0d tests", tests);
        else             $display("FAILED: %0d of %0d tests", errors, tests);
        $finish;
    end
endmodule
