cmake_minimum_required(VERSION 3.12)
project(AlgoritmosP3 C)

set(CMAKE_C_STANDARD 99)

add_executable(AlgoritmosP3 main.c
        dicotomia/dicotomia.c
        dicotomia/d_vector/persist_to_txt/readWriteData_v.c
        dicotomia/d_hash_tables/persist_to_txt/readWriteData_h.c
        dicotomia/configDico.c
        dicotomia/d_vector/test_d_vector/test_v.c
        dicotomia/d_vector/d_vector.c
        dicotomia/d_hash_tables/d_hash.c)