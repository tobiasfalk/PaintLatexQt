add_library(PaintLatexQt SHARED
  PaintLatexQt_global.h
  paintlatexqt.cpp
  paintlatexqtfgetsetter.cpp
  paintlatexqt.h
)

target_link_libraries(PaintLatexQt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(PaintLatexQt PRIVATE PAINTLATEXQT_LIBRARY)

target_include_directories(PaintLatexQt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
