
#### 文件夹 Core   ###############################################################

# 构建CPlusPlusReflection动态库和静态库
# add_subdirectory(CPlusPlusReflection)
# set_property(TARGET CPlusPlusReflection PROPERTY FOLDER "Core")

#### 文件夹 Base   ###############################################################

# 单例基类封装
add_subdirectory(LSingleton)
set_property(TARGET LSingleton PROPERTY FOLDER "Base")

add_subdirectory(LBaseCommon)
set_property(TARGET LBaseCommon PROPERTY FOLDER "Base")

add_subdirectory(LWebView)
set_property(TARGET LWebView PROPERTY FOLDER "Base")

# add_subdirectory(QtJsWebEngine)
# set_property(TARGET QtJsWebEngine PROPERTY FOLDER "Base")

add_subdirectory(LUserDialog)
set_property(TARGET LUserDialog PROPERTY FOLDER "Base")

add_subdirectory(LBaseGrap)
set_property(TARGET LBaseGrap PROPERTY FOLDER "Base")

#### 文件夹 Lib   ###############################################################


#### 文件夹 plugins   ###############################################################

# 构建MyPlugin动态库
# add_subdirectory(MyPlugin)
# set_property(TARGET MyPlugin PROPERTY FOLDER "plugins")


#### 文件夹 Tools   ###############################################################

# 构建main可执行文件
# add_subdirectory(main)
# set_property(TARGET main PROPERTY FOLDER "Tools")

# add_subdirectory(TestEventBus)
# set_property(TARGET TestEventBus PROPERTY FOLDER "Tools")


# add_subdirectory(HRNetLinkTopDiaPlugin)
# set_property(TARGET HRNetLinkTopDiaPlugin PROPERTY FOLDER "Tools")

# add_subdirectory(GDALTest)
# set_property(TARGET GDALTest PROPERTY FOLDER "Tools")


add_subdirectory(CentralizedControlSystem)
set_property(TARGET CentralizedControlSystem PROPERTY FOLDER "Tools")


