// Copyright 2026 maxim (necromax) alekseenko

#ifndef SRC_INTERFACE_CORE_INTERFACE_IMPL_H_
#define SRC_INTERFACE_CORE_INTERFACE_IMPL_H_

#include <necroutils/logger.h>

#include <memory>
#include <stack>
#include <string>
#include <utility>

#include "interface/core/interface.h"
#include "interface/core/settings.h"
#include "interface/gui/manager.h"
#include "interface/lumen/manager.h"
#include "interface/misc/rect.h"
#include "interface/window/manager.h"

struct Interface::Impl {
 public:  // -------------------- CONSTRUCTOR --------------------
  Impl();

 public:  // -------------------- MEMBERS --------------------
  /// @brief Pointer to public part of pimpl implementation.
  Interface* self;

  /// @brief Settings for this engine instance.
  InterfaceSettings settings;

  /// @brief Logger for this interface.
  Logger logger;

  /// @brief Lumen manager for this interface.
  interfaceengine::lumen::LumenManager lumen_manager;

  /// @brief Window manager for this interface.
  interfaceengine::window::WindowManager window_manager;

  /// @brief Core GUI component.
  interfaceengine::gui::GuiManager gui_manager_;

  // Component::Ptr root_gui_component{std::make_unique<Component>()};

  /// @brief Defines if application is running.
  bool is_running{false};

  /// @brief Stores time of the last frame's tick in milliseconds.
  uint64_t last_frame_tick{0u};

  bool is_initialized{false};

 public:  // -------------------- FUNCTIONS --------------------
  /// @brief Renders content.
  void Render();

  /// @brief Handles incoming events.
  void HandleEvents();

  /// @brief Delays the tickrate down to fps setting.
  void TickDelay();

  void UpdateRootComponentSize();
};

#endif  // SRC_INTERFACE_CORE_INTERFACE_IMPL_H_
