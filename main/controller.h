#pragma once
#include <Bluepad32.h>

namespace robotathon::controller {

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


//
// README FIRST, README FIRST, README FIRST
//
// Bluepad32 has a built-in interactive console.
// By default it is enabled (hey, this is a great feature!).
// But it is incompatible with Arduino "Serial" class.
//
// Instead of using "Serial" you can use Bluepad32 "Console" class instead.
// It is somewhat similar to Serial but not exactly the same.
//
// Should you want to still use "Serial", you have to disable the Bluepad32's console
// from "sdkconfig.defaults" with:
//    CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE=n

GamepadPtr gamepads[BP32_MAX_GAMEPADS]{};


// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedGamepad(GamepadPtr gp) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (!gamepads[i]) {
            Console.printf("CALLBACK: Gamepad is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            GamepadProperties properties = gp->getProperties();
            Console.printf("Gamepad model: %s, VID=0x%04x, PID=0x%04x\n", gp->getModelName(), properties.vendor_id,
                        properties.product_id);
            gamepads[i] = gp;
            return;
        }
    }
    Console.println("CALLBACK: Gamepad connected, but could not found empty slot");
}


void onDisconnectedGamepad(GamepadPtr gp) {
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (gamepads[i] == gp) {
            Console.printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
            gamepads[i] = nullptr;
            return;
        }
    }
    Console.println("CALLBACK: Gamepad disconnected, but not found in gamepads");
}


void setup() {
    // Console.printf("Firmware: %s\n", BP32.firmwareVersion());

    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But might also fix some connection / re-connection issues.
    // BP32.forgetBluetoothKeys();
}


void loop() {
    // This call fetches all the gamepad info from the NINA (ESP32) module.
    // Just call this function in your main loop.
    // The gamepads pointer (the ones received in the callbacks) gets updated
    // automatically.
    BP32.update();

    // It is safe to always do this before using the gamepad API.
    // This guarantees that the gamepad is valid and connected.
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        GamepadPtr gamepad = gamepads[i];
        if (gamepad && gamepad->isConnected())
            controller::read(gamepad);
    }
}

}