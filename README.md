# Omron D6T Thermal Sensor Node Library

## Getting Started

This nodejs module is tested with the D6T_44L_06 module on
a beaglebone black. To get started, see example/d6t_withwrapper.js.
There are two examples, one that makes direct calls to the d6t c
library, and one that makes calls using wrapper functions. I
recommend the one using wrapper functions as the code is much
cleaner, but I left the ability to call the libraries directly as
those calls will be faster.

<ul>
  <li> cd examples </li>
  <li> node d6t_withwrapper.js </li>
</ul>

That's it! If everything goes smoothly, you should see temperature
values on standard out. If you would like to see this library used
in a nodejs express app that displays a thermal image map, see
https://github.com/jmromeo/paradise-castle.

