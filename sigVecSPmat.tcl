# Usage: vitis_hls -f <this_tcl_file.tcl>

# select what needs to run
set CSIM 0
set CSYNTH 0
set COSIM 0
set VIVADO_SYN 0
set VIVADO_IMPL 0
set SOLN "solution1"

# setup includes. Vitis_Libraries should be cloned at the same level of dstg_anti_jam
set XF_PROJ_ROOT "C:/Data/Vitis_Libraries"
set INCL "-I${XF_PROJ_ROOT}/solver/L1/include -I${XF_PROJ_ROOT}/solver/L2/include"
set INCL_TB "${INCL} -I${XF_PROJ_ROOT}/solver/L1/tests"

# setup hardware
set CLKP 300MHz
set XPART xczu28dr-ffvg1517-2-e

# setup project name based on this tcl file name
set PROJ [file rootname [file tail [ dict get [ info frame 0 ] file ]]]
puts "PROJ=${PROJ}"

# HLS
open_project -reset "proj_${PROJ}_v"

add_files "${PROJ}.cpp" -cflags "${INCL}"
add_files "${PROJ}.h" -cflags "${INCL}"
add_files -tb "${PROJ}_tb.cpp" -cflags "${INCL_TB}"
set_top "${PROJ}"

open_solution -reset $SOLN
set_part $XPART
create_clock -period $CLKP
set_clock_uncertainty 12.5%
config_export -format syn_dcp -rtl verilog -vivado_phys_opt all

if {$CSIM == 1} {
  csim_design
}
if {$CSYNTH == 1} {
  csynth_design
}
if {$COSIM == 1} {
  cosim_design -trace_level all
}
if {$VIVADO_SYN == 1} {
  export_design -flow syn -rtl verilog
}
if {$VIVADO_IMPL == 1} {
  export_design -flow impl -rtl verilog -format syn_dcp
}

exit
