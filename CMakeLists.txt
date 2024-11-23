# CMakeの使い方
# 1. プロジェクトディレクトリでmkdir build
# 2. cd build
# 3. cmake ..
# 4. make install

# 最小CMakeバージョンを指定
cmake_minimum_required(VERSION 3.10)

# プロジェクト名とバージョンを指定
project(GHOST VERSION 1.0 LANGUAGES CXX)

# 必要な外部ライブラリをすべて探す
# OpenALを探す
find_package(OpenAL REQUIRED)

find_package(glfw3 REQUIRED)  # GLFWを探す

# GLEWを探す
find_package(GLEW REQUIRED)

# Freetypeを探す
find_package(Freetype REQUIRED)

# 使用するコンパイラの指定（オプション）
set(CMAKE_CXX_STANDARD 17)  # C++17を使用

# ソースファイルを自動取得（カレントディレクトリ内のすべての .cpp ファイルを取得）
file(GLOB_RECURSE SOURCE_CPP_FILES "*.cpp")

# ソースファイルを自動取得（カレントディレクトリ内のすべての .c ファイルを取得）
file(GLOB_RECURSE SOURCE_C_FILES "*.c")

# ソースファイルを自動取得（カレントディレクトリ内のすべての .hpp ファイルを取得）
file(GLOB_RECURSE HEADER_CPP_FILES "*.hpp")

# ソースファイルを自動取得（カレントディレクトリ内のすべての .h ファイルを取得）
file(GLOB_RECURSE HEADER_C_FILES "*.h")

# ビルドディレクトリ（CMAKE_BINARY_DIR）を除外
list(FILTER SOURCE_CPP_FILES EXCLUDE REGEX "${CMAKE_BINARY_DIR}/.*")
list(FILTER SOURCE_C_FILES EXCLUDE REGEX "${CMAKE_BINARY_DIR}/.*")
list(FILTER HEADER_CPP_FILES EXCLUDE REGEX "${CMAKE_BINARY_DIR}/.*")
list(FILTER HEADER_C_FILES EXCLUDE REGEX "${CMAKE_BINARY_DIR}/.*")

# すべてのファイルをSOURCE_FILESに結合
list(APPEND SOURCE_FILES ${SOURCE_CPP_FILES} ${SOURCE_C_FILES} ${HEADER_CPP_FILES} ${HEADER_C_FILES})

# ターゲットのインクルードディレクトリ設定（インクルードディレクトリはtarget_include_directoriesを使って指定）
include_directories(${PROJECT_SOURCE_DIR})  # プロジェクトソースディレクトリを追加

# すべてのソースファイルで実行可能ファイルを作成
add_executable(GHOST ${SOURCE_FILES})

# GLEWライブラリをリンク
target_link_libraries(GHOST PRIVATE GLEW::GLEW)

# Freetypeライブラリをリンク
target_link_libraries(GHOST PRIVATE Freetype::Freetype)

# OpenAL ライブラリをリンク
target_link_libraries(GHOST PRIVATE OpenAL::OpenAL)

# GLFW ライブラリをリンク
target_link_libraries(GHOST PRIVATE glfw)

# GLMの実験的な拡張を有効にする（ターゲットに定義を追加）
target_compile_definitions(GHOST PRIVATE GLM_ENABLE_EXPERIMENTAL)

# インクルードディレクトリの追加（GLEWとFreetypeのインクルードディレクトリを明示的に設定）
target_include_directories(GHOST
    PRIVATE
        ${GLEW_INCLUDE_DIRS}        # GLEWのインクルードディレクトリ
        ${FREETYPE_INCLUDE_DIRS}    # Freetypeのインクルードディレクトリ
)

# インストールの設定
install(TARGETS GHOST DESTINATION bin)

# 必要に応じて、ライブラリをさらにリンクする場合
# target_link_libraries(GHOST PRIVATE AnotherLibrary)

# CMakeが生成するMakefileに含めるカスタムコマンド（オプション）
# add_custom_command(...)
