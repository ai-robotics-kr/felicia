# Copyright (c) 2019 The Felicia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

load(
    "//bazel:felicia.bzl",
    "if_mac",
    "if_windows",
)
load(
    "//bazel:felicia_cc.bzl",
    "fel_cc_library",
    "fel_objc_library",
)

package(default_visibility = ["//felicia:internal"])

SHARED_MEMORY_HDRS = [
    "platform_handle_broker.h",
    "read_only_shared_buffer.h",
    "shared_buffer.h",
    "shared_memory.h",
    "writable_shared_buffer.h",
]

fel_cc_library(
    name = "shared_memory",
    srcs = [
        "read_only_shared_buffer.cc",
        "shared_memory.cc",
        "writable_shared_buffer.cc",
    ] + select({
        "//felicia:mac": ["platform_handle_broker_mac.cc"],
        "//felicia:windows": [
            "named_pipe_server.cc",
            "platform_handle_broker_win.cc",
        ],
        "//conditions:default": ["platform_handle_broker_posix.cc"],
    }),
    hdrs = SHARED_MEMORY_HDRS + if_windows([
        "named_pipe_server.h",
    ]) + if_mac([":shared_memory_mac_hdrs"]),
    deps = [
        "//felicia/core/channel:channel_impl",
        "//felicia/core/channel/socket",
    ] + if_mac([":shared_memory_mac"]),
)

filegroup(
    name = "shared_memory_mac_hdrs",
    srcs = SHARED_MEMORY_HDRS + [
        "mach_port_broker.h",
    ],
)

fel_objc_library(
    name = "shared_memory_mac",
    hdrs = if_mac([
        ":shared_memory_mac_hdrs",
    ]),
    non_arc_srcs = if_mac([
        "mach_port_broker.mm",
    ]),
    deps = [
        "//felicia/core/lib",
    ],
)
