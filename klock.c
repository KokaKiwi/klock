#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <crypt.h>
#include <shadow.h>
#include <pwd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define lock_width 28
#define lock_height 40
#define lock_x_hot 14
#define lock_y_hot 21
static char lock_bits[] =
{
    0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xf8, 0xff, 0x7f, 0x00, 0xe0, 0xff,
    0x3f, 0x00, 0xc0, 0xff, 0x1f, 0x00, 0x80, 0xff, 0x0f, 0xfc, 0x03, 0xff,
    0x0f, 0xfe, 0x07, 0xff, 0x0f, 0xff, 0x0f, 0xff, 0x07, 0xff, 0x0f, 0xfe,
    0x87, 0xff, 0x1f, 0xfe, 0x87, 0xff, 0x1f, 0xfe, 0x87, 0xff, 0x1f, 0xfe,
    0x87, 0xff, 0x1f, 0xfe, 0x87, 0xff, 0x1f, 0xfe, 0x87, 0xff, 0x1f, 0xfe,
    0x87, 0xff, 0x1f, 0xfe, 0x87, 0xff, 0x1f, 0xfe, 0x87, 0xff, 0x1f, 0xfe,
    0x87, 0xff, 0x1f, 0xfe, 0x01, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0xf8,
    0x01, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0xf8, 0x01, 0xf0, 0x00, 0xf8,
    0x01, 0xf8, 0x01, 0xf8, 0x01, 0xf8, 0x01, 0xf8, 0x01, 0xf8, 0x01, 0xf8,
    0x01, 0xf8, 0x01, 0xf8, 0x01, 0xf0, 0x00, 0xf8, 0x01, 0x60, 0x00, 0xf8,
    0x01, 0x60, 0x00, 0xf8, 0x01, 0x60, 0x00, 0xf8, 0x01, 0x60, 0x00, 0xf8,
    0x01, 0x60, 0x00, 0xf8, 0x01, 0x60, 0x00, 0xf8, 0x01, 0x00, 0x00, 0xf8,
    0x01, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0xf8, 0x01, 0x00, 0x00, 0xf8,
    0xff, 0xff, 0xff, 0xff
};

#define mask_width 28
#define mask_height 40
#define mask_x_hot 14
#define mask_y_hot 21
// static char mask_bits[] =
// {
//    0x00, 0xfe, 0x07, 0x00, 0x80, 0xff, 0x1f, 0x00, 0xc0, 0xff, 0x3f, 0x00,
//    0xe0, 0xff, 0x7f, 0x00, 0xf0, 0xff, 0xff, 0x00, 0xf8, 0xff, 0xff, 0x01,
//    0xf8, 0x03, 0xfc, 0x01, 0xf8, 0x01, 0xf8, 0x01, 0xfc, 0x01, 0xf8, 0x03,
//    0xfc, 0x00, 0xf0, 0x03, 0xfc, 0x00, 0xf0, 0x03, 0xfc, 0x00, 0xf0, 0x03,
//    0xfc, 0x00, 0xf0, 0x03, 0xfc, 0x00, 0xf0, 0x03, 0xfc, 0x00, 0xf0, 0x03,
//    0xfc, 0x00, 0xf0, 0x03, 0xfc, 0x00, 0xf0, 0x03, 0xfc, 0x00, 0xf0, 0x03,
//    0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f,
//    0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f,
//    0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f,
//    0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f,
//    0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f,
//    0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f,
//    0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f, 0xff, 0xff, 0xff, 0x0f,
//    0xff, 0xff, 0xff, 0x0f
// };
static char mask_bits[] =
{
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00
};

#define MAXGOODWILL 150000
#define GOODWILLPORTION 0.3
#define TIMEOUTPERATTEMPT 30000

int main(int argc, char **argv)
{
    Display *d;
    Window w;
    XSetWindowAttributes attr;
    Pixmap csr_source, csr_mask;
    XColor csr_fg, csr_bg, dummy;
    int clen, rlen = 0;
    char cbuf[10], rbuf[128];
    XEvent ev;
    Cursor cursor;
    Bool running = True;
    double timeout = 0, goodwill = 150000;
    uid_t uid = getuid();
    struct spwd *spw = getspnam(getenv("USER"));
    KeySym ks;

    endspent();

    if (spw == NULL)
    {
        fprintf(stderr, "klock: not rooted user.\n");
        exit(1);
    }

    d = XOpenDisplay(0);
    if (d == NULL)
    {
        fprintf(stderr, "klock: cannot open display.\n");
        exit(1);
    }

    attr.override_redirect = True;
    w = XCreateWindow(d, DefaultRootWindow(d), 0, 0, 1, 1, 0,
                        CopyFromParent, InputOnly, CopyFromParent,
                        CWOverrideRedirect, &attr);

    XSelectInput(d, w, KeyPressMask | KeyReleaseMask);

    csr_source = XCreateBitmapFromData(d, w, lock_bits, lock_width, lock_height);
    csr_mask = XCreateBitmapFromData(d, w, mask_bits, mask_width, mask_height);

    if (XAllocNamedColor(d, DefaultColormap(d, DefaultScreen(d)), "steelblue3", &dummy, &csr_bg) == 0)
        XAllocNamedColor(d, DefaultColormap(d, DefaultScreen(d)), "black", &dummy, &csr_bg);

    if (XAllocNamedColor(d, DefaultColormap(d, DefaultScreen(d)), "grey25", &dummy, &csr_fg) == 0)
        XAllocNamedColor(d, DefaultColormap(d, DefaultScreen(d)), "white", &dummy, &csr_fg);

    cursor = XCreatePixmapCursor(d, csr_source, csr_mask, &csr_fg, &csr_bg, lock_x_hot, lock_y_hot);

    XMapWindow(d, w);

    if (XGrabKeyboard(d, w, False, GrabModeAsync, GrabModeAsync, CurrentTime) != GrabSuccess)
    {
        fprintf(stderr, "klock: failed to grab keyboard.\n");
        exit(1);
    }

    if (XGrabPointer(d, w, False, (KeyPressMask | KeyReleaseMask) &0, GrabModeAsync, GrabModeAsync, None, cursor, CurrentTime) != GrabSuccess)
    {
        XUngrabKeyboard(d, CurrentTime);
        fprintf(stderr, "klock: failed to grab pointer.\n");
        exit(1);
    }

    while (running == True)
    {
        XNextEvent(d, &ev);
        switch (ev.type)
        {
            case KeyPress:
                if (ev.xkey.time < timeout)
                {
                    XBell(d, 0);
                    break;
                }
                clen = XLookupString(&(ev.xkey), cbuf, 9, &ks, 0);
                switch (ks)
                {
                    case XK_Escape:
                    case XK_Clear:
                        rlen = 0;
                        break;

                    case XK_Delete:
                    case XK_BackSpace:
                        if (rlen > 0)
                            rlen--;
                        break;

                    case XK_Linefeed:
                    case XK_Return:
                    case XK_KP_Enter:
                        if (rlen == 0)
                            break;
                        rbuf[rlen] = 0;
                        char *encrypted = crypt(rbuf, spw->sp_pwdp);
                        if (strcmp(encrypted, spw->sp_pwdp) == 0)
                        {
                            running = False;
                            break;
                        }
                        XBell(d, 0);
                        rlen = 0;
                        if (timeout)
                        {
                            goodwill += ev.xkey.time - timeout;
                            if (goodwill > MAXGOODWILL)
                                goodwill = MAXGOODWILL;
                        }
                        timeout = -goodwill * GOODWILLPORTION;
                        goodwill += timeout;
                        timeout += ev.xkey.time + TIMEOUTPERATTEMPT;
                        break;

                    default:
                        if (clen != 1)
                            break;

                        if (rlen < (sizeof(rbuf) - 1))
                        {
                            rbuf[rlen] = cbuf[0];
                            rlen++;
                        }
                        break;
                }
                break;
            default:
                break;
        }
    }

    XUngrabKeyboard(d, CurrentTime);
    XUngrabPointer(d, CurrentTime);

    return EXIT_SUCCESS;
}
