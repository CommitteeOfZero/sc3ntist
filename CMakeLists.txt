project(SCXParser)





#target_link_libraries(SCXParser)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SCXParser src/SCXParser.cpp src/parser/SC3Argument.cpp src/parser/SC3Expression.cpp src/parser/SGHDDisassembler.cpp src/parser/SC3BaseDisassembler.cpp src/parser/CCCharset.cpp src/parser/SC3StringDecoder.cpp src/parser/SCXFile.cpp src/parser/SC3CodeBlock.cpp src/parser/ZeroDisassembler.cpp src/parser/CCDisassembler.cpp src/parser/RNEDisassembler.cpp src/parser/RNECharset.cpp src/parser/SGHDCharset.cpp src/parser/SGHDCharset.h)
#target_link_libraries(SCXParser stdc++experimental)