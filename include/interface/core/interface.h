// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_INTERFACE_CORE_INTERFACE_H_
#define INCLUDE_INTERFACE_CORE_INTERFACE_H_

#include <necroutils/logger.h>

#include <string>

#include "interface/dll.h"
#include "interface/gui/gui_component.h"

/// @brief Core object of the engine.
///
/// In order to use it you must `Initialize()` it first.
///
/// Run it with `Run()` function.
///
/// Set root GUI component with `SetRootGuiComponent()` function.
///
/// @note Must be run in main thread.
class INTERFACE_API Interface {
 public:  // -------------------- CONSTRUCTORS --------------------
  Interface();
  ~Interface();

 public:  // -------------------- PUBLIC METHODS --------------------
  /// @brief Initializes the engine.
  void Initialize();

  /// @brief Starts the engine and goes to the main loop.
  void Run();

  /// @brief Sets the root gui component.
  ///
  /// Root gui component is stretched to the scale of a screen.
  /// @param gui_component Pointer to new core gui component.
  void SetRootGuiComponent(GuiComponent::Ptr gui_component);

  /// @brief Sends a message down the ui tree.
  /// @param message Message to send.
  /// @param receiver_id Id of the receiver of the message.
  /// @param single_receiver Should stop after finding first receiver?
  /// (i.e. set to true if you want for only first component with
  /// `receiver_id` to get the message)
  void SendMessageToGui(GuiComponent::Message message,
                        GuiComponent::Id receiver_id,
                        bool single_receiver = false);

  /// @brief Loads data from a lumen package.
  /// @param package_path path to a lum package to load.
  void LoadLumens(std::string package_path);

  /// @brief Forces app to quit.
  void Quit();

  /// @brief Add a logger sink for logs of this interface.
  /// @note Interface's logger name is "`InterfaceEngine`"
  void AddLogSink(Logger::SinkPtr sink);

 private:  // -------------------- PIMPL --------------------
  struct Impl;
  Impl* impl_;
};

#endif  // INCLUDE_INTERFACE_CORE_INTERFACE_H_
