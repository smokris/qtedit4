# testcase 2
# should output:

;#	NAME -> "tests/test1.pro"
;#	TARGET -> "test1"
;#	"SOURCES" -> ("file2_1.cpp", "file2_2.cpp")
;#	"HEADERS" -> ("file2_1.h"  , "file2_2.h")


TARGET=test2
TEMPLATE = app
HEADERS += file2_1.h   file2_2.h
SOURCES += file2_1.cpp file2_2.cpp
