package(default_visibility = ["//visibility:public"])

java_library(
    name = "quickjs-jvm",
    srcs = glob(["lukekaalim/quickjs/*.java"]),
)

cc_library(
    name = "jni",
    srcs = glob(["lukekaalim/quickjs/*.c", "lukekaalim/quickjs/*.h"]),
    deps = [
        "@quickjs",
        "@bazel_tools//tools/jdk:jni"
    ],
)

cc_binary(
    name = "jni_shared",
    linkshared = True,
    deps = [":jni"],
)

java_binary(
    name = "runner",

    deps = [":quickjs-jvm"],
    data = [":jni_shared"],

    main_class = "Runner",
    srcs = ["Runner.java"],
    jvm_flags = ["-Djava.library.path=quickjs-jvm"],
)
