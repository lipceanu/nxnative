// Whether the Mac front end looks for another screen mode in fullscreen (front_mac.m screen_match_apply), apart from AppKit so t_present_rescue checks it.
// DM_SCREEN_MATCH: "0" keeps every screen's mode; "vrr" also switches a variable-refresh screen (ProMotion) whose rate is a multiple of the game's to a fixed-rate mode at
// a multiple (120 → 60 Hz); unset (or anything else), only a screen whose rate is not a multiple of the game's (144 → 120 Hz) switches. A variable-refresh screen keeps its
// own mode by default and the 120 Hz link paces it (nvn_mtl.m link_vrr_120): on an M1 Pro the fixed 60 Hz mode about halved the GPU's throughput, Tears of the Kingdom
// 60 → 30 fps (docs/reference/frame-rate.md, The 60 Hz link at 40 fps in fullscreen on a ProMotion screen)
#pragma once
#include <math.h>
#include <string.h>
enum { SCREEN_MATCH_OFF, SCREEN_MATCH_FIXED, SCREEN_MATCH_VRR };
static inline int screen_match_mode(const char *env) { return !env ? SCREEN_MATCH_FIXED : !strcmp(env, "0") ? SCREEN_MATCH_OFF : !strcmp(env, "vrr") ? SCREEN_MATCH_VRR : SCREEN_MATCH_FIXED; }
static inline int screen_hz_multiple(double hz, int fps) { double k = round(hz / fps); return k >= 1 && fabs(hz / fps - k) < 0.01; }   // 59.94 Hz counts as 60
// 1: look for a mode at a multiple of want; 0: the current mode stays. hz: the current mode's rate (0 = not known); vrr: the current mode is variable-refresh
static inline int screen_match_wanted(int mode, int vrr, double hz, int want) {
    if (mode == SCREEN_MATCH_OFF || want <= 0) return 0;
    if (vrr && mode == SCREEN_MATCH_VRR) return 1;   // a fixed-rate mode though the rate may already be a multiple
    return hz > 0 && !screen_hz_multiple(hz, want);
}
