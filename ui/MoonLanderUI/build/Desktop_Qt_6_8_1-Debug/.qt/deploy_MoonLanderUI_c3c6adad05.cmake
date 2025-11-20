include(/home/gerd-lrt-dev/Code/codingForFun/moonlander/ui/MoonLanderUI/build/Desktop_Qt_6_8_1-Debug/.qt/QtDeploySupport.cmake)
include("${CMAKE_CURRENT_LIST_DIR}/MoonLanderUI-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE /home/gerd-lrt-dev/Code/codingForFun/moonlander/ui/MoonLanderUI/build/Desktop_Qt_6_8_1-Debug/MoonLanderUI
    GENERATE_QT_CONF
)
