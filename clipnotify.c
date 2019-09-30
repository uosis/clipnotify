#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void) {
    Display *disp = XOpenDisplay(NULL);
    if (!disp) {
        perror("Can't open X display");
        exit(1);
    }

    int event_base, error_base;
    assert(XFixesQueryExtension(disp, &event_base, &error_base));

    Window root = DefaultRootWindow(disp);

    Atom clipboard = XInternAtom(disp, "CLIPBOARD", False);

    XFixesSelectSelectionInput(disp, root, XA_PRIMARY, XFixesSetSelectionOwnerNotifyMask);
    XFixesSelectSelectionInput(disp, root, clipboard, XFixesSetSelectionOwnerNotifyMask);

    XEvent evt;

    while (True)
    {
        XNextEvent(disp, &evt);

        if (evt.type == event_base + XFixesSelectionNotify)
        {
            XFixesSelectionNotifyEvent *e = (XFixesSelectionNotifyEvent*)&evt;

            if (e->selection == XA_PRIMARY)
                printf("primary ");
            else if (e->selection == clipboard)
                printf("clipboard ");
            else {
                perror("Unknown selection");
                exit(1);        
            }

            printf("0x%08x\n", e->owner);
        } else {
            perror("Unknown event type");
            exit(1);
        }

        fflush(stdout);
    }
}
