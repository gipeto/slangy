from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class slangyRecipe(ConanFile):
    name = "slangy"
    version = "0.2.0"
    package_type = "library"

    # Optional metadata
    license = "https://github.com/gipeto/slangy/blob/main/LICENSE"
    author = "Javier Perez"
    url = "https://github.com/gipeto/slangy"
    description = "A utility library for creating and deploying applications using shaders written in slang."
    topics = ("slang", "shaders")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
    }

    test_requires = "gtest/1.16.0"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "libs/*", "cmake/*"

    def requirements(self):
        self.requires("slang/[>=2025.5.3]")

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.builddirs = ["lib/cmake/slangy"]
        self.cpp_info.set_property("cmake_find_mode", "none")
