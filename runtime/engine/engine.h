// Copyright 2025 The ODML Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef THIRD_PARTY_ODML_LITERT_LM_RUNTIME_ENGINE_ENGINE_H_
#define THIRD_PARTY_ODML_LITERT_LM_RUNTIME_ENGINE_ENGINE_H_

#include <memory>
#include <vector>

#include "absl/status/status.h"  // from @com_google_absl
#include "absl/status/statusor.h"  // from @com_google_absl
#include "absl/strings/string_view.h"  // from @com_google_absl
#include "absl/time/time.h"  // from @com_google_absl
#include "runtime/engine/engine_settings.h"
#include "runtime/engine/io_types.h"

namespace litert::lm {

// Engine is the interface for the LLM runtime. It is responsible for
// - Initializing the LLM model and related resources, e.g. tokenizer,
//   embedder, etc.
// - Providing the APIs to create the Session.
//
// The Session is responsible for hosting the internal state (e.g. conversation
// history) of each separate interaction with LLM. It is created by the Engine
// and is responsible for:
// - Generating content from the input prompt/query.
// - Running the prefill and decode processes.
//
// Example usage:
//   // Create the model assets.
//   auto model_assets = ModelAssets::Create(model_path);
//   CHECK_OK(model_assets);
//
//   // Create the engine.
//   auto engine = Engine::CreateEngine(EngineSettings::CreateDefault(
//       model_assets, litert::lm::Backend::CPU));
//   CHECK_OK(engine);
//
//   // Create the session.
//   auto session = engine->CreateSession(SessionConfig::CreateDefault());
//   CHECK_OK(session);
//
//   // Run generate content.
//   auto responses = (*session)->GenerateContent({InputText("What's the tallest
//   building in the world?")});
//   CHECK_OK(responses);
//
//   // Print the response.
//   std::cout << *responses << std::endl;
class Engine {
 public:
  virtual ~Engine() = default;

  // Session is responsible for hosting the internal state (e.g. conversation
  // history) of each separate interaction with LLM.
  class Session {
   public:
    virtual ~Session() = default;

    // High-level API to generate content from the input prompt/query. This
    // function will handle the prefill and decode processes internally and
    // the usage is similar to the Gemini Text Generation API
    // (https://ai.google.dev/gemini-api/docs/text-generation).
    virtual absl::StatusOr<Responses> GenerateContent(
        const std::vector<InputData>& contents) = 0;
    virtual absl::Status GenerateContentStream(
        const std::vector<InputData>& contents,
        InferenceObservable* observer) = 0;

    // Adds the input prompt/query to the model for starting the prefilling
    // process. Note that the user can break down their prompt/query into
    // multiple chunks and call this function multiple times.
    //
    // This is a blocking call and the function will return when the prefill
    // process is done.
    virtual absl::Status RunPrefill(const std::vector<InputData>& contents) = 0;

    // This is a not blocking call and the function will return right away. The
    // processing status will be signaled through the observer.
    virtual absl::Status RunPrefillAsync(const std::vector<InputData>& contents,
                                         InferenceObservable* observer) {
      return absl::UnimplementedError("Not implemented.");
    }

    // Starts the decoding process for the model to predict the response based
    // on the input prompt/query added after using RunPrefill* functions.
    // This is a blocking call and the function will return when the decoding
    // process is done.
    virtual absl::StatusOr<Responses> RunDecode() = 0;

    // Startes the decoding process for the model to predict the response based
    // on the input prompt/query added after using RunPrefill* functions.
    // This is a not blocking call and the function will return right away. The
    // result will be streamed through the observer.
    virtual absl::Status RunDecodeAsync(InferenceObservable* observer) {
      return absl::UnimplementedError("Not implemented.");
    }

    // Returns the benchmark info for the session. Returns error if the
    // benchmark is not enabled.
    virtual absl::StatusOr<BenchmarkInfo> GetBenchmarkInfo() = 0;
  };

  // Method to create Engine.
  static absl::StatusOr<std::unique_ptr<Engine>> CreateEngine(
      EngineSettings settings);

  // Method to create the Session.
  virtual absl::StatusOr<std::unique_ptr<Session>> CreateSession(
      const SessionConfig& session_config) const = 0;

  // Waits until the engine is done with all the tasks. The function will
  // return error if the timeout is reached.
  virtual absl::Status WaitUntilDone(absl::Duration timeout) {
    return absl::UnimplementedError("Not implemented.");
  }

  // Default timeout duration for the engine/session processes.
  static constexpr absl::Duration kDefaultTimeout = absl::Minutes(10);
};

}  // namespace litert::lm

#endif  // THIRD_PARTY_ODML_LITERT_LM_RUNTIME_ENGINE_ENGINE_H_
