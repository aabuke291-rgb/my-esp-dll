#include <windows.h>
#include <d3d9.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

// Состояние интерфейса
bool show_visuals_tab = true;
bool feature_esp = false;
bool feature_tracers = false;
float color_picker[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

// Функция отрисовки меню
void RenderUI() {
    ImGui::Begin("Custom UI Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Настройки отображения");
    ImGui::Separator();

    // Чекбоксы и настройки
    ImGui::Checkbox("Включить подсветку (ESP)", &feature_esp);
    ImGui::Checkbox("Отображать линии (Tracers)", &feature_tracers);

    // Выбор цвета
    ImGui::ColorEdit4("Цвет элементов", color_picker);

    if (ImGui::Button("Сбросить настройки")) {
        feature_esp = false;
        feature_tracers = false;
    }

    ImGui::End();
}
