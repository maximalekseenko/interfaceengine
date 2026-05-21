// Copyright 2026 maxim (necromax) alekseenko

#include "interface/core/interface.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <necroutils/colexc.h>
#include <necroutils/logger.h>

#include <memory>
#include <stack>
#include <string>
#include <utility>

#include "interface/core/settings.h"
#include "interface/lumen/manager/manager.h"
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
  std::shared_ptr<Logger> logger;

  /// @brief Lumen manager for this interface.
  interfaceengine::lumen::manager::LumenManager lumen_manager;

  /// @brief Window manager for this interface.
  interfaceengine::window::WindowManager window_manager;

  /// @brief Core GUI component.
  GuiComponent::Ptr root_gui_component{std::make_unique<GuiComponent>()};

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

  /// @brief Converts lumen rules of a gui component
  /// to render rules of window manager.
  /// @param gui_component Component this lumen rule belongs to.
  /// @param lumen_rules Lumen rule to convert.
  /// @return Render rules for rendering a lumen.
  interfaceengine::window::RenderRules MakeRenderRules(
      const GuiComponent* gui_component, const LumenRules& lumen_rules);

  void UpdateRootComponentSize();
};

Interface::Impl::Impl()
    : logger{std::make_shared<Logger>("InterfaceEngine")},
      window_manager{logger} {}

Interface::Interface() : impl{new Impl()} { this->impl->self = this; }
Interface::~Interface() { delete this->impl; }

void Interface::Initialize() {
  // Initialize SDL.
  if (!SDL_Init(SDL_INIT_VIDEO))
    impl->logger->Log("SDL had failed to initialize", LogLevel::Error);
  if (!TTF_Init())
    impl->logger->Log("SDL_TTF had failed to initialize", LogLevel::Error);

  this->impl->window_manager.CreateWindow();
  this->impl->settings.executable_path = SDL_GetBasePath();

  impl->logger->Log("Interface initialized.", LogLevel::Debug);
}

void Interface::Run() {
  this->impl->is_running = true;
  while (this->impl->is_running) {
    this->impl->HandleEvents();
    this->impl->Render();
    this->impl->TickDelay();
  }
}

void Interface::SetRootGuiComponent(GuiComponent::Ptr gui_component) {
  this->impl->root_gui_component = std::move(gui_component);

  this->impl->UpdateRootComponentSize();
}

void Interface::SendMessageToGui(GuiComponent::Message message,
                                 GuiComponent::Id receiver_id,
                                 bool single_receiver) {
  impl->root_gui_component->DispatchMessage(message, receiver_id,
                                            single_receiver);
}

void Interface::Quit() { this->impl->is_running = false; }

void Interface::AddLogSink(Logger::SinkPtr sink) {
  impl->logger->AddSink(sink);
}

void Interface::Impl::Render() {
  // Clear past.
  this->window_manager.ClearRender();

  // Initialize a stack with gui components yet to render.
  std::stack<GuiComponent*> stack_to_render;
  stack_to_render.push(root_gui_component.get());

  // Iterate through rendering stack.
  CollectiveException col_exc(
      "Had finished rendering with following exceptions:");

  while (!stack_to_render.empty()) {
    try {
      // Extract next gui component to render.
      GuiComponent* gui_component_to_render = stack_to_render.top();
      stack_to_render.pop();

      // Iterate and render lumen rules of that gui component.
      auto lumen_rules_list = gui_component_to_render->GetLumenRules();
      for (const LumenRules& lumen_rules_to_render : lumen_rules_list) {
        this->window_manager.RenderTexture(
            this->lumen_manager.GetLumenTexture(
                lumen_rules_to_render.package_name,
                lumen_rules_to_render.lumen_name, lumen_rules_to_render.data),
            this->MakeRenderRules(gui_component_to_render,
                                  lumen_rules_to_render));
      }

      // Add children to stack.
      for (auto& component_child : gui_component_to_render->children) {
        stack_to_render.push(component_child.get());
      }
    } catch (...) {
      col_exc.Add(std::current_exception());
    }
  }
  if (!col_exc.empty()) logger->Log(col_exc.what(), LogLevel::Error);

  // Apply rendering.
  this->window_manager.ApplyRender();
}

void Interface::Impl::TickDelay() {
  // Get tick of current frame.
  auto curr_frame_tick = SDL_GetTicks();

  // Get frame duration.
  auto ms_per_frame = 1000 / this->settings.fps;
  auto curr_frame_duration = curr_frame_tick - this->last_frame_tick;

  // Wait if frame rendering was too fast.
  if (ms_per_frame > curr_frame_duration)
    SDL_Delay(ms_per_frame - curr_frame_duration);

  // Save current frame
  this->last_frame_tick = curr_frame_tick;
}

void Interface::Impl::HandleEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) this->self->Quit();
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
      root_gui_component->DispatchMouseClick({.is_down = true,
                                              .button = event.button.button,
                                              .x = event.button.x,
                                              .y = event.button.y});
    if (event.type == SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED)
      UpdateRootComponentSize();
  }
}

void Interface::LoadLumens(std::string package_path) {
  SDL_Renderer* renderer = this->impl->window_manager.GetRenderer();

  try {
    this->impl->lumen_manager.LoadLumPackage(package_path, renderer);
  } catch (const std::exception& e) {
    impl->logger->Log(std::string("Failed to load lum package:") + e.what(),
                      LogLevel::Error);
  } catch (...) {
    impl->logger->Log("Interface::LoadLumens",
                      LogLevel::Error);  // TODO(necromax): fix all thisd
                                         // sdifhsdfiouhfsdiuasdfhgSDO!
  }
}

interfaceengine::window::RenderRules Interface::Impl::MakeRenderRules(
    const GuiComponent* gui_component, const LumenRules& lumen_rules) {
  interfaceengine::window::RenderRules new_render_rules;
  PosPixel component_x, component_y, component_w, component_h;
  gui_component->GetSelfRect(&component_x, &component_y, &component_w,
                             &component_h);

  // Get rect from component to lumen rules.
  GetRect(&new_render_rules.x, &new_render_rules.y,            //
          &new_render_rules.w, &new_render_rules.h,            //
          component_x, component_y, component_w, component_h,  //
          lumen_rules.x_percent, lumen_rules.x_offset,         //
          lumen_rules.y_percent, lumen_rules.y_offset,         //
          lumen_rules.w_percent, lumen_rules.w_offset,         //
          lumen_rules.h_percent, lumen_rules.h_offset,         //
          lumen_rules.horizontal_alignment, lumen_rules.vertical_alignment);

  // Set anchors.
  new_render_rules.horizontal_anchor = lumen_rules.horizontal_anchor;
  new_render_rules.vertical_anchor = lumen_rules.vertical_anchor;

  // Set rotations.
  new_render_rules.rotation_angle = lumen_rules.rotation_angle;
  new_render_rules.rot_x_offset = lumen_rules.rot_x_offset;
  new_render_rules.rot_x_percent = lumen_rules.rot_x_percent;
  new_render_rules.rot_y_offset = lumen_rules.rot_y_offset;
  new_render_rules.rot_y_percent = lumen_rules.rot_y_percent;

  // Return rules.
  return new_render_rules;
}

void Interface::Impl::UpdateRootComponentSize() {
  int window_w, window_h;
  window_manager.GetWindowSize(&window_w, &window_h);
  root_gui_component->SetWidth(0, window_w);
  root_gui_component->SetHeight(0, window_h);
}
