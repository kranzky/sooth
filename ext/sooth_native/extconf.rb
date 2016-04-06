require 'mkmf'

$CFLAGS << ' -Wall -Ofast -std=gnu99'

create_makefile('sooth_native')
