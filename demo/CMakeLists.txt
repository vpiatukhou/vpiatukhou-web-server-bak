add_executable(demo Main.cpp)

target_include_directories(demo PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(demo PRIVATE webserver)

#BEGIN release
install(TARGETS demo)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/config/application.json DESTINATION /etc/webserver/config/)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/ssl/ DESTINATION /etc/webserver/ssl/)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources/web_content/ DESTINATION /var/www/webserver/)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_FILE_NAME demo-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
#END
