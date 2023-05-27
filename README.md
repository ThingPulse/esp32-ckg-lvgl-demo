# ThingPulse Color Kit Grande LVGL Demo

This demo shows how to develop applications for the [ThingPulse Color Kit Grande](https://thingpulse.com/product/esp32-wifi-color-display-kit-grande/) 
by using the LVGL UI library. By switching the environment in platformio.ini
you can also compile and run the code locally. The UI will then be rendered
in a window on your local machine.

![LVGL Demo on the ThingPulse Color Kit Grande](/documentation/CKG_LVGL_DEMO.jpeg)

![LVGL Demo running natively on Mac OS X](/documentation/SDL_LVGL_Demo.png)

## Changing targets

Change the default target by uncommenting the right environment in platformio.ini
```ini
default_envs = emulator_64bits
;default_envs = thingpulse-color-kit-grande
```
