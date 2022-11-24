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

Folding multipliers
-------------------

The folding multipliers with the default settings work as bipolar amplifiers
except that when the input signal exceeds the output range, the input signal
folds back on itself. Inputs a and b and the scale value are multiplied

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
- `##`: white noise
- `*1`: folding multiplier 1 output
- `*2`: folding multiplier 2 output
- `lp`: filter low-pass output
- `hp`: filter high-pass output
- `bp`: filter band-pass output
- `+1`: unit; simply a high signal
- `~>`: output 1 feedback
- `<~`: output 2 feedback

Matrix sinks
------------

- `~1`: oscillator 1 pitch
- `/1`: oscillator 1 phase offset
- `~2`: oscillator 2 pitch
- `/2`: oscillator 2 phase offset
- `a1`: folder 1 factor A
- `b1`: folder 1 factor B
- `a2`: folder 2 factor A
- `b2`: folder 2 factor B
- `/f`: filter cut-off frequency
- `>f`: filter input
- `~>`: output 1
- `<~`: output 2