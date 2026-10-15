`timescale 10ns/1ns

/////////////////////////////////////////////
// aes_core_trace_tb
//   Round-by-round tracer for YOUR aes_core, using FIPS-197 Appendix B
//
//   testbench_aes_core only tells you whether the final cyphertext is right.
//   This testbench watches a register inside your design and, every time it
//   changes, looks the new value up in the full Appendix B table. It prints
//   which step of which round the value matches, and the first value that
//   matches nothing. That is where your bug is: somewhere between the last
//   value that matched and the first one that didn't.
//
//   It does not assume anything about your timing or architecture, so it
//   works whether you register the state once per round or after every step.
//
//   Setup:
//     1. Add this file to the same project as your aes_core and its submodules.
//     2. Point state_probe (and optionally key_probe) at your registers below.
//     3. Select aes_core_trace_tb as the testbench and run.
/////////////////////////////////////////////

module aes_core_trace_tb();
    logic         clk, load, done;
    logic [127:0] key, plaintext, cyphertext, expected;

    // device under test
    aes_core dut(clk, load, key, plaintext, done, cyphertext);

    // TODO: point state_probe at the register in YOUR aes_core that holds the
    // state, using its hierarchical name (e.g. dut.state). This line will not
    // compile until you do.
    logic [127:0] state_probe, key_probe;
    assign state_probe = dut.YOUR_STATE_REGISTER;

    // Optional: also trace your round key. Point key_probe at your round key
    // register (e.g. dut.roundkey) and set TRACE_KEY to 1.
    localparam TRACE_KEY = 0;
    assign key_probe = 'x;

    // give up if done has not been asserted after this many cycles
    localparam MAX_CYCLES = 200;

    // every intermediate value in FIPS-197 Appendix B, in order
    localparam N_REF = 52;
    logic [127:0] ref_value [0:N_REF-1];
    string        ref_label [0:N_REF-1];

    initial begin
        ref_value[ 0] = 128'h3243f6a8885a308d313198a2e0370734;  ref_label[ 0] = "input";
        ref_value[ 1] = 128'h2b7e151628aed2a6abf7158809cf4f3c;  ref_label[ 1] = "round_key[0]";
        ref_value[ 2] = 128'h193de3bea0f4e22b9ac68d2ae9f84808;  ref_label[ 2] = "round 1 start";
        ref_value[ 3] = 128'hd42711aee0bf98f1b8b45de51e415230;  ref_label[ 3] = "round 1 after SubBytes";
        ref_value[ 4] = 128'hd4bf5d30e0b452aeb84111f11e2798e5;  ref_label[ 4] = "round 1 after ShiftRows";
        ref_value[ 5] = 128'h046681e5e0cb199a48f8d37a2806264c;  ref_label[ 5] = "round 1 after MixColumns";
        ref_value[ 6] = 128'ha0fafe1788542cb123a339392a6c7605;  ref_label[ 6] = "round_key[1]";
        ref_value[ 7] = 128'ha49c7ff2689f352b6b5bea43026a5049;  ref_label[ 7] = "round 2 start";
        ref_value[ 8] = 128'h49ded28945db96f17f39871a7702533b;  ref_label[ 8] = "round 2 after SubBytes";
        ref_value[ 9] = 128'h49db873b453953897f02d2f177de961a;  ref_label[ 9] = "round 2 after ShiftRows";
        ref_value[10] = 128'h584dcaf11b4b5aacdbe7caa81b6bb0e5;  ref_label[10] = "round 2 after MixColumns";
        ref_value[11] = 128'hf2c295f27a96b9435935807a7359f67f;  ref_label[11] = "round_key[2]";
        ref_value[12] = 128'haa8f5f0361dde3ef82d24ad26832469a;  ref_label[12] = "round 3 start";
        ref_value[13] = 128'hac73cf7befc111df13b5d6b545235ab8;  ref_label[13] = "round 3 after SubBytes";
        ref_value[14] = 128'hacc1d6b8efb55a7b1323cfdf457311b5;  ref_label[14] = "round 3 after ShiftRows";
        ref_value[15] = 128'h75ec0993200b633353c0cf7cbb25d0dc;  ref_label[15] = "round 3 after MixColumns";
        ref_value[16] = 128'h3d80477d4716fe3e1e237e446d7a883b;  ref_label[16] = "round_key[3]";
        ref_value[17] = 128'h486c4eee671d9d0d4de3b138d65f58e7;  ref_label[17] = "round 4 start";
        ref_value[18] = 128'h52502f2885a45ed7e311c807f6cf6a94;  ref_label[18] = "round 4 after SubBytes";
        ref_value[19] = 128'h52a4c89485116a28e3cf2fd7f6505e07;  ref_label[19] = "round 4 after ShiftRows";
        ref_value[20] = 128'h0fd6daa9603138bf6fc0106b5eb31301;  ref_label[20] = "round 4 after MixColumns";
        ref_value[21] = 128'hef44a541a8525b7fb671253bdb0bad00;  ref_label[21] = "round_key[4]";
        ref_value[22] = 128'he0927fe8c86363c0d9b1355085b8be01;  ref_label[22] = "round 5 start";
        ref_value[23] = 128'he14fd29be8fbfbba35c89653976cae7c;  ref_label[23] = "round 5 after SubBytes";
        ref_value[24] = 128'he1fb967ce8c8ae9b356cd2ba974ffb53;  ref_label[24] = "round 5 after ShiftRows";
        ref_value[25] = 128'h25d1a9adbd11d168b63a338e4c4cc0b0;  ref_label[25] = "round 5 after MixColumns";
        ref_value[26] = 128'hd4d1c6f87c839d87caf2b8bc11f915bc;  ref_label[26] = "round_key[5]";
        ref_value[27] = 128'hf1006f55c1924cef7cc88b325db5d50c;  ref_label[27] = "round 6 start";
        ref_value[28] = 128'ha163a8fc784f29df10e83d234cd503fe;  ref_label[28] = "round 6 after SubBytes";
        ref_value[29] = 128'ha14f3dfe78e803fc10d5a8df4c632923;  ref_label[29] = "round 6 after ShiftRows";
        ref_value[30] = 128'h4b868d6d2c4a8980339df4e837d218d8;  ref_label[30] = "round 6 after MixColumns";
        ref_value[31] = 128'h6d88a37a110b3efddbf98641ca0093fd;  ref_label[31] = "round_key[6]";
        ref_value[32] = 128'h260e2e173d41b77de86472a9fdd28b25;  ref_label[32] = "round 7 start";
        ref_value[33] = 128'hf7ab31f02783a9ff9b4340d354b53d3f;  ref_label[33] = "round 7 after SubBytes";
        ref_value[34] = 128'hf783403f27433df09bb531ff54aba9d3;  ref_label[34] = "round 7 after ShiftRows";
        ref_value[35] = 128'h1415b5bf461615ec274656d7342ad843;  ref_label[35] = "round 7 after MixColumns";
        ref_value[36] = 128'h4e54f70e5f5fc9f384a64fb24ea6dc4f;  ref_label[36] = "round_key[7]";
        ref_value[37] = 128'h5a4142b11949dc1fa3e019657a8c040c;  ref_label[37] = "round 8 start";
        ref_value[38] = 128'hbe832cc8d43b86c00ae1d44dda64f2fe;  ref_label[38] = "round 8 after SubBytes";
        ref_value[39] = 128'hbe3bd4fed4e1f2c80a642cc0da83864d;  ref_label[39] = "round 8 after ShiftRows";
        ref_value[40] = 128'h00512fd1b1c889ff54766dcdfa1b99ea;  ref_label[40] = "round 8 after MixColumns";
        ref_value[41] = 128'head27321b58dbad2312bf5607f8d292f;  ref_label[41] = "round_key[8]";
        ref_value[42] = 128'hea835cf00445332d655d98ad8596b0c5;  ref_label[42] = "round 9 start";
        ref_value[43] = 128'h87ec4a8cf26ec3d84d4c46959790e7a6;  ref_label[43] = "round 9 after SubBytes";
        ref_value[44] = 128'h876e46a6f24ce78c4d904ad897ecc395;  ref_label[44] = "round 9 after ShiftRows";
        ref_value[45] = 128'h473794ed40d4e4a5a3703aa64c9f42bc;  ref_label[45] = "round 9 after MixColumns";
        ref_value[46] = 128'hac7766f319fadc2128d12941575c006e;  ref_label[46] = "round_key[9]";
        ref_value[47] = 128'heb40f21e592e38848ba113e71bc342d2;  ref_label[47] = "round 10 start";
        ref_value[48] = 128'he9098972cb31075f3d327d94af2e2cb5;  ref_label[48] = "round 10 after SubBytes";
        ref_value[49] = 128'he9317db5cb322c723d2e895faf090794;  ref_label[49] = "round 10 after ShiftRows";
        ref_value[50] = 128'hd014f9a8c9ee2589e13f0cc8b6630ca6;  ref_label[50] = "round_key[10]";
        ref_value[51] = 128'h3925841d02dc09fbdc118597196a0b32;  ref_label[51] = "output";
    end

    // test case: FIPS-197 Appendix A.1, B (same as testbench_aes_core)
    initial begin
        key       = 128'h2B7E151628AED2A6ABF7158809CF4F3C;
        plaintext = 128'h3243F6A8885A308D313198A2E0370734;
        expected  = 128'h3925841D02DC09FBDC118597196A0B32;
    end

    // generate clock and load signals
    always begin
        clk = 1'b0; #5;
        clk = 1'b1; #5;
    end

    initial begin
        load = 1'b1; #22; load = 1'b0;   // pulse load to start encryption
    end

    integer       cycle = 0, mismatches = 0;
    integer       state_good = -1, key_good = -1, state_x = 0, key_x = 0, done_x = 0;
    logic [127:0] state_last = 'x, key_last = 'x;

    // look a value up in the reference table: returns its index, or -1 if it is not there
    function integer find_ref(input logic [127:0] v);
        integer i;
        find_ref = -1;
        for (i = 0; i < N_REF; i = i + 1)
            if (find_ref == -1 && ref_value[i] === v) find_ref = i;
    endfunction

    // report a probe whenever its value changes
    task trace(input string name, input logic [127:0] v, inout logic [127:0] last,
               inout integer last_good, inout integer warned_x);
        integer i;
        string  good_label;
        if (v !== last) begin
            last = v;
            if ($isunknown(v)) begin
                if (!warned_x) $display("cycle %3d  %s = %h  contains x (not reset, or sbox.txt not loaded?)",
                                        cycle, name, v);
                warned_x = 1;
            end else begin
                i = find_ref(v);
                if (i >= 0) begin
                    $display("cycle %3d  %s = %h  matches %s", cycle, name, v, ref_label[i]);
                    last_good = i;
                end else begin
                    $display("cycle %3d  %s = %h  NO MATCH", cycle, name, v);
                    if (mismatches == 0) begin
                        good_label = "(none)";
                        if (last_good >= 0) good_label = ref_label[last_good];
                        $display("           ^ first divergence: the last %s value that matched was %s",
                                 name, good_label);
                    end
                    mismatches = mismatches + 1;
                end
            end
        end
    endtask

    always @(posedge clk) begin
        #1;   // let the registers settle after the edge before sampling
        cycle = cycle + 1;

        trace("state", state_probe, state_last, state_good, state_x);
        if (TRACE_KEY) trace("key  ", key_probe, key_last, key_good, key_x);

        if (done === 1'bx && !done_x) begin
            $error("done is x at cycle %0d. Does load (or a reset) put your FSM in a known state?", cycle);
            done_x = 1;
        end

        if (done === 1'b1) begin
            if (cyphertext === expected) $display("PASSED: cyphertext = %h after %0d cycles", cyphertext, cycle);
            else begin
                $error("cyphertext = %h, expected %h", cyphertext, expected);
                $display("FAILED");
            end
            $finish;
        end

        if (cycle == MAX_CYCLES) begin
            $error("done was never asserted in %0d cycles", MAX_CYCLES);
            $display("FAILED: timeout");
            $finish;
        end
    end
endmodule
