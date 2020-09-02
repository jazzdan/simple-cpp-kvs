#TODO(dmiller): move this in to its own directory
cc_library(
  name = "kvslib",
  srcs = ["kvslib.cpp"],
  hdrs = ["kvslib.h"],
)

cc_library(
  name = "cli",
  srcs = ["cli.cpp"],
  hdrs = ["cli.h"],
  deps = [
    "@com_google_absl//absl/strings",
  ]
)

cc_test(
  name = "tests",
  srcs = ["cli.cpp", "cli.h", "cli_test.cpp"],
  deps = [
    ":cli",
    "@gtest//:gtest_main",
  ]
)

cc_binary(
  name = "server",
  deps = [
    # "@com_google_absl//absl/flags:flag",
    # "@com_google_absl//absl/flags:parse",
    ":kvslib",
    ":cli",
  ],
  srcs = ["server.cpp"],
)