#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

#include "WindowUtils.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

namespace ImOf
{
    inline void SetFont() {
        ImGuiIO * io = &ImGui::GetIO();
        ImFontConfig font_config;
        font_config.OversampleH = 4;
        font_config.OversampleV = 4;
        io->Fonts->AddFontFromFileTTF(&ofToDataPath("Karla-Regular.ttf")[0], 14.f, &font_config);
        io->Fonts->AddFontFromFileTTF(&ofToDataPath("FiraCode-Regular.ttf")[0], 16.f, &font_config);
        io->Fonts->AddFontFromFileTTF(&ofToDataPath("FiraCode-Regular.ttf")[0], 28.f, &font_config);
    }
    
    inline void PushMonospaceFont() {
        ImGuiIO * io = &ImGui::GetIO();
        ImFont *mono = io->Fonts->Fonts[1];
        ImGui::PushFont(mono);
    }
    
    inline void PushMonospaceLargeFont() {
        ImGuiIO * io = &ImGui::GetIO();
        ImFont *mono = io->Fonts->Fonts[2];
        ImGui::PushFont(mono);
    }
    
    inline void SetStyle() {
        ImGuiStyle& style = ImGui::GetStyle();
        
        style.WindowRounding = 0;
        style.ScrollbarSize = 3;
        style.ScrollbarRounding = 0;
        style.FramePadding = ImVec2(6, 4);
        style.ItemSpacing = ImVec2(4, 8);
        style.FrameRounding = 2;
        style.GrabRounding = 2;
        
        style.Colors[ImGuiCol_Text]                  = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
        style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.86f, 0.93f, 0.89f, 0.58f);
        style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.17f, 0.70f);
        style.Colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.20f, 0.22f, 0.27f, 0.58f);
        style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.13f, 0.14f, 0.17f, 0.92f);
        style.Colors[ImGuiCol_Border]                = ImVec4(0.13f, 0.14f, 0.17f, 0.00f);
        style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.13f, 0.14f, 0.17f, 0.00f);
        style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.17f, 0.19f, 0.23f, 0.55f);
        style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.00f, 0.63f, 0.75f, 0.78f);
        style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.00f, 0.63f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
        style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.00f, 0.63f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
        style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.47f, 0.77f, 0.83f, 0.21f);
        style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.00f, 0.63f, 0.75f, 0.78f);
        style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.00f, 0.63f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_ComboBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.00f, 0.63f, 0.75f, 0.80f);
        style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.00f, 0.63f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_Button]                = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.00f, 0.63f, 0.75f, 0.86f);
        style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.00f, 0.63f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_Header]                = ImVec4(0.21f, 0.26f, 0.35f, 0.76f);
        style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.00f, 0.63f, 0.75f, 0.86f);
        style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.00f, 0.63f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_Column]                = ImVec4(0.47f, 0.77f, 0.83f, 0.32f);
        style.Colors[ImGuiCol_ColumnHovered]         = ImVec4(0.00f, 0.63f, 0.75f, 0.78f);
        style.Colors[ImGuiCol_ColumnActive]          = ImVec4(0.00f, 0.63f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
        style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.00f, 0.63f, 0.75f, 0.78f);
        style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.00f, 0.63f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_CloseButton]           = ImVec4(0.86f, 0.93f, 0.89f, 0.16f);
        style.Colors[ImGuiCol_CloseButtonHovered]    = ImVec4(0.86f, 0.93f, 0.89f, 0.39f);
        style.Colors[ImGuiCol_CloseButtonActive]     = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
        style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
        style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.00f, 0.63f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
        style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.00f, 0.63f, 0.75f, 1.00f);
        style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.00f, 0.63f, 0.75f, 0.43f);
        style.Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);
    }
    
    inline void BeginTransparentWindow() {
        static bool p_open = true;
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
        ImGui::Begin("Show Control", &p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    }
    
    inline void EndTransparentWindow() {
        ImGui::End();
        ImGui::PopStyleColor();
    }
    
    inline void BeginPopup() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2);
        ImGui::Begin("", NULL, ImVec2(0,0), -1.0f, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
    }
    
    inline void EndPopup() {
        ImGui::End();
        ImGui::PopStyleVar();
    }
    
    inline void Alert(const string title, const string text, bool *show) {
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2);
        
        if (*show) {
            ImGui::OpenPopup(title.c_str());
            *show = false;
        }
        
        if (ImGui::BeginPopupModal(title.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("%s", text.c_str());
            ImGui::Separator();
            
            if (ImGui::Button("OK")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        
        ImGui::PopStyleVar();
    }
    
    inline ofFileDialogResult SystemLoadDialog(string windowTitle, bool bFolderSelection, string defaultPath = "") {
        bool windowTop = WindowUtils::getWindowOnTop();
        WindowUtils::setWindowOnTop(false);
        ofFileDialogResult result = ofSystemLoadDialog(windowTitle, bFolderSelection, defaultPath);
        WindowUtils::setWindowOnTop(windowTop);
        return result;
    }
    
    inline ImVec2 CalcItemSize(ImVec2 size) {
        ImVec2 itemSize;
        
        ImGui::PushItemWidth(size.x);
        itemSize.x = ImGui::CalcItemWidth();
        ImGui::PopItemWidth();
        
        if (size.y < 0.0f) {
            itemSize.y = ImGui::GetContentRegionAvail().y - 2;
            if (size.y < -1.0f) itemSize.y += size.y;
        } else if (size.y == 0.0f) {
            itemSize.y = ImGui::GetWindowFontSize() + ImGui::GetStyle().FramePadding.y * 2;
        } else {
            itemSize.y = size.y;
        }
        
        itemSize.y = (float)(int)itemSize.y;
        
        return itemSize;
    }
    
    
    static bool PlayToggle(const char* label, bool *playing, const ImVec2& size = ImVec2(30,0)) {
        
        static ImDrawList* drawList = ImGui::GetWindowDrawList();
        static ImVec2 pos, itemSize;
        static int cx, cy;
        static const ImU32 textColor = ImGui::ColorConvertFloat4ToU32(ImGui::GetStyle().Colors[ImGuiCol_Text]);
        
        pos = ImGui::GetCursorScreenPos();
        itemSize = CalcItemSize(size);
        cx = pos.x + itemSize.x / 2;
        cy = pos.y + itemSize.y / 2;
        
        static bool result;
        result = ImGui::Button(label, size);
        
        // draw icon
        if (*playing) {
            drawList->AddRectFilled(ImVec2(cx-6, cy-7), ImVec2(cx-2, cy+7), textColor);
            drawList->AddRectFilled(ImVec2(cx+2, cy-7), ImVec2(cx+6, cy+7), textColor);
        } else {
            drawList->AddTriangleFilled(ImVec2(cx+7, cy), ImVec2(cx-4, cy+7), ImVec2(cx-4, cy-7), textColor);
        }
        
        if (result) {
            *playing = !(*playing);
        }
        
        return result;
    }
    
    static bool Seekbar(const char* label, int* v, int v_min, int v_max, const ImVec2& size = ImVec2(0,0)) {
        
        static ImDrawList* drawList = ImGui::GetWindowDrawList();
        static ImVec2 pos, itemSize;
        static float cy;
        static bool result;
        static const ImU32 textColor = [] {
            ImVec4 c = ImGui::GetStyle().Colors[ImGuiCol_Text];
            return ImGui::ColorConvertFloat4ToU32(ImVec4(c.x * 0.8, c.y * 0.8, c.z * 0.8, 0.5f));
        }();
        static const ImVec4 sliderGrabColor = [] {
            ImVec4 c = ImGui::GetStyle().Colors[ImGuiCol_Text];
            return ImVec4(c.x * 0.8, c.y * 0.8, c.z * 0.8, 1.0f);
        }();
        
        pos = ImGui::GetCursorScreenPos();
        itemSize = CalcItemSize(size);
        cy = pos.y + itemSize.y / 2;
        
        drawList->AddLine(ImVec2(pos.x, cy), ImVec2(pos.x + itemSize.x, cy), textColor);
        
        // draw slider
        ImGuiStyle& style = ImGui::GetStyle();
        static const int prevGrabRounding = style.GrabRounding;
        
        ImGui::PushItemWidth(size.x);
        ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 18);
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, sliderGrabColor);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        style.GrabRounding = 9;
        
        result = ImGui::SliderInt(label, v, v_min, v_max, "");
        
        style.GrabRounding = prevGrabRounding;
        ImGui::PopStyleColor(); ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        ImGui::PopItemWidth();
        
        return result;
    }
}
