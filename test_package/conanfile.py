import os

from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout
from conan.tools.build import can_run
from conan.tools.files import copy


class slangTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def layout(self):
        cmake_layout(self)

    def generate(self):
        for dep in self.dependencies.values():
            copy(
                self,
                "*.dll",
                dep.cpp_info.bindirs[0],
                os.path.join(self.build_folder, self.cpp.build.bindir),
            )

    def test(self):
        if can_run(self):
            cmd = os.path.join(self.cpp.build.bindir, "testPackageApp")
            self.run(cmd, env="conanrun")
