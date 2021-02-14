cmake_minimum_required(VERSION 3.17)
project(Semester3Project)

set(CMAKE_CXX_STANDARD 14)

add_executable(Semester3Project main.cpp Menu.cpp Menu.h cpp/Citizen.cpp headers/Citizen.h cpp/Party.cpp headers/Party.h cpp/Round.cpp headers/Round.h cpp/CitizenMap.cpp headers/CitizenMap.h cpp/District.cpp headers/District.h cpp/RegularRound.cpp headers/RegularRound.h cpp/SimpleRound.cpp headers/SimpleRound.h headers/configs.h headers/DynamicArray.h cpp/DividedDistrict.cpp headers/DividedDistrict.h cpp/UnitedDistrict.cpp headers/UnitedDistrict.h)