/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 8;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int usealtbar          = 1;        /* 1 means use non-dwm status bar */
static const char *altbarclass      = "Polybar"; /* Alternate bar class name */
static const char *alttrayname      = "tray";    /* Polybar tray instance name */
static const char *fonts[]          = { "Roboto Mono Medium for Powerline:style=Medium:size=10", 
										"JoyPixels:pixelsize=14:antialias=true:autohint=true", 
										"Font Awesome 5 Brands,Font Awesome 5 Brands Regular:style=Regular:size=10",
										"Font Awesome 5 Free,Font Awesome 5 Free Solid:style=Solid:size=10",
										"Font Awesome 5 Free,Font Awesome 5 Free Regular:style=Regular:size=10" };
static const char dmenufont[]       = "Roboto Mono Medium for Powerline:style=Medium:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#0C112F";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#081B3A";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* /1* tagging *1/ */
/* static const char *tags[] = { "", "", "", "", "", "", "", "", "" }; */

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      				instance    title       tags mask     isfloating   monitor */
	{ "Brave-browser",     		NULL,       NULL,       1 << 0,       0,           -1 },
	{ "firefox",  				NULL,       NULL,       1 << 0,       0,           -1 },
	{ "VSCodium",  				NULL,       NULL,       1 << 2,       0,           -1 },
	{ "jetbrains-pycharm-ce",  	NULL,       NULL,       1 << 3,       0,           -1 },
	{ "jetbrains-pycharm-ce",  	NULL,       "win0",     1 << 3,       1,           -1 },
	{ "zoom",  					NULL,       NULL,       1 << 5,       1,           -1 },
	{ "st",				NULL,       "ranger",   1 << 6,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] 		= { "dmenu_run", "-b", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  		= { "st", NULL };
static const char *vscodecmd[]  	= { "vscodium", NULL };
static const char *zoomcmd[]  		= { "zoom", NULL };
static const char *pycharmcmd[]  	= { "pycharm", NULL };
static const char *rangercmd[]  	= { "st", "-t", "lf", "-e", "lf", NULL };
static const char scratchpadname[] 	= "scratchpad";
static const char *scratchpadcmd[] 	= { "st", "-t", scratchpadname, "-o", "window.dimensions.columns=150", "-o", "window.dimensions.lines=30", NULL };
static const char *bravecmd[]  		= { "custom-brave", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ 0,                            XF86XK_AudioLowerVolume, 	spawn, 			SHCMD("sound_volume -;") },
	{ 0,                            XF86XK_AudioMute, 			spawn, 			SHCMD("sound_volume mute;") },
	{ 0,                            XF86XK_AudioRaiseVolume, 	spawn, 			SHCMD("sound_volume +;") },
	{ 0,                            XF86XK_MonBrightnessUp, 	spawn, 			SHCMD("brightness_level +") },
	{ 0,                            XF86XK_MonBrightnessDown, 	spawn, 			SHCMD("brightness_level -") },
    { MODKEY,                       XK_BackSpace,               spawn,          SHCMD("sysact") },
    { MODKEY|ShiftMask,             XK_w,                       spawn,          SHCMD("st -e sudo nmtui") },
    { MODKEY,                       XK_p,      					spawn,         	SHCMD("rofi -show drun") },
	{ MODKEY,             			XK_Return, 					spawn,         	{.v = termcmd } },
	{ MODKEY,                       XK_grave,  					togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY|ShiftMask,             XK_v,      					spawn,         	{.v = vscodecmd } },
	{ MODKEY|ShiftMask,             XK_z,      					spawn,         	{.v = zoomcmd } },
	{ MODKEY|ShiftMask,             XK_p,      					spawn,         	{.v = pycharmcmd } },
	{ MODKEY,                       XK_r,      					spawn,         	{.v = rangercmd } },
	{ MODKEY|ShiftMask,             XK_r,                       spawn,          SHCMD("st -e htop") },
    { MODKEY,                       XK_x,                       spawn,          SHCMD("st -e curl rate.sx") },
    { MODKEY,                       XK_n,                       spawn,          SHCMD("st -e nmtui") },
    { MODKEY,                       XK_w,      					spawn,         	{.v = bravecmd } },
    { MODKEY,                       XK_d,                       spawn,          {.v = dmenucmd } },
    { MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  spawn,               1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};

