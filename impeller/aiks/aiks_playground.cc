// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/aiks/aiks_playground.h"

#include <memory>

#include "impeller/aiks/aiks_context.h"
#include "impeller/typographer/backends/skia/typographer_context_skia.h"
#include "impeller/typographer/typographer_context.h"
#include "third_party/imgui/imgui.h"

namespace impeller {

AiksPlayground::AiksPlayground()
    : typographer_context_(TypographerContextSkia::Make()) {}

AiksPlayground::~AiksPlayground() = default;

void AiksPlayground::SetTypographerContext(
    std::shared_ptr<TypographerContext> typographer_context) {
  typographer_context_ = std::move(typographer_context);
}

bool AiksPlayground::OpenPlaygroundHere(const Picture& picture) {
  return OpenPlaygroundHere(
      [&picture](AiksContext& renderer, RenderTarget& render_target) -> bool {
        return renderer.Render(picture, render_target);
      });
}

bool AiksPlayground::OpenPlaygroundHere(AiksPlaygroundCallback callback) {
  if (!switches_.enable_playground) {
    return true;
  }

  AiksContext renderer(GetContext(), typographer_context_);

  if (!renderer.IsValid()) {
    return false;
  }

  return Playground::OpenPlaygroundHere(
      [&renderer, &callback](RenderTarget& render_target) -> bool {
        static bool wireframe = false;
        if (ImGui::IsKeyPressed(ImGuiKey_Z)) {
          wireframe = !wireframe;
          renderer.GetContentContext().SetWireframe(wireframe);
        }
        return callback(renderer, render_target);
      });
}

}  // namespace impeller
