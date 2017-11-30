var d6t     = require('../d6t').d6t;
var ref     = require('ref');
var sleep   = require('sleep')

// creating devh pointer used by d6t library
var d6t_devh = new d6t.d6t_devh_t();

// opening d6t device
d6t.d6t_open(d6t_devh.ref(), d6t.D6T_44L_06, null);
//d6t.d6t_open(d6t_devh.ref(), d6t.D6T_44L_06, '/dev/i2c-1');

while(1)
{
    sleep.sleep(1);

    // performing read, getting pointer to data from d6t device handle
    d6t.d6t_read(d6t_devh.ref());
    var dataptr = ref.reinterpret(d6t_devh.rdbuf, d6t_devh.bufsize, 0);

    // first word is temperature of the thermal sensor itself
    console.log("Thermal Sensor Temperature: %d", ref.get(dataptr, 0, ref.types.uint16) / 10);

    // all remaining words are pixel temperatures
    var temp;
    for (var i = 2; i < d6t_devh.bufsize-1; i = i + 2)
    {
        temp = ref.get(dataptr, i, ref.types.uint16) / 10;
        console.log("%d: %d", i, temp);
    }

    // last byte is error check code used for crc
    var pec = ref.get(dataptr, d6t_devh.bufsize-1, ref.types.uint8);
    console.log("pec: 0x%s", pec.toString(16));
}


d6t.d6t_close(d6t_devh.ref());

