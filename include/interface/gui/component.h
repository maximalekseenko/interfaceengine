// Copyright 2026 maxim (necromax) alekseenko

#ifndef INCLUDE_INTERFACE_GUI_COMPONENT_H_
#define INCLUDE_INTERFACE_GUI_COMPONENT_H_

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

  void SetHorizontalPosition(RelScalar value,
                             HorizontalAlignment const& alignment) {
    x_ = value;
    horizontal_alignment_ = alignment;
  }

  void SetVerticalPosition(RelScalar value,
                           VerticalAlignment const& alignment) {
    y_ = value;
    vertical_alignment_ = alignment;
  }

  void SetWidth(RelScalar value) { w_ = value; }

  void SetHeight(RelScalar value) { h_ = value; }

  void GetSelfRect(PosPixel* out_x, PosPixel* out_y, PosPixel* out_w,
                   PosPixel* out_h) const;

  bool IsPointWithin(PosPixel x, PosPixel y);

 private:  // -------------------- PRIVATE MEMBERS --------------------
  friend class GuiManager;

  /// @brief Unique identifier for this component.
  Id id_;

  RelScalar x_{0, 0}, y_{0, 0}, w_{1, 0}, h_{1, 0};

  /// @brief Horizontal alignment of the component to its perent.
  HorizontalAlignment horizontal_alignment_{HorizontalAlignment::Left};

  /// @brief vertical alignment of the component to its perent.
  VerticalAlignment vertical_alignment_{VerticalAlignment::Top};

  /// @brief Parent component to this one.
  /// @note Is not set for root components.
  Component* parent_{nullptr};

  /// @brief List of pointers to child components.
  std::vector<Ptr> children_;
};

}  // namespace interfaceengine::gui

#endif  // INCLUDE_INTERFACE_GUI_COMPONENT_H_
