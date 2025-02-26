INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# value types
SET(Intrepid2_TEST_ETI_VALUETYPE_NAME "")
SET(Intrepid2_TEST_ETI_VALUETYPE "")

LIST(APPEND Intrepid2_TEST_ETI_VALUETYPE_NAME "DOUBLE")
LIST(APPEND Intrepid2_TEST_ETI_VALUETYPE      "double")
LIST(APPEND Intrepid2_TEST_ETI_SACADO         "0")

LIST(LENGTH Intrepid2_TEST_ETI_VALUETYPE_NAME ETI_VALUETYPE_COUNT)
MATH(EXPR ETI_VALUETYPE_COUNT "${ETI_VALUETYPE_COUNT}-1")

# Host test
SET(Intrepid2_TEST_ETI_FILE "")
LIST(APPEND Intrepid2_TEST_ETI_FILE 
  "test_convergence_HEX"
  "test_convergence_QUAD"
  "test_convergence_TET"
  "test_convergence_TRI"
  "test_DeRham_commutativity_HEX"
  "test_DeRham_commutativity_QUAD"
  "test_DeRham_commutativity_TET"
  "test_DeRham_commutativity_TRI"
  "test_interpolation_projection_HEX"
  "test_interpolation_projection_QUAD"
  "test_interpolation_projection_TET"
  "test_interpolation_projection_TRI")

SET(Intrepid2_TEST_ETI_DEVICE_NAME "")
SET(Intrepid2_TEST_ETI_DEVICE "")
IF(Kokkos_ENABLE_SERIAL)
  LIST(APPEND Intrepid2_TEST_ETI_DEVICE_NAME "Serial")
  LIST(APPEND Intrepid2_TEST_ETI_DEVICE      "Kokkos::Device<Kokkos::Serial,Kokkos::HostSpace>")
ENDIF()
IF(Kokkos_ENABLE_OPENMP)
  LIST(APPEND Intrepid2_TEST_ETI_DEVICE_NAME "OpenMP")
  LIST(APPEND Intrepid2_TEST_ETI_DEVICE      "Kokkos::Device<Kokkos::OpenMP,Kokkos::HostSpace>")
ENDIF()
IF(Kokkos_ENABLE_CUDA)
  LIST(APPEND Intrepid2_TEST_ETI_DEVICE_NAME "CUDA")
  LIST(APPEND Intrepid2_TEST_ETI_DEVICE      "Kokkos::Device<Kokkos::Cuda,Kokkos::CudaSpace>")
ENDIF()
IF(Kokkos_ENABLE_HIP)
  LIST(APPEND Intrepid2_TEST_ETI_DEVICE_NAME "HIP")
  LIST(APPEND Intrepid2_TEST_ETI_DEVICE      "Kokkos::Device<Kokkos::Experimental::HIP,Kokkos::Experimental::HIPSpace>")
ENDIF()

LIST(LENGTH Intrepid2_TEST_ETI_DEVICE_NAME ETI_DEVICE_COUNT)
MATH(EXPR ETI_DEVICE_COUNT "${ETI_DEVICE_COUNT}-1")

IF (${ETI_DEVICE_COUNT} GREATER_EQUAL 0)
  FOREACH(I RANGE ${ETI_DEVICE_COUNT})
    LIST(GET Intrepid2_TEST_ETI_DEVICE_NAME ${I} ETI_DEVICE_NAME)
    LIST(GET Intrepid2_TEST_ETI_DEVICE      ${I} ETI_DEVICE)
    #MESSAGE(STATUS "Generating TEST ProjectionTools for ${ETI_DEVICE_NAME} with ${ETI_DEVICE}")
    FOREACH(J RANGE ${ETI_VALUETYPE_COUNT})
      LIST(GET Intrepid2_TEST_ETI_VALUETYPE_NAME ${J} ETI_VALUETYPE_NAME)
      LIST(GET Intrepid2_TEST_ETI_VALUETYPE      ${J} ETI_VALUETYPE)
      LIST(GET Intrepid2_TEST_ETI_SACADO         ${J} ETI_SACADO)
      FOREACH(ETI_FILE IN LISTS Intrepid2_TEST_ETI_FILE)
        SET(ETI_NAME "${ETI_FILE}_${ETI_DEVICE_NAME}_${ETI_VALUETYPE_NAME}")
        MESSAGE(STATUS "Generating TEST: ProjectionTools ${ETI_NAME}.cpp")
        CONFIGURE_FILE(eti/${ETI_FILE}_ETI.in ${ETI_NAME}.cpp)

        TRIBITS_ADD_EXECUTABLE_AND_TEST(
          ${ETI_NAME}
          SOURCES ${ETI_NAME}.cpp 
          ARGS PrintItAll
          NUM_MPI_PROCS 1
          PASS_REGULAR_EXPRESSION "TEST PASSED"
          ADD_DIR_TO_NAME
        )

      ENDFOREACH()
    ENDFOREACH()
  ENDFOREACH()
ENDIF()

