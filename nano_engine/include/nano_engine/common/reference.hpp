#pragma once

#include <memory>

#include <nano_engine/common/assert.hpp>

namespace nano_engine
{
	template<typename T>
	class Reference
	{
	public:
		Reference() : m_holder(nullptr) {}
		Reference(T& ref) : m_holder(new ReferenceHolder(ref)){}
		~Reference() { m_holder = nullptr; }

		Reference& operator=(T& ref)
		{
			Assign(ref);
			return *this;
		}

		Reference(const Reference& other) : m_holder(other.m_holder) {}
		Reference(Reference&& other)
		{
			m_holder = other.m_holder;
			other.m_holder = nullptr;
		}

		T& Get()
		{
			NANO_ASSERT(IsValid(), "This reference is invalid");
			return m_holder->m_ref;
		}

		const T& Get() const
		{
			NANO_ASSERT(IsValid(), "This reference is invalid");
			return m_holder->m_ref;
		}

		T& operator* ()
		{
			NANO_ASSERT(IsValid(), "This reference is invalid");
			return m_holder->m_ref;
		}

		const T& operator* () const
		{
			NANO_ASSERT(IsValid(), "This reference is invalid");
			return m_holder->m_ref;
		}

		void Assign(T& ref)
		{
			if (IsValid())
			{
				m_holder = nullptr;
			}
			
			m_holder = std::make_shared<ReferenceHolder>(ref);
		}
		bool IsValid() const { return m_holder != nullptr; }
	private:
		struct ReferenceHolder
		{
			T& m_ref;
			ReferenceHolder(T& ref) : m_ref(ref) {}
		};

		std::shared_ptr<ReferenceHolder> m_holder;
	};
}