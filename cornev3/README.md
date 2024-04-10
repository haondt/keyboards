Build instructions:

- copy the contents of this folder into 

```
~qmk_firmware/keyboards/crkbd/keymaps/haondt/
```

- edit keymap.c as necessary

- build new version

```shell
qmk compile -kb crkbd/rev1 -km haondt -e CONVERT_TO=helios
```

- navigate to `qmk_firmware` folder, there should be a new file called `crkbd_rev1_haondt_helios.uf2`. this is the new firmware file.

- disconnect the trs cable, and plug in the boards one at a time.
- for each board,
  - press the reset button for at least 500ms
  - the board will show up as a drive named `RPI-RP2`
  - copy the firmware file onto this drive. Both halves use the same firmware.


## Layers

- hold mod + sym to get to num layer

## Credits

- layer behavior is borrowed from [callum](https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum)
