# Omron D6T Thermal Sensor Node Library

## Getting Started

There are still a lot of things that need to be fixed, for example,
library is hard coded to use i2c-dev1, but basic functionality is
there. To get up and running quickly (assuming your device is hooked up
to i2c-dev1):

<ul>
  <li> cd examples/noweb </li>
  <li> node d6t_noweb.js </li>
</ul>

That's it! If everything goes smoothly, you should see temperature
values on standard out. If you would like to see this library used
in a nodejs express app that displays a thermal image map, see
[https://github.com/jmromeo/paradise-castle].

