// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

// This is base sequence for rv_dm
// It runs with actual CPU (not stub) and assumes
// cpu is in operation at the beginning.
// So different from rv_dm block sequence,
// dmcontrol.dmactive needs to be set not in dut_init but
// another explicit call at any appropriate point in each test.
class chip_jtag_base_vseq extends chip_sw_base_vseq;
  uvm_mem test_mems[$];
  `uvm_object_utils(chip_jtag_base_vseq)

  `uvm_object_new
  jtag_dmi_reg_block jtag_dmi_ral;

  virtual function void set_handles();
    super.set_handles();
    jtag_dmi_ral = cfg.jtag_dmi_ral;
  endfunction // set_handles

  virtual task pre_start();
    select_jtag = SelectRVJtagTap;
    super.pre_start();
    max_outstanding_accesses = 1;
  endtask

  task debug_mode_en();
    // "Activate" the DM to facilitate ease of testing.
    csr_wr(.ptr(jtag_dmi_ral.dmcontrol.dmactive), .value(1), .blocking(1), .predict(1));
    cfg.clk_rst_vif.wait_clks(5);
    csr_wr(.ptr(jtag_dmi_ral.dmcontrol.ndmreset), .value(1));
    cfg.clk_rst_vif.wait_clks(5);
  endtask

   task ndm_reset_off();
      csr_wr(.ptr(jtag_dmi_ral.dmcontrol.ndmreset), .value(0));
      cfg.clk_rst_vif.wait_clks(5);
   endtask

  virtual task apply_reset(string kind = "HARD");
    fork
      if (kind inside {"HARD", "TRST"}) begin
        jtag_dmi_ral.reset("HARD");
      end
      super.apply_reset(kind);
    join
  endtask
endclass
