# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/espressif_ins/Espressif/frameworks/esp-idf-v5.2.1/components/bootloader/subproject"
  "D:/espressif_ins/Espressif/frameworks/esp-idf-v5.2.1/examples/get-started/Video_3/build/bootloader"
  "D:/espressif_ins/Espressif/frameworks/esp-idf-v5.2.1/examples/get-started/Video_3/build/bootloader-prefix"
  "D:/espressif_ins/Espressif/frameworks/esp-idf-v5.2.1/examples/get-started/Video_3/build/bootloader-prefix/tmp"
  "D:/espressif_ins/Espressif/frameworks/esp-idf-v5.2.1/examples/get-started/Video_3/build/bootloader-prefix/src/bootloader-stamp"
  "D:/espressif_ins/Espressif/frameworks/esp-idf-v5.2.1/examples/get-started/Video_3/build/bootloader-prefix/src"
  "D:/espressif_ins/Espressif/frameworks/esp-idf-v5.2.1/examples/get-started/Video_3/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/espressif_ins/Espressif/frameworks/esp-idf-v5.2.1/examples/get-started/Video_3/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/espressif_ins/Espressif/frameworks/esp-idf-v5.2.1/examples/get-started/Video_3/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
