// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_INTERFACE_GUI_GUI_COMPONENT_H_
#define INCLUDE_INTERFACE_GUI_GUI_COMPONENT_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "interface/dll.h"
#include "interface/event.h"
#include "interface/gui/lumen_rules.h"
#include "interface/position_types.h"

/// @brief A component that represents an element on the screen.
///
/// To set position of a component use
/// `SetWidth` `SetHorizontalPosition`
/// `SetHeight` `SetVerticalPosition` functions.
///
/// To set the looks for a component do one of the following:
///
/// - Fill `lumen_rules` with rules if this component does not
/// change the visuals.
///
/// - Set the `get_lumen_rules` to a custom function.
/// It will be called
class INTERFACE_API GuiComponent {
 public:  // -------------------- CONSTRUCTOR --------------------
  GuiComponent() = default;
  virtual ~GuiComponent() = default;

 public:  // -------------------- TYPE DEFINITIONS --------------------
  /// @brief Pointer type for storing components.
  using Ptr = std::unique_ptr<GuiComponent>;

  /// @brief Type of unique identifier for a component.
  using Id = std::string;

  /// @brief Type of message that is being sent.
  using Message = std::string;

  using OnMessage = std::function<void(Message)>;
  using OnMouseOver = std::function<void(MouseEvent)>;
  using OnMouseClick = std::function<void(MouseEvent)>;

 public:  // -------------------- VIRTUAL MEMBERS --------------------
  /// @brief A list of lumen rules that is used with
  /// default `get_lumen_rules` function;
  std::vector<LumenRules> lumen_rules;

  /// @brief A function for retrieving lumen rules during render cycle.
  /// @note By default, returns `lumen_rules`.
  virtual std::vector<LumenRules> GetLumenRules();

  // / @brief Called when message is being received.
  /// @param message A message that is being received.
  OnMessage on_message;
  OnMouseOver on_mouse_over;
  OnMouseClick on_mouse_click;

 public:  // -------------------- PUBLIC METHODS --------------------
  /// @brief Sets id of this component.
  /// @note Id can repeat throughout the gui tree.
  /// @param id Identifier to set this id to.
  void SetId(Id id);

  /// @brief Retrieves id of the component.
  /// @return Identifier of this component.
  Id GetId();

  /// @brief Adds child component to this component.
  /// @param child_ptr A unique pointer to the child component.
  void AddChildComponent(Ptr child_ptr);

  /// @brief Sets the width.
  /// @param percent The width percentage of the parent width.
  /// @param offset An additional width offset in pixels.
  /// @note Does not affect root components.
  void SetWidth(PosPercent const& percent, PosPixel const& offset);

  /// @brief Sets the horizontal position to left.
  /// @param percent The horizontal alignment percentage of the parent width.
  /// @param offset An additional horizontal alignment offset in pixels.
  /// @note Does not affect root components.
  void SetHorizontalPosition(PosPercent const& percent, PosPixel const& offset,
                             HorizontalAlignment const& alignment);

  /// @brief Sets the height.
  /// @param percent The height percentage of the parent height.
  /// @param offset An additional height offset in pixels.
  /// @note Does not affect root components.
  void SetHeight(PosPercent const& percent, PosPixel const& offset);

  /// @brief Sets the vertical position.
  /// @param percent The vertical alignment percentage of the parent height.
  /// @param offset An additional vertical alignment offset in pixels.
  /// @param alignment The vertical alignment.
  /// @note Does not affect root components.
  void SetVerticalPosition(PosPercent const& percent, PosPixel const& offset,
                           VerticalAlignment const& alignment);

  /// @brief Retrieves a calculated rect of the component.
  /// @param[out] out_x The x-coordinate of the
  /// top-left corner of the component.
  /// @param[out] out_y The y-coordinate of the
  /// top-left corner of the component.
  /// @param[out] out_w The width of the component.
  /// @param[out] out_h The height of the component.
  /// @note You can pass a `nullptr` if an output is not required.
  void GetSelfRect(PosPixel* out_x, PosPixel* out_y, PosPixel* out_w,
                   PosPixel* out_h) const;

 private:  // -------------------- PRIVATE MEMBERS --------------------
  friend class Interface;

  /// @brief Sends a message down the ui tree.
  /// @param message Message to send.
  /// @param receiver_id Id of the receiver of the message.
  /// @param single_receiver Should stop after finding first receiver?
  /// (i.e. set to true if you want for only first component with
  /// `receiver_id` to get the message)
  void DispatchMessage(Message message, Id receiver_id,
                       bool single_receiver = false);
  void DispatchMouseOver(MouseEvent event);
  void DispatchMouseClick(MouseEvent event);

  /// @brief Unique identifier for this component.
  Id id_;

  /// @brief Size of the component in percent of a parent.
  PosPercent w_percent{1}, h_percent{1};

  /// @brief An additional size of the component in pixels.
  PosPixel w_offset{0}, h_offset{0};

  /// @brief Position of the component in percent from alignment point.
  PosPercent x_percent{0}, y_percent{0};

  /// @brief An additional postion offset of the component in pixels.
  PosPixel x_offset = {0}, y_offset{0};

  /// @brief Horizontal alignment of the component to its perent.
  HorizontalAlignment horizontal_alignment{HorizontalAlignment::Left};

  /// @brief vertical alignment of the component to its perent.
  VerticalAlignment vertical_alignment{VerticalAlignment::Top};

  /// @brief Parent component to this one.
  /// @note Is not set for root components.
  GuiComponent* parent{nullptr};

  /// @brief List of pointers to child components.
  std::vector<Ptr> children;
};

#endif  // INCLUDE_INTERFACE_GUI_GUI_COMPONENT_H_
