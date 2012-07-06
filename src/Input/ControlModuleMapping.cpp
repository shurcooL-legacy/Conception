#include "../Main.h"

#if 0
ControlModuleMapping::ControlModuleMapping()
	: InputListener(),
	  m_HoverModule(nullptr),
	  m_ActiveModule(nullptr),
	  m_Entries(),
	  m_ButtonMappings(),
	  m_SliderMappings(),
	  m_AxisMappings(),
	  m_PositiveConstraintMappings(),
	  m_NegativeConstraintMappings()
{
}

ControlModuleMapping::~ControlModuleMapping()
{
}

void ControlModuleMapping::AddMapping(ControlModule * ControlModule, sint32 Priority, std::vector<InputManager::InputId> ButtonMappings, std::vector<InputManager::InputId> SliderMappings, std::vector<InputManager::InputId> AxisMappings, std::vector<InputManager::InputId> PositiveConstraints, std::vector<InputManager::InputId> NegativeConstraints)
{
	ControlModule->VerifyMapping(ButtonMappings.size(), SliderMappings.size(), AxisMappings.size());

	ControlModule->SetActiveModulePointer(&m_ActiveModule);

	m_Entries.push_back(ControlModuleMappingEntry(ControlModule, Priority, PositiveConstraints.size(), NegativeConstraints.size()));

	for (auto it0 = ButtonMappings.begin(); it0 != ButtonMappings.end(); ++it0)
		m_ButtonMappings.insert(std::pair<InputManager::InputId, std::pair<class ControlModule *, uint8>>((*it0), std::pair<class ControlModule *, uint8>(ControlModule, it0 - ButtonMappings.begin())));
	for (auto it0 = SliderMappings.begin(); it0 != SliderMappings.end(); ++it0)
		m_SliderMappings.insert(std::pair<InputManager::InputId, std::pair<class ControlModule *, uint8>>((*it0), std::pair<class ControlModule *, uint8>(ControlModule, it0 - SliderMappings.begin())));
	for (auto it0 = AxisMappings.begin(); it0 != AxisMappings.end(); ++it0)
		m_AxisMappings.insert(std::pair<InputManager::InputId, std::pair<class ControlModule *, uint8>>((*it0), std::pair<class ControlModule *, uint8>(ControlModule, it0 - AxisMappings.begin())));

	for (auto it0 = PositiveConstraints.begin(); it0 != PositiveConstraints.end(); ++it0)
		m_PositiveConstraintMappings.insert(std::pair<InputManager::InputId, std::pair<class ControlModule *, uint8>>((*it0), std::pair<class ControlModule *, uint8>(ControlModule, it0 - PositiveConstraints.begin())));
	for (auto it0 = NegativeConstraints.begin(); it0 != NegativeConstraints.end(); ++it0)
		m_NegativeConstraintMappings.insert(std::pair<InputManager::InputId, std::pair<class ControlModule *, uint8>>((*it0), std::pair<class ControlModule *, uint8>(ControlModule, it0 - NegativeConstraints.begin())));
}

// TODO: Consider moving this to AddMapping()
// Call this function after all modules are added, it makes sure to enable one of them if it should get enabled
void ControlModuleMapping::DoneAdding()
{
	// Active Module change
	{
		auto NewActiveModule = m_ActiveModule;
		
		// Check if there is a new active module
		for (auto & Entry : m_Entries)
		{
			uint8 OverallPositive = GetOverallConstraintState(Entry.m_PositiveConstraintStates, 2);
			uint8 OverallNegative = GetOverallConstraintState(Entry.m_NegativeConstraintStates, 0);
			
			bool HigherPriority = (nullptr == GetModuleEntry(NewActiveModule) || Entry.m_Priority > GetModuleEntry(NewActiveModule)->m_Priority);
			
			if (2 == OverallPositive && 0 == OverallNegative && HigherPriority && Entry.m_ControlModule->ShouldActivate())
			{
				NewActiveModule = Entry.m_ControlModule;
			}
		}

		if (m_ActiveModule != NewActiveModule)
		{
			ChangeActiveModule(NewActiveModule);
		}
	}
}

void ControlModuleMapping::RemoveFromMultimap(std::multimap<InputManager::InputId, std::pair<ControlModule *, uint8>> & Multimap, ControlModule * ControlModule)
{
	while (true) {
		bool Erased = false;

		for (auto it0 = Multimap.begin(); Multimap.end() != it0; ++it0)
		{
			if (it0->second.first == ControlModule) {
				Multimap.erase(it0);
				Erased = true;
				break;
			}
		}

		if (!Erased)
		{
			break;
		}
	}
}

void ControlModuleMapping::RemoveMapping(ControlModule * ControlModule)
{
	for (auto it0 = m_Entries.begin(); it0 != m_Entries.end(); ++it0)
	{
		if (it0->m_ControlModule == ControlModule)
		{
			m_Entries.erase(it0);
			break;
		}
	}

	RemoveFromMultimap(m_ButtonMappings, ControlModule);
	RemoveFromMultimap(m_SliderMappings, ControlModule);
	RemoveFromMultimap(m_AxisMappings, ControlModule);

	RemoveFromMultimap(m_PositiveConstraintMappings, ControlModule);
	RemoveFromMultimap(m_NegativeConstraintMappings, ControlModule);

	// Deactivate the current m_ActiveModule if it was removed
	if (m_ActiveModule == ControlModule)
	{
		// Active Module change
		{
			// The old active module becomes inactive (since it was removed)
			class ControlModule * NewActiveModule = nullptr;

			// Check if there is a new active module
			for (auto & Entry : m_Entries)
			{
				uint8 OverallPositive = GetOverallConstraintState(Entry.m_PositiveConstraintStates, 2);
				uint8 OverallNegative = GetOverallConstraintState(Entry.m_NegativeConstraintStates, 0);

				bool HigherPriority = (nullptr == GetModuleEntry(NewActiveModule) || Entry.m_Priority > GetModuleEntry(NewActiveModule)->m_Priority);

				if (2 == OverallPositive && 0 == OverallNegative && HigherPriority && Entry.m_ControlModule->ShouldActivate())
				{
					NewActiveModule = Entry.m_ControlModule;
				}
			}

			if (m_ActiveModule != NewActiveModule)
			{
				ChangeActiveModule(NewActiveModule);
			}
		}
	}
}

void ControlModuleMapping::ProcessButton(InputManager::InputId ButtonId, bool Pressed)
{
	auto Range = m_ButtonMappings.equal_range(ButtonId);
	for (auto it0 = Range.first; it0 != Range.second; ++it0)
		it0->second.first->ProcessButton(it0->second.second, Pressed);

	// Update Positive Constraint states
	Range = m_PositiveConstraintMappings.equal_range(ButtonId);
	for (auto it0 = Range.first; it0 != Range.second; ++it0)
	{
		for (auto it1 = m_Entries.begin(); it1 != m_Entries.end(); ++it1)
		{
			if (it0->second.first == it1->m_ControlModule)
			{
				it1->m_PositiveConstraintStates[it0->second.second] = Pressed;

				break;
			}
		}
	}

	// Update Negative Constraint states
	Range = m_NegativeConstraintMappings.equal_range(ButtonId);
	for (auto it0 = Range.first; it0 != Range.second; ++it0)
	{
		for (auto it1 = m_Entries.begin(); it1 != m_Entries.end(); ++it1)
		{
			if (it0->second.first == it1->m_ControlModule)
			{
				it1->m_NegativeConstraintStates[it0->second.second] = Pressed;

				break;
			}
		}
	}

	// Active Module change
	{
		auto NewActiveModule = m_ActiveModule;

		// Check if the old active module becomes inactive
		if (nullptr != GetActiveModuleEntry())
		{
			uint8 OverallPositive = GetOverallConstraintState(GetActiveModuleEntry()->m_PositiveConstraintStates, 2);
			uint8 OverallNegative = GetOverallConstraintState(GetActiveModuleEntry()->m_NegativeConstraintStates, 0);

			if (!(2 == OverallPositive && 0 == OverallNegative) || GetActiveModuleEntry()->m_ControlModule->ShouldDeactivate())
			{
				NewActiveModule = nullptr;
			}
		}

		// Check if there is a new active module
		for (auto & Entry : m_Entries)
		{
			uint8 OverallPositive = GetOverallConstraintState(Entry.m_PositiveConstraintStates, 2);
			uint8 OverallNegative = GetOverallConstraintState(Entry.m_NegativeConstraintStates, 0);

			bool HigherPriority = (nullptr == GetModuleEntry(NewActiveModule) || Entry.m_Priority > GetModuleEntry(NewActiveModule)->m_Priority);

			if (2 == OverallPositive && 0 == OverallNegative && HigherPriority && Entry.m_ControlModule->ShouldActivate())
			{
				NewActiveModule = Entry.m_ControlModule;
			}
		}

		if (m_ActiveModule != NewActiveModule)
		{
			ChangeActiveModule(NewActiveModule);

			auto Range = m_ButtonMappings.equal_range(ButtonId);
			for (auto it0 = Range.first; it0 != Range.second; ++it0)
				it0->second.first->ProcessButton(it0->second.second, Pressed);
		}
	}
}

void ControlModuleMapping::ProcessSlider(InputManager::InputId SliderId, double MovedAmount)
{
	auto Range = m_SliderMappings.equal_range(SliderId);
	for (auto it0 = Range.first; it0 != Range.second; ++it0)
		it0->second.first->ProcessSlider(it0->second.second, MovedAmount);
}

void ControlModuleMapping::ProcessAxis(InputManager::InputId AxisId, Input::AxisState AxisState)
{
	auto Range = m_AxisMappings.equal_range(AxisId);
	for (auto it0 = Range.first; it0 != Range.second; ++it0)
		it0->second.first->ProcessAxis(it0->second.second, AxisState);
}

void ControlModuleMapping::Process2Axes(InputManager::InputId FirstAxisId, Input::AxisState AxisState[2])
{
	auto Range = m_AxisMappings.equal_range(FirstAxisId);
	for (auto it0 = Range.first; it0 != Range.second; ++it0)
		it0->second.first->Process2Axes(it0->second.second, AxisState);

	// Active Module change
	{
		auto NewActiveModule = m_ActiveModule;

		// Check if the old active module becomes inactive
		if (nullptr != GetActiveModuleEntry())
		{
			uint8 OverallPositive = GetOverallConstraintState(GetActiveModuleEntry()->m_PositiveConstraintStates, 2);
			uint8 OverallNegative = GetOverallConstraintState(GetActiveModuleEntry()->m_NegativeConstraintStates, 0);

			if (!(2 == OverallPositive && 0 == OverallNegative) || GetActiveModuleEntry()->m_ControlModule->ShouldDeactivate())
			{
				NewActiveModule = nullptr;
			}
		}

		// Check if there is a new active module
		for (auto & Entry : m_Entries)
		{
			uint8 OverallPositive = GetOverallConstraintState(Entry.m_PositiveConstraintStates, 2);
			uint8 OverallNegative = GetOverallConstraintState(Entry.m_NegativeConstraintStates, 0);

			bool HigherPriority = (nullptr == GetModuleEntry(NewActiveModule) || Entry.m_Priority > GetModuleEntry(NewActiveModule)->m_Priority);
			
			if (2 == OverallPositive && 0 == OverallNegative && HigherPriority && Entry.m_ControlModule->ShouldActivate())
			{
				NewActiveModule = Entry.m_ControlModule;
			}
		}

		if (m_ActiveModule != NewActiveModule)
		{
			ChangeActiveModule(NewActiveModule);

			auto Range = m_AxisMappings.equal_range(FirstAxisId);
			for (auto it0 = Range.first; it0 != Range.second; ++it0)
				it0->second.first->Process2Axes(it0->second.second, AxisState);
		}
	}
}

void ControlModuleMapping::ProcessCharacter(int Character, bool Pressed)
{
	for (auto & Entry : m_Entries)
	{
		Entry.m_ControlModule->ProcessCharacter(Character, Pressed);
	}
}

void ControlModuleMapping::ProcessTimePassed(double TimePassed)
{
	for (auto & Entry : m_Entries)
	{
		Entry.m_ControlModule->ProcessTimePassed(TimePassed);
	}
}

void ControlModuleMapping::ChangeActiveModule(ControlModule * ActiveModule)
{
/*if (nullptr == ActiveModule) printf("ChangeActiveModule(nullptr)\n");
else if (typeid(UnrealCameraModule) == typeid(*ActiveModule)) printf("ChangeActiveModule(UnrealCameraModule)\n");
else if (typeid(WidgetModule) == typeid(*ActiveModule)) printf("ChangeActiveModule(WidgetModule)\n");
else if (typeid(TypingModule) == typeid(*ActiveModule)) printf("ChangeActiveModule(TypingModule)\n");*/

	if (m_ActiveModule == ActiveModule) {
		std::cerr << "Exception: ChangeActiveModule() called with same active module.\n";
		throw 0;
	}

	if (m_ActiveModule) m_ActiveModule->ProcessDeactivation(ActiveModule);
	auto PreviousActiveModule = m_ActiveModule;
	m_ActiveModule = ActiveModule;
	if (ActiveModule) ActiveModule->ProcessActivation(PreviousActiveModule);

	g_InputManager->SetMouseCursorVisibility(nullptr == m_ActiveModule || m_ActiveModule->ShouldMouseCursorVisible());
}

ControlModuleMapping::ControlModuleMappingEntry * ControlModuleMapping::GetActiveModuleEntry()
{
	return GetModuleEntry(m_ActiveModule);
}

ControlModuleMapping::ControlModuleMappingEntry * ControlModuleMapping::GetModuleEntry(ControlModule * Module)
{
	if (nullptr == Module) return nullptr;

	for (auto & Entry : m_Entries)
	{
		if (Module == Entry.m_ControlModule)
			return &Entry;
	}

	throw 0;
}

// Returns: Default if there are no states
//          0 if no states are true
//			1 if some states are true (but not all)
//			2 if all states are true
uint8 ControlModuleMapping::GetOverallConstraintState(std::vector<bool> & ConstraintStates, uint8 Default)
{
	if (0 == ConstraintStates.size()) return Default;

	uint8 TrueStates = 0;

	for (auto it0 = ConstraintStates.begin(); it0 != ConstraintStates.end(); ++it0)
	{
		if (*it0) ++TrueStates;
	}

	if (0 == TrueStates) return 0;
	else if (ConstraintStates.size() == TrueStates) return 2;
	else return 1;
}
#endif
