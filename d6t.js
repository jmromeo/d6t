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

  'd6t_open'  : [ 'int',   [ d6t_devh_ptr_t, 'int' ] ],
  'd6t_close' : [ 'void',  [ d6t_devh_ptr_t        ] ],
  'd6t_read'  : [ 'int',   [ d6t_devh_ptr_t        ] ],

});

// adding devh type to d6t export
d6t.d6t_devh_t = d6t_devh_t;

// exporting d6t module
exports.d6t = d6t;
