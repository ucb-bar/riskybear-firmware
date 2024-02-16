# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/naichenzhao/esp/esp-idf/components/bootloader/subproject"
  "/Users/naichenzhao/Desktop/SLICE/SLICE-Spider/Firmware/V1/spider_sub/build/bootloader"
  "/Users/naichenzhao/Desktop/SLICE/SLICE-Spider/Firmware/V1/spider_sub/build/bootloader-prefix"
  "/Users/naichenzhao/Desktop/SLICE/SLICE-Spider/Firmware/V1/spider_sub/build/bootloader-prefix/tmp"
  "/Users/naichenzhao/Desktop/SLICE/SLICE-Spider/Firmware/V1/spider_sub/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/naichenzhao/Desktop/SLICE/SLICE-Spider/Firmware/V1/spider_sub/build/bootloader-prefix/src"
  "/Users/naichenzhao/Desktop/SLICE/SLICE-Spider/Firmware/V1/spider_sub/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/naichenzhao/Desktop/SLICE/SLICE-Spider/Firmware/V1/spider_sub/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/naichenzhao/Desktop/SLICE/SLICE-Spider/Firmware/V1/spider_sub/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
