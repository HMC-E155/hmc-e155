`timescale 10ns/1ns
/////////////////////////////////////////////
// testbench_aes_sbox
// Demonstrates 1-cycle latency of RAM blocks doing s-box substitution
/////////////////////////////////////////////

module testbench_aes_sbox();
    logic clk, load, done, sck, sdi, sdo;
    logic [127:0] key, plaintext, cyphertext, expected;
	  logic [255:0] comb;

    // Added delay
    logic delay;

    logic [31:0] input_bytes_2d [3:0]; // Logic array 32 x 4
    logic [127:0] input_bytes_1d;

    logic [31:0] output_bytes_2d [3:0]; // Logic array 32 x 4
    logic [127:0] output_bytes_1d;

    assign input_bytes_2d[0] = input_bytes_1d[127:96];
    assign input_bytes_2d[1] = input_bytes_1d[95:64];
    assign input_bytes_2d[2] = input_bytes_1d[63:32];
    assign input_bytes_2d[3] = input_bytes_1d[31:0];

    assign output_bytes_2d[0] = output_bytes_1d[127:96];
    assign output_bytes_2d[1] = output_bytes_1d[95:64];
    assign output_bytes_2d[2] = output_bytes_1d[63:32];
    assign output_bytes_2d[3] = output_bytes_1d[31:0];

    logic [7:0] a, y;
    
    // device under test
    sbox_sync dut(.clk(clk), .a(a), .y(y));
    
    // test case
    initial begin   
        // Test case from FIPS-197 Appendix A.1, B
        key       <= 128'h2B7E151628AED2A6ABF7158809CF4F3C;
        plaintext <= 128'h3243F6A8885A308D313198A2E0370734;
        expected  <= 128'h3925841D02DC09FBDC118597196A0B32;

        // Alternate test case from Appendix C.1
        // key       <= 128'h000102030405060708090A0B0C0D0E0F;
        // plaintext <= 128'h00112233445566778899AABBCCDDEEFF;
        // expected  <= 128'h69C4E0D86A7B0430D8CDB78070B4C55A;
    end
    
    // generate clock and load signals
    always begin
      clk = 1'b0; #5;
      clk = 1'b1; #5;
    end

    assign input_bytes_1d = key;

    // Generate signal to demonstrate one clock cycle latency of the synchronous s-box substitution.
    // You must wait until the next rising edge for the output data (substituted byte) to be valid
    initial begin
      a = input_bytes_2d[0][31:24];
      #22;
      a = input_bytes_2d[0][23:16];
      #22;
      a = input_bytes_2d[0][15:8];
      #22;
      a = input_bytes_2d[0][7:0];
      #22;
      $stop();
    end

  
    
endmodule