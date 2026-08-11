cmake_minimum_required(VERSION 3.24)
project(BitcoinWalletPlugin VERSION 3.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Qt6 REQUIRED COMPONENTS Core)


include(FetchContent)

FetchContent_Declare(WalletInterfaces
    GIT_REPOSITORY https://github.com/xXLegionBinFrogXx/WalletInterfaces.git
    GIT_TAG        v2.0.0
    GIT_SHALLOW    TRUE
    FIND_PACKAGE_ARGS 2.0 NAMES WalletInterfaces
)

FetchContent_MakeAvailable(WalletInterfaces)

# Add Bitcoin key utils library (brings in secp256k1-wrapper itself)
FetchContent_Declare(
    BitcoinKeyUtils
    GIT_REPOSITORY https://github.com/xXLegionBinFrogXx/bitcoin-key-utils.git
    GIT_TAG v2.1.1
)

FetchContent_MakeAvailable(BitcoinKeyUtils)

# Define the plugin library
add_library(BitcoinWalletPlugin SHARED
    src/bitcoinwalletplugin.cpp
    src/bitcoinwalletplugin.h
    bitcoinwalletplugin.json
)

# Link dependencies
target_link_libraries(BitcoinWalletPlugin
    PRIVATE
    Qt6::Core
    WalletInterfaces::WalletInterfaces
    bitcoin-key-utils::static
)

target_compile_options(BitcoinWalletPlugin PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Set output directory for the plugin
set_target_properties(BitcoinWalletPlugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)
