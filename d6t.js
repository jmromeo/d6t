var ffi        = require('ffi');
var ref        = require('ref');
var StructType = require('ref-struct');

// Omron D6T Device Handle
var d6t_devh_t = StructType({
    fd:         ref.types.int,                  // file descriptor
    sensor:     ref.types.int,                  // sensor type enum
    rdbuf:      ref.refType(ref.types.uint8),   // buffer where sensor data is stored during read
    bufsize:    ref.types.uint8                 // size of read buffer
});

var d6t_devh_ptr_t = ref.refType(d6t_devh_t);

// importing d6t library
var d6t = ffi.Library('/usr/lib/libd6t', {

  'd6t_open'  : [ 'int',   [ d6t_devh_ptr_t, 'int', 'string' ] ],
  'd6t_close' : [ 'void',  [ d6t_devh_ptr_t                  ] ],
  'd6t_read'  : [ 'int',   [ d6t_devh_ptr_t                  ] ],

});

// adding d6t wrappers to make calls to library cleaner
d6t.d6t_open_js = function(d6t_devh, sensor, i2c_devname)
{
    return d6t.d6t_open(d6t_devh.ref(), sensor, i2c_devname);
}
d6t.d6t_close_js = function(d6t_devh)
{
    d6t.d6t_close(d6t_devh.ref());
}
d6t.d6t_read_js = function(d6t_devh)
{
    var data = [];

    d6t.d6t_read(d6t_devh.ref());
    var dataptr = ref.reinterpret(d6t_devh.rdbuf, d6t_devh.bufsize, 0);

    // pushing all temperature but final byte
    for (var i = 0; i < d6t_devh.bufsize-1; i = i + 2)
    {
        data.push(ref.get(dataptr, i, ref.types.uint16) / 10);
    }

    // last byte is pec error check code used for crc
    data.push(ref.get(dataptr, d6t_devh.bufsize-1, ref.types.uint8));

    return data;
}

// adding devh type to d6t export
d6t.d6t_devh_t = d6t_devh_t;

// exporting d6t module
exports.d6t = d6t;
