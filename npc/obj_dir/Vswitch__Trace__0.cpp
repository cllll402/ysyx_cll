// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "Vswitch__Syms.h"


void Vswitch___024root__trace_chg_sub_0(Vswitch___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vswitch___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vswitch___024root__trace_chg_top_0\n"); );
    // Init
    Vswitch___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vswitch___024root*>(voidSelf);
    Vswitch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vswitch___024root__trace_chg_sub_0((&vlSymsp->TOP), bufp);
}

void Vswitch___024root__trace_chg_sub_0(Vswitch___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vswitch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vswitch___024root__trace_chg_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    bufp->chgBit(oldp+0,(vlSelf->a));
}

void Vswitch___024root__trace_chg_sub_1(Vswitch___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vswitch___024root__trace_chg_top_1(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vswitch___024root__trace_chg_top_1\n"); );
    // Init
    Vswitch___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vswitch___024root*>(voidSelf);
    Vswitch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vswitch___024root__trace_chg_sub_1((&vlSymsp->TOP), bufp);
}

void Vswitch___024root__trace_chg_sub_1(Vswitch___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vswitch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vswitch___024root__trace_chg_sub_1\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 2);
    // Body
    bufp->chgBit(oldp+0,(vlSelf->b));
}

void Vswitch___024root__trace_chg_sub_2(Vswitch___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void Vswitch___024root__trace_chg_top_2(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vswitch___024root__trace_chg_top_2\n"); );
    // Init
    Vswitch___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vswitch___024root*>(voidSelf);
    Vswitch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vswitch___024root__trace_chg_sub_2((&vlSymsp->TOP), bufp);
}

void Vswitch___024root__trace_chg_sub_2(Vswitch___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vswitch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vswitch___024root__trace_chg_sub_2\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 3);
    // Body
    bufp->chgBit(oldp+0,(vlSelf->f));
}

void Vswitch___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vswitch___024root__trace_cleanup\n"); );
    // Init
    Vswitch___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vswitch___024root*>(voidSelf);
    Vswitch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VlUnpacked<CData/*0:0*/, 1> __Vm_traceActivity;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        __Vm_traceActivity[__Vi0] = 0;
    }
    // Body
    vlSymsp->__Vm_activity = false;
    __Vm_traceActivity[0U] = 0U;
}
