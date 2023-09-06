add_subdirectory(main)
set_property(TARGET main PROPERTY FOLDER "App")

add_subdirectory(study)
set_property(TARGET study PROPERTY FOLDER "App")

# add_subdirectory(TestProto)
# set_property(TARGET TestProto PROPERTY FOLDER "App")

add_subdirectory(WebViewTest)
set_property(TARGET WebViewTest PROPERTY FOLDER "App")



