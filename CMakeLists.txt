include_directories(
${CHOQOK_INCLUDES}
)

set( choqok_kili_us_SRCS
kili_us.cpp )

kde4_add_plugin(choqok_kili_us ${choqok_kili_us_SRCS})

target_link_libraries(choqok_kili_us ${KDE4_KIO_LIBS} ${QJSON_LIBRARY} choqok)

install(TARGETS choqok_kili_us DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES choqok_kili_us.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
 
