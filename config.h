/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx = 1;        /* border pixel of windows */
static unsigned int snap = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray = 1;     /* 0 means no systray */
static int showbar = 1;        /* 0 means no bar */
static int topbar = 1;        /* 0 means bottom bar */
static const int user_bh = 10;        /* 2 is the default spacing around the bar's font */
static char font[] = "monospace:size=10";
static char dmenufont[] = "monospace:size=10";
static const char *fonts[] = {font};
static char normbgcolor[] = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[] = "#bbbbbb";
static char selfgcolor[] = "#eeeeee";
static char selbordercolor[] = "#005577";
static char selbgcolor[] = "#005577";
static char *colors[][3] = {
        /*               fg           bg           border   */
        [SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor},
        [SchemeSel]  = {selfgcolor, selbgcolor, selbordercolor},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
        /* xprop(1):
         *	WM_CLASS(STRING) = instance, class
         *	WM_NAME(STRING) = title
         */
        /* class      instance    title tags mask           isfloating   monitor */
        {"Gimp",            NULL, NULL,                     0,      0, -1},
        {"Firefox",         NULL, NULL,                     0,      0, -1},
        {"ssh-askpass",     NULL, NULL,                     0,      1, -1},
        {"XCalc",           NULL, NULL,                     0,      1, -1},
        {"jetbrains-clion", NULL, "splash",                 0,      1, -1},
        {NULL,              NULL, "zoom ",                  1<<7,   1, -1},
        {NULL,              NULL, "Zoom Meeting",           1<<7,   0, -1},
        {NULL,              NULL, "Zoom Cloud Meetings",    1<<7,   0, -1},
        {"Thunderbird",     NULL, NULL,                     1<<6,   0, -1},
        {"TelegramDesktop", NULL, NULL,                     2,      0, -1},
        {"Skype",           NULL, NULL,                     2,      0, -1},
};

/* layout(s) */
static float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
        /* symbol     arrange function */
        {"[]=", tile},    /* first entry is default */
        {"[M]", monocle},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define HOLDKEY 0xffe4 // replace 0 with the keysym to activate holdbar

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", NULL};
static const char *termcmd[] = {"x-terminal-emulator", NULL};
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = {"x-terminal-emulator", "-t", scratchpadname, "-g", "120x34", NULL};

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
        {"font",            STRING,  &font},
        {"dmenufont",       STRING,  &dmenufont},
        {"normbgcolor",     STRING,  &normbgcolor},
        {"normbordercolor", STRING,  &normbordercolor},
        {"normfgcolor",     STRING,  &normfgcolor},
        {"selbgcolor",      STRING,  &selbgcolor},
        {"selbordercolor",  STRING,  &selbordercolor},
        {"selfgcolor",      STRING,  &selfgcolor},
        {"borderpx",        INTEGER, &borderpx},
        {"snap",            INTEGER, &snap},
        {"showbar",         INTEGER, &showbar},
        {"topbar",          INTEGER, &topbar},
        {"nmaster",         INTEGER, &nmaster},
        {"resizehints",     INTEGER, &resizehints},
        {"mfact",           FLOAT,   &mfact},
};

static const char *calculator[] = {"xcalc", NULL};
static const char *dwmreset[] = {"restart-dwm", NULL};
static const char *filecmd[] = {"x-terminal-emulator", "-e", "nnn", NULL};
static const char *fileman[] = {"pcmanfm", NULL};
static const char *helpmenu[] = {"dmenu_man", NULL};
static const char *vpnmenu[] = {"dmenu_nordvpn", NULL};
static const char *killcmd[] = {"xkill", NULL};
static const char *leave[] = {"quitmenu", NULL};
static const char *locker[] = {"slock", NULL};
static const char *mailman[] = {"thunderbird", NULL};
static const char *monitormenu[] = {"monitormenu", NULL};
static const char *musicp[] = {"x-terminal-emulator", "-e", "ncmpcpp", NULL};
static const char *netman[] = {"x-terminal-emulator", "-e", "nmtui", NULL};
static const char *pasmenu[] = {"passmenu", NULL};
static const char *procman[] = {"x-terminal-emulator", "-e", "htop", NULL};
static const char *screensh[] = {"flameshot", "gui", NULL};
static const char *searchmenu[] = {"selsearchmenu", NULL};
static const char *settings[] = {"lxappearance", NULL};
static const char *tmuxsessions[] = {"dmenux", NULL};
static const char *volman[] = {"x-terminal-emulator", "-e", "pulsemixer", NULL};

static const char *upvol[] = {"pulsemixer", "--change-volume", "+10", NULL};
static const char *dovol[] = {"pulsemixer", "--change-volume", "-10", NULL};
static const char *muvol[] = {"pulsemixer", "--toggle-mute", NULL};

static const char *mediaprev[] = {"mpc", "prev", NULL};
static const char *mediatoggle[] = {"mpc", "toggle", NULL};
static const char *medianext[] = {"mpc", "next", NULL};

#include <X11/XF86keysym.h>

static Key keys[] = {
        /* modifier                     key        function        argument */
        {MODKEY, XK_space, spawn, {.v = dmenucmd}},
        {MODKEY, XK_Return, spawn, {.v = termcmd}},
        {MODKEY, XK_grave, togglescratch, {.v = scratchpadcmd}},
        {MODKEY, XK_b, togglebar, {0}},
        {MODKEY, XK_j, focusstack, {.i = -1}},
        {MODKEY, XK_k, focusstack, {.i = +1}},
        {Mod1Mask, XK_Tab, focusstack, {.i = +1}},
        {Mod1Mask | ShiftMask, XK_Tab, focusstack, {.i = -1}},
        {MODKEY, XK_i, incnmaster, {.i = +1}},
        {MODKEY, XK_d, incnmaster, {.i = -1}},
        {MODKEY, XK_h, setmfact, {.f = -0.01}},
        {MODKEY, XK_l, setmfact, {.f = +0.01}},
        {MODKEY | ShiftMask, XK_Return, zoom, {0}},
        {MODKEY, XK_Tab, view, {0}},
        {MODKEY | ShiftMask, XK_c, killclient, {0}},
        {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
        {MODKEY, XK_m, setlayout, {.v = &layouts[1]}},
        {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
        {MODKEY, XK_0, view, {.ui = ~0}},
        {MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
        {MODKEY, XK_comma, focusmon, {.i = -1}},
        {MODKEY, XK_period, focusmon, {.i = +1}},
        {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
        {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
        TAGKEYS(XK_1, 0)
        TAGKEYS(XK_2, 1)
        TAGKEYS(XK_3, 2)
        TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4)
        TAGKEYS(XK_6, 5)
        TAGKEYS(XK_7, 6)
        TAGKEYS(XK_8, 7)
        TAGKEYS(XK_9, 8)
        {0, HOLDKEY, holdbar, {0}},
        {MODKEY | ShiftMask, XK_q, quit, {0}},
        /* app launchers */
        {MODKEY, XK_a, spawn, {.v = volman}},
        {MODKEY, XK_e, spawn, {.v = fileman}},
        {MODKEY | ShiftMask, XK_e, spawn, {.v = filecmd}},
        {MODKEY | ShiftMask, XK_r, spawn, {.v = dwmreset}},
        {MODKEY, XK_o, spawn, {.v = monitormenu}},
        {MODKEY, XK_p, spawn, {.v = pasmenu}},
        {MODKEY, XK_n, spawn, {.v = netman}},
        {MODKEY, XK_s, spawn, {.v = searchmenu}},
        {MODKEY, XK_u, spawn, {.v = musicp}},
        {MODKEY, XK_v, spawn, {.v = vpnmenu}},
        {MODKEY, XK_w, spawn, {.v = tmuxsessions}},
        {MODKEY, XK_F1, spawn, {.v = helpmenu}},
        {MODKEY, XK_Delete, spawn, {.v = locker}},
        {MODKEY, XK_BackSpace, spawn, {.v = killcmd}},
        {MODKEY, XK_Escape, spawn, {.v = leave}},
        {ControlMask, XK_Escape, spawn, {.v = settings}},
        {ControlMask | ShiftMask, XK_Escape, spawn, {.v = procman}},
        {0, XK_Print, spawn, {.v = screensh}},

        {0, XF86XK_Calculator, spawn, {.v = termcmd}},
        {0, XF86XK_HomePage, spawn, {.v = fileman}},
        {0, XF86XK_Tools, spawn, {.v = calculator}},
        {0, XF86XK_Mail, spawn, {.v = mailman}},
        {0, XF86XK_Search, spawn, {.v = searchmenu}},

        {0, XF86XK_AudioLowerVolume, spawn, {.v = dovol}},
        {0, XF86XK_AudioRaiseVolume, spawn, {.v = upvol}},
        {0, XF86XK_AudioMute, spawn, {.v = muvol}},

        {0, XF86XK_AudioPrev, spawn, {.v = mediaprev}},
        {0, XF86XK_AudioPlay, spawn, {.v = mediatoggle}},
        {0, XF86XK_AudioNext, spawn, {.v = medianext}},

        {ShiftMask, XF86XK_AudioLowerVolume, spawn, {.v = mediaprev}},
        {ShiftMask, XF86XK_AudioRaiseVolume, spawn, {.v = medianext}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
        /* click                event mask      button          function        argument */
        {ClkLtSymbol,   0,     Button1, setlayout,      {0}},
        {ClkLtSymbol,   0,     Button3, setlayout,      {.v = &layouts[2]}},
        {ClkWinTitle,   0,     Button2, zoom,           {0}},
        {ClkStatusText, 0,     Button2, spawn,          {.v = termcmd}},
        {ClkClientWin, MODKEY, Button1, movemouse,      {0}},
        {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
        {ClkClientWin, MODKEY, Button3, resizemouse,    {0}},
        {ClkTagBar,     0,     Button1, view,           {0}},
        {ClkTagBar,     0,     Button3, toggleview,     {0}},
        {ClkTagBar,    MODKEY, Button1, tag,            {0}},
        {ClkTagBar,    MODKEY, Button3, toggletag,      {0}},
};

