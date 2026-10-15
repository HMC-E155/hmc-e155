`timescale 10ns/1ns

/////////////////////////////////////////////
// state_to_output
//   Copy the 4x4 state array back into a 128-bit value
//   Section 3.4
//   Purely combinational
//
//   The reverse of input_to_state, with the same packing. With both, a
//   module with 128-bit ports like the ones in aes_core can do its work
//   on rows and columns inside.
/////////////////////////////////////////////

module state_to_output(input  logic [0:3][0:3][7:0] a,
                       output logic [127:0]         y);

    // TODO: Your code goes here

endmodule
