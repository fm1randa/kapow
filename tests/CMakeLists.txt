include_directories(../src)

# Test filters
qt6_add_executable(test_filters
	test_filters/test_filters.cpp
	../src/filter_model.cpp
	../src/session.cpp
	../src/session_model.cpp
)
target_link_libraries(test_filters PRIVATE Qt6::Core Qt6::Test)
add_test(NAME test_filters COMMAND test_filters)

# Test sessions
qt6_add_executable(test_sessions
	test_sessions/test_sessions.cpp
	../src/session.cpp
	../src/session_model.cpp
)
target_link_libraries(test_sessions PRIVATE Qt6::Core Qt6::Test)
add_test(NAME test_sessions COMMAND test_sessions)

# Test project
qt6_add_executable(test_project
	test_project/test_project.cpp
	../src/filter_model.cpp
	../src/project.cpp
	../src/session.cpp
	../src/session_model.cpp
)
target_link_libraries(test_project PRIVATE Qt6::Core Qt6::Widgets Qt6::Test)
add_test(NAME test_project COMMAND test_project)
set_tests_properties(test_project PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

# Test start time editor
qt6_add_executable(test_start_time_editor
	test_start_time_editor/test_start_time_editor.cpp
	../src/date_editor.cpp
	../src/start_time_editor.cpp
	../src/time_editor.cpp
)
target_link_libraries(test_start_time_editor PRIVATE Qt6::Core Qt6::Widgets Qt6::Test)
add_test(NAME test_start_time_editor COMMAND test_start_time_editor)
set_tests_properties(test_start_time_editor PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
