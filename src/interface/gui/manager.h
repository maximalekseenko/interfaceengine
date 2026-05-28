// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_GUI_MANAGER_H_
#define SRC_INTERFACE_GUI_MANAGER_H_

#include <vector>

#include "interface/dll.h"
#include "interface/event.h"
#include "interface/gui/component.h"
#include "interface/render/render_request.h"

namespace interfaceengine::gui {

/// @brief A class for loading and managing lumens.
class INTERFACE_INTERNAL Manager {
 public:  // -------------------- CONSTRUCTORS --------------------
  Manager() = default;

 public:  // -------------------- DESTRUCTORS --------------------
  ~Manager() = default;

 public:  // -------------------- PUBLIC METHODS --------------------
  Component* root_component() { return root_component_.get(); }

  void SetNewRootComponent(Component::Ptr new_root_component);

  void DispatchMessage(Component::Message message, Component::Id receiver_id,
                       bool single_receiver);

  void DispatchMouseOver(const MouseEvent& event);

  void DispatchMouseClick(const MouseEvent& event);

  void GatherRenderRequests(std::vector<render::RenderRequest>& out_requests);

  void UpdateForWindowSize(int window_w, int window_h);

 private:  // -------------------- PRIVATE MEMBERS --------------------
  Component::Ptr root_component_{nullptr};
};

}  // namespace interfaceengine::gui

#endif  // SRC_INTERFACE_GUI_MANAGER_H_
