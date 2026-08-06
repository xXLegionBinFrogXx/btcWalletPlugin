cmake_minimum_required(VERSION 3.24)
project(BitcoinWalletPlugin LANGUAGES C CXX)

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

# Add the secp256k1-wrapper library
FetchContent_Declare(
    secp256k1-wrapper
    GIT_REPOSITORY https://github.com/xXLegionBinFrogXx/secp256k1-wrapper.git
    GIT_TAG v1.0.0
)

# Make the fetched content available
FetchContent_MakeAvailable(secp256k1-wrapper)

# Add Bitcoin key utils library
FetchContent_Declare(
    BitcoinKeyUtils
    GIT_REPOSITORY https://github.com/xXLegionBinFrogXx/bitcoin-key-utils.git
    GIT_TAG v1.1.0
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
    secp256k1-wrapper::static
    bitcoin-key-utils::static
)

target_include_directories(BitcoinWalletPlugin
    PRIVATE
    ${secp256k1-wrapper_SOURCE_DIR}/include
)

# Set output directory for the plugin
set_target_properties(BitcoinWalletPlugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
)
