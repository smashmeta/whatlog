
message ("whatlog directory ${whatlog_DIR}")

set(whatlog_INCLUDE_DIRS ${whatlog_DIR}/include)
set(whatlog_LIBRARY ${whatlog_DIR}/build/$<CONFIG>/whatlog.lib)
set(whatlog_LIBRARIES ${whatlog_DIR}/build/$<CONFIG>/whatlog.lib)

message ("whatlog include dir ${whatlog_INCLUDE_DIRS}")
message ("whatlog library dir ${whatlog_LIBRARIES}")