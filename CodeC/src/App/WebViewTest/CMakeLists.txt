# 引入外部函数
include(${ROOT_DIR}/cmake/module.cmake)

# 获取绝对路径
set(AbsolutePathProject ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(ProjectName ${AbsolutePathProject} NAME)

# Qt 库
set(QT_LIBRARY_LIST "Core" "Gui" "Widgets" "Network" "WebEngineWidgets")

# 链接自身库
set(SELF_LIBRARY_LIST "LWebView")

# 创建项目
CreateTarget(${ProjectName} "Exe")
