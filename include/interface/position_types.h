/// @file position_types.h
/// @author maxim (necromax) alekseenko
/// @brief Defines common types for coordinate system.

#pragma once

/// @brief Used to define horizontal alignment of an object.
enum class HorizontalAlignment { Left, Center, Right };

/// @brief Used to define vertical alignment of an object.
enum class VerticalAlignment { Top, Center, Bottom };

/// @brief Position of an object in pixels.
using PosPixel = float;

/// @brief Position of an object in percent (usually, of a parent).
using PosPercent = float;

/// @brief Rotation of an object in degrees (usually counter-clockwise).
using PosAngle = double;