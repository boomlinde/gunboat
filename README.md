gunboat
=======

Modular feedback modulation synthesizer

Basic controls
--------------

* Hover a slider to read a verbose description of its function in the bottom
  left corner
* Left click and drag a slider to change its value
* Right click a slider to restore it to its default value
* Middle click a slider to set it to its max value
* While hovering a slider, press backtick, 0 or number keys 1-9 to set the
  slider to an absolute value
* Press the f key to toggle integer scaling. Leaving it on looks nicer, but
  turning it off uses the window more efficiently
* Press alt+enter to toggle full screen mode
* Press ctrl+q to quit. OS specific application exit shortcuts should work as
  well

Folding multipliers
-------------------

The folding multipliers with the default settings work as bipolar amplifiers
except that when the input signal exceeds the output range, the input signal
folds back on itself. Inputs a and b and the scale value are multiplied

DC blocker
----------

Feedback networks can result in unpredictable low frequency sound and standing
waves. The audio outputs pass through a DC blocker to prevent this. The
cut-off frequency of the blocker can be set with a slider and ranges from 5 Hz
at its lowest setting to 100 Hz at its highest.

Matrix
------

In the patch matrix, each row represents a source (or an output) and each
column represents a sink (or an input). You connect a source to a sink by
increasing the value of the slider in the corresponding row and column. The
value of the slider represents a factor from 0 to 1.

Matrix sources
--------------

- `~1`: oscillator 1 output
- `~2`: oscillator 2 output
- `~3`: oscillator 3 output
- `##`: clocked random output
- `*1`: folding multiplier 1 output
- `*2`: folding multiplier 2 output
- `lp`: filter low-pass output
- `hp`: filter high-pass output
- `bp`: filter band-pass output
- `+1`: unit; simply a high signal
- `<>`: audio output feedback

Matrix sinks
------------

- `~1`: oscillator 1 pitch
- `/1`: oscillator 1 phase offset
- `~2`: oscillator 2 pitch
- `/2`: oscillator 2 phase offset
- `~3`: oscillator 3 pitch
- `/3`: oscillator 3 phase offset
- `#~`: clocked random clock speed
- `a1`: folder 1 factor A
- `b1`: folder 1 factor B
- `a2`: folder 2 factor A
- `b2`: folder 2 factor B
- `/f`: filter cut-off frequency
- `>f`: filter input
- `><`: audio output panning
- `<>`: audio output
