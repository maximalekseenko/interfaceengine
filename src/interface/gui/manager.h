// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_GUI_MANAGER_H_
#define SRC_INTERFACE_GUI_MANAGER_H_

#include "interface/dll.h"
#include "interface/gui/component.h"  // TODO(necromax): rename to just component

namespace interfaceengine::gui {

/// @brief A class for loading and managing lumens.
class INTERFACE_INTERNAL Manager {
 public:  // -------------------- TYPES --------------------
 public:  // -------------------- CONSTRUCTORS --------------------
  Manager() = default;

 public:  // -------------------- DESTRUCTORS --------------------
  ~Manager() = default;

 public:  // -------------------- PUBLIC METHODS --------------------
  Component* root_component() { return root_component_.get(); }

  void SetNewRootComponent(Component::Ptr new_root_component);
  void UpdateForWindowSize(int window_w, int window_h);

 private:  // -------------------- PRIVATE MEMBERS --------------------
  Component::Ptr root_component_{nullptr};
};

}  // namespace interfaceengine::gui

#endif  // SRC_INTERFACE_GUI_MANAGER_H_
