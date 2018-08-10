// swift-tools-version:4.0
import PackageDescription

let package = Package(
    name: "swift-nio-ssl",
    products: [
        .library(name: "Termbox", targets: ["Termbox"]),
        .executable(name: "Test", targets: ["Test"])
    ],
    dependencies: [],
    targets: [
        .target(name: "Termbox", path: "src/"),
        .target(name: "Test", dependencies: ["Termbox"])
    ]
    
)