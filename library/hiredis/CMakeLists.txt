include_directories(./include)

add_library(hiredis STATIC ./src/net.c
                           ./src/hiredis.c
                           ./src/async.c
                           ./src/sds.c
                           ./src/read.c)
