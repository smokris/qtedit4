# testcase 3
# should output:

;#	NAME -> "tests/test1.pro"
;#	TARGET -> "test1"
;#	"SOURCES" -> ("file2.cpp")
;#	"HEADERS" -> ("file1.h", "file2.h")


TARGET=test3
TEMPLATE = app
HEADERS += file1.h file2.h
SOURCES += file1.cpp file2.cpp
SOURCES -= file1.cpp

QT+=debug