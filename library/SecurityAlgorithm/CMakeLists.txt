include_directories(./include)

add_library(SecurityAlgorithm STATIC ./src/MD5.c
                                     ./src/Aes.c
                                     ./src/AES2.c
                                     ./src/AESECB.c
                                     ./src/AESCBC.c
                                     ./src/AESCFB.c
                                     ./src/AESOFB.c)

#add_executable(md5_file Demo/02_MD5File.cpp)
#target_link_libraries(md5_file SecurityAlgorithm)

#add_executable(md5_string Demo/01_MD5String.cpp)
#target_link_libraries(md5_string SecurityAlgorithm)

add_executable(ase_demo1 Demo/03_AES.cpp)
target_link_libraries(ase_demo1 SecurityAlgorithm)