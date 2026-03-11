# Ergonomic and mnemonic keymap for writing code and stuff.
The keymap and some fancy effects for my keyboard (Aurora Sofle v2 from Splikb, not sponsored).

**Warning!** This is a personal project, I might make undocumented breaking changes at any time and offer no support.

Feel free to *borrow* whatever you want from this though.

![Photo](aurora_sofle.jpeg)

## Install
This project uses QMK's userspace feature to define the keymap and effects. Check out the instructions found in this [repo](https://github.com/qmk/qmk_userspace).

## Keymap
![Keymap](assets/aurora_sofle.svg)

The sent codes assume that the keyboard layout is set to German QWERTZ layout and will behave incorrectly otherwise.

### Features
- Colemak letter distribution.
- Symbols are arranged to make common characters and groups (e.g. '->', '>=', '<=', ':='...) easiest to type.
- Semantically similar symbols are grouped.
- 'Mute' and 'Media Play' buttons are rotary encoders with (cw, ccw) actions bound to (Volume Up/Down) and (Page Up/Down) respectively.
- German Dead Keys (´, ^, `) are also availible as regular keys on the symbol layer.
- Layer with arrow keys can be locked for prolonged navigation without holding a key.
- Numbers 1 and 0 are duplicate on the symbol layer for better access.

## OLED Displays
![OLED](assets/keyboard_display.gif)
- The four quarter circles display the current layer.
- The symbols beneath them show (left to right, top to bottom)
  - Shift
  - Layout (does nothing at the moment)
  - Alt
  - Super
  - Caps Lock
  - Ctrl
- The Celeste animation on the master side can be toggled to instead play the entire Bad Apple animation!
  - There is a flag to exclude it from compilation, the frames take more space then the rest of the firmware and effects combined...

## RGB Matrix effect
Custom effect composed of Starlight (value pulses with random phase offset) and vertically hue shifting at half frequency in a limited hue range, centered on blue.

## Source files
The [Pixelorama](https://pixelorama.org/) source files of the artworks on the OLED displays are included [here](assets), as well as the [script](assets/png2bytes.py) to turn the exported png into a C style array.
