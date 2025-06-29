# Copyright 2025 The ODML Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

package(
    default_hdrs_check = "strict",
    default_visibility = [
        "//:__subpackages__",
    ],
)

licenses(["notice"])

cc_library(
    name = "executor_settings_base",
    srcs = ["executor_settings_base.cc"],
    hdrs = ["executor_settings_base.h"],
    deps = [
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "//runtime/util:file_util",
        "//runtime/util:litert_status_util",
        "//runtime/util:scoped_file",
    ],
)

cc_test(
    name = "executor_settings_base_test",
    srcs = ["executor_settings_base_test.cc"],
    deps = [
        ":executor_settings_base",
        "@com_google_googletest//:gtest_main",
        "//runtime/util:test_utils",
    ],
)

cc_library(
    name = "litert_compiled_model_executor_utils",
    srcs = ["litert_compiled_model_executor_utils.cc"],
    hdrs = ["litert_compiled_model_executor_utils.h"],
    deps = [
        ":executor_settings_base",
        "@com_google_absl//absl/container:btree",
        "@com_google_absl//absl/container:flat_hash_set",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/log:absl_log",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@litert//litert/cc:litert_buffer_ref",
        "@litert//litert/cc:litert_expected",
        "@litert//litert/cc:litert_macros",
        "@litert//litert/cc:litert_model",
        "//runtime/components:model_resources",
        "//runtime/components:model_resources_litert_lm",
        "//runtime/components:model_resources_task",
        "//runtime/components:sentencepiece_tokenizer",
        "//runtime/util:external_file_cc_proto",
        "//runtime/util:file_format_util",
        "//runtime/util:litert_lm_loader",
        "//runtime/util:litert_status_util",
        "//runtime/util:memory_mapped_file",
        "//runtime/util:model_asset_bundle_resources",
        "//runtime/util:scoped_file",
    ] + select({
        "//:litert_lm_link_capi_so": [
            "@litert//litert/cc:litert_tensor_buffer",
        ],
        "//conditions:default": [
            "@litert//litert/cc/internal:litert_tensor_buffer",
        ],
    }),
)

cc_test(
    name = "litert_compiled_model_executor_utils_test",
    srcs = ["litert_compiled_model_executor_utils_test.cc"],
    data = ["//runtime/testdata"],
    deps = [
        ":litert_compiled_model_executor_utils",
        ":llm_executor_settings",
        "@com_google_googletest//:gtest_main",
        "//runtime/components:model_resources",
        "//runtime/util:scoped_file",
        "//runtime/util:test_utils",
    ],
)

cc_library(
    name = "llm_litert_compiled_model_executor",
    srcs = ["llm_litert_compiled_model_executor.cc"],
    hdrs = ["llm_litert_compiled_model_executor.h"],
    deps = [
        ":executor_settings_base",
        ":litert_compiled_model_executor_utils",
        ":llm_executor",
        ":llm_executor_io_types",
        ":llm_executor_settings",
        "@com_google_absl//absl/base:nullability",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/log:absl_log",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:string_view",
        "@com_google_absl//absl/types:span",
        "@litert//litert/c:litert_common",
        "@litert//litert/c:litert_environment_options",
        "@litert//litert/cc:litert_expected",
        "@litert//litert/cc:litert_model",
        "//runtime/components:embedding_lookup_text",
        "//runtime/components:model_resources",
        "//runtime/components:model_resources_litert_lm",
        "//runtime/components:model_resources_task",
        "//runtime/components:sampler",
        "//runtime/components:sampler_factory",
        "//runtime/util:convert_tensor_buffer",
        "//runtime/util:file_util",
        "//runtime/util:litert_status_util",
    ] + select({
        "//:litert_lm_link_capi_so": [
            "@litert//litert/cc:litert_compiled_model",
            "@litert//litert/cc:litert_environment",
            "@litert//litert/cc:litert_options",
            "@litert//litert/cc:litert_tensor_buffer",
            "@litert//litert/cc/options:litert_cpu_options",
            "@litert//litert/cc/options:litert_gpu_options",
            "@litert//litert/cc/options:litert_runtime_options",
        ],
        "//conditions:default": [
            "@litert//litert/cc/internal:litert_compiled_model",
            "@litert//litert/cc/internal:litert_cpu_options",
            "@litert//litert/cc/internal:litert_environment",
            "@litert//litert/cc/internal:litert_gpu_options",
            "@litert//litert/cc/internal:litert_options",
            "@litert//litert/cc/internal:litert_runtime_options",
            "@litert//litert/cc/internal:litert_tensor_buffer",
        ],
    }) + select({
        "//conditions:default": [],
    }),
)

cc_test(
    name = "llm_litert_compiled_model_executor_test",
    srcs = ["llm_litert_compiled_model_executor_test.cc"],
    data = ["//runtime/testdata"],
    tags = ["requires-gpu-nvidia"],
    deps = [
        ":executor_settings_base",
        ":llm_executor_settings",
        ":llm_litert_compiled_model_executor",
        "@com_google_googletest//:gtest_main",
        "@com_google_absl//absl/cleanup",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "//runtime/components:model_resources",
        "//runtime/components:model_resources_task",
        "//runtime/util:model_asset_bundle_resources",
        "//runtime/util:scoped_file",
        "//runtime/util:test_utils",
    ],
)

cc_library(
    name = "fake_llm_executor",
    srcs = ["fake_llm_executor.cc"],
    hdrs = ["fake_llm_executor.h"],
    deps = [
        ":executor_settings_base",
        ":llm_executor",
        ":llm_executor_io_types",
        ":llm_executor_settings",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:string_view",
        "@com_google_absl//absl/time",
        "@com_google_absl//absl/types:span",
        "@litert//litert/cc:litert_macros",
        "//runtime/util:convert_tensor_buffer",
        "//runtime/util:litert_status_util",
    ] + select({
        "//:litert_lm_link_capi_so": [
            "@litert//litert/cc:litert_tensor_buffer",
        ],
        "//conditions:default": [
            "@litert//litert/cc/internal:litert_tensor_buffer",
        ],
    }),
)

cc_test(
    name = "fake_llm_executor_test",
    srcs = ["fake_llm_executor_test.cc"],
    deps = [
        ":fake_llm_executor",
        ":llm_executor_io_types",
        "@com_google_googletest//:gtest_main",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/types:span",
        "@litert//litert/test:matchers",
        "//runtime/util:convert_tensor_buffer",
        "//runtime/util:test_utils",
    ],
)

cc_library(
    name = "llm_executor",
    hdrs = ["llm_executor.h"],
    deps = [
        ":llm_executor_base",
        "//runtime/util:logging_tensor_buffer",
    ],
)

cc_library(
    name = "llm_executor_base",
    hdrs = ["llm_executor_base.h"],
    deps = [
        ":llm_executor_io_types",
        ":llm_executor_settings",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:string_view",
    ] + select({
        "//:litert_lm_link_capi_so": [
            "@litert//litert/cc:litert_tensor_buffer",
        ],
        "//conditions:default": [
            "@litert//litert/cc/internal:litert_tensor_buffer",
        ],
    }),
)

cc_library(
    name = "llm_litert_npu_compiled_model_executor",
    srcs = ["llm_litert_npu_compiled_model_executor.cc"],
    hdrs = ["llm_litert_npu_compiled_model_executor.h"],
    deps = [
        ":executor_settings_base",
        ":litert_compiled_model_executor_utils",
        ":llm_executor",
        ":llm_executor_io_types",
        ":llm_executor_settings",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/log:absl_log",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:string_view",
        "@com_google_absl//absl/time",
        "@com_google_absl//absl/types:span",
        "@litert//litert/c:litert_common",
        "@litert//litert/c:litert_model",
        "@litert//litert/cc:litert_layout",
        "@litert//litert/cc:litert_macros",
        "@litert//litert/cc:litert_model",
        "//runtime/components:model_resources",
        "//runtime/components:model_resources_litert_lm",
        "//runtime/components:model_resources_task",
        "//runtime/util:convert_tensor_buffer",
        "//runtime/util:litert_status_util",
    ] + select({
        "//:litert_lm_link_capi_so": [
            "@litert//litert/cc:litert_compiled_model",
            "@litert//litert/cc:litert_environment",
            "@litert//litert/cc:litert_tensor_buffer",
        ],
        "//conditions:default": [
            "@litert//litert/cc/internal:litert_compiled_model",
            "@litert//litert/cc/internal:litert_environment",
            "@litert//litert/cc/internal:litert_google_tensor_options",
            "@litert//litert/cc/internal:litert_mediatek_options",
            "@litert//litert/cc/internal:litert_options",
            "@litert//litert/cc/internal:litert_qualcomm_options",
            "@litert//litert/cc/internal:litert_tensor_buffer",
        ],
    }),
)

cc_library(
    name = "llm_executor_settings",
    srcs = ["llm_executor_settings.cc"],
    hdrs = ["llm_executor_settings.h"],
    deps = [
        ":executor_settings_base",
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "//runtime/util:litert_status_util",
        "//runtime/util:logging",
    ],
)

cc_test(
    name = "llm_executor_settings_test",
    srcs = ["llm_executor_settings_test.cc"],
    data = ["//runtime/testdata"],
    deps = [
        ":executor_settings_base",
        ":llm_executor_settings",
        "@com_google_googletest//:gtest_main",
        "@com_google_absl//absl/status",
        "//runtime/util:scoped_file",
        "//runtime/util:test_utils",
    ],
)

cc_library(
    name = "llm_executor_io_types",
    srcs = ["llm_executor_io_types.cc"],
    hdrs = ["llm_executor_io_types.h"],
    deps = [
        "@com_google_absl//absl/status",
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "//runtime/util:logging_tensor_buffer",
    ] + select({
        "//:litert_lm_link_capi_so": [
            "@litert//litert/cc:litert_tensor_buffer",
        ],
        "//conditions:default": [
            "@litert//litert/cc/internal:litert_tensor_buffer",
        ],
    }),
)

cc_test(
    name = "llm_executor_io_types_test",
    srcs = ["llm_executor_io_types_test.cc"],
    deps = [
        ":llm_executor_io_types",
        "@com_google_googletest//:gtest_main",
        "@com_google_absl//absl/status:statusor",
        "@litert//litert/c:litert_dispatch_headers",
        "@litert//litert/cc:litert_element_type",
        "@litert//litert/cc:litert_layout",
        "@litert//litert/cc:litert_model",
    ] + select({
        "//:litert_lm_link_capi_so": [
            "@litert//litert/cc:litert_tensor_buffer",
        ],
        "//conditions:default": [
            "@litert//litert/cc/internal:litert_tensor_buffer",
        ],
    }),
)

cc_library(
    name = "vision_executor",
    hdrs = ["vision_executor.h"],
    deps = [":vision_executor_base"],
)

cc_library(
    name = "vision_executor_base",
    hdrs = ["vision_executor_base.h"],
    deps = [
        ":llm_executor_io_types",
        "@com_google_absl//absl/status:statusor",
    ] + select({
        "//:litert_lm_link_capi_so": [
            "@litert//litert/cc:litert_tensor_buffer",
        ],
        "//conditions:default": [
            "@litert//litert/cc/internal:litert_tensor_buffer",
        ],
    }),
)

cc_library(
    name = "audio_executor",
    hdrs = ["audio_executor.h"],
    deps = [":audio_executor_base"],
)

cc_library(
    name = "audio_executor_base",
    hdrs = ["audio_executor_base.h"],
    deps = [
        ":llm_executor_io_types",
        "@com_google_absl//absl/status:statusor",
    ] + select({
        "//:litert_lm_link_capi_so": [
            "@litert//litert/cc:litert_tensor_buffer",
        ],
        "//conditions:default": [
            "@litert//litert/cc/internal:litert_tensor_buffer",
        ],
    }),
)
