add_executable(${PROJECT_NAME}
	main.cpp
)

set_target_properties(${PROJECT_NAME} 
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
	${CMAKE_BINARY_DIR}/bin
)
