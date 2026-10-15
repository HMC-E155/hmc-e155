`timescale 10ns/1ns

/////////////////////////////////////////////
// state_to_output_tb
//   Self-checking testbench for state_to_output
//   The first two tests give a state array, written row by row, and check the
//   128-bit value that comes out. The last one runs a value through
//   input_to_state and then state_to_output, and checks it comes back unchanged.
/////////////////////////////////////////////

module state_to_output_tb();
    logic [0:3][0:3][7:0] a;
    logic [127:0]         y;
    integer               tests, errors;

    // device under test
    state_to_output dut(.a(a), .y(y));

    // the round trip: rt_in -> input_to_state -> state_to_output -> rt_out
    logic [127:0]         rt_in, rt_out;
    logic [0:3][0:3][7:0] rt_state;
    input_to_state  rt_its(.a(rt_in), .y(rt_state));
    state_to_output rt_sto(.a(rt_state), .y(rt_out));

    // apply one state array and compare against the expected 128-bit value
    task check(input string name, input logic [0:3][0:3][7:0] in, input logic [127:0] expected);
        tests = tests + 1;
        if ($isunknown(in) || $isunknown(expected)) begin
            $error("%s: test vector still contains x. Fill in the TODO.", name);
            errors = errors + 1;
        end else begin
            a = in;
            #1;
            if (y === expected) $display("PASS  %s", name);
            else begin
                $error("%s\n  got      = %h\n  expected = %h", name, y, expected);
                errors = errors + 1;
            end
        end
    endtask

    initial begin
        tests = 0; errors = 0;

        // Test 1: FIPS-197 Appendix B, "input", the other way round.
        check("Appendix B input",
              {8'h32, 8'h88, 8'h31, 8'he0,
               8'h43, 8'h5a, 8'h31, 8'h37,
               8'hf6, 8'h30, 8'h98, 8'h07,
               8'ha8, 8'h8d, 8'ha2, 8'h34},
              128'h3243f6a8885a308d313198a2e0370734);

        // Test 2: FIPS-197 Appendix B, Round 1, "Start of Round". You packed
        // this grid by hand in Part A.
        // TODO: fill in the expected 128-bit value.
        check("Appendix B round 1 start",
              {8'h19, 8'ha0, 8'h9a, 8'he9,
               8'h3d, 8'hf4, 8'hc6, 8'hf8,
               8'he3, 8'he2, 8'h8d, 8'h48,
               8'hbe, 8'h2b, 8'h2a, 8'h08},
              128'hx);

        // Test 3: there and back again. Uses your input_to_state too.
        tests = tests + 1;
        rt_in = 128'h000102030405060708090a0b0c0d0e0f;
        #1;
        if (rt_out === rt_in) $display("PASS  round trip");
        else begin
            $error("round trip\n  in  = %h\n  out = %h", rt_in, rt_out);
            errors = errors + 1;
        end

        if (errors == 0) $display("PASSED: all %0d tests", tests);
        else             $display("FAILED: %0d of %0d tests", errors, tests);
        $finish;
    end
endmodule
