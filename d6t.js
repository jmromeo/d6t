var ffi        = require('ffi');
var ref        = require('ref');
var StructType = require('ref-struct');

var d6t_devh_t = StructType({
    fd:         ref.types.int,
    sensor:     ref.types.int,
    rdbuf:      ref.refType(ref.types.uint8),
    bufsize:    ref.types.uint8
});

var d6t_devh_ptr_t = ref.refType(d6t_devh_t);

// importing d6t library
var d6t = ffi.Library('/usr/lib/libd6t', {

  'd6t_open'  : [ 'int',   [ d6t_devh_ptr_t, 'int' ] ],
  'd6t_close' : [ 'void',  [ d6t_devh_ptr_t        ] ],
  'd6t_read'  : [ 'void',  [ d6t_devh_ptr_t        ] ],

});

var d6t_devh = new d6t_devh_t();
d6t.d6t_open(d6t_devh.ref(), 0);

exports.d6t = d6t;
