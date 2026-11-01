from conan import ConanFile
from conan.tools.cmake import cmake_layout


class ScoreEngineConan(ConanFile):
    name = "score_engine"
    version = "0.1"
    package_type = "application"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("nlohmann_json/3.11.3")
        self.requires("protobuf/6.33.5")
        self.requires("grpc/1.78.1")
        self.requires("lua/5.4.6")
        self.requires("sol2/3.3.1")
