#include <SHADERed/UI/CodeEditorUI.h>
#include <SHADERed/UI/PinnedUI.h>
#include <SHADERed/UI/Icons.h>
#include <imgui/imgui.h>

#include <SHADERed/Objects/Logger.h>

namespace ed {
	void PinnedUI::OnEvent(const SDL_Event& e)
	{
	}
	void PinnedUI::Update(float delta)
	{
		for (int i = 0; i < m_pinnedVars.size(); i++) { 
			ShaderVariable* var = m_pinnedVars[i];
			ImGui::PushID(i);

			if (ImGui::Button(UI_ICON_REMOVE, ImVec2(28, 0))) {
				Remove(var->Name);
				m_data->Parser.ModifyProject();
			}
			
			ImGui::SameLine();

			m_editor.Open(var);
			if (m_editor.Update())
				m_data->Parser.ModifyProject();

			ImGui::PopID();
		}

		if (m_pinnedVars.size() == 0)
			ImGui::TextWrapped("Pin variables here for easy access");
	}
	void PinnedUI::Add(ed::ShaderVariable* inp)
	{
		if (Contains(inp->Name)) {
			Logger::Get().Log("Cannot pin variable " + std::string(inp->Name) + " - it is already pinned", true);
			return;
		}

		Logger::Get().Log("Pinning variable " + std::string(inp->Name));

		m_data->Parser.ModifyProject();
		m_pinnedVars.push_back(inp);
	}
	void PinnedUI::Remove(const char* name)
	{
		for (int i = 0; i < m_pinnedVars.size(); i++) {
			if (strcmp(name, m_pinnedVars[i]->Name) == 0) {
				Logger::Get().Log("Unpinned variable " + std::string(name), true);
				m_pinnedVars.erase(m_pinnedVars.begin() + i);
				m_data->Parser.ModifyProject();
				return;
			}
		}

		Logger::Get().Log("Failed to unpin variable " + std::string(name) + " - not pinned", true);
	}
	bool PinnedUI::Contains(const char* data)
	{
		for (auto& var : m_pinnedVars)
			if (strcmp(var->Name, data) == 0)
				return true;
		return false;
	}
}