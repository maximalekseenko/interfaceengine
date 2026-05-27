// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_INTERFACE_GUI_COMPONENT_H_
#define INCLUDE_INTERFACE_GUI_COMPONENT_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "interface/dll.h"
#include "interface/event.h"
#include "interface/gui/lumen_rules.h"
#include "interface/position_types.h"

namespace interfaceengine::gui {

class INTERFACE_API Component {
 public:  // -------------------- CONSTRUCTOR --------------------
  Component() = default;
  virtual ~Component() = default;

 public:  // -------------------- TYPE DEFINITIONS --------------------
  /// @brief Pointer type for storing components.
  using Ptr = std::unique_ptr<Component>;

  /// @brief Type of unique identifier for a component.
  using Id = std::string;

  /// @brief Type of message that is being sent.
  using Message = std::string;

 public:  // -------------------- VIRTUAL MEMBERS --------------------
  virtual std::vector<LumenRules> GetLumenRules() { return {}; }
  void virtual OnMessage(const Message&) {}
  void virtual OnMouseOver(const MouseEvent&) {}
  void virtual OnMouseClick(const MouseEvent&) {}

 public:  // -------------------- PUBLIC METHODS --------------------
  void SetId(Id id) { id_ = id; }

  Id id() { return id_; }

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
  friend class Manager;

  /// @brief Unique identifier for this component.
  Id id_;

  /// @brief Size of the component in percent of a parent.
  PosPercent w_percent_{1}, h_percent_{1};

  /// @brief An additional size of the component in pixels.
  PosPixel w_offset_{0}, h_offset_{0};

  /// @brief Position of the component in percent from alignment point.
  PosPercent x_percent_{0}, y_percent_{0};

  /// @brief An additional postion offset of the component in pixels.
  PosPixel x_offset_{0}, y_offset_{0};

  /// @brief Horizontal alignment of the component to its perent.
  HorizontalAlignment horizontal_alignment_{HorizontalAlignment::Left};

  /// @brief vertical alignment of the component to its perent.
  VerticalAlignment vertical_alignment_{VerticalAlignment::Top};

  /// @brief Parent component to this one.
  /// @note Is not set for root components.
  Component* parent{nullptr};

  /// @brief List of pointers to child components.
  std::vector<Ptr> children;
};

}  // namespace interfaceengine::gui

#endif  // INCLUDE_INTERFACE_GUI_COMPONENT_H_
