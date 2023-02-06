
//`#start header` -- edit after this line, do not edit this line
// ========================================
//
// Copyright YOUR COMPANY, THE YEAR
// All Rights Reserved
// UNPUBLISHED, LICENSED SOFTWARE.
//
// CONFIDENTIAL AND PROPRIETARY INFORMATION
// WHICH IS THE PROPERTY OF your company.
//
// ========================================
`include "cypress.v"
//`#end` -- edit above this line, do not edit this line
// Generated on 02/06/2023 at 17:36
// Component: verilog_example_1
module verilog_example_1 (
	output [7:0] o_lsb_data,
	output [7:0] o_msb_data,
	input  [7:0] i_data
);

//`#start body` -- edit after this line, do not edit this line

//        Your code goes here
wire [15:0] w_concat;

assign  w_concat = { i_data, i_data };
assign { o_msb_data, o_lsb_data } = w_concat;

//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
