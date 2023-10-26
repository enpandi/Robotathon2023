#include <ArduinoGamepad.h>

namespace Robotathon {

struct Controller {
    class Button {
        bool wasDown{}, isDown{};
        using Function = void (*)();
    public:
        Function onUp{}, onUpDown{}, onDown{}, onDownUp{};
        void read(bool down) {
            wasDown = isDown;
            isDown = down;
            if (wasDown != isDown) {
                Function func = down ? onUpDown : onDownUp;
                if (func) func();
            }
            {
                Function func = down ? onDown : onUp;
                if (func) func();
            }
        }
    };
    Button
        du, dd, dl, dr,       // dpad
        a, b, x, y,           // A B X Y
        l, r, zl, zr, sl, sr, // bumpers, triggers, sticks
        m, p, /*t,*/ h; /*o*/ // - + T H O
        // disable T because controller uses it for turbo mode and vibration intensity
        // disable O because it doesn't register?
    float lx, ly, rx, ry; // joysticks
    void read(GamepadPtr gamepad) {
        du.read(gamepad->dpad() & DPAD_UP);
        dd.read(gamepad->dpad() & DPAD_DOWN);
        dl.read(gamepad->dpad() & DPAD_LEFT);
        dr.read(gamepad->dpad() & DPAD_RIGHT);
        a.read(gamepad->b()); // bruh
        b.read(gamepad->a()); // bruh
        x.read(gamepad->y()); // bruh
        y.read(gamepad->x()); // bruh
        l.read(gamepad->l1());
        r.read(gamepad->r1());
        zl.read(gamepad->l2());
        zr.read(gamepad->r2());
        sl.read(gamepad->thumbL());
        sr.read(gamepad->thumbR());
        m.read(gamepad->miscBack());
        p.read(gamepad->miscHome()); // bruh
        h.read(gamepad->miscSystem()); // bruh
        lx = gamepad->axisX() / 512.0f;
        ly = gamepad->axisY() / -512.0f;
        rx = gamepad->axisRX() / 512.0f;
        ry = gamepad->axisRY() / -512.0f;
    }
};

}