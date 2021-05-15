/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 5;       /* snap pixel */
static const int showbar            = 0;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Lato:size=9" };
static const char col_gray1[]       = "#000000";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#300a24";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { "#ffffff", col_gray1, col_gray2 },
	[SchemeSel]  = { "#ffffff", "#1a1e21",  "#dd722e"  },
	[SchemeHid]  = { "#ff0000",  col_gray1, col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance    title       tags mask     isfloating   monitor */
	{ "Gimp",           NULL,       NULL,       0,            0,           -1 },
	{ "Firefox",        NULL,       NULL,       0,            0,           -1 },
	{ "ssh-askpass",    NULL,       NULL,       0,            1,           -1 },
	{ "XCalc",          NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define HOLDKEY 0xffe4 // replace 0 with keysym to activate holdbar

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "j4-dmenu-desktop", NULL };
static const char *termcmd[]  = { "x-terminal-emulator", NULL };

static const char *calculator[]     = { "x-terminal-emulator", "qalc", NULL };
static const char *comptontgl[]     = { "toggle-compton", NULL };
static const char *dwmreset[]       = { "restart-dwm", NULL };
static const char *filecmd[]        = { "x-terminal-emulator", "-e", "ranger", NULL };
static const char *fileman[]        = { "pcmanfm-qt", NULL };
static const char *helpmenu[]       = { "dmenu_man", NULL };
static const char *vpnmenu[]       = { "dmenu_nordvpn", NULL };
static const char *killcmd[]        = { "xkill", NULL };
static const char *leave[]          = { "quitmenu", NULL };
static const char *locker[]         = { "slock", NULL };
static const char *mailman[]        = { "thunderbird", NULL };
static const char *monitormenu[]    = { "monitormenu", NULL };
static const char *musicp[]         = { "x-terminal-emulator", "-e", "ncmpcpp", NULL };
static const char *netman[]         = { "x-terminal-emulator", "-e", "nmtui", NULL };
static const char *pasmenu[]        = { "passmenu", NULL };
static const char *procman[]        = { "x-terminal-emulator", "-e", "htop", NULL };
static const char *screensh[]       = { "flameshot", "gui", NULL };
static const char *searchmenu[]     = { "selsearchmenu", NULL };
static const char *settings[]       = { "lxqt-config", NULL };
static const char *volman[]         = { "x-terminal-emulator", "-e", "pulsemixer", NULL };
static const char *webmenu[]        = { "dmenu_websearch", NULL };
static const char *wwwbrowser[]     = { "x-www-browser", NULL };

static const char *upvol[]          = {"pulsemixer", "--change-volume", "+10", NULL };
static const char *dovol[]          = {"pulsemixer", "--change-volume", "-10", NULL };
static const char *muvol[]          = {"pulsemixer", "--toggle-mute", NULL };

static const char *mediaprev[]      = {"mpc", "prev", NULL };
static const char *mediatoggle[]    = {"mpc", "toggle", NULL };
static const char *medianext[]      = {"mpc", "next", NULL };

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = +1 } },
	{ Mod1Mask,                     XK_Tab,    focusstack,     {.i = +1 } },
	{ Mod1Mask|ShiftMask,           XK_Tab,    focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.01} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.01} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ 0,            				HOLDKEY,   holdbar,	        {0} },
    { MODKEY|ShiftMask,             XK_q,       quit,           {0} },
	/* app launchers */
    { MODKEY,                       XK_a,      spawn,          {.v = volman } },
    { MODKEY,                       XK_c,      spawn,          {.v = calculator } },
    { MODKEY,                       XK_e,      spawn,          {.v = fileman } },
    { MODKEY|ShiftMask,             XK_e,      spawn,          {.v = filecmd } },
    { MODKEY|ShiftMask,             XK_r,      spawn,          {.v = dwmreset} },
    { MODKEY,                       XK_o,      spawn,          {.v = monitormenu } },
    { MODKEY,                       XK_p,      spawn,          {.v = pasmenu } },
    { MODKEY,                       XK_n,      spawn,          {.v = netman } },
    { MODKEY,                       XK_s,      spawn,          {.v = searchmenu } },
    { MODKEY,                       XK_u,      spawn,          {.v = musicp } },
    { MODKEY,                       XK_v,      spawn,          {.v = vpnmenu } },
    { MODKEY,                       XK_w,      spawn,          {.v = webmenu } },
    { MODKEY,                       XK_F1,     spawn,          {.v = helpmenu } },
    { MODKEY,                       XK_Delete, spawn,          {.v = locker } },
    { MODKEY,                       XK_BackSpace, spawn,       {.v = killcmd } },
    { MODKEY,                       XK_Escape, spawn,          {.v = leave } },
    { ControlMask,                  XK_Escape, spawn,          {.v = settings } },
    { ControlMask|ShiftMask,        XK_Escape, spawn,          {.v = procman } },
    { 0,                            XK_Print,  spawn,          {.v = screensh } },

    { MODKEY,                       XK_F12,    spawn,          {.v = comptontgl } },

    { 0,                            XF86XK_Calculator,          spawn, {.v = termcmd } },
    { 0,                            XF86XK_HomePage,            spawn, {.v = fileman } },
    { 0,                            XF86XK_Tools,               spawn, {.v = wwwbrowser } },
    { 0,                            XF86XK_Mail,                spawn, {.v = mailman } },
    { 0,                            XF86XK_Search,              spawn, {.v = webmenu } },

    { 0,                            XF86XK_AudioLowerVolume,    spawn, {.v = dovol } },
    { 0,                            XF86XK_AudioRaiseVolume,    spawn, {.v = upvol } },
    { 0,                            XF86XK_AudioMute,           spawn, {.v = muvol } },

    { 0,                            XF86XK_AudioPrev,           spawn, {.v = mediaprev } },
    { 0,                            XF86XK_AudioPlay,           spawn, {.v = mediatoggle } },
    { 0,                            XF86XK_AudioNext,           spawn, {.v = medianext } },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        killclient,     {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

