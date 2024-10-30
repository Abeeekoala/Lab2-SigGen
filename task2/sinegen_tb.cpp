#include "Vsinegen.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env){
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vsinegen* top = new Vsinegen;

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;

    top->trace(tfp, 99);
    tfp->open ("sinegen.vcd");

    //init Vbuddy
    if (vbdOpen() != 1) return(-1);
    vbdHeader("Lab 2: Sinegen");
    vbdSetMode(1);

    top->clk = 1;
    top->rst = 0;
    top->en = 1;
    top->incr = 3; // increase frequency so that we see more waves
    top->phase = 64;
    //run simulation for many clock cycles
    for (i=0; i<1000000; i++){
        top->phase = int(vbdValue()/25 * 64); // 0 - 100 represent 0 - 2pi phase offset
        for (clk=0; clk<2; clk++){
            tfp->dump (2*i+clk);
            top->clk = !top->clk;   // unit is in ps!!!
            top->eval ();
        }

        vbdPlot(int(top->dout1), 0, 255);
        vbdPlot(int(top->dout2), 0, 255);

        vbdCycle(i+1);
            // either simulation finished, or 'q' is pressed
        if ((Verilated::gotFinish()) || (vbdGetkey()=='q')) 
            exit(0);                // ... exit if finish OR 'q' pressed
        if (Verilated::gotFinish()) exit(0);

    }
    tfp->close();
    exit(0);
}