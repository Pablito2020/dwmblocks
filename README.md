# dwmblocks

Modular status bar for `dwm` written in c.

## Installation

Enter the following command to build and install `dwmblocks` (if necessary as
root):

```sh
make clean install
```

## Running dwmblocks

Add the following line to your `.xinitrc` to start `dwmblocks` using startx:

```sh
exec dwm
```

## Configuration

The configuration of `dwmblocks` is done by creating a custom `config.h`
and (re)compiling the source code.

The statusbar is made from text output from commandline programs.

## Signalling changes

For example, the audio module has the update signal 10 by default.
Thus, running `pkill -RTMIN+10 dwmblocks` will update it.

## Remotes

```sh
git remote add upstream https://github.com/torrinfail/dwmblocks
```
