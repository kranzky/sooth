require 'mkmf'

$CFLAGS << ' -Wall -O3 -std=gnu99'

create_makefile('sooth_native')
