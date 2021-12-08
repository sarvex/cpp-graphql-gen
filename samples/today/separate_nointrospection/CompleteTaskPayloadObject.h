// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef COMPLETETASKPAYLOADOBJECT_H
#define COMPLETETASKPAYLOADOBJECT_H

#include "TodaySchema.h"

namespace graphql::today::object {
namespace methods::CompleteTaskPayloadHas {

template <class TImpl>
concept getTaskWithParams = requires (TImpl impl, service::FieldParams params) 
{
	{ service::FieldResult<std::shared_ptr<Task>> { impl.getTask(std::move(params)) } };
};

template <class TImpl>
concept getTask = requires (TImpl impl) 
{
	{ service::FieldResult<std::shared_ptr<Task>> { impl.getTask() } };
};

template <class TImpl>
concept getClientMutationIdWithParams = requires (TImpl impl, service::FieldParams params) 
{
	{ service::FieldResult<std::optional<std::string>> { impl.getClientMutationId(std::move(params)) } };
};

template <class TImpl>
concept getClientMutationId = requires (TImpl impl) 
{
	{ service::FieldResult<std::optional<std::string>> { impl.getClientMutationId() } };
};

template <class TImpl>
concept beginSelectionSet = requires (TImpl impl, const service::SelectionSetParams params) 
{
	{ impl.beginSelectionSet(params) };
};

template <class TImpl>
concept endSelectionSet = requires (TImpl impl, const service::SelectionSetParams params) 
{
	{ impl.endSelectionSet(params) };
};

} // namespace methods::CompleteTaskPayloadHas

class CompleteTaskPayload
	: public service::Object
{
private:
	service::AwaitableResolver resolveTask(service::ResolverParams&& params) const;
	service::AwaitableResolver resolveClientMutationId(service::ResolverParams&& params) const;

	service::AwaitableResolver resolve_typename(service::ResolverParams&& params) const;

	struct Concept
	{
		virtual ~Concept() = default;

		virtual void beginSelectionSet(const service::SelectionSetParams& params) const = 0;
		virtual void endSelectionSet(const service::SelectionSetParams& params) const = 0;

		virtual service::FieldResult<std::shared_ptr<Task>> getTask(service::FieldParams&& params) const = 0;
		virtual service::FieldResult<std::optional<std::string>> getClientMutationId(service::FieldParams&& params) const = 0;
	};

	template <class T>
	struct Model
		: Concept
	{
		Model(std::shared_ptr<T>&& pimpl) noexcept
			: _pimpl { std::move(pimpl) }
		{
		}

		service::FieldResult<std::shared_ptr<Task>> getTask(service::FieldParams&& params) const final
		{
			if constexpr (methods::CompleteTaskPayloadHas::getTaskWithParams<T>)
			{
				return { _pimpl->getTask(std::move(params)) };
			}
			else if constexpr (methods::CompleteTaskPayloadHas::getTask<T>)
			{
				return { _pimpl->getTask() };
			}
			else
			{
				throw std::runtime_error(R"ex(CompleteTaskPayload::getTask is not implemented)ex");
			}
		}

		service::FieldResult<std::optional<std::string>> getClientMutationId(service::FieldParams&& params) const final
		{
			if constexpr (methods::CompleteTaskPayloadHas::getClientMutationIdWithParams<T>)
			{
				return { _pimpl->getClientMutationId(std::move(params)) };
			}
			else if constexpr (methods::CompleteTaskPayloadHas::getClientMutationId<T>)
			{
				return { _pimpl->getClientMutationId() };
			}
			else
			{
				throw std::runtime_error(R"ex(CompleteTaskPayload::getClientMutationId is not implemented)ex");
			}
		}

		void beginSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::CompleteTaskPayloadHas::beginSelectionSet<T>)
			{
				_pimpl->beginSelectionSet(params);
			}
		}

		void endSelectionSet(const service::SelectionSetParams& params) const final
		{
			if constexpr (methods::CompleteTaskPayloadHas::endSelectionSet<T>)
			{
				_pimpl->endSelectionSet(params);
			}
		}

	private:
		const std::shared_ptr<T> _pimpl;
	};

	CompleteTaskPayload(std::unique_ptr<Concept>&& pimpl) noexcept;

	service::TypeNames getTypeNames() const noexcept;
	service::ResolverMap getResolvers() const noexcept;

	void beginSelectionSet(const service::SelectionSetParams& params) const final;
	void endSelectionSet(const service::SelectionSetParams& params) const final;

	const std::unique_ptr<Concept> _pimpl;

public:
	template <class T>
	CompleteTaskPayload(std::shared_ptr<T> pimpl) noexcept
		: CompleteTaskPayload { std::unique_ptr<Concept> { std::make_unique<Model<T>>(std::move(pimpl)) } }
	{
	}
};

} // namespace graphql::today::object

#endif // COMPLETETASKPAYLOADOBJECT_H
