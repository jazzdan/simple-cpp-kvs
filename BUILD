#TODO(dmiller): move this in to its own directory
cc_library(
  name = "kvslib",
  srcs = ["kvslib.cpp"],
  hdrs = ["kvslib.h"],
)

cc_binary(
  name = "server",
  deps = [
    # "@com_google_absl//absl/flags:flag",
    # "@com_google_absl//absl/flags:parse",
    ":kvslib"
  ],
  srcs = ["server.cpp"],
)