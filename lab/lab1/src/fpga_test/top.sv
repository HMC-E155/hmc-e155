module top(
	input 	logic	mcu_blink_in,
	output 	logic fpga_blink_out, mcu_echo_led
);

	logic int_osc;
	logic pulse;
	logic led_state = 0;
	logic [24:0] counter = 0;
	
	// Internal high-speed oscillator
	HSOSC hf_osc (.CLKHFPU(1'b1), .CLKHFEN(1'b1), .CLKHF(int_osc));
	
	// Simple clock divider
	always_ff @(posedge int_osc)
		begin
			counter <= counter + 1;
		end
		
  assign fpga_blink_out = counter[24];
	assign mcu_echo_led = mcu_blink_in;

endmodule